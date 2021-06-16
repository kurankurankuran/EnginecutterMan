//=============================================================================
//
// 死亡エフェクト処理 [deatheffect.cpp]
// Author : 三宅　秀太郎
// ----------------------------------------------------------------------------
// 2020/12/23 DrawDeathEffect()の最初と最後にオフにしたりオンにする処理を追加。三宅
//
//=============================================================================
#include "deatheffect.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_DEATHEFFECT		L"data/texture/pipo-mapeffect005.png"// 読み込むテクスチャファイル名
#define	DEATHEFFECT_SIZE_X		(5.0f)							// ビルボードの幅
#define	DEATHEFFECT_SIZE_Y		(5.0f)							// ビルボードの高さ

#define	MAX_DEATHEFFECT			(128)							// ビルボード最大数

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(0.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct DEATHEFFECT {
	XMFLOAT4X4	mtxWorld;	// ワールドマトリックス
	XMFLOAT3	pos;		// 位置
	XMFLOAT4	col;		// 色
	float		fSizeX;		// 幅
	float		fSizeY;		// 高さ
	int			nCounter;	// カウンター
	int			nPattern;	// パターンNo.
	float		fAlpha;		// アルファ値
	bool		bUse;		// 使用しているかどうか
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexDeathEffect(ID3D11Device* pDevice);
void SetVertexDeathEffect(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorDeathEffect(int nIdxDeathEffect, XMFLOAT4 col);
void SetTextureDeathEffect(int nIdxDeathEffect, int nPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH				g_mesh;
static MATERIAL			g_material;
static DEATHEFFECT		g_deatheffect[MAX_DEATHEFFECT];	// 爆発ワーク
static CPlayer*         g_pPlayer;
static CCamera*         g_pCamera;
static bool             g_bLoadMesh = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDeathEffect(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	/*ZeroMemory(&g_mesh, sizeof(MESH));*/

	//// 頂点情報の作成
	//MakeVertexDeathEffect(pDevice);

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {

		ZeroMemory(&g_mesh, sizeof(MESH));

		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,				// デバイスへのポインタ
			TEXTURE_DEATHEFFECT,	// ファイルの名前
			&g_mesh.pTexture);	// 読み込むメモリー
		// テクスチャ―の保存
		SaveMesh(&g_mesh);

		// 頂点情報の作成
		MakeVertexDeathEffect(pDevice);

		g_bLoadMesh = true;
	}	
	
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());
	
	// マテリアルの設定
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_mesh.pMaterial = &g_material;

	DEATHEFFECT* pHE = g_deatheffect;
	for (int i = 0; i < MAX_DEATHEFFECT; ++i, ++pHE) {
		pHE->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pHE->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pHE->fSizeX = DEATHEFFECT_SIZE_X;
		pHE->fSizeY = DEATHEFFECT_SIZE_Y;
		pHE->nCounter = 0;
		pHE->nPattern = 0;
		pHE->fAlpha = 1.0f;
		pHE->bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDeathEffect(void)
{
#if MESH_RELEASE_ON
		// メッシュの開放
		ReleaseMesh(&g_mesh);
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDeathEffect(void)
{
	DEATHEFFECT* pHE = g_deatheffect;
	for (int i = 0; i < MAX_DEATHEFFECT; ++i, ++pHE)
{
		if (pHE->bUse)
		{
			++pHE->nCounter;
			
			// ヒットエフェクトの速度設定
			if (pHE->nCounter % 6 == 0)
			{
				pHE->nPattern++;
			}
			
			// 画像の個数分出す
			if (pHE->nPattern >= 10)
			{
				pHE->bUse = false;
				continue;
			}


			// 頂点座標の設定
			pHE->fSizeX += 0.5f;
			pHE->fSizeY += 0.5f;

			SetVertexDeathEffect(i, pHE->fSizeX, pHE->fSizeY);

			if (pHE->nPattern >= 2)
			{
				// 頂点カラーの設定
				pHE->fAlpha -= 0.02f;

				if (pHE->nCounter % 10 == 0)
				{
					pHE->nPattern++;

				}

				if (pHE->fAlpha < 0.0f)
				{
					pHE->fAlpha = 0.0f;
				}
				SetColorDeathEffect(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pHE->fAlpha));
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDeathEffect(void)
{
	// ここでオフにする
	CLight::Get()->SetDisable();

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// ビューマトリックスを取得
	//XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	XMFLOAT4X4& mtxView = CCamera::Get()/*g_pCamera*/->GetViewMatrix();
	// 加算合成
	SetBlendState(BS_ADDITIVE);

	DEATHEFFECT* pHE = g_deatheffect;
	for (int i = 0; i < MAX_DEATHEFFECT; ++i, ++pHE) {
		if (pHE->bUse) {
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&pHE->mtxWorld, mtxWorld);

			pHE->mtxWorld._11 = mtxView._11;
			pHE->mtxWorld._12 = mtxView._21;
			pHE->mtxWorld._13 = mtxView._31;
			pHE->mtxWorld._21 = mtxView._12;
			pHE->mtxWorld._22 = mtxView._22;
			pHE->mtxWorld._23 = mtxView._32;
			pHE->mtxWorld._31 = mtxView._13;
			pHE->mtxWorld._32 = mtxView._23;
			pHE->mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&pHE->mtxWorld);

			// スケールを反映
			mtxScale = XMMatrixScaling(pHE->fSizeX, pHE->fSizeY, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(pHE->pos.x, pHE->pos.y, pHE->pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			XMStoreFloat4x4(&pHE->mtxWorld, mtxWorld);
			g_mesh.mtxWorld = pHE->mtxWorld;

			// テクスチャ座標の設定
			SetTextureDeathEffect(i, pHE->nPattern);

			// カラーの設定
			g_material.Diffuse = pHE->col;

			// メッシュの描画
			DrawMesh(pDeviceContext, &g_mesh);
		}
	}

	// 半透明処理無効
	SetBlendState(BS_NONE);


	// ここでオンにする
	CLight::Get()->SetEnable();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexDeathEffect(ID3D11Device* pDevice)
{
	// オブジェクトの頂点配列を生成
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	// 頂点座標の設定
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[1].vtx = XMFLOAT3(-1.0f / 2, 1.0f  / 2, 0.0f);
	pVtx[2].vtx = XMFLOAT3(1.0f  / 2, -1.0f / 2, 0.0f);
	pVtx[3].vtx = XMFLOAT3(1.0f  / 2, 1.0f  / 2, 0.0f);

	// 法線の設定
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 反射光の設定
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

	// インデックス配列を生成
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[g_mesh.nNumIndex];

	// インデックス配列の中身を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// 頂点バッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// 一時配列の解放
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexDeathEffect(int nIdxDeathEffect, float fSizeX, float fSizeY)
{
	if (nIdxDeathEffect >= 0 && nIdxDeathEffect < MAX_DEATHEFFECT) {
		DEATHEFFECT* pExp = &g_deatheffect[nIdxDeathEffect];
		pExp->fSizeX = fSizeX;
		pExp->fSizeY = fSizeY;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorDeathEffect(int nIdxDeathEffect, XMFLOAT4 col)
{
	if (nIdxDeathEffect >= 0 && nIdxDeathEffect < MAX_DEATHEFFECT) {
		g_deatheffect[nIdxDeathEffect].col = col;
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureDeathEffect(int nIdxDeathEffect, int nPattern)
{
	XMMATRIX mtxTexture, mtxScale, mtxTranslate;

	// テクスチャマトリックスの初期化
	mtxTexture = XMMatrixIdentity();

	// スケールを反映
	mtxScale = XMMatrixScaling(0.1f, 1.0f, 1.0f);		  // Xは1÷分割数
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxScale);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(0.1f * nPattern, 0.0f, 0.0f); 
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxTranslate);

	// テクスチャマトリックスの設定
	XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTexture);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetDeathEffect(XMFLOAT3 pos, float fSizeX, float fSizeY)
{
	int nIdxSetHitEffect = -1;


	DEATHEFFECT* pHE = g_deatheffect;
	for (int i = 0; i < MAX_DEATHEFFECT; ++i, ++pHE) {
		if (!pHE->bUse) {
			pHE->pos = pos;
			pHE->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			pHE->nCounter = 0;
			pHE->nPattern = 0;
			pHE->fAlpha = 1.0f;
			pHE->bUse = true;

			// 頂点座標の設定
			SetVertexDeathEffect(i, fSizeX, fSizeY);

			nIdxSetHitEffect = i;
			break;
		}
	}

	return nIdxSetHitEffect;
}

void SetDeathEffectCamera(CCamera* pCamera)
{
	g_pCamera = pCamera;

}