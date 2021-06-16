//=============================================================================
//
// 敵パーツ処理 [enemyParts.cpp]
// Author : 三宅　秀太郎
//
//=============================================================================
//#include "enemyParts.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "meshKirime.h"
#include "fade.h"
#include "scene.h"
#include "Collision.h"
#include "Camera.h"
#include "Light.h"
#include "model.h"

//#include "bilibili.h"
//#include "shadow.h"
//#include "control.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMYPARTS_0			"data/model/Enemy/EnemyArmor.fbx"
#define ENEMYPARTS_1			"data/model/Enemy/EnemyArmor1.fbx"
#define ENEMYPARTS_2			"data/model/Enemy/EnemyArmor2.fbx"
#define ENEMYPARTS_3			"data/model/Enemy/EnemyArmor3.fbx"

//#define ENEMYPARTS_0			"data/model/armor1test.fbx"
//#define ENEMYPARTS_1			"data/model/armor2test.fbx"
//#define ENEMYPARTS_2			"data/model/armor3test.fbx"
//#define ENEMYPARTS_3			"data/model/armor4test.fbx"



#define	VALUE_MOVE_ENEMYPARTS	(2.0f)		// 移動速度
#define	RATE_MOVE_ENEMYPARTS	(0.020f)	// 移動慣性係数
#define	VALUE_ROTATE_ENEMYPARTS	(9.0f)		// 回転速度
#define	RATE_ROTATE_ENEMYPARTS	(0.20f)		// 回転慣性係数

#define ENEMY_LIFE			(4)			 // 敵体力
#define ENEMY_GRAVITY		(0.015f)     // 敵落下速度

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAssimpModel	g_enemyParts[MAX_ENEMY_PARTS];		// モデル

//static CCamera*		g_pCamera;		// カメラ
//static CLight*		g_pLight;		// ライト
//static int			g_nShadow[MAX_ENEMY_PARTS];		// 丸影番号

static float		g_fGravity;		// 重力

static int          g_nKillCnt;

static XMFLOAT3     g_vPosCamera; // カメラ座標保存用
static XMFLOAT3     g_vTargetCamera; // カメラターゲット座標保存用
static XMFLOAT3     g_vUpCamera; // カメラ↑座標保存用

//static XMFLOAT3     m_pos_bilibili[55];

static bool g_bModelLoad = false;


static XINPUT_STATE* g_pXController;


// コンストラクタ
EnemyParts::EnemyParts()
{

}

