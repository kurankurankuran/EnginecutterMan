//=============================================================================
//
// ゲーム画面 [rushScene.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "camera.h"
#include "Light.h"


HRESULT InitRush(void);
void UninitRush(void);
void UpdateRush(void);
void DrawRush(void);

void SetRushCamera(CCamera*);
void SetRushLight(CLight*);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

