//=============================================================================
//
// 爆発処理 [bilibili.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"
#include "camera.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBilibili(void);
void UninitBilibili(void);
void UpdateBilibili(void);
void DrawBilibili(void);

int SetBilibili(XMFLOAT3 pos, float fSizeX, float fSizeY);
//void SetBilibiliPlayer(CPlayer* pPlayer);

void SetBilibiliCamera(CCamera* pCamera);