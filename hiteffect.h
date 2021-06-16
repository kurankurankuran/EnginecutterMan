//=============================================================================
//
// ヒットエフェクト処理 [hiteffect.h]
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
HRESULT InitHitEffect(void);
void UninitHitEffect(void);
void UpdateHitEffect(void);
void DrawHitEffect(void);

int SetHitEffect(XMFLOAT3 pos, float fSizeX, float fSizeY);
//void SetExplosionPlayer(CPlayer* pPlayer);

void SetHitEffectCamera(CCamera* pCamera);