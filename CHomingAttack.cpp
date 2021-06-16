//=============================================================================
//
// モデル処理 [CHomingAttack.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
//#include "main.h"
#include "AssimpModel.h"
#include "CHomingAttack.h"
#include "Collision.h"
#include "hiteffect.h"
#include "model.h"
#include "explosion.h"
#include "Sound.h"
#include "CHitStop.h"

#include "Light.h"
#include "Camera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define HOMINGATTACK_MODEL			"data/model/car001.fbx"
#define MODEL_HOMINGATKSET		"data/model/Enemy/sphere.fbx"
#define MODEL_HOMINGATKBULLET		"data/model/EnemyCore/LeftHand_Attack.fbx"
#define HOMINGATTACK_MOVE		(1)			// 敵追尾速度
#define HOMINGATTACK_DAMAGE      (5)        // ダメージ
#define HOMINGATTACK_FALLTIME    (5*60)    // 追尾時間
#define HOMINGATTACK_SETY        (5.0f)     // 攻撃の初速度Y 
#define HOMINGATTACK_TIME        (4*60)     // 攻撃が時間の間隔 
//*****************************************************************************
// グローバル変数
//*****************************************************************************
//CAssimpModel	g_homing[MAX_HOMINGATK];		        // モデル
//CAssimpModel	g_homingset[MAX_HOMINGATKSETNUM];		// モデル

CAssimpModel	g_homing;		        // モデル
CAssimpModel	g_homingset;		// モデル

static bool g_bModelLoad = false;

CHomingAttack::CHomingAttack()
{

}

