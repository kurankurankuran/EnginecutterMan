//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "particle.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "scene.h"
//#include "shadow.h"
//#include "model.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PARTICLE	L"data/texture/effect000.jpg"	// 読み込むテクスチャファイル名

#define	PARTICLE_SIZE_X		(10.0f)							// ビルボードの幅
#define	PARTICLE_SIZE_Y		(10.0f)							// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE	(1.0f)							// 移動速度

#define	MAX_PARTICLE		(100)							// エフェクト最大数

//#define	DISP_SHADOW			// 影の表示
#undef DISP_SHADOW

#define MATERIAL_DIFFUSE	XMFLOAT4(1,1,1,1)
#define MATERIAL_AMBIENT	XMFLOAT4(1,1,1,1)
#define MATERIAL_SPECULAR	XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE	XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER		(0.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	XMFLOAT3 pos;			// 位置
	XMFLOAT3 rot;			// 回転
	XMFLOAT3 move;			// 移動量
	XMFLOAT4 col;			// 色
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nIdxShadow;			// 影ID
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか
} PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle(ID3D11Device* pDevice);
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY);
void SetColorParticle(int nIdxParticle, XMFLOAT4 col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH				g_meshParticle;					// メッシュ情報
static MATERIAL			g_material;						// マテリアル

static PARTICLE			g_aParticle[MAX_PARTICLE];		// パーティクルワーク
static XMFLOAT3			g_posBase;						// ビルボード発生位置
static float			g_fWidthBase = 5.0f;			// 基準の幅
static float			g_fHeightBase = 10.0f;			// 基準の高さ
static bool				g_bPause = false;				// ポーズON/OFF
static int              g_nScene;

static CPlayer*         g_pPlayer;
static CRushPlayer*     g_pRushPlayer;
static CCamera*         g_pCamera;

static XMMATRIX         g_RotNormtx;
XMVECTOR                g_vParticleRot;

static bool             g_bLoadMesh = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(void)
{
	g_nScene = GetScene();

	g_RotNormtx = XMMatrixIdentity();
	g_vParticleRot = XMVectorSet(0.0f, 0.0f, -70.0f, 1.0f);

	ID3D11Device* pDevice = GetDevice();

	//// 頂点情報の作成
	//MakeVertexParticle(pDevice);

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		// テクスチャの読み込み
		CreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_PARTICLE,			// ファイルの名前
			&g_meshParticle.pTexture);	// 読み込むメモリー

		SaveMesh(&g_meshParticle);

		// 頂点情報の作成
		MakeVertexParticle(pDevice);

		g_bLoadMesh = true;
	}
	XMStoreFloat4x4(&g_meshParticle.mtxTexture, XMMatrixIdentity());

	// マテリアルの設定
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_meshParticle.pMaterial = &g_material;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		g_aParticle[nCntParticle].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fSizeX = PARTICLE_SIZE_X;
		g_aParticle[nCntParticle].fSizeY = PARTICLE_SIZE_Y;
		g_aParticle[nCntParticle].nIdxShadow = -1;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
#ifdef DISP_SHADOW
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (g_aParticle[nCntParticle].bUse) {
			// 影削除
			ReleaseShadow(g_aParticle[nCntParticle].nIdxShadow);
			g_aParticle[nCntParticle].nIdxShadow = -1;
			g_aParticle[nCntParticle].bUse = false;
		}
	}
