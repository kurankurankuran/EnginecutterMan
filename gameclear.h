//=============================================================================
//
// �Q�[���N���A��ʏ��� [gameclear.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGameClear();
void UninitGameClear();
void UpdateGameClear();
void DrawGameClear();
void SetGameClearControllerr(XINPUT_STATE* pControlller);