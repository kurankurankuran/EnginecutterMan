//=============================================================================
//
// 死亡エフェクト処理 [deatheffect.h]
// Author : 三宅　秀太郎
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"
#include "camera.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDeathEffect(void);
void UninitDeathEffect(void);
void UpdateDeathEffect(void);
void DrawDeathEffect(void);

int SetDeathEffect(XMFLOAT3 pos, float fSizeX, float fSizeY);
//void SetExplosionPlayer(CPlayer* pPlayer);

void SetDeathEffectCamera(CCamera* pCamera);