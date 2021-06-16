//=============================================================================
//
// ゲーム画面 [game.cpp]
// Author : RYUSUKE	YAMADA
//-----------------------------------------------------------------------------
// 2020/12/23 SetMeshLight(&g_light);をSetMeshLight(CLight::Get());に変更したように
//			  (&g_light)になってるところを(CLight::Get())に変更しました。三宅
//=============================================================================

// ****** インクルード部 ******
#include "game.h"
#include "fade.h"
#include "enemyVacuum.h"
#include "mesh.h"
#include "scene.h"
#include "input.h"
#include "particle.h"
#include "model.h"
#include "changeSpace.h"
//#include "Enemylife.h"
#include "field.h"
#include "explosion.h"
#include "CLEAR.h"
#include "hiteffect.h"
#include "pause.h"
#include "Sound.h"
#include "deatheffect.h"
#include "CHitStop.h"
#include "bilibili.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int		        g_nFade = FADE_IN;
static bool	            g_bPause = false;				// 一時停止中
static bool	            g_bBGM = false;				// BGM再生中

//static CCamera*				g_camera;				// ゲームカメラ
//static CLight					g_light;					// ゲーム光源

CPlayer*				g_Player;					// プレイヤー
static CEnemyVacuum           g_EnemyVacuum;	// 敵(掃除機型)
//static CEnemyVacuum           g_InEnemyVacuum;	// 敵(掃除機型)
CField*                 g_pField;   // フィールド

static bool   g_bInstansGenerate = true;

static XINPUT_STATE* g_pXController;
static bool  g_bNowPress;
static bool  g_bOldPress;

//=============================================================================
// ゲーム画面の初期化処理
//=============================================================================
HRESULT InitGame()
{
	g_bNowPress = false;
	g_bOldPress = false;
	HRESULT hr = S_OK;

	//// メッシュ初期化
	//hr = InitMesh();
	//if (FAILED(hr))
	//	return hr;

	//g_camera = new CCamera;
	CCamera::Get()->Init();
   	g_Player = new CPlayer(30);
	
	
	
	SetMeshCamera(CCamera::Get());
	SetMeshLight(CLight::Get());

	if (g_bInstansGenerate) {
		
		g_pField = new CField;
		//g_EnemyVacuum = new CEnemyVacuum;
		g_EnemyVacuum.InitEnemy();
		//g_EnemyVacuum->InitEnemy();
		//g_InEnemyVacuum.InitEnemy();
	}
	else
	{
		g_pField->Init();
		//g_EnemyVacuum->InitEnemy();
	}
	//*************************************************
	/*g_EnemyVacuum.SetCamera(CCamera::Get());
	g_EnemyVacuum.SetVacuumEnemyLight(CLight::Get());*/
	g_EnemyVacuum.SetEnemyPlayer(g_Player);
	/*g_EnemyVacuum->SetCamera(CCamera::Get());
	g_EnemyVacuum->SetVacuumEnemyLight(CLight::Get());
	g_EnemyVacuum->SetEnemyPlayer(g_Player);*/
	/*g_InEnemyVacuum.SetCamera(CCamera::Get());
	g_InEnemyVacuum.SetVacuumEnemyLight(CLight::Get());
	g_InEnemyVacuum.SetEnemyPlayer(g_Player);*/
	//**************************************************
	//g_Player->SetPlayerCamera(CCamera::Get());
	//g_Player->SetPlayerLight(CLight::Get());
	g_Player->SetPlayerEnemy(&g_EnemyVacuum);
	/*g_Player->SetPlayerEnemy(g_EnemyVacuum);*/
	/*g_Player->SetPlayerEnemy(&g_InEnemyVacuum);*/

	

	//*************************************************:
	SetChangeSpacePlayer(g_Player);
	SetChangeSpaceEnemyVacuum(&g_EnemyVacuum);
	//SetChangeSpaceEnemyVacuum(g_EnemyVacuum);
	/*SetChangeSpaceEnemyVacuum(&g_InEnemyVacuum);*/
	//**************************************************
	//g_pField->SetCamera(CCamera::Get());
	//g_pField->SetLight(CLight::Get());
	
	//**************************************************
	InitExplosion();
	SetExplosionCamera(CCamera::Get());
	
	////******************************************************
	// パーティクル初期化
	hr = InitParticle();
	if (FAILED(hr))
		return hr;
	SetParticlePlayer(g_Player);
	SetParticleCamera(CCamera::Get());
	////******************************************************
	CCamera::Get()->SetCameraEnemy(&g_EnemyVacuum);
	/*CCamera::Get()->SetCameraEnemy(g_EnemyVacuum);*/
	//CCamera::Get()->SetCameraEnemy(&g_InEnemyVacuum);
	CCamera::Get()->SetCameraPlayer(g_Player);

	//InitClear();

	// エフェクト初期化
	InitHitEffect();
	SetHitEffectCamera(CCamera::Get());

	// ビリビリ初期化
	InitBilibili();
	SetBilibiliCamera(CCamera::Get());

	// 死亡エフェクト初期化
	InitDeathEffect();
	SetDeathEffectCamera(CCamera::Get());



	// ポーズ初期化
	hr = InitPause();
	if (FAILED(hr))
		return hr;
	g_bPause = false;


	// ヒットストップ初期化
	CHitStop::Get()->Init();

	return hr;
}


//=============================================================================
// ゲーム画面の終了処理
//=============================================================================
void UninitGame()
{
	// ヒットストップ終了処理
	CHitStop::Get()->Uninit();

	// 一時停止終了処理
	UninitPause();

	if (g_bBGM)
	{
		// BGM停止
		g_bBGM = false;
		CSound::Stop(BGM_GAME1);
	}

	// 死亡エフェクト終了
	UninitDeathEffect();

	// ビリビリ終了
	UninitBilibili();

	// ヒットエフェクト終了
	UninitHitEffect();
	
	// パーティクル終了処理
	UninitParticle();

	

	UninitExplosion();

	if (g_bInstansGenerate) {
		delete g_pField;
		//delete g_EnemyVacuum;
		g_EnemyVacuum.UninitEnemy();
		/*g_EnemyVacuum->UninitEnemy();*/
		//g_InEnemyVacuum.UninitEnemy();
		
	}
	else {
		g_pField->Uninit();
		//g_EnemyVacuum->UninitEnemy();
	}
	delete g_Player;
	//delete CCamera::Get();
 
	//// メッシュ終了処理
	//UninitMesh();
}

//=============================================================================
// ゲーム画面の更新処理
//=============================================================================
void UpdateGame()
{
	g_bOldPress = g_bNowPress;



	if (!g_bBGM)
	{
		// フェードインが完了したらBGM再生
		int nFade = GetFade();
		if (g_nFade == FADE_IN && nFade == FADE_NONE)
		{
			// BGM再生開始
			g_bBGM = true;
			CSound::Play(BGM_GAME1);
		}
		g_nFade = nFade;
	}

	if (g_bPause)
	{
		// 一時停止更新
		UpdatePause();
	}
	else
	{
		if (!(CHitStop::Get()->GetHitStopflg())) {

			// フィールド更新
			g_pField->Update();

			if (GetWorldMoveflg()) {
				// モデル更新
				g_Player->Update();

				// 敵更新	
				g_EnemyVacuum.UpdateEnemy();
				/*g_EnemyVacuum->UpdateEnemy();*/
				/*g_InEnemyVacuum.UpdateEnemy();*/

				// エクスプローション更新
				UpdateExplosion();

				// パーティクル更新
				UpdateParticle();

				// ヒットエフェクト更新
				UpdateHitEffect();

				// ビリビリエフェクト更新
				UpdateBilibili();

				// 死亡エフェクト更新
				UpdateDeathEffect();
			}
			else
			{
				switch (CCamera::Get()->GetUpdateIns())
				{
				case INS_CENEMY:
					g_EnemyVacuum.UpdateEnemy();
					//g_EnemyVacuum->UpdateEnemy();
					//g_InEnemyVacuum.UpdateEnemy();
					break;
				}
			}


			// カメラの更新
			CCamera::Get()->Update();
		}

		CHitStop::Get()->Update();
	}

	// ポーズON/OFF
#if SETCONTROLLER
	if (g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y) {
		g_bNowPress = true;
	}
	else
	{
		g_bNowPress = false;
	}
#endif
		if ((!g_bNowPress&&g_bOldPress||GetKeyTrigger(VK_P) || GetKeyTrigger(VK_PAUSE)
			))
		{
			if (GetFade() == FADE_NONE)
			{
				g_bPause = !g_bPause;
				if (g_bPause)
				{
					CSound::Pause();
					//CSound::Play(SE_DECIDE);	// 効果音再生
					ResetPauseMenu();
				}
				else
				{
					//CSound::Play(SE_CANCEL);	// 効果音再生
					CSound::Resume();
				}
			}
		}

	// ポーズ中
	if (g_bPause)
	{
		if (GetFade() == FADE_NONE)
		{
			if (GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE)
#if SETCONTROLLER
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_B
#endif
				)
			{
				switch (GetPauseMenu())
				{
				case PAUSE_MENU_CONTINUE:
					g_bPause = !g_bPause;
					//CSound::Play(SE_CANCEL);	// 効果音再生
					CSound::Resume();
					break;
				case PAUSE_MENU_RETRY:
					SetFadeOut(SCENE_GAME);
					break;
				case PAUSE_MENU_QUIT:
					SetFadeOut(SCENE_TITLE);
					break;
				}
			}
		}
	}
}

//=============================================================================
// ゲーム画面の描画処理
//=============================================================================
void DrawGame()
{
	//フィールド描画
	g_pField->Draw();

	// 敵描画
	g_EnemyVacuum.DrawEnemy();
	//g_EnemyVacuum->DrawEnemy();
	//g_InEnemyVacuum.DrawEnemy();

	// モデル描画
	g_Player->Draw();

	DrawExplosion();

	// パーティクル描画
	DrawParticle();

	// ヒットエフェクト描画
	DrawHitEffect();

	// ビリビリエフェクト描画
	DrawBilibili();

	// 死亡エフェクト描画
	DrawDeathEffect();

	// ポーズ処理の描画
	if (g_bPause) {
		DrawPause();
	}
}


//=============================================================================
// コントローラーオブジェクト取得
//=============================================================================
void SetGameControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}

