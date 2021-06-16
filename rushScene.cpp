//=============================================================================
//
// ゲーム画面 [game.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================

// ****** インクルード部 ******
#include "game.h"
#include "fade.h"
#include "enemyRushVacuum.h"
#include "mesh.h"
#include "scene.h"
#include "input.h"
#include "particle.h"
#include "rushPlayer.h"
#include "changeSpace.h"
//#include "Enemylife.h"
#include "field.h"
#include "explosion.h"
#include "CHitStop.h"
#include "deatheffect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int		                   g_nFade = FADE_IN;
static bool	                       g_bPause = false;				// 一時停止中
static bool	                       g_bBGM = false;				// BGM再生中

//static CCamera*				       g_camera;				// ゲームカメラ
//static CLight					   g_light;					// ゲーム光源

static CRushPlayer*	     		   g_Player;					// プレイヤー
static CEnemyRushVacuum*           g_EnemyRushVacuum;	// 敵(掃除機型)
CField*                     g_Field;   // フィールド

//=============================================================================
// ゲーム画面の初期化処理
//=============================================================================
HRESULT InitRush()
{
	HRESULT hr = S_OK;

	//// メッシュ初期化
	//	hr = InitMesh();
	//if (FAILED(hr))
	//	return hr;

	//g_camera = new CCamera;
	g_Player = new CRushPlayer(30);
	g_Field = new CField;

	CCamera::Get()->Init();

	SetMeshCamera(CCamera::Get()/*g_camera*/);
	SetMeshLight(CLight::Get()/*&g_light*/);

	g_EnemyRushVacuum = new CEnemyRushVacuum;

	//*************************************************
	//g_EnemyRushVacuum->SetCamera(g_camera);
	//g_EnemyRushVacuum->SetVacuumEnemyLight(CLight::Get()/*&g_light*/);

	//**************************************************
	g_Player->SetPlayerCamera(CCamera::Get()/*g_camera*/);
	g_Player->SetPlayerLight(CLight::Get()/*&g_light*/);
	g_Player->SetPlayerEnemy(g_EnemyRushVacuum);

	//**************************************************
	InitExplosion();
	SetExplosionCamera(CCamera::Get()/*g_camera*/);
	//**************************************************
	//g_Field->SetCamera(g_camera);
	//g_Field->SetLight(CLight::Get()/*&g_light*/);
	//**************************************************
	
	////******************************************************
	// パーティクル初期化
	hr = InitParticle();
	if (FAILED(hr))
		return hr;
	SetParticleRushPlayer(g_Player);
	SetParticleCamera(CCamera::Get()/*g_camera*/);
	////******************************************************
	CCamera::Get()->SetCameraRushEnemy(g_EnemyRushVacuum);
	CCamera::Get()->SetCameraRushPlayer(g_Player);

	//********************************************************
	// ヒットストップ初期化
	CHitStop::Get()->Init();

	// 死亡エフェクト初期化
	InitDeathEffect();
	SetDeathEffectCamera(CCamera::Get());

	return hr;
}


//=============================================================================
// ゲーム画面の終了処理
//=============================================================================
void UninitRush()
{
	// 死亡エフェクト終了
	UninitDeathEffect();

	// ヒットストップ終了処理
	CHitStop::Get()->Uninit();
	
	// パーティクル終了処理
	UninitParticle();

	delete g_Field;

	UninitExplosion();

	delete g_EnemyRushVacuum;	
	delete g_Player;
	//delete g_camera;
	//// メッシュ終了処理
	//UninitMesh();
}

//=============================================================================
// ゲーム画面の更新処理
//=============================================================================
void UpdateRush()
{
	if (!(CHitStop::Get()->GetHitStopflg())) {
		// フィールド更新
		g_Field->Update();

		// モデル更新
		g_Player->Update();

		// 敵更新	
		g_EnemyRushVacuum->UpdateEnemy();

		UpdateExplosion();

		// 死亡エフェクト更新
		UpdateDeathEffect();

		// パーティクル更新
		UpdateParticle();

		// カメラの高更新
		CCamera::Get()->Update();
	}
	CHitStop::Get()->Update();
	if (GetKeyTrigger(VK_P))
	{
		SetFadeOut(SCENE_TITLE);
	}
}

//=============================================================================
// ゲーム画面の描画処理
//=============================================================================
void DrawRush()
{
	//フィールド描画
	g_Field->Draw();

	// 敵描画
	g_EnemyRushVacuum->DrawEnemy();

	// モデル描画
	g_Player->Draw();

	DrawExplosion();

	// パーティクル描画
	DrawParticle();

	// 死亡エフェクト描画
	DrawDeathEffect();

	//// 敵ライフ描画
	//DrawEnemyLife();	
}

