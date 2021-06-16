//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>

// �|�[�Y���j���[
typedef enum
{
	GAME_START = 0,		// �Q�[���X�^�[�g
	TUTORIAL_START,		// �`���[�g���A��
	GAME_END,				// �Q�[���I��

	TITLE_MENU_MAX
} TITLE_MENU;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle();
void UninitTitle();
void UpdateTitle();
void DrawTitle();

void SetTitleMenu(void);
TITLE_MENU GetTitleMenu(void);
void SetTitleControllerr(XINPUT_STATE* pControlller);