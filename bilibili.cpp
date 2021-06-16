//=============================================================================
//
// 爆発処理 [bilibili.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "bilibili.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include <stdio.h>
#include <time.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BILIBILI		L"data/texture/bilibili.png"// 読み込むテクスチャファイル名
#define	BILIBILI_SIZE_X		(5.0f)							// ビルボードの幅
#define	BILIBILI_SIZE_Y		(5.0f)							// ビルボードの高さ

#define	MAX_BILIBILI		(128)							// ビルボード最大数

#define MAX_BILIBILI_LIFE		(600)

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(0.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct BILIBILI {
	XMFLOAT4X4	mtxWorld;	// ワールドマトリックス
	XMFLOAT3	pos;		// 位置
	XMFLOAT4	col;		// 色
	float		fSizeX;		// 幅
	float		fSizeY;		// 高さ
	int			nCounter;	// カウンター
	int			nPattern;	// パターンNo.
	float		fAlpha;		// アルファ値
	int			nLife;		// 発生時間
	bool		bUse;		// 使用しているかどうか
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBilibili(ID3D11Device* pDevice);
void SetVertexBilibili(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorBilibili(int nIdxBilibili, XMFLOAT4 col);
void SetTextureBilibili(int nIdxBilibili, int nPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH				g_mesh;
static MATERIAL			g_material;
static BILIBILI		g_bilibili[MAX_BILIBILI];	// 爆発ワーク
static CPlayer*         g_pPlayer;
static CCamera*         g_pCamera;
static bool             g_bLoadMesh = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBilibili(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		ZeroMemory(&g_mesh, sizeof(MESH));

		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,				// デバイスへのポインタ
			TEXTURE_BILIBILI,	// ファイルの名前
			&g_mesh.pTexture);	// 読み込むメモリー
		// テクスチャ―の保存
		SaveMesh(&g_mesh);
		// 頂点情報の作成
		MakeVertexBilibili(pDevice);

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

	srand((unsigned int)time(NULL));

	BILIBILI* pBili = g_bilibili;
	for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili) {
		pBili->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pBili->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pBili->fSizeX = BILIBILI_SIZE_X;
		pBili->fSizeY = BILIBILI_SIZE_Y;
		pBili->nCounter = 0;
		pBili->nPattern = 0;/*((rand() % 8) == 1);*/
		pBili->fAlpha = 1.0f;
		pBili->nLife = 0/*60*MAX_BILIBILI_LIFE*/;
		pBili->bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBilibili(void)
{
#if MESH_RELEASE_ON
		// メッシュの開放
		ReleaseMesh(&g_mesh);
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBilibili(void)
{
	//BILIBILI* pBili = g_bilibili;
	//for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili)
	//{
	//	if (pBili->bUse)
	//	{
	//		++pBili->nCounter;
	//		//pBili->nPattern++;

	//		//発生間隔
	//		if (pBili->nCounter % 30 == 0)
	//		{
	//			pBili->nPattern++;
	//		}


	//		/*if (pBili->nPattern >= 8)
	//		{
	//			pBili->bUse = false;
	//			continue;
	//		}*/


	//		// 頂点座標の設定
	//		pBili->fSizeX += 0.5f;
	//		pBili->fSizeY += 0.5f;

	//		SetVertexBilibili(i, pBili->fSizeX, pBili->fSizeY);

	//		if (pBili->nPattern >= 2)
	//		{
	//			// 頂点カラーの設定
	//			pBili->fAlpha -= 0.02f;

	//			if (pBili->nCounter % 30 == 0)
	//			{
	//				pBili->nPattern++;
	//				pBili->nPattern = (rand() % 9);
	//				if (pBili->nPattern == 8)
	//				{
	//					pBili->bUse = false;
	//				}
	//				
	//				/*if (pBili->nLife > 0)
	//				{
	//					if (pBili->nPattern > 8)
	//					{
	//						pBili->nPattern == 0;
	//					}
	//				}
	//				else
	//				{
	//					pBili->bUse = false;
	//				}
	//				pBili->nLife -= 1;*/
	//			}

	//			if (pBili->fAlpha < 0.0f)
	//			{
	//				pBili->fAlpha = 0.0f;
	//			}
	//			SetColorBilibili(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pBili->fAlpha));
	//		}
	//	}
	//	/*pBili->nLife -= 1;
	//	if (pBili->nLife > 0)
	//	{
	//		if (pBili->nPattern > 8)
	//		{
	//			pBili->nPattern == 0;
	//		}
	//	}
	//	else
	//	{
	//		pBili->bUse = false;
	//	}*/
	//}
	BILIBILI* pBili = g_bilibili;
	for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili)
	{
		if (pBili->bUse)
		{
			++pBili->nCounter;
			//pBili->nPattern++;
			pBili->nLife++;

			////発生間隔
			if (pBili->nCounter % 30 == 0)
			{
				pBili->nPattern++;
				pBili->nPattern = (rand() % 9);

			}
			//                   
			if (pBili->nLife >= MAX_BILIBILI_LIFE)
			{

				pBili->bUse = false;
				pBili->nLife = 0;
				continue;

			}

			///*if (pBili->nPattern >= 8)
			//{
				//pBili->bUse = false;
			continue;
			//}*/


			// 頂点座標の設定
			pBili->fSizeX += 0.5f;
			pBili->fSizeY += 0.5f;

			SetVertexBilibili(i, pBili->fSizeX, pBili->fSizeY);

			if (pBili->nPattern >= 2)
			{
				// 頂点カラーの設定
				//pBili->fAlpha -= 0.02f;

				/*if (pBili->nCounter % 30 == 0)
				{
					pBili->nPattern++;
					pBili->nPattern = (rand() % 9);
					if (pBili->nPattern == 8)
					{
						pBili->bUse = false;
					}*/

					/*if (pBili->nLife > 0)
					{
						if (pBili->nPattern > 8)
						{
							pBili->nPattern == 0;
						}
					}
					else
					{
						pBili->bUse = false;
					}
					pBili->nLife -= 1;*/
					//}

					/*if (pBili->fAlpha < 0.0f)
					{
						pBili->fAlpha = 0.0f;
					}*/
				SetColorBilibili(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pBili->fAlpha));
			}
		}
		/*pBili->nLife -= 1;
		if (pBili->nLife > 0)
		{
			if (pBili->nPattern > 8)
			{
				pBili->nPattern == 0;
			}
		}
		else
		{
			pBili->bUse = false;
		}*/
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBilibili(void)
{
	//ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	//XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	//// ビューマトリックスを取得
	////XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	//XMFLOAT4X4& mtxView = g_pCamera->GetViewMatrix();
	//// 加算合成
	//SetBlendState(BS_ADDITIVE);

	//BILIBILI* pBili = g_bilibili;
	//for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili) {
	//	if (pBili->bUse) {
	//		// ワールドマトリックスの初期化
	//		mtxWorld = XMMatrixIdentity();
	//		XMStoreFloat4x4(&pBili->mtxWorld, mtxWorld);

	//		pBili->mtxWorld._11 = mtxView._11;
	//		pBili->mtxWorld._12 = mtxView._21;
	//		pBili->mtxWorld._13 = mtxView._31;
	//		pBili->mtxWorld._21 = mtxView._12;
	//		pBili->mtxWorld._22 = mtxView._22;
	//		pBili->mtxWorld._23 = mtxView._32;
	//		pBili->mtxWorld._31 = mtxView._13;
	//		pBili->mtxWorld._32 = mtxView._23;
	//		pBili->mtxWorld._33 = mtxView._33;

	//		mtxWorld = XMLoadFloat4x4(&pBili->mtxWorld);

	//		// スケールを反映
	//		mtxScale = XMMatrixScaling(pBili->fSizeX, pBili->fSizeY, 1.0f);
	//		mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

	//		// 移動を反映
	//		mtxTranslate = XMMatrixTranslation(pBili->pos.x, pBili->pos.y, pBili->pos.z);
	//		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//		// ワールドマトリックスの設定
	//		XMStoreFloat4x4(&pBili->mtxWorld, mtxWorld);
	//		g_mesh.mtxWorld = pBili->mtxWorld;

	//		// テクスチャ座標の設定
	//		SetTextureBilibili(i, pBili->nPattern);

	//		// カラーの設定
	//		g_material.Diffuse = pBili->col;

	//		// メッシュの描画
	//		DrawMesh(pDeviceContext, &g_mesh);
	//	}
	//}

	//// 半透明処理無効
	//SetBlendState(BS_NONE);

	// ここでオフにする
	CLight::Get()->SetDisable();

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// ビューマトリックスを取得
	//XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	XMFLOAT4X4& mtxView = g_pCamera->GetViewMatrix();
	// 加算合成
	SetBlendState(BS_ADDITIVE);

	BILIBILI* pBili = g_bilibili;
	for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili) {
		if (pBili->bUse) {
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&pBili->mtxWorld, mtxWorld);

			pBili->mtxWorld._11 = mtxView._11;
			pBili->mtxWorld._12 = mtxView._21;
			pBili->mtxWorld._13 = mtxView._31;
			pBili->mtxWorld._21 = mtxView._12;
			pBili->mtxWorld._22 = mtxView._22;
			pBili->mtxWorld._23 = mtxView._32;
			pBili->mtxWorld._31 = mtxView._13;
			pBili->mtxWorld._32 = mtxView._23;
			pBili->mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&pBili->mtxWorld);

			// スケールを反映
			mtxScale = XMMatrixScaling(pBili->fSizeX, pBili->fSizeY, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(pBili->pos.x, pBili->pos.y, pBili->pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			XMStoreFloat4x4(&pBili->mtxWorld, mtxWorld);
			g_mesh.mtxWorld = pBili->mtxWorld;

			// テクスチャ座標の設定
			SetTextureBilibili(i, pBili->nPattern);

			// カラーの設定
			g_material.Diffuse = pBili->col;

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
HRESULT MakeVertexBilibili(ID3D11Device* pDevice)
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
void SetVertexBilibili(int nIdxBilibili, float fSizeX, float fSizeY)
{
	if (nIdxBilibili >= 0 && nIdxBilibili < MAX_BILIBILI) {
		BILIBILI* pBili = &g_bilibili[nIdxBilibili];
		pBili->fSizeX = fSizeX;
		pBili->fSizeY = fSizeY;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorBilibili(int nIdxBilibili, XMFLOAT4 col)
{
	if (nIdxBilibili >= 0 && nIdxBilibili < MAX_BILIBILI) {
		g_bilibili[nIdxBilibili].col = col;
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureBilibili(int nIdxBilibili, int nPattern)
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
	XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTexture);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetBilibili(XMFLOAT3 pos, float fSizeX, float fSizeY)
{
	int nIdxBilibili = -1;


	BILIBILI* pBili = g_bilibili;
	for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili) {
		if (!pBili->bUse) {
			pBili->pos = pos;
			pBili->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			pBili->nCounter = 0;
			pBili->nPattern = 0;/*((rand() % 8) == 1);*/
			pBili->nLife = 0;
			pBili->fAlpha = 1.0f;
			pBili->bUse = true;

			// 頂点座標の設定
			SetVertexBilibili(i, fSizeX, fSizeY);

			nIdxBilibili = i;
			break;
		}
	}

	return nIdxBilibili;
}

//void SetBilibiliPlayer(CPlayer* pPlayer)
//{
//	g_pPlayer = pPlayer;
//
//
//
//}

void SetBilibiliCamera(CCamera* pCamera)
{


	g_pCamera = pCamera;


}