#endif
#if MESH_RELEASE_ON
		// メッシュの開放
		ReleaseMesh(&g_meshParticle);
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	//XMFLOAT3 rotCamera;

	// カメラの回転を取得
	//rotCamera = CCamera::Get()->GetAngle();

	
	if (!g_bPause) {
		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
			if (g_aParticle[nCntParticle].bUse) {
				// 使用中

				g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
				g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
				
				g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z/2;
				if (g_aParticle[nCntParticle].pos.z <= g_aParticle[nCntParticle].fSizeY/* 実はZ *// 2) {
					// 着地した
					g_aParticle[nCntParticle].nLife = 0;
				}

				g_aParticle[nCntParticle].move.x += (0.0f - g_aParticle[nCntParticle].move.x) * 0.015f;
				g_aParticle[nCntParticle].move.z -= 0.25f;
				g_aParticle[nCntParticle].move.y += (0.0f - g_aParticle[nCntParticle].move.y) * 0.015f;

#ifdef DISP_SHADOW
				float colA;
				if (g_aParticle[nCntParticle].nIdxShadow != -1) {
					// 影使用中
					//float colA;

					// 影の位置設定
					MoveShadow(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT3(g_aParticle[nCntParticle].pos.x, 0.1f, g_aParticle[nCntParticle].pos.z));

					// 影のα値設定
					if (g_aParticle[nCntParticle].col.w > 0.0f) {
						colA = (300.0f - (g_aParticle[nCntParticle].pos.y - 9.0f)) / (300.0f / g_aParticle[nCntParticle].col.w);
						if (colA < 0.0f) {
							colA = 0.0f;
						}
					} else {
						colA = 0.0f;
					}

					// 影の色の設定
					SetShadowColor(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT4(0.15f, 0.15f, 0.15f, colA));
				}
#endif
				g_aParticle[nCntParticle].nLife--;
				if (g_aParticle[nCntParticle].nLife <= 0) {
					g_aParticle[nCntParticle].bUse = false;
					//ReleaseShadow(g_aParticle[nCntParticle].nIdxShadow);
					g_aParticle[nCntParticle].nIdxShadow = -1;
				} else {
					if (g_aParticle[nCntParticle].nLife <= 80) {
						g_aParticle[nCntParticle].col.x = 0.60f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.06f + 0.2f;
						g_aParticle[nCntParticle].col.y = 0.70f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
						g_aParticle[nCntParticle].col.z = 0.05f;
					}

					if (g_aParticle[nCntParticle].nLife <= 20) {
						// α値設定
						g_aParticle[nCntParticle].col.w -= 0.05f;
						if (g_aParticle[nCntParticle].col.w < 0.0f) {
							g_aParticle[nCntParticle].col.w = 0.0f;
						}
					}

					// 色の設定
					SetColorParticle(nCntParticle, g_aParticle[nCntParticle].col);
				}
			}
		}

		// パーティクル発生