TAssimpMaterial g_DefMat;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT EnemyParts::InitEnemy()
{
	m_GravitySpace[0].f3Pos = XMFLOAT3(500.0f, 500.0f, 800.0f + 1000.0f / 2.0f);
	m_GravitySpace[1].f3Pos = XMFLOAT3(-500.0f, 500.0f, 800.0f + 1000.0f / 2.0f);
	m_GravitySpace[2].f3Pos = XMFLOAT3(-500.0f, -500.0f, 800.0f + 1000.0f / 2.0f);
	m_GravitySpace[3].f3Pos = XMFLOAT3(500.0f, -500.0f, 800.0f + 1000.0f / 2.0f);

	m_GravitySpace[0].f3Scl = XMFLOAT3(1000.0f, 1000.0f, 1000.0f);
	m_GravitySpace[1].f3Scl = XMFLOAT3(1000.0f, 1000.0f, 1000.0f);
	m_GravitySpace[2].f3Scl = XMFLOAT3(1000.0f, 1000.0f, 1000.0f);
	m_GravitySpace[3].f3Scl = XMFLOAT3(1000.0f, 1000.0f, 1000.0f);


	m_rad = MODEL_POSY + 50.0f;
	g_nKillCnt = 0;


	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	g_fGravity = ENEMY_GRAVITY;

	// 吹き飛ぶ力の速度
	m_fPower[0] = XMFLOAT3(6.0f, 6.0f, 2.0f);
	m_fPower[1] = XMFLOAT3(-6.0f, 6.0f, 2.0f);
	m_fPower[2] = XMFLOAT3(-6.0f, -6.0f, 2.0f); //　左奥
	m_fPower[3] = XMFLOAT3(6.0f, -6.0f, 2.0f);

	// 位置・回転・スケールの初期設定
	m_posEnemy[0] = XMFLOAT3(0.0f, 0.0f, 50.0f); // 初期座標
	m_posEnemy[1] = XMFLOAT3(0.0f, 0.0f, 50.0f);
	m_posEnemy[2] = XMFLOAT3(0.0f, 0.0f, 50.0f); // 初期座標
	m_posEnemy[3] = XMFLOAT3(0.0f, 0.0f, 50.0f);

	m_sclEnemy[0] = XMFLOAT3(155.0f, 155.0f, 155.0f);
	m_sclEnemy[1] = XMFLOAT3(155.0f, 155.0f, 155.0f);
	m_sclEnemy[2] = XMFLOAT3(155.0f, 155.0f, 155.0f);
	m_sclEnemy[3] = XMFLOAT3(155.0f, 155.0f, 155.0f);

	m_moveEnemy[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_moveEnemy[1] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_moveEnemy[2] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_moveEnemy[3] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_rotEnemy[0] = XMFLOAT3(90.0f, 0.0f, 0.0f);
	m_rotEnemy[1] = XMFLOAT3(90.0f, 0.0f, 0.0f);
	m_rotEnemy[2] = XMFLOAT3(90.0f, 0.0f, 0.0f);
	m_rotEnemy[3] = XMFLOAT3(90.0f, 0.0f, 0.0f);
	/*g_rotDestEnemy = XMFLOAT3(0.0f, 0.0f, 0.0f);*/

	for (int i = 0; i < MAX_ENEMY_PARTS; ++i) {

		m_fallenParts[i] = false;
		m_fall[i] = false;
		m_nHp[i] = 1;
		m_bStat[i] = true;

		g_DefMat.Kd = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		if (g_bModelLoad) {
			g_enemyParts[i].SetMaterial(&g_DefMat);
		}
		//g_enemyParts[i].GetMaterial()->Kd.w = 1.0f;


		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// 大きさを反映
		mtxScl = XMMatrixScaling(m_sclEnemy[i].x, m_sclEnemy[i].y, m_sclEnemy[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotEnemy[i].x),
			XMConvertToRadians(m_rotEnemy[i].y), XMConvertToRadians(m_rotEnemy[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		//// 自由軸回転行列を反映
		//if (m_fall[i]) {
		//	mtxWorld = XMMatrixMultiply(mtxWorld, AxisRotmtx);
		//}

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_posEnemy[i].x, m_posEnemy[i].y, m_posEnemy[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);
	}
#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif

	if (!g_bModelLoad) {
		// モデルデータの読み込み
		if (!g_enemyParts[0].Load(pDevice, pDeviceContext, ENEMYPARTS_0)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_enemyParts[0]);

		if (!g_enemyParts[1].Load(pDevice, pDeviceContext, ENEMYPARTS_1)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_enemyParts[1]);

		if (!g_enemyParts[2].Load(pDevice, pDeviceContext, ENEMYPARTS_2)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_enemyParts[2]);

		if (!g_enemyParts[3].Load(pDevice, pDeviceContext, ENEMYPARTS_3)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_enemyParts[3]);
		
		g_bModelLoad = true;

	}

	

	m_Smoke.InitSmoke();



	return hr;
}


//=============================================================================
// 終了処理
//=============================================================================
EnemyParts::~EnemyParts()
{
	m_Smoke.UninitSmoke();
	//UninitEnemy();
}

void EnemyParts::UninitEnemy()
{
	
	// モデルの解放
#if MODEL_RELEASE_ON
		for (int i = 0; i < MAX_ENEMY_PARTS; ++i) {
			g_enemyParts[i].Release();
		}
#endif

	

}

//=============================================================================
// 更新処理
//=============================================================================
void EnemyParts::UpdateEnemy()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	bool bCameraMove = false;

	XMMATRIX AxisRotmtx;

	// 敵の体力が0になったら落下

	for (int i = 0; i < MAX_ENEMY_PARTS; ++i) {

		switch (i) {
		case 0:// 右下パーツ
			if (GetDestflg(0) && GetDestflg(2))
				--m_nHp[0];
			
			break;
		case 1:// 左下パーツ
			if (GetDestflg(0) && GetDestflg(3))
				--m_nHp[1];

		
			break;
		case 2:// 左上パーツ
			if (GetDestflg(1) && GetDestflg(3))
				--m_nHp[2];

			break;
		case 3:// 右上パーツ
			if (GetDestflg(1) && GetDestflg(2))
				--m_nHp[3];

			break;
		}

		if (m_bStat[i]) {
			if (m_nHp[i] <= 0)
			{
				if (!m_fall[i]) {
					if (!bCameraMove) {
						
						bCameraMove = true;
					}
				}

				m_fall[i] = true;
				// 敵の体力が0でかつx座標が0.0より大きいなら右方向に吹っ飛ぶ
				if (m_nHp[i] == 0 /*&& m_posEnemy[i].x > 0.0f*/)
				{
					m_moveEnemy[i].x = m_fPower[i].x;
					m_moveEnemy[i].y = m_fPower[i].y;
					m_moveEnemy[i].z = m_fPower[i].z;

					if (GetWorldMoveflg()) {
						g_vPosCamera = CCamera::Get()->GetPos();
						g_vTargetCamera = CCamera::Get()->GetTarget();
						g_vUpCamera = *(CCamera::Get()->GetCameraUp());

						SetWorldMoveflg(false);
						CCamera::Get()->SetUpdateIns(INS_CENEMY);
					}
				}

				if (m_fall[i]) {

					XMFLOAT3 vCameraPos;

					switch (i) {
					case 0:// 右下パーツ
						vCameraPos = XMFLOAT3(2000.0f, 2000.0f, 800.0f);
						// 自由軸回転行列を作成
						m_fAxisRot[i] += (2 * 3.14159265359f) / (60 * 2.0f);
						AxisRotmtx = XMMatrixRotationAxis(XMVectorSet(-1.0f, -1.0f, 0.0f, 1.0f), m_fAxisRot[i]);


						break;
					case 1:// 左下パーツ
						vCameraPos = XMFLOAT3(-2000.0f, 2000.0f, 800.0f);

						// 自由軸回転行列を作成
						m_fAxisRot[i] += (2 * 3.14159265359f) / (60 * 2.0f);
						AxisRotmtx = XMMatrixRotationAxis(XMVectorSet(-1.0f, -1.0f, 0.0f, 1.0f), m_fAxisRot[i]);

						break;
					case 2:// 左上パーツ
						vCameraPos = XMFLOAT3(-2000.0f, -2000.0f, 800.0f);

						// 自由軸回転行列を作成
						m_fAxisRot[i] += -(2 * 3.14159265359f) / (60 * 2.0f);
						AxisRotmtx = XMMatrixRotationAxis(XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f), m_fAxisRot[i]);

						break;
					case 3:// 右上パーツ
						vCameraPos = XMFLOAT3(2000.0f, -2000.0f, 800.0f);

						// 自由軸回転行列を作成
						m_fAxisRot[i] += -(2 * 3.14159265359f) / (60 * 2.0f);
						AxisRotmtx = XMMatrixRotationAxis(XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f), m_fAxisRot[i]);

						break;

					}

					if (!GetWorldMoveflg()) {

						XMFLOAT3 vTargetPos = XMFLOAT3(0.0f, 0.0f, 400.0f);
						CCamera::Get()->SetPos(vCameraPos);
 						CCamera::Get()->SetTarget(vTargetPos);

						XMFLOAT3 vCameraUp = XMFLOAT3(0.0f, 0.0f, 1.0f);
						CCamera::Get()->SetUpVector(vCameraUp);
					}
					// それぞれの座標の移動量設定
					m_moveEnemy[i].x += (0.0f - m_moveEnemy[i].x) * RATE_MOVE_ENEMYPARTS;
					m_moveEnemy[i].z = m_moveEnemy[i].z - g_fGravity;
					m_moveEnemy[i].y += (0.0f - m_moveEnemy[i].y) * RATE_MOVE_ENEMYPARTS;



				}
			}


		
			// 位置移動
			m_posEnemy[i].x += m_moveEnemy[i].x;
			m_posEnemy[i].y += m_moveEnemy[i].y;
			m_posEnemy[i].z += m_moveEnemy[i].z;

			if (m_fall[i]) {
				// 落ちたパーツが床についたらスモーク出して表現させる
				if (m_posEnemy[i].z < -1000.0f)
				{
					m_bStat[i] = false;
					m_posEnemy[i].z = -1000.0f;
					m_moveEnemy[i].x = 0.0f;
					m_moveEnemy[i].y = 0.0f;
					m_moveEnemy[i].z = 0.0f;
					m_fall[i] = false;
					m_fallenParts[i] = true;

					switch (i) {
					case 0:// 右下パーツ
						m_Smoke.SetCallSmoke(XMFLOAT3(100.0f, 500.0f, 0.0f), 100.0f, 100.0f);
						break;
					case 1:// 左下パーツ
						m_Smoke.SetCallSmoke(XMFLOAT3(-500.0f, 500.0f, 0.0f), 100.0f, 100.0f);
						break;
					case 2:// 左上パーツ
						m_Smoke.SetCallSmoke(XMFLOAT3(-500.0f, -100.0f, 0.0f), 100.0f, 100.0f);
						break;
					case 3:// 右上パーツ
						m_Smoke.SetCallSmoke(XMFLOAT3(-50.0f, -100.0f, 0.0f), 100.0f, 100.0f);
						break;
					}


					if (!GetWorldMoveflg()) {
						SetWorldMoveflg(true);
						CCamera::Get()->SetPos(g_vPosCamera);
						CCamera::Get()->SetTarget(g_vTargetCamera);
						CCamera::Get()->SetUpVector(g_vUpCamera);
					}

				}

				if (!GetWorldMoveflg()) {
					// スペースキー又はEnterで決定
					if (GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE)
#if SETCONTROLLER
						|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
						|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_B
						|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
						|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y
#endif
						)
					{
						SetWorldMoveflg(true);
						CCamera::Get()->SetPos(g_vPosCamera);
						CCamera::Get()->SetTarget(g_vTargetCamera);
						CCamera::Get()->SetUpVector(g_vUpCamera);
					}
				}
			}
		}
		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// 大きさを反映
		mtxScl = XMMatrixScaling(m_sclEnemy[i].x, m_sclEnemy[i].y, m_sclEnemy[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotEnemy[i].x),
			XMConvertToRadians(m_rotEnemy[i].y), XMConvertToRadians(m_rotEnemy[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		//// 自由軸回転行列を反映
		//if (m_fall[i]) {
		//	mtxWorld = XMMatrixMultiply(mtxWorld, AxisRotmtx);
		//}

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_posEnemy[i].x, m_posEnemy[i].y, m_posEnemy[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);

		//// 丸影の移動
		//MoveShadow(g_nShadow[i], m_posEnemy[i]);



	}

	for (int i = 0; i <= MAX_ENEMY_PARTS; ++i) {
		if (m_bStat[i]) {
			g_nKillCnt = 0;
			break;
		}
		g_nKillCnt++;
		if (g_nKillCnt == MAX_ENEMY_PARTS) {
			SetTime2(true);
		}
	}



	m_Smoke.UpdateSmoke();



}



//=============================================================================
// 描画処理
//=============================================================================
void EnemyParts::DrawEnemy()
{
	// カメラ、光源設定
	for (int i = 0; i < MAX_ENEMY_PARTS; ++i) {
		g_enemyParts[i].SetCamera(CCamera::Get());
		g_enemyParts[i].SetLight(CLight::Get());
	}


	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{
		// 不透明部分を描画
		g_enemyParts[i].Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);
		// 床に落下してスモーク出ると同時に段々透明化して消滅
		if (m_fallenParts[i])
		{
			g_enemyParts[i].Draw(pDeviceContext, m_mtxWorld[i], DanDan);
		}

		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない
		g_enemyParts[i].Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);

		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効


	}

	m_Smoke.DrawSmoke();
}



//=============================================================================
// パーツ半径設定用
//=============================================================================
int EnemyParts::ColGravitySpace(XMFLOAT3 f3Pos)
{
	for (int i = 0; i < MAX_ENEMY_PARTS; ++i) {
		if (!m_bStat[i] || m_fall[i])
			continue;

		if (CollisionRectToPoint(m_GravitySpace[i].f3Pos, m_GravitySpace[i].f3Scl, f3Pos)) {
			return 1;
		}
	}
	return -1;
}

//=============================================================================
// コントローラーオブジェクト取得
//=============================================================================
void SetControllerrEnemyParts(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}