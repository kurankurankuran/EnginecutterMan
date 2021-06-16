//=============================================================================
//
// �q�b�g�G�t�F�N�g���� [hiteffect.h]
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
HRESULT InitHitEffect(void);
void UninitHitEffect(void);
void UpdateHitEffect(void);
void DrawHitEffect(void);

int SetHitEffect(XMFLOAT3 pos, float fSizeX, float fSizeY);
//void SetExplosionPlayer(CPlayer* pPlayer);

void SetHitEffectCamera(CCamera* pCamera);