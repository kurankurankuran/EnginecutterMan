//=============================================================================
//
// ゲームクリア画面処理 [gameclear.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGameClear();
void UninitGameClear();
void UpdateGameClear();
void DrawGameClear();
void SetGameClearControllerr(XINPUT_STATE* pControlller);