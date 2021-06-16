//=============================================================================
//
// モデル処理 [EnemyVacuum.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "enemyRushWash.h"
#include "AssimpModel.h"
#include "deatheffect.h"
#include <stdio.h>
#include <time.h>
#include "scene.h"
#include "fade.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPHER_POSX           (0.0f)
#define SPHER_POSY           (0.0f)
#define SPHER_POSZ           (800.0f)

#define EFFECT_CNT           (10)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAssimpModel		g_enemyCore[MAX_COREPARTS_NUM];
static CCamera*			g_pCamera;
static CLight*			g_pLight;

static bool g_bModelLoad = false;

static LPCSTR c_aFileNameCore[MAX_COREPARTS_NUM] =
{
	"data/model/EnemyCore/Armor.fbx",
	"data/model/EnemyCore/Armor1.fbx",
	"data/model/EnemyCore/Armor2.fbx",
	"data/model/EnemyCore/Armor3.fbx",
	"data/model/EnemyCore/Armor4.fbx",
	"data/model/EnemyCore/Armor5.fbx",
	"data/model/EnemyCore/Armor6.fbx",
	"data/model/EnemyCore/Armor7.fbx",
	"data/model/EnemyCore/LeftHand_Stand.fbx",
	"data/model/EnemyCore/RightHand_Stand.fbx",
	"data/model/EnemyCore/Body.fbx",//
	//"data/model/EnemyCore/BodyCenter.fbx",//3
	//"data/model/EnemyCore/BodyGlass.fbx",//3
	
	/*"data/model/EnemyCore/WeakPoint.fbx",*/
};
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemyRushWash::InitEnemy(void)
{
	m_nDeathEffectTime = 10 * 60;
	m_bDeathflg = false;
	m_nEffectCnt = EFFECT_CNT;

	// 位置・回転・スケールの初期設定(敵)
	for (int i = 0; i < MAX_COREPARTS_NUM; ++i) {
		m_Pos[i] = XMFLOAT3(0.0f, 0.0f, 600.0f);
		if (i == 8) {
			m_Pos[i] = XMFLOAT3(450.0f, 0.0f, 750.0f);
		}else if (i == 9) {
			m_Pos[i] = XMFLOAT3(-450.0f, 0.0f, 750.0f);
		}
		m_Rot[i] = XMFLOAT3(90.0f, 0.0f, 0.0f);
		if (i == 8) {
			m_Rot[i] = XMFLOAT3(90.0f, 0.0f,-75.0f);
		}
		else if (i == 9) {
			m_Rot[i] = XMFLOAT3(90.0f, 0.0f, 75.0f);
		}
		m_Scl[i] = XMFLOAT3(20.0f, 20.0f, 20.0f);
	}

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	hr = S_OK;
	hr = m_SpherFront.InitEnemy();
	if (FAILED(hr))
		return hr;
	m_SpherFront.SetPos(XMFLOAT3(0.0f, 0.0f, 800.0f));
	m_SpherFront.SetScl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_SpherFront.SetAlphaType(Clear);


	// モデルデータの読み込み

#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif
	if (!g_bModelLoad) {
		for (int i = 0; i < MAX_COREPARTS_NUM; ++i) {
			if (!g_enemyCore[i].Load(pDevice, pDeviceContext, c_aFileNameCore[i]/*MODEL_VACUUM*/)) {
				hr = E_FAIL;
				MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitWash", MB_OK);
				return hr;
			}
			SaveModel(&g_enemyCore[i]);
		}
		g_bModelLoad = true;
	}
	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		
		/*m_pLife[i] = new CEnemyLife(XMFLOAT3(0.0f, 0.0f, 1400.0f - i * 25.0f), 100, false);*/
		m_Life[i].Init(XMFLOAT3(0.0f, 0.0f, 1400.0f - i * 25.0f), 100, false);
	}
	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemyRushWash::UninitEnemy(void)
{
	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		/*delete m_pLife[i];*/
		m_Life[i].Uninit();
	}
	m_SpherFront.UninitEnemy();
	
	//// モデルの解放
	//for (int i = 0; i < MAX_COREPARTS_NUM; ++i) {
	//	g_enemyCore[i].Release();
	//}
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemyRushWash::UpdateEnemy(void)
{
	

	m_SpherFront.UpdateEnemy();
	
	if (!m_bDeathflg) {
		CheckClearflg();
	}
	else
	{
		m_SpherFront.SetScl(XMFLOAT3(m_SpherFront.GetScl().x + 2.0f, m_SpherFront.GetScl().y + 2.0f, m_SpherFront.GetScl().z + 2.0f));

		XMFLOAT3 vCameraPos = XMFLOAT3(-3000.0f, 3000.0f, 1000.0f);
		XMFLOAT3 vCameraTarget = XMFLOAT3( 0.0f,  0.0f, 800.0f);
		XMFLOAT3 vCameraUp = XMFLOAT3(0.0f, 0.0f, 1.0f);
		CCamera::Get()->SetPos(vCameraPos);
		CCamera::Get()->SetTarget(vCameraTarget);
		CCamera::Get()->SetUpVector(vCameraUp);

		--m_nDeathEffectTime;
		if (m_nDeathEffectTime < 0) {
			SetFadeOut(SCENE_GAMECLEAR);
		}

		m_nEffectCnt = 2;

		srand((unsigned int)time(NULL));
		int nVec = (rand() % 300);

		int nXYZ = nVec % 3;

		if (nVec % 2 == 0) {
			nVec = -1 * nVec;
		}

		XMFLOAT3 f3Vec;
		switch (nXYZ)
		{
		case 0:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 3), (float)nVec * (rand() % 5), (float)nVec * (rand() % 5));
			break;
		case 1:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 4), (float)nVec * (rand() % 4), (float)nVec * (rand() % 3));
			break;
		case 2:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 5), (float)nVec * (rand() % 3), (float)nVec * (rand() % 2));
			break;
		}
		SetDeathEffect(XMFLOAT3(f3Vec.x * 1.2f + 0.0f, f3Vec.y * 0.8f + 0.0f, f3Vec.z * 1.2f + 800.0f), 1000.0f, 1000.0f);

		SetDeathEffect(XMFLOAT3(f3Vec.x * 1.2f + 0.0f, f3Vec.y * 1.5f + 0.0f, f3Vec.z * 0.5f + 800.0f), 1000.0f, 1000.0f);

		SetDeathEffect(XMFLOAT3(f3Vec.x * 1.5f + 0.0f, f3Vec.y * 1.3f + 0.0f, f3Vec.z * 1.4f + 800.0f), 1000.0f, 1000.0f);

		SetDeathEffect(XMFLOAT3(f3Vec.x + 0.0f, f3Vec.y + 0.0f, f3Vec.z + 800.0f), 1000.0f, 1000.0f);

	}
	

	
	--m_nEffectCnt;
	if (m_nEffectCnt < 0) {
		
		m_nEffectCnt = EFFECT_CNT;
				
		srand((unsigned int)time(NULL));
		int nVec = (rand() % 300);
		
		int nXYZ = nVec % 3;

		if (nVec % 2 == 0) {
			nVec = -1 * nVec;
		}

		XMFLOAT3 f3Vec;
		switch (nXYZ)
		{
		case 0:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 5), (float)nVec * (rand() % 3), (float)nVec * (rand() % 2));
			break;
		case 1:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 3), (float)nVec * (rand() % 2), (float)nVec * (rand() % 5));
			break;
		case 2:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 2), (float)nVec * (rand() % 5), (float)nVec * (rand() % 3));
			break;
		}
		SetDeathEffect(XMFLOAT3(f3Vec.x * 1.2f + 0.0f, f3Vec.y * 0.8f + 0.0f, f3Vec.z * 1.2f + 800.0f), 200.0f, 200.0f);

		SetDeathEffect(XMFLOAT3(f3Vec.x * 1.2f + 0.0f, f3Vec.y * 1.5f + 0.0f, f3Vec.z * 0.5f + 800.0f), 200.0f, 200.0f);

		SetDeathEffect(XMFLOAT3(f3Vec.x + 0.0f, f3Vec.y + 0.0f, f3Vec.z + 800.0f), 200.0f, 200.0f);
	}

	// カメラの向き取得
	//XMFLOAT3 rotCamera = g_pCamera->GetAngle();

	//==============================================================================================
	for (int i = 0; i < MAX_COREPARTS_NUM; ++i) {
		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_Scl[i].x, m_Scl[i].y, m_Scl[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rot[i].x),
			XMConvertToRadians(m_Rot[i].y), XMConvertToRadians(m_Rot[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		mtxTranslate = XMMatrixTranslation(m_Pos[i].x, m_Pos[i].y, m_Pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);
	
	// カメラ、光源設定
		g_enemyCore[i].SetCamera(CCamera::Get()/*g_pCamera*/);
		g_enemyCore[i].SetLight(CLight::Get()/*g_pLight*/);
		
	}

	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		/*m_pLife[i]->Update();*/
		m_Life[i].Update();
		//m_pLife[i]->SetCamera(g_pCamera);
	}

	
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemyRushWash::DrawEnemy(void)
{
	//***********************************************************************
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_COREPARTS_NUM; ++i) {
		// 不透明部分を描画
		g_enemyCore[i].Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);

		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない

		g_enemyCore[i].Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);

		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}
	//******************************************************************************************
	
	m_SpherFront.DrawEnemy();

	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		//m_Life[i].SetCamera(g_pCamera);
		m_Life[i].Draw();
		//m_pLife[i]->Draw();
	}
}