//		if((rand() % 2) == 0)
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;



			/*XMFLOAT3 ModelPos = GetModelPos();
			pos = ModelPos;*/

			/*XMFLOAT3 ModelPos = g_pPlayer->GetPosModel();
			pos = ModelPos;*/



			fAngle = rand() % 360 - 180.0f;
			fLength = rand() % (int)(g_fWidthBase * 100) / 100.0f - g_fWidthBase;
			//			move.x = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;
			move.x = SinDeg(fAngle) * fLength;
			move.y = rand() % 300 / 100.0f + g_fHeightBase;
			//			move.z = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;
			move.z = CosDeg(fAngle) * fLength;

			XMVECTOR vParticleRot;
			switch (g_nScene)
			{
			case SCENE_GAME:
				pos.x = g_pPlayer->GetPosCut().x;
				pos.y = g_pPlayer->GetPosCut().y;
				pos.z = g_pPlayer->GetPosCut().z;

				vParticleRot = XMVectorSet(move.x, move.y, move.z, 1.0f);
				vParticleRot = XMVector3TransformNormal(vParticleRot, XMLoadFloat4x4(&g_pPlayer->GetTotateWorldMatrix()));
				
				XMStoreFloat3(&move, vParticleRot);
				//***************************************************************************

				nLife = rand() % 100 + 150;

				fSize = (float)(rand() % 30 + 20);

				if (g_pPlayer->GetBreakflg()) {
					// ビルボードの設定
					SetParticle(pos, move, XMFLOAT4(0.8f, 0.7f, 0.2f, 0.85f), fSize, fSize, nLife);
				}
				break;

			case SCENE_RUSH:
				pos.x = g_pRushPlayer->GetPosCut().x;
				pos.y = g_pRushPlayer->GetPosCut().y;
				pos.z = g_pRushPlayer->GetPosCut().z;

				vParticleRot = XMVectorSet(move.x, move.y, move.z, 1.0f);
				vParticleRot = XMVector3TransformNormal(vParticleRot, XMLoadFloat4x4(&g_pRushPlayer->GetTotateWorldMatrix()));

				XMStoreFloat3(&move, vParticleRot);
				//***************************************************************************

				nLife = rand() % 100 + 150;

				fSize = (float)(rand() % 30 + 20);

				if (g_pRushPlayer->GetBreakflg()) {
					// ビルボードの設定
					SetParticle(pos, move, XMFLOAT4(0.8f, 0.7f, 0.2f, 0.85f), fSize, fSize, nLife);
				}
				break;
			}

			
}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	// ここでオフにする
	CLight::Get()->SetDisable();

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// 加算合成
	SetBlendState(BS_ADDITIVE);

	// ビューマトリックスを取得
	/*XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();*/
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix(); 

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (g_aParticle[nCntParticle].bUse) {
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&g_meshParticle.mtxWorld, mtxWorld);

			g_meshParticle.mtxWorld._11 = mtxView._11;
			g_meshParticle.mtxWorld._12 = mtxView._21;
			g_meshParticle.mtxWorld._13 = mtxView._31;
			g_meshParticle.mtxWorld._21 = mtxView._12;
			g_meshParticle.mtxWorld._22 = mtxView._22;
			g_meshParticle.mtxWorld._23 = mtxView._32;
			g_meshParticle.mtxWorld._31 = mtxView._13;
			g_meshParticle.mtxWorld._32 = mtxView._23;
			g_meshParticle.mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&g_meshParticle.mtxWorld);

			// スケールを反映
			mtxScale = XMMatrixScaling(g_aParticle[nCntParticle].fSizeX/3, g_aParticle[nCntParticle].fSizeY/3, 5.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			////　プレイヤーの回転を反映
			//mtxWorld = XMMatrixMultiply(mtxWorld, g_pPlayer->GetRotateNorMatrix());
			//mtxWorld = XMMatrixMultiply(mtxWorld, g_pPlayer->GetTotateWorldMatrix());

			// ワールドマトリックスの設定
			XMStoreFloat4x4(&g_meshParticle.mtxWorld, mtxWorld);

			// カラーの設定
			g_material.Diffuse = g_aParticle[nCntParticle].col;

			// メッシュの描画
			DrawMesh(pDeviceContext, &g_meshParticle);
		}
	}

	// 半透明合成無
	SetBlendState(BS_NONE);

	// ここでオンにする
	CLight::Get()->SetEnable();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticle(ID3D11Device* pDevice)
{
	// オブジェクトの頂点配列を生成
	g_meshParticle.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshParticle.nNumVertex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	// 頂点座標の設定
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[1].vtx = XMFLOAT3( 1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[2].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2, -1.0f / 2, 0.0f);

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
	pVtx[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 1.0f);

	// インデックス配列を生成
	g_meshParticle.nNumIndex = 4;
	int* pIndexWk = new int[g_meshParticle.nNumIndex];

	// インデックス配列の中身を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// 頂点バッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_meshParticle, pVertexWk, pIndexWk);

	// 一時配列の解放
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY)
{
	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_aParticle[nIdxParticle].fSizeX = fSizeX;
		g_aParticle[nIdxParticle].fSizeY = fSizeY;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorParticle(int nIdxParticle, XMFLOAT4 col)
{
	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_aParticle[nIdxParticle].col = col;
	}
}

//=============================================================================
// エフェクトの設定
//=============================================================================
int SetParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (!g_aParticle[nCntParticle].bUse) {
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;

			// 頂点座標の設定
			SetVertexParticle(nCntParticle, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorParticle(nCntParticle, col);

			nIdxParticle = nCntParticle;

#ifdef DISP_SHADOW
			// 影の設定
			g_aParticle[nCntParticle].nIdxShadow = CreateShadow(pos, fSizeX);
			if (g_aParticle[nCntParticle].nIdxShadow != -1) {
				SetShadowColor(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxParticle;
}

//パーティクル発生呼び出し
int SpawnParticle(XMFLOAT3 pos, XMFLOAT3 dir)
{
	//XMFLOAT3 ModelPos = GetModelPos();
	XMFLOAT3 ModelPos;

	switch (g_nScene) {
	case SCENE_GAME:
		ModelPos = g_pPlayer->GetPosModel();
		break;

	case SCENE_RUSH:
		ModelPos = g_pRushPlayer->GetPosModel();
		break;
	}
	
	PARTICLE* pParticle = g_aParticle;
	for (int i = 0; i < MAX_PARTICLE; ++i, ++pParticle) {
		if (pParticle->bUse) continue;

		pos = ModelPos;

		pParticle->pos = pos;

		//pParticle->nIdxShadow = CreateShadow(pos, PARTICLE_SIZE_X);
		pParticle->bUse = 1;
		return i;
	}
	return -1;
}

void SetParticlePlayer(CPlayer* pPlayer)
{
	g_pPlayer = pPlayer;
}

void SetParticleRushPlayer(CRushPlayer* pPlayer)
{
	g_pRushPlayer = pPlayer;
}

void SetParticleCamera(CCamera* pCamera)
{
	g_pCamera = pCamera;
}