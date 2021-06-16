//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"

// �t�F�[�h����
enum EFade
{
	FADE_NONE = 0,		// �������Ă��Ȃ�
	FADE_IN,						// �t�F�[�h�C������
	FADE_OUT,					// �t�F�[�h�A�E�g����
	
	MAX_FADE
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFade();
void UninitFade();
void UpdateFade();
void DrawFade();

void SetFadeOut(int nNextScene);						// �t�F�[�h�A�E�g�J�n(���̃V�[��)
int	GetFade();														// �t�F�[�h�C�� / �A�E�g�̏��
void SetFadeColor(float fR, float fG, float fB);	// �t�F�[�h�A�E�g���̉�ʂ̐F