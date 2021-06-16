//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "model.h"
#include "rushPlayer.h"
#include "camera.h"
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);

int SpawnParticle(XMFLOAT3 pos, XMFLOAT3 dir);

void SetParticlePlayer(CPlayer* pPlayer);
void SetParticleRushPlayer(CRushPlayer* pPlayer);

void SetParticleCamera(CCamera* pCamera);