CHomingAttack::~CHomingAttack()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CHomingAttack::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 内容はここで決めてよい


	for (int i = 0; i <MAX_HOMINGATKSETNUM; ++i) {
		m_bUseSet[i] = false;
		m_nAtkSetTime[i] = 0;
		m_bUseSet[i] = false;
		m_posSet[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);   // 配置位置（反応範囲の頭上）
		m_radSet[i] = 0.0f;   // ダメージ球の半径
		m_sclSet[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);   // 反応範囲の大きさ（横幅X、縦幅Y、奥行きZ）
		m_rotSet[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);   // 回転角度 (斜めにするときだけ)
		/*m_nLifeSet[i] = 100;*/

		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(m_sclSet[i].x, m_sclSet[i].y, m_sclSet[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotSet[i].x),
			XMConvertToRadians(m_rotSet[i].y), XMConvertToRadians(m_rotSet[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_posSet[i].x, m_posSet[i].y, m_posSet[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_mtxWorldSet[i], mtxWorld);

		//// モデルデータの読み込み
		//if (!g_homingset.Load(pDevice, pDeviceContext, MODEL_HOMINGATK)) {
		//	hr = E_FAIL;
		//	MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		//	return hr;
		//}
	}
	
	m_bUseSet[0] = true;
	m_posSet[0] = XMFLOAT3(0.0f, 530.0f, 750.0f);
	m_radSet[0] = 40.0f * 2;
	m_sclSet[0] = XMFLOAT3(m_radSet[0] / 2, m_radSet[0] / 2, m_radSet[0] / 2);   // 反応範囲の大きさ（横幅X、縦幅Y、奥行きZ）

	m_bUseSet[1] = true;
	m_posSet[1] = XMFLOAT3(0.0f, -530.0f, 750.0f);
	m_radSet[1] = 40.0f * 2;
	m_sclSet[1] = XMFLOAT3(m_radSet[1] / 2, m_radSet[1] / 2, m_radSet[1] / 2);   // 反応範囲の大きさ（横幅X、縦幅Y、奥行きZ）

	//m_pLife[0] = new CEnemyLife(m_posSet[0], 100/*m_nLifeSet[0]*/,true);
	//m_pLife[1] = new CEnemyLife(m_posSet[1], 100/*m_nLifeSet[1]*/,true);

	m_Life[0].Init(m_posSet[0], 100/*m_nLifeSet[1]*/, true);
	m_Life[1].Init(m_posSet[1], 100/*m_nLifeSet[1]*/, true);

	//************************************************************************************************:

	for (int i = 0; i < MAX_FALLATK; ++i) {
		m_bUseAtk[i] = false;
		m_pos[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rad[i] = 30.0f;
		m_scl[i] = XMFLOAT3(20.0f, 20.0f, 20.0f);
		m_rot[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_move[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_nDamage[i] = HOMINGATTACK_DAMAGE;
		m_nHomingTime[i] = 20 * 60;
		m_nLife[i] = 1;

		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(m_scl[i].x, m_scl[i].y, m_scl[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rot[i].x),
			XMConvertToRadians(m_rot[i].y), XMConvertToRadians(m_rot[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_pos[i].x, m_pos[i].y, m_pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);


		//// モデルデータの読み込み
		//if (!g_homing.Load(pDevice, pDeviceContext, MODEL_HOMINGATK)) {
		//	hr = E_FAIL;
		//	MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		//	return hr;
		//}
	}
#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif
	if (!g_bModelLoad) {
		// モデルデータの読み込み
		if (!g_homingset.Load(pDevice, pDeviceContext, MODEL_HOMINGATKSET)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_homingset);

		// モデルデータの読み込み
		if (!g_homing.Load(pDevice, pDeviceContext, MODEL_HOMINGATKBULLET)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_homing);

		g_bModelLoad = true;
	}
	//// 丸影の生成
	//for (int i = 0; i < MAX_HOMING_PARTS; i++)
	//{
	//	g_nShadow[i] = CreateShadow(m_posEnemy[i], 20.0f);
	//}

	return hr;
}


//=============================================================================
// 終了処理
//=============================================================================

void CHomingAttack::Uninit()
{
	//for (int i = 0; i < MAX_HOMING_PARTS; i++)
	//{
	//	// 丸影の解放
	//	ReleaseShadow(g_nShadow[i]);
	//}

	/*delete m_pLife[1];
	delete m_pLife[0];*/
	m_Life[1].Uninit();
	m_Life[0].Uninit();


	// モデルの解放
#if MODEL_RELEASE_ON
		for (int i = 0; i < MAX_HOMINGATKSETNUM; ++i) {
			g_homingset.Release();
		}
		for (int i = 0; i < MAX_FALLATK; ++i) {
			g_homing.Release();
		}
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void CHomingAttack::Update()
{

	// 発射台一つ一つについての発射時間管理
	for (int i = 0; i <MAX_HOMINGATKSETNUM; ++i) {
		if (!m_bUseSet[i])continue;

		--m_nAtkSetTime[i];

		if (m_nAtkSetTime[i] < 0) {
			if (GetWorldMoveflg()) {
				SetAtkHoming(i);
				m_nAtkSetTime[i] = HOMINGATTACK_TIME;
			}
		}
		if (CollisionCircleToCircle(m_posSet[i], m_pPlayer->GetPosCut(), m_radSet[i], 50.0f)) {
			
			if (m_pPlayer->GetBreakflg()) {
				/*--m_nLifeSet[i];*/
				//m_pLife[i]->DelEnemyLife(10);
				m_Life[i].DelEnemyLife(10);
				SetExplosion(m_posSet[i], 700.0f, 700.0f);

				CSound::Play(SE_PATK);	// 効果音再生
				CSound::SetVolume(SE_PATK, 3.0f);

				CHitStop::Get()->HitStopOn(2);
			}
				
				
			if (m_Life[i].GetLife() <= 0/*m_pLife[i]->GetLife() <= 0*/) {
				m_bUseSet[i] = false;
				SetExplosion(XMFLOAT3(m_posSet[i].x+50.0f, m_posSet[i].y + 50.0f,m_posSet[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_posSet[i].x + (-50.0f), m_posSet[i].y + 50.0f, m_posSet[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_posSet[i].x + (-50.0f), m_posSet[i].y + (-50.0f), m_posSet[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_posSet[i].x + (-50.0f), m_posSet[i].y + (-50.0f), m_posSet[i].z + (-50.0f)),
					700.0f, 700.0f);

			}
		}

		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(m_sclSet[i].x, m_sclSet[i].y, m_sclSet[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotSet[i].x),
			XMConvertToRadians(m_rotSet[i].y), XMConvertToRadians(m_rotSet[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_posSet[i].x, m_posSet[i].y, m_posSet[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_mtxWorldSet[i], mtxWorld);

		
	}


	// 追尾処理とそれがプレイヤーと当たれば体力を減らす処理
	for (int i = 0; i < MAX_FALLATK; ++i) {
		if (!m_bUseAtk[i]) continue;
		--m_nHomingTime[i];
		if (m_nHomingTime[i] > 0) {
			m_move[i].x += 0.0f;
			m_move[i].y += 0.0f;
			m_move[i].z += 0.0f;
			m_move[i].x = (m_pPlayer->GetPosModel().x - m_pos[i].x) / 30.0f/*(200.0f * 60.0f)*/;
			m_move[i].y = (m_pPlayer->GetPosModel().y - m_pos[i].y) / 30.0f/*(200.0f * 60.0f)*/;
			m_move[i].z = (m_pPlayer->GetPosModel().z - m_pos[i].z) / 30.0f/*(200.0f * 60.0f)*/;

			m_pos[i].x += m_move[i].x/**HOMINGATTACK_MOVE*/;
			m_pos[i].y += m_move[i].y/**HOMINGATTACK_MOVE*/;
			m_pos[i].z += m_move[i].z/**HOMINGATTACK_MOVE*/;

			if (m_pos[i].z < 800.0f) {
				if (CollisionCircleToCircle(m_pos[i], XMFLOAT3(0.0f, 0.0f, m_pos[i].z), m_rad[i], MODEL_POSY)) {
					XMVECTOR vUnGravity = XMVector3Normalize(
						XMVectorSet(
							m_pos[i].x - 0.0f,
							m_pos[i].y - 0.0f,
							0.0f, 1.0f)
					);

					m_pos[i].x += XMVectorGetX(vUnGravity) * 10;
					m_pos[i].y += XMVectorGetY(vUnGravity) * 10;
					m_pos[i].z += XMVectorGetZ(vUnGravity) * 10;
				}
			}
			else
			{
				if (CollisionCircleToCircle(m_pos[i], XMFLOAT3(0.0f, 0.0f, 800.0f), m_rad[i], MODEL_POSY)) {
					XMVECTOR vUnGravity = XMVector3Normalize(
						XMVectorSet(
							m_pos[i].x - 0.0f,
							m_pos[i].y - 0.0f,
							0.0f, 1.0f)
					);

					m_pos[i].x += XMVectorGetX(vUnGravity) * 10;
					m_pos[i].y += XMVectorGetY(vUnGravity) * 10;
					m_pos[i].z += XMVectorGetZ(vUnGravity) * 10;
				}
			}

			if (CollisionRectToPoint(XMFLOAT3(0.0f, 530.0f, 380.0f), XMFLOAT3(400.0f, 400.0f, 400.0f), m_pos[i]))
			{
				m_bUseAtk[i] = false;
				SetExplosion(XMFLOAT3(m_pos[i].x + 50.0f, m_pos[i].y + 50.0f, m_pos[i].z + 50.0f),
					300.0f, 300.0f);
			}


			if (CollisionCircleToCircle(m_pos[i], m_pPlayer->GetPosModel(), m_rad[i], 50.0f)) {
				// 敵に当たったらエフェクト出してHP減らして
				// 点滅してるときにはエフェクト出さず、HPも減らさない。
				if (!m_pPlayer->GetInvflg())
				{
					m_pPlayer->GetLife()->DelLife(m_nDamage[i]);
					SetHitEffect(m_pPlayer->GetPosModel(), 400.0f, 400.0f);


					CSound::Play(SE_PATK);	// 効果音再生
					CSound::SetVolume(SE_PATK, 3.0f);

					CHitStop::Get()->HitStopOn(2);

					// SE再生開始
					//CSound::Play(SE_HIT);
				}

				m_bUseAtk[i] = false;
				m_pPlayer->SetInvflg(true);
			}
		}
		//else if( -200<m_pos[i].x)
		else{
			m_bUseAtk[i] = false;
			SetExplosion(XMFLOAT3(m_pos[i].x + 50.0f, m_pos[i].y + 50.0f, m_pos[i].z + 50.0f),
				700.0f, 700.0f);
			SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + 50.0f, m_pos[i].z + 50.0f),
				700.0f, 700.0f);
			SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + (-50.0f), m_pos[i].z + 50.0f),
				700.0f, 700.0f);
			SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + (-50.0f), m_pos[i].z + (-50.0f)),
				700.0f, 700.0f);
		}

		if (CollisionCircleToCircle(m_pos[i], m_pPlayer->GetPosCut(), m_radSet[i], 5.0f)) {
			if (m_pPlayer->GetBreakflg()) {
				--m_nLife[i];
				SetExplosion(m_pos[i], 700.0f, 700.0f);
			}
			if (m_nLife[i] <= 0) {
				m_bUseAtk[i] = false;
				SetExplosion(XMFLOAT3(m_pos[i].x + 50.0f, m_pos[i].y + 50.0f, m_pos[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + 50.0f, m_pos[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + (-50.0f), m_pos[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + (-50.0f), m_pos[i].z + (-50.0f)),
					700.0f, 700.0f);

			}
		}


		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(m_scl[i].x, m_scl[i].y, m_scl[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rot[i].x),
			XMConvertToRadians(m_rot[i].y), XMConvertToRadians(m_rot[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_pos[i].x, m_pos[i].y, m_pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);

		//// 丸影の移動
		//MoveShadow(g_nShadow[i], m_posEnemy[i]);

	   

	}

	/*m_pLife[0]->Update();
	m_pLife[1]->Update();*/

	m_Life[0].Update();
	m_Life[1].Update(); 
}



//=============================================================================
// 描画処理
//=============================================================================
void CHomingAttack::Draw()
{
	/*m_pLife[0]->SetCamera(m_pCamera);
	m_pLife[1]->SetCamera(m_pCamera);*/
	/*m_Life[0].SetCamera(m_pCamera);
	m_Life[1].SetCamera(m_pCamera);*/

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_FALLATK; ++i) {
		if (!m_bUseAtk[i])continue;

		// カメラ、光源設定
		g_homing.SetCamera(CCamera::Get());
		g_homing.SetLight(CLight::Get());


		// 不透明部分を描画
		g_homing.Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);

		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		g_homing.Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);

		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}

	for (int i = 0; i <MAX_HOMINGATKSETNUM; ++i) {
		if (!m_bUseSet[i])continue;

		
		// カメラ、光源設定
		g_homingset.SetCamera(CCamera::Get());
		g_homingset.SetLight(CLight::Get());
		
		// 不透明部分を描画
		g_homingset.Draw(pDeviceContext, m_mtxWorldSet[i], eOpacityOnly);

		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		g_homingset.Draw(pDeviceContext, m_mtxWorldSet[i], eTransparentOnly);

		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
		
	}

	/*m_pLife[0]->Draw();
	m_pLife[1]->Draw();*/
	m_Life[0].Draw();
	m_Life[1].Draw();
}

void CHomingAttack::SetAtkHoming(int nNum)
{
	for (int i = 0; i < MAX_FALLATK; ++i) {
		if (m_bUseAtk[i])continue;
		m_bUseAtk[i] = true;
		m_nHomingTime[i] = HOMINGATTACK_FALLTIME;
		m_pos[i].x = m_posSet[nNum].x;
		m_pos[i].y = m_posSet[nNum].y;
		m_pos[i].z = m_posSet[nNum].z;
		m_move[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		/*m_move[i] = XMFLOAT3(
			m_pPlayer->GetModelY().x * HOMINGATTACK_SETY, 
			m_pPlayer->GetModelY().y * HOMINGATTACK_SETY,
			m_pPlayer->GetModelY().z * HOMINGATTACK_SETY
		);*/

		break;
	}
}
