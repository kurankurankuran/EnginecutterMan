//=============================================================================
//
// ゲーム画面 [game.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
#include "camera.h"
#include "Light.h"
#include <Xinput.h>		

HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameCamera(CCamera*);
void SetGameLight(CLight*);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void SetGameControllerr(XINPUT_STATE* pControlller);
