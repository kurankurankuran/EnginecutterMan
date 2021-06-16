//=============================================================================
//
// ���S�G�t�F�N�g���� [deatheffect.h]
// Author : �O��@�G���Y
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"
#include "camera.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDeathEffect(void);
void UninitDeathEffect(void);
void UpdateDeathEffect(void);
void DrawDeathEffect(void);

int SetDeathEffect(XMFLOAT3 pos, float fSizeX, float fSizeY);
//void SetExplosionPlayer(CPlayer* pPlayer);

void SetDeathEffectCamera(CCamera* pCamera);