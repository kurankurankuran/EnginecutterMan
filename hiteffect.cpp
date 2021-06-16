//=============================================================================
//
// ヒットエフェクト処理 [hiteffect.cpp]
// Author : 三宅　秀太郎
// ----------------------------------------------------------------------------
// 2020/12/23 DrawHitEffect()の最初と最後にオフにしたりオンにする処理を追加。三宅
//
//=============================================================================
#include "hiteffect.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "light.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EXPLOSION		L"data/texture/pipo-btleffect003.png"// 読み込むテクスチャファイル名
#define	HITEFFECT_SIZE_X		(5.0f)							// ビルボードの幅
#define	HITEFFECT_SIZE_Y		(5.0f)							// ビルボードの高さ

#define	MAX_HITEFFECT			(128)							// ビルボード最大数

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(0.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct HITEFFECT {
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
HRESULT MakeVertexHitEffect(ID3D11Device* pDevice);
void SetVertexHitEffect(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorHitEffect(int nIdxHitEffect, XMFLOAT4 col);
void SetTextureHitEffect(int nIdxHitEffect, int nPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH				g_mesh;
static MATERIAL			g_material;
static HITEFFECT		g_hiteffect[MAX_HITEFFECT];	// 爆発ワーク
static CPlayer*         g_pPlayer;
static CCamera*         g_pCamera;
static bool             g_bLoadMesh = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHitEffect(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	/*ZeroMemory(&g_mesh, sizeof(MESH));
*/
	//// 頂点情報の作成
	//MakeVertexHitEffect(pDevice);

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {

		//ZeroMemory(&g_mesh, sizeof(MESH));

		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,				// デバイスへのポインタ
			TEXTURE_EXPLOSION,	// ファイルの名前
			&g_mesh.pTexture);	// 読み込むメモリー

		// テクスチャ―の保存
		SaveMesh(&g_mesh);

		// 頂点情報の作成
		MakeVertexHitEffect(pDevice);

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

	HITEFFECT* pHE = g_hiteffect;
	for (int i = 0; i < MAX_HITEFFECT; ++i, ++pHE) {
		pHE->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pHE->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pHE->fSizeX = HITEFFECT_SIZE_X;
		pHE->fSizeY = HITEFFECT_SIZE_Y;
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
void UninitHitEffect(void)
{
#if MESH_RELEASE_ON
		// メッシュの開放
		ReleaseMesh(&g_mesh);
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHitEffect(void)
{
	HITEFFECT* pHE = g_hiteffect;
	for (int i = 0; i < MAX_HITEFFECT; ++i, ++pHE)
{
		if (pHE->bUse)
		{
			++pHE->nCounter;
			
			// ヒットエフェクトの速度設定
			if (pHE->nCounter % 2 == 0)
			{
				pHE->nPattern++;
			}
			
			// 画像の個数分出す
			if (pHE->nPattern >= 5)
			{
				pHE->bUse = false;
				continue;
			}


			// 頂点座標の設定
			pHE->fSizeX += 0.5f;
			pHE->fSizeY += 0.5f;

			SetVertexHitEffect(i, pHE->fSizeX, pHE->fSizeY);

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
				SetColorHitEffect(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pHE->fAlpha));
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHitEffect(void)
{
	// ここでオフにする
	CLight::Get()->SetDisable();

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// ビューマトリックスを取得
	//XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	// 加算合成
	SetBlendState(BS_ADDITIVE);

	HITEFFECT* pHE = g_hiteffect;
	for (int i = 0; i < MAX_HITEFFECT; ++i, ++pHE) {
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
			SetTextureHitEffect(i, pHE->nPattern);

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
HRESULT MakeVertexHitEffect(ID3D11Device* pDevice)
{
	// オブジェクトの頂点配列を生成
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	// 頂点座標の設定
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[1].vtx = XMFLOAT3(-1.0f / 2, 1.0f / 2, 0.0f);
	pVtx[2].vtx = XMFLOAT3(1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[3].vtx = XMFLOAT3(1.0f / 2, 1.0f / 2, 0.0f);

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
void SetVertexHitEffect(int nIdxHitEffect, float fSizeX, float fSizeY)
{
	if (nIdxHitEffect >= 0 && nIdxHitEffect < MAX_HITEFFECT) {
		HITEFFECT* pExp = &g_hiteffect[nIdxHitEffect];
		pExp->fSizeX = fSizeX;
		pExp->fSizeY = fSizeY;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorHitEffect(int nIdxHitEffect, XMFLOAT4 col)
{
	if (nIdxHitEffect >= 0 && nIdxHitEffect < MAX_HITEFFECT) {
		g_hiteffect[nIdxHitEffect].col = col;
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureHitEffect(int nIdxHitEffect, int nPattern)
{
	XMMATRIX mtxTexture, mtxScale, mtxTranslate;

	// テクスチャマトリックスの初期化
	mtxTexture = XMMatrixIdentity();

	// スケールを反映
	mtxScale = XMMatrixScaling(0.2f, 1.0f, 1.0f);		  // Xは1÷分割数
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxScale);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(0.2f * nPattern, 0.0f, 0.0f); 
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxTranslate);

	// テクスチャマトリックスの設定
	XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTexture);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetHitEffect(XMFLOAT3 pos, float fSizeX, float fSizeY)
{
	int nIdxSetHitEffect = -1;


	HITEFFECT* pHE = g_hiteffect;
	for (int i = 0; i < MAX_HITEFFECT; ++i, ++pHE) {
		if (!pHE->bUse) {
			pHE->pos = pos;
			pHE->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			pHE->nCounter = 0;
			pHE->nPattern = 0;
			pHE->fAlpha = 1.0f;
			pHE->bUse = true;

			// 頂点座標の設定
			SetVertexHitEffect(i, fSizeX, fSizeY);

			nIdxSetHitEffect = i;
			break;
		}
	}

	return nIdxSetHitEffect;
}

void SetHitEffectCamera(CCamera* pCamera)
{
	g_pCamera = pCamera;


}