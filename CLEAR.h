//=============================================================================
//
// �^�C�g����ʏ��� [GAME CLEAR.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitClear();
void UninitClear();
void UpdateClear();
void DrawClear();

void SetClearControllerr(XINPUT_STATE* pControlller);
void GetKey(void);