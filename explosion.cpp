//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author : HIROHIKO HAMAYA
//
// ----------------------------------------------------------------------------
// 2020/12/23 DrawExplosion()の最初と最後にオフにしたりオンにする処理を追加。三宅
//
//=============================================================================
#include "explosion.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EXPLOSION		L"data/texture/EX2.png"// 読み込むテクスチャファイル名
#define	EXPLOSION_SIZE_X		(5.0f)							// ビルボードの幅
#define	EXPLOSION_SIZE_Y		(5.0f)							// ビルボードの高さ

#define	MAX_EXPLOSION			(128)							// ビルボード最大数

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(0.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct EXPLOSION {
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
HRESULT MakeVertexExplosion(ID3D11Device* pDevice);
void SetVertexExplosion(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorExplosion(int nIdxExplosion, XMFLOAT4 col);
void SetTextureExplosion(int nIdxExplosion, int nPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH				g_meshExplosion;
static MATERIAL			g_material;
static EXPLOSION		g_explosion[MAX_EXPLOSION];	// 爆発ワーク
static CPlayer*         g_pPlayer;
static CCamera*         g_pCamera;

static bool             g_bLoadMesh = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitExplosion(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	//ZeroMemory(&g_meshExplosion, sizeof(MESH));

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {

		//ZeroMemory(&g_meshExplosion, sizeof(MESH));

		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,				// デバイスへのポインタ
			TEXTURE_EXPLOSION,	// ファイルの名前
			&g_meshExplosion.pTexture);	// 読み込むメモリー

		SaveMesh(&g_meshExplosion);

		// 頂点情報の作成
		MakeVertexExplosion(pDevice);

		g_bLoadMesh = true;
	}
	XMStoreFloat4x4(&g_meshExplosion.mtxTexture, XMMatrixIdentity());

	

	// マテリアルの設定
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_meshExplosion.pMaterial = &g_material;

	EXPLOSION* pExp = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExp) {
		pExp->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pExp->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pExp->fSizeX = EXPLOSION_SIZE_X;
		pExp->fSizeY = EXPLOSION_SIZE_Y;
		pExp->nCounter = 0;
		pExp->nPattern = 0;
		pExp->fAlpha = 1.0f;
		pExp->bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitExplosion(void)
{
#if MESH_RELEASE_ON
		// メッシュの開放
		ReleaseMesh(&g_meshExplosion);
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateExplosion(void)
{
	EXPLOSION* pExp = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExp) 
{
		if (pExp->bUse) 
		{
			++pExp->nCounter;
			//pExp->nPattern++;

			
				if (pExp->nCounter % 2 == 0)
				{
					pExp->nPattern++;


				}
			
			
			if (pExp->nPattern >= 8)
			{
				pExp->bUse = false;
				continue;
			}


			// 頂点座標の設定
			pExp->fSizeX += 0.5f;
			pExp->fSizeY += 0.5f;

			SetVertexExplosion(i, pExp->fSizeX, pExp->fSizeY);

			if (pExp->nPattern >= 2)
			{
				// 頂点カラーの設定
				pExp->fAlpha -= 0.02f;

				/*if (pExp->nCounter % 10 == 0)
				{
					pExp->nPattern++;

				}*/

				if (pExp->fAlpha < 0.0f) 
				{
					pExp->fAlpha = 0.0f;
				}
				SetColorExplosion(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pExp->fAlpha));
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawExplosion(void)
{
	// ここでオフにする
	CLight::Get()->SetDisable();

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// ビューマトリックスを取得
	//XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	XMFLOAT4X4& mtxView = g_pCamera->GetViewMatrix();
	// 加算合成
	SetBlendState(/*BS_SUBTRACTION*/BS_ADDITIVE);

	EXPLOSION* pExp = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExp) {
		if (pExp->bUse) {
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&pExp->mtxWorld, mtxWorld);

			pExp->mtxWorld._11 = mtxView._11;
			pExp->mtxWorld._12 = mtxView._21;
			pExp->mtxWorld._13 = mtxView._31;
			pExp->mtxWorld._21 = mtxView._12;
			pExp->mtxWorld._22 = mtxView._22;
			pExp->mtxWorld._23 = mtxView._32;
			pExp->mtxWorld._31 = mtxView._13;
			pExp->mtxWorld._32 = mtxView._23;
			pExp->mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&pExp->mtxWorld);

			// スケールを反映
			mtxScale = XMMatrixScaling(pExp->fSizeX, pExp->fSizeY, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(pExp->pos.x, pExp->pos.y, pExp->pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			XMStoreFloat4x4(&pExp->mtxWorld, mtxWorld);
			g_meshExplosion.mtxWorld = pExp->mtxWorld;

			// テクスチャ座標の設定
			SetTextureExplosion(i, pExp->nPattern);

			// カラーの設定
			g_material.Diffuse = pExp->col;

			// メッシュの描画
			DrawMesh(pDeviceContext, &g_meshExplosion);
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
HRESULT MakeVertexExplosion(ID3D11Device* pDevice)
{
	// オブジェクトの頂点配列を生成
	g_meshExplosion.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshExplosion.nNumVertex];

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
	g_meshExplosion.nNumIndex = 4;
	int* pIndexWk = new int[g_meshExplosion.nNumIndex];

	// インデックス配列の中身を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// 頂点バッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_meshExplosion, pVertexWk, pIndexWk);

	// 一時配列の解放
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexExplosion(int nIdxExplosion, float fSizeX, float fSizeY)
{
	if (nIdxExplosion >= 0 && nIdxExplosion < MAX_EXPLOSION) {
		EXPLOSION* pExp = &g_explosion[nIdxExplosion];
		pExp->fSizeX = fSizeX;
		pExp->fSizeY = fSizeY;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorExplosion(int nIdxExplosion, XMFLOAT4 col)
{
	if (nIdxExplosion >= 0 && nIdxExplosion < MAX_EXPLOSION) {
		g_explosion[nIdxExplosion].col = col;
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureExplosion(int nIdxExplosion, int nPattern)
{
	XMMATRIX mtxTexture, mtxScale, mtxTranslate;

	// テクスチャマトリックスの初期化
	mtxTexture = XMMatrixIdentity();

	// スケールを反映
	mtxScale = XMMatrixScaling(0.125f, 1.0f, 1.0f);
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxScale);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(0.125f * nPattern, 0.0f, 0.0f);
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxTranslate);

	// テクスチャマトリックスの設定
	XMStoreFloat4x4(&g_meshExplosion.mtxTexture, mtxTexture);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetExplosion(XMFLOAT3 pos, float fSizeX, float fSizeY)
{
	int nIdxExplosion = -1;


	EXPLOSION* pExp = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExp) {
		if (!pExp->bUse) {
			pExp->pos = pos;
			pExp->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			pExp->nCounter = 0;
			pExp->nPattern = 0;
			pExp->fAlpha = 1.0f;
			pExp->bUse = true;

			// 頂点座標の設定
			SetVertexExplosion(i, fSizeX, fSizeY);

			nIdxExplosion = i;
			break;
		}
	}

	return nIdxExplosion;
}

//void SetExplosionPlayer(CPlayer* pPlayer)
//{
//	g_pPlayer = pPlayer;
//
//
//
//}

void SetExplosionCamera(CCamera* pCamera)
{


	g_pCamera = pCamera;


}