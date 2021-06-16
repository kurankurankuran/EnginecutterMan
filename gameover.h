//=============================================================================
//
// ゲームオーバー画面処理 [gameover.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGameOver();
void UninitGameOver();
void UpdateGameOver();
void DrawGameOver();
void SetGameOverControllerr(XINPUT_STATE* pControlller);