////=============================================================================
//// カメラの設定
////=============================================================================
//void CEnemyRushWash::SetCamera(CCamera* Camera)
//{
//	m_SpherFront.SetCamera(Camera);
//	
//	g_pCamera = Camera;
//}

////=============================================================================
//// ライトの設定
////=============================================================================
//void CEnemyRushWash::SetLight(CLight* Light)
//{
//	m_SpherFront.SetEnemyLight(Light);
//	
//	g_pLight = Light;
//
//}


//=============================================================================
// コアエネミーライフインスタンス取得
//=============================================================================
CEnemyLife* CEnemyRushWash::GetInstansEnemyLife()
{
	int nNum = 0;
	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		/*if (m_pLife[i]->GetStat() == 0) continue;*/
		if (m_Life[i].GetStat() == 0) continue;

		nNum = i;
		break;
	}

	return /*m_pLife[nNum];*/ &m_Life[nNum];
}

//=============================================================================
// ライフを参照し、全て０なら爆発フラグON
//=============================================================================
void CEnemyRushWash::CheckClearflg(void)
{
	bool bClearflg = true;
	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		//if (m_pLife[i]->GetStat() == 0) continue;
		if (m_Life[i].GetStat() == 0) continue;
		bClearflg = false;

		;
		break;
	}


	if (bClearflg) {
		m_bDeathflg = true;
		//SetFadeOut(SCENE_GAMECLEAR);
	}
}