//=============================================================================
//
// �Q�[���I�[�o�[��ʏ��� [gameover.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGameOver();
void UninitGameOver();
void UpdateGameOver();
void DrawGameOver();
void SetGameOverControllerr(XINPUT_STATE* pControlller);
