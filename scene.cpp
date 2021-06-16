//=============================================================================
//
// �V�[���J�ڏ��� [scene.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================

// ===========================
// �C���N���[�h��
// ===========================
#include "scene.h"
#include "enemyVacuum.h"
#include "title.h"
#include "game.h"
#include "StageSelect.h"
#include "gameclear.h"
#include "gameover.h"
#include "rushScene.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_nScene;		// ���݂̃V�[��
//static int g_gScene;		// ���݂̃V�[���Q

//=============================================================================
// �V�[���̏���������
//=============================================================================
HRESULT InitScene()
{
	SetScene(SCENE_TITLE);		// �ŏ��ɕ\������V�[��(�^�C�g��)
	return S_OK;
}

//=============================================================================
// �V�[���̏I������
//=============================================================================
void UninitScene()
{
	SetScene(SCENE_NONE);		// ���݂̃V�[�����I��
	//SetScene(SCENE_NONE1);		// ���݂̃V�[�����I��
}

//=============================================================================
// �V�[���̍X�V����
//=============================================================================
void UpdateScene()
{
	switch (g_nScene)
	{
		// �^�C�g�����
	case SCENE_TITLE:
		UpdateTitle();
		break;

		// �X�e�[�W�Z���N�g���
	case SCENE_STAGE_SELECT:
		UpdateStageSelect();
		break;

		// �Q�[�����
	case SCENE_GAME:
		UpdateGame();
		break;

		// ���b�V�����
	case SCENE_RUSH:
		UpdateRush();
		break;
		// �N���A���
	case SCENE_GAMECLEAR:
		UpdateGameClear();
		break;
		// �I�[�o�[���
	case SCENE_GAMEOVER:
		UpdateGameOver();
		break;
	}

	//switch (g_gScene)
	//{
		// �X�e�[�W�P
	//case SCENE_STAGE1:
		
		// �X�e�[�W�Q
	//case SCENE_STAGE2:

		// �X�e�[�W�R
	//case SCENE_STAGE3:

//	}
}

//=============================================================================
// �V�[���J�ڏ���
//=============================================================================
void DrawScene()
{
	switch (g_nScene)
	{
		// �^�C�g�����
	case SCENE_TITLE:
		DrawTitle();
		break;

		// �X�e�[�W�Z���N�g�`��
	case SCENE_STAGE_SELECT:
		DrawStageSelect();
		break;

		// �Q�[�����
	case SCENE_GAME:
		DrawGame();
		break;


		// ���b�V�����
	case SCENE_RUSH:
		DrawRush();
		break;
		// �N���A�[���
	case SCENE_GAMECLEAR:
		DrawGameClear();
		break;
		// �I�[�o�[���
	case SCENE_GAMEOVER:
		DrawGameOver();
		break;
	}
}


//=============================================================================
// �V�[���؂�ւ�菈��
//=============================================================================
void SetScene(int nScene)
{
	// ���݂̃V�[�����I��
	switch (g_nScene)
	{
		// �^�C�g�����
	case SCENE_TITLE:
		UninitTitle();
		break;

		// �X�e�[�W�Z���N�g���
	case SCENE_STAGE_SELECT:
		UninitStageSelect();
		break;

		// �Q�[�����
	case SCENE_GAME:
		UninitGame();
		break;

		// ���b�V�����
	case SCENE_RUSH:
		UninitRush();
		break;	
		// �N���A�[���
	case SCENE_GAMECLEAR:
		UninitGameClear();
		break;	// �Q�[�����
	case SCENE_GAMEOVER:
		UninitGameOver();
		break;
	}
	// �V�[���̐؂�ւ�
	g_nScene = nScene;

	// ���̃V�[���̏�����
	switch (nScene)
	{
		// �^�C�g�����
	case SCENE_TITLE:
		InitTitle();
		break;

		// �X�e�[�W�Z���N�g���
	case SCENE_STAGE_SELECT:
		InitStageSelect();
		break;

		// �Q�[�����
	case SCENE_GAME:
		InitGame();
		break;

		// ���b�V�����
	case SCENE_RUSH:
		InitRush();
		break;
		// �Q�[�����
	case SCENE_GAMECLEAR:
		InitGameClear();
		break;
		// �Q�[�����
	case SCENE_GAMEOVER:
		InitGameOver();
		break;
	}
}

int GetScene(void)
{
	return g_nScene;
}