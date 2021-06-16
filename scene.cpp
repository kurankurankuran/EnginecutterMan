//=============================================================================
//
// シーン遷移処理 [scene.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================

// ===========================
// インクルード部
// ===========================
#include "scene.h"
#include "enemyVacuum.h"
#include "title.h"
#include "game.h"
#include "StageSelect.h"
#include "gameclear.h"
#include "gameover.h"
#include "rushScene.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_nScene;		// 現在のシーン
//static int g_gScene;		// 現在のシーン２

//=============================================================================
// シーンの初期化処理
//=============================================================================
HRESULT InitScene()
{
	SetScene(SCENE_TITLE);		// 最初に表示するシーン(タイトル)
	return S_OK;
}

//=============================================================================
// シーンの終了処理
//=============================================================================
void UninitScene()
{
	SetScene(SCENE_NONE);		// 現在のシーンを終了
	//SetScene(SCENE_NONE1);		// 現在のシーンを終了
}

//=============================================================================
// シーンの更新処理
//=============================================================================
void UpdateScene()
{
	switch (g_nScene)
	{
		// タイトル画面
	case SCENE_TITLE:
		UpdateTitle();
		break;

		// ステージセレクト画面
	case SCENE_STAGE_SELECT:
		UpdateStageSelect();
		break;

		// ゲーム画面
	case SCENE_GAME:
		UpdateGame();
		break;

		// ラッシュ画面
	case SCENE_RUSH:
		UpdateRush();
		break;
		// クリア画面
	case SCENE_GAMECLEAR:
		UpdateGameClear();
		break;
		// オーバー画面
	case SCENE_GAMEOVER:
		UpdateGameOver();
		break;
	}

	//switch (g_gScene)
	//{
		// ステージ１
	//case SCENE_STAGE1:
		
		// ステージ２
	//case SCENE_STAGE2:

		// ステージ３
	//case SCENE_STAGE3:

//	}
}

//=============================================================================
// シーン遷移処理
//=============================================================================
void DrawScene()
{
	switch (g_nScene)
	{
		// タイトル画面
	case SCENE_TITLE:
		DrawTitle();
		break;

		// ステージセレクト描画
	case SCENE_STAGE_SELECT:
		DrawStageSelect();
		break;

		// ゲーム画面
	case SCENE_GAME:
		DrawGame();
		break;


		// ラッシュ画面
	case SCENE_RUSH:
		DrawRush();
		break;
		// クリアー画面
	case SCENE_GAMECLEAR:
		DrawGameClear();
		break;
		// オーバー画面
	case SCENE_GAMEOVER:
		DrawGameOver();
		break;
	}
}


//=============================================================================
// シーン切り替わり処理
//=============================================================================
void SetScene(int nScene)
{
	// 現在のシーンを終了
	switch (g_nScene)
	{
		// タイトル画面
	case SCENE_TITLE:
		UninitTitle();
		break;

		// ステージセレクト画面
	case SCENE_STAGE_SELECT:
		UninitStageSelect();
		break;

		// ゲーム画面
	case SCENE_GAME:
		UninitGame();
		break;

		// ラッシュ画面
	case SCENE_RUSH:
		UninitRush();
		break;	
		// クリアー画面
	case SCENE_GAMECLEAR:
		UninitGameClear();
		break;	// ゲーム画面
	case SCENE_GAMEOVER:
		UninitGameOver();
		break;
	}
	// シーンの切り替え
	g_nScene = nScene;

	// 次のシーンの初期化
	switch (nScene)
	{
		// タイトル画面
	case SCENE_TITLE:
		InitTitle();
		break;

		// ステージセレクト画面
	case SCENE_STAGE_SELECT:
		InitStageSelect();
		break;

		// ゲーム画面
	case SCENE_GAME:
		InitGame();
		break;

		// ラッシュ画面
	case SCENE_RUSH:
		InitRush();
		break;
		// ゲーム画面
	case SCENE_GAMECLEAR:
		InitGameClear();
		break;
		// ゲーム画面
	case SCENE_GAMEOVER:
		InitGameOver();
		break;
	}
}

int GetScene(void)
{
	return g_nScene;
}