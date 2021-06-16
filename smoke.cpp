//=============================================================================
//
// スモーク処理 [smoke.cpp]
// Author : 釜本　麟太郎
//
//=============================================================================
//～更新履歴～
// 12/08  スモークの発生場所を決めて一定時間出させるように
//=============================================================================
#include "smoke.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
//#include "camera.h"
//#include "light.h"
//#include "shadow.h"
//#include "model.h"
//#include "enemyParts.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SMOKE	L"data/texture/smoke.png"	// 読み込むテクスチャファイル名

#define	SMOKE_SIZE_X		(500.0f)					// ビルボードの幅
#define	SMOKE_SIZE_Y		(500.0f)					// ビルボードの高さ
#define	VALUE_MOVE_SMOKE	(1.0f)						// 移動速度

#define SMOKE_TIMER			(1)							// スモーク発生させる時間

//#define	DISP_SHADOW			// 影の表示
#undef DISP_SHADOW

#define MATERIAL_DIFFUSE	XMFLOAT4(1,1,1,1)
#define MATERIAL_AMBIENT	XMFLOAT4(1,1,1,1)
#define MATERIAL_SPECULAR	XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE	XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER		(0.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSmoke(ID3D11Device* pDevice);
//void SetVertexSmoke(int nIdxSmoke, float fSizeX, float fSizeY);
//void SetColorSmoke(int nIdxSmoke, XMFLOAT4 col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH				g_meshSmoke;					// メッシュ情報
static MATERIAL			g_material;						// マテリアル
static bool             g_bLoadMesh = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSmoke::InitSmoke(void)
{
	/*g_RotNormtx = XMMatrixIdentity();
	g_vSmokeRot = XMVectorSet(0.0f, 0.0f, -70.0f, 1.0f);*/

	m_posBase = XMFLOAT3(0.0f, 0.0f, 400.0f);
	m_nTimer = SMOKE_TIMER * 60;
	m_bGenerate = false;

	ID3D11Device* pDevice = GetDevice();

	

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		// テクスチャの読み込み
		CreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_SMOKE,			// ファイルの名前
			&g_meshSmoke.pTexture);	// 読み込むメモリー
		/*g_bLoadMesh = true;*/

		// 頂点情報の作成
		MakeVertexSmoke(pDevice);

		SaveMesh(&g_meshSmoke);

		

		g_bLoadMesh = true;
	}
	

	XMStoreFloat4x4(&g_meshSmoke.mtxTexture, XMMatrixIdentity());

	// マテリアルの設定
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_meshSmoke.pMaterial = &g_material;

	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; ++nCntSmoke) {
		m_aSmoke[nCntSmoke].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_aSmoke[nCntSmoke].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_aSmoke[nCntSmoke].move = XMFLOAT3(1.0f, 1.0f, 1.0f);
		m_aSmoke[nCntSmoke].fSizeX = SMOKE_SIZE_X;
		m_aSmoke[nCntSmoke].fSizeY = SMOKE_SIZE_Y;
		m_aSmoke[nCntSmoke].nIdxShadow = -1;
		m_aSmoke[nCntSmoke].nLife = 10;
		m_aSmoke[nCntSmoke].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSmoke::UninitSmoke(void)
{
#ifdef DISP_SHADOW
	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; ++nCntSmoke) {
		if (m_aSmoke[nCntSmoke].bUse) {
			// 影削除
			ReleaseShadow(m_aSmoke[nCntSmoke].nIdxShadow);
			m_aSmoke[nCntSmoke].nIdxShadow = -1;
			m_aSmoke[nCntSmoke].bUse = false;
		}
	}
#endif
#if MESH_RELEASE_ON
		// メッシュの開放
		ReleaseMesh(&g_meshSmoke);
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void CSmoke::UpdateSmoke(void)
{
	//XMFLOAT3 rotCamera;

	// カメラの回転を取得
	//rotCamera = CCamera::Get()->GetAngle();

	
	if (!m_bPause) {


		for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; ++nCntSmoke) {
			if (m_aSmoke[nCntSmoke].bUse) {
				// 使用中

				
				m_aSmoke[nCntSmoke].pos.x += m_aSmoke[nCntSmoke].move.x;
				m_aSmoke[nCntSmoke].pos.y += m_aSmoke[nCntSmoke].move.y;
				
				m_aSmoke[nCntSmoke].pos.z += m_aSmoke[nCntSmoke].move.z/2;
				if (m_aSmoke[nCntSmoke].pos.z <= m_aSmoke[nCntSmoke].fSizeY/* 実はZ *// 2) {
					// 着地した
					/*m_aSmoke[nCntSmoke].pos.y = m_aSmoke[nCntSmoke].fSizeY / 2;
					m_aSmoke[nCntSmoke].move.y = -m_aSmoke[nCntSmoke].move.y * 0.75f;*/
					m_aSmoke[nCntSmoke].nLife = 0;
				}

				/*if (m_aSmoke[nCntSmoke].pos.y > 1)
				{
					m_aSmoke[nCntSmoke].nLife = 0;
				}*/
			
				/*m_aSmoke[nCntSmoke].move.x += (0.0f - m_aSmoke[nCntSmoke].move.x) * 0.015f;
				m_aSmoke[nCntSmoke].move.z -= 0.25f;
				m_aSmoke[nCntSmoke].move.y += (0.0f - m_aSmoke[nCntSmoke].move.y) * 0.015f;*/



#ifdef DISP_SHADOW
				float colA;
				if (m_aSmoke[nCntSmoke].nIdxShadow != -1) {
					// 影使用中
					//float colA;

					// 影の位置設定
					MoveShadow(m_aSmoke[nCntSmoke].nIdxShadow, XMFLOAT3(m_aSmoke[nCntSmoke].pos.x, 0.1f, m_aSmoke[nCntSmoke].pos.z));

					// 影のα値設定
					if (m_aSmoke[nCntSmoke].col.w > 0.0f) {
						colA = (300.0f - (m_aSmoke[nCntSmoke].pos.y - 9.0f)) / (300.0f / m_aSmoke[nCntSmoke].col.w);
						if (colA < 0.0f) {
							colA = 0.0f;
						}
					} else {
						colA = 0.0f;
					}

					// 影の色の設定
					SetShadowColor(m_aSmoke[nCntSmoke].nIdxShadow, XMFLOAT4(0.15f, 0.15f, 0.15f, colA));
				}
#endif
				m_aSmoke[nCntSmoke].nLife--;
				if (m_aSmoke[nCntSmoke].nLife <= 0) {
					m_aSmoke[nCntSmoke].bUse = false;
					//ReleaseShadow(m_aSmoke[nCntSmoke].nIdxShadow);
					m_aSmoke[nCntSmoke].nIdxShadow = -1;
				} else {
					/*if (m_aSmoke[nCntSmoke].nLife <= 80) {
						m_aSmoke[nCntSmoke].col.x = 1.0f - (float)(80 - m_aSmoke[nCntSmoke].nLife) / 8.0f * 0.06f + 0.2f;
						m_aSmoke[nCntSmoke].col.y = 1.0f - (float)(80 - m_aSmoke[nCntSmoke].nLife) / 8.0f * 0.07f;
						m_aSmoke[nCntSmoke].col.z = 1.0f;
					}*/
					/*if (m_aSmoke[nCntSmoke].nLife <= 80) {
						m_aSmoke[nCntSmoke].col.x = 0.60f - (float)(80 - m_aSmoke[nCntSmoke].nLife) / 8.0f * 0.06f + 0.2f;
						m_aSmoke[nCntSmoke].col.y = 0.70f - (float)(80 - m_aSmoke[nCntSmoke].nLife) / 8.0f * 0.07f;
						m_aSmoke[nCntSmoke].col.z = 0.05f;
					}*/

					//if (m_aSmoke[nCntSmoke].nLife <= 10) {
					//	// α値設定
					//	m_aSmoke[nCntSmoke].col.w -= 0.05f;
					//	if (m_aSmoke[nCntSmoke].col.w < 0.0f) {
					//		m_aSmoke[nCntSmoke].col.w = 1.0f;

					if (m_aSmoke[nCntSmoke].bUse) {
						// α値設定
						m_aSmoke[nCntSmoke].col.w -= 0.05f;
						if (m_aSmoke[nCntSmoke].col.w < 0.0f) {
							m_aSmoke[nCntSmoke].col.w = 0.0f;
						}
					}

					// 色の設定
					SetColorSmoke(nCntSmoke, m_aSmoke[nCntSmoke].col);
				}
			}
		}

		if (m_bGenerate) {
			--m_nTimer;

			if (m_nTimer > 0)
			{

				// パーティクル発生
		//		if((rand() % 2) == 0)
				{
					XMFLOAT3 pos;
					XMFLOAT3 move;
					float fAngle, fLength;
					int nLife;
					float fSize;
					float lenX;
					float lenY;


					/*XMFLOAT3 ModelPos = GetModelPos();
					pos = ModelPos;*/

					//XMFLOAT3 ModelPos = m_pEnemyParts->GetPosModel();
					pos = m_posBase/*ModelPos*/;

					// m_lenX,m_lenY内でランダムな値を作り、lenX,lenYに入れる
					m_lenX = (float)(rand() % 300);
					m_lenY = (float)(rand() % 100);

					lenX = m_lenX;
					lenY = m_lenY;

					pos.x += lenX;
					pos.y += lenY;

					fAngle = rand() % 360 - 180.0f;
					fLength = rand() % (int)(m_fWidthBase * 100) / 100.0f - m_fWidthBase;
					//			move.x = rand() % (int)(m_fWidthBase * 200) / 100.0f - m_fWidthBase;
					move.x = SinDeg(fAngle) * fLength;
					move.y = rand() % 300 / 100.0f + m_fHeightBase;
					//			move.z = rand() % (int)(m_fWidthBase * 200) / 100.0f - m_fWidthBase;
					move.z = CosDeg(fAngle) * fLength;

					//***************************************************************************
					//XMFLOAT3 vVecSmoke;
					//m_vSmokeRot = XMVector3TransformNormal(m_vSmokeRot, m_pEnemyParts->GetRotateNorMatrix());
					//if (!XMMatrixIsIdentity(m_pEnemyParts->GetRotateMatrix())) {
					//	m_vSmokeRot = XMVector3TransformNormal(m_vSmokeRot, m_pEnemyParts->GetRotateMatrix());
					//}
					//
					////m_RotNormtx = XMMatrixMultiply(m_RotNormtx, m_pPlayer->GetRotateNorMatrix());
					////vSmokeRot = XMVector3TransformNormal(vSmokeRot, m_RotNormtx);
					//
					//XMStoreFloat3(&vVecSmoke, m_vSmokeRot);



					//pos.x += vVecSmoke.x;
					//pos.y += vVecSmoke.y;
					//pos.z += vVecSmoke.z;

					//XMVECTOR vSmokeRot;
					//vSmokeRot = XMVectorSet(move.x, move.y, move.z, 1.0f);
					//vSmokeRot = XMVector3TransformNormal(vSmokeRot, m_pEnemyParts->GetTotateWorldMatrix());
					////vSmokeRot = XMVector3TransformNormal(vSmokeRot, m_pPlayer->GetRotateMatrix());
					///*vSmokeRot = XMVector3TransformNormal(vSmokeRot, XMMatrixRotationX(-3.14159265359f / 3.5f));
					//vSmokeRot = XMVector3TransformNormal(vSmokeRot, m_pPlayer->GetRotateNorMatrix());*/

					//XMStoreFloat3(&move, vSmokeRot);
					//***************************************************************************

					nLife = rand() % 40 + 10;

					fSize = (float)(rand() % 500 + 20);

					//pos.y += fSize / 2;

					// ビルボードの設定
					SetSmoke(pos, move/*f3ParMove*/, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife);
				}
			}
			else
			{
				m_nTimer = SMOKE_TIMER * 60;
				m_bGenerate = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CSmoke::DrawSmoke(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// 加算合成
	SetBlendState(BS_ADDITIVE);

	// ビューマトリックスを取得
	/*XMFLOAT4X4& mtxView = CSmoke::Get()->GetViewMatrix();*/
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();/*m_pCamera->GetViewMatrix(); */

	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; ++nCntSmoke) {
		if (m_aSmoke[nCntSmoke].bUse) {
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&g_meshSmoke.mtxWorld, mtxWorld);

			g_meshSmoke.mtxWorld._11 = mtxView._11;
			g_meshSmoke.mtxWorld._12 = mtxView._21;
			g_meshSmoke.mtxWorld._13 = mtxView._31;
			g_meshSmoke.mtxWorld._21 = mtxView._12;
			g_meshSmoke.mtxWorld._22 = mtxView._22;
			g_meshSmoke.mtxWorld._23 = mtxView._32;
			g_meshSmoke.mtxWorld._31 = mtxView._13;
			g_meshSmoke.mtxWorld._32 = mtxView._23;
			g_meshSmoke.mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&g_meshSmoke.mtxWorld);

			// スケールを反映
			mtxScale = XMMatrixScaling(m_aSmoke[nCntSmoke].fSizeX, m_aSmoke[nCntSmoke].fSizeY, 5.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(m_aSmoke[nCntSmoke].pos.x, m_aSmoke[nCntSmoke].pos.y, m_aSmoke[nCntSmoke].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			//　プレイヤーの回転を反映
			//mtxWorld = XMMatrixMultiply(mtxWorld, m_pPlayer->GetRotateNorMatrix());
			//mtxWorld = XMMatrixMultiply(mtxWorld, m_pPlayer->GetTotateWorldMatrix());

			// ワールドマトリックスの設定
			XMStoreFloat4x4(&g_meshSmoke.mtxWorld, mtxWorld);

			// カラーの設定
			g_material.Diffuse = m_aSmoke[nCntSmoke].col;

			// メッシュの描画
			DrawMesh(pDeviceContext, &g_meshSmoke);
		}
	}

	 //半透明合成無
	SetBlendState(BS_NONE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexSmoke(ID3D11Device* pDevice)
{
	// オブジェクトの頂点配列を生成
	g_meshSmoke.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshSmoke.nNumVertex];

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
	g_meshSmoke.nNumIndex = 4;
	int* pIndexWk = new int[g_meshSmoke.nNumIndex];

	// インデックス配列の中身を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// 頂点バッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_meshSmoke, pVertexWk, pIndexWk);

	// 一時配列の解放
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void CSmoke::SetVertexSmoke(int nIdxSmoke, float fSizeX, float fSizeY)
{
	if (nIdxSmoke >= 0 && nIdxSmoke < MAX_SMOKE) {
		m_aSmoke[nIdxSmoke].fSizeX = fSizeX;
		m_aSmoke[nIdxSmoke].fSizeY = fSizeY;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void CSmoke::SetColorSmoke(int nIdxSmoke, XMFLOAT4 col)
{
	if (nIdxSmoke >= 0 && nIdxSmoke < MAX_SMOKE) {
		m_aSmoke[nIdxSmoke].col = col;
	}
}

//=============================================================================
// エフェクトの設定
//=============================================================================
int CSmoke::SetSmoke(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxSmoke = -1;

	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; ++nCntSmoke) {
		if (!m_aSmoke[nCntSmoke].bUse) {
			m_aSmoke[nCntSmoke].pos = pos;
			m_aSmoke[nCntSmoke].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_aSmoke[nCntSmoke].move = move;
			m_aSmoke[nCntSmoke].nLife = nLife;
			m_aSmoke[nCntSmoke].bUse = true;

			// 頂点座標の設定
			SetVertexSmoke(nCntSmoke, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorSmoke(nCntSmoke, col);

			nIdxSmoke = nCntSmoke;

#ifdef DISP_SHADOW
			// 影の設定
			m_aSmoke[nCntSmoke].nIdxShadow = CreateShadow(pos, fSizeX);
			if (m_aSmoke[nCntSmoke].nIdxShadow != -1) {
				SetShadowColor(m_aSmoke[nCntSmoke].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxSmoke;
}

////パーティクル発生呼び出し
//int SpawnSmoke(XMFLOAT3 pos, XMFLOAT3 dir)
//{
//	//XMFLOAT3 ModelPos = GetModelPos();
//	XMFLOAT3 ModelPos = m_pEnemyParts->GetPosModel();
//
//
//	
//	SMOKE* pSmoke = m_aSmoke;
//	for (int i = 0; i < MAX_SMOKE; ++i, ++pSmoke) {
//		if (pSmoke->bUse) continue;
//
//		pos = ModelPos;
//
//		pSmoke->pos = pos;
//
//		//pSmoke->nIdxShadow = CreateShadow(pos, SMOKE_SIZE_X);
//		pSmoke->bUse = 1;
//		return i;
//	}
//	return -1;
//}

//void SetSmokeEnemyParts(EnemyParts* pEnemyParts)
//{
//	m_pEnemyParts = pEnemyParts;
//}

//void CSmoke::SetCamera(CCamera* pCamera)
//{
//	m_pCamera = pCamera;
//}

//=============================================================================
// スモーク呼び出し関数
//=============================================================================
void CSmoke::SetCallSmoke(XMFLOAT3 pos, float lenX,float lenY)
{
	m_posBase = pos;
	m_lenX = lenX;
	m_lenY = lenY;

	m_bGenerate = true;
}