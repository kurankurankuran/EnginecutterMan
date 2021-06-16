//=============================================================================
//
// タイトル画面処理 [GAME CLEAR.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitClear();
void UninitClear();
void UpdateClear();
void DrawClear();

void SetClearControllerr(XINPUT_STATE* pControlller);
void GetKey(void);