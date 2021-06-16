//=============================================================================
//
// �X�e�[�W�Z���N�g���� [StageSelect.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================

#pragma once
#include <Xinput.h>
// �|�[�Y���j���[
typedef enum
{
	/* �X�e�[�W�ǉ��̏ꍇ���ɒǋL */
	STAGE_1,	
	STAGE_2,
	STAGE_3,

	STAGE_SELECT_MAX
} STAGE_SELECT_MENU;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitStageSelect();
void UninitStageSelect();
void UpdateStageSelect();
void DrawStageSelect();

void SetStageSelect(void);
STAGE_SELECT_MENU GetStageSelect(void);
void SetStageSelectControllerr(XINPUT_STATE* pControlller);