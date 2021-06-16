//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>

// ポーズメニュー
typedef enum
{
	GAME_START = 0,		// ゲームスタート
	TUTORIAL_START,		// チュートリアル
	GAME_END,				// ゲーム終了

	TITLE_MENU_MAX
} TITLE_MENU;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle();
void UninitTitle();
void UpdateTitle();
void DrawTitle();

void SetTitleMenu(void);
TITLE_MENU GetTitleMenu(void);
void SetTitleControllerr(XINPUT_STATE* pControlller);