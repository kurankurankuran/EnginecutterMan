//=============================================================================
//
// �������� [explosion.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"
#include "camera.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);

int SetExplosion(XMFLOAT3 pos, float fSizeX, float fSizeY);
//void SetExplosionPlayer(CPlayer* pPlayer);

void SetExplosionCamera(CCamera* pCamera);