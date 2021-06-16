#pragma once
#include "main.h"
//-------	XInput.h �Z�b�g	--------//
//#include <Windows.h>				//.�{����<Windows.h>���K�v������"main.h"�ɂ���̂ŏȗ�
#include <Xinput.h>					//.<XInput.h>�͓����������ɂ̂�Include�ŏ\��
#pragma comment(lib, "xinput.lib")	//
//----------------------------------//
#include "model.h"
#include "engine.h"
#include "title.h"
#include "StageSelect.h"
#include "gameclear.h"
#include "gameover.h"
#include "rushPlayer.h"
#include "enemyParts.h"
#include "pause.h"
#include "game.h"
//.			�\����			.//

//.XINPUT_STATE �̃C���X�^���X��p��
XINPUT_STATE			Xstate;
//---	XINPUT_STATE ����	---//
//	dwPacketNumber	->	�R���g���[���[�̎��ʔԍ��i�PP,�QP�Ȃǁj�A�ő�S�i�O�`�R�j�܂Őڑ��\
//	Gamepad			->	�R���g���[���[�̃{�^���̏��i�ǂ��̃{�^����������Ă��邩�Ȃǁj


//.XINPUT_VIBRATION �̃C���X�^���X��p��
XINPUT_VIBRATION		Vibration;	//.�o�C�u���[�V�����@�\���g�p���Ȃ��ꍇ�͕s�v
//---	XINPUT_VIBRATION ����	---//
//	wLeftMotorSpeed		->	���̃o�C�u���[�V�����i���[�^�[�j�̋���
//	wRightMotorSpeed	->	�E�̃o�C�u���[�V�����i���[�^�[�j�̋���

//.			�萔��`		.//

#define MAX_THUMB		32700
#define MAX_PLAYER		(4)

//.		�O���[�o����		.//
using namespace			std;		//.iostream�p
bool					Continue;	//.main�֐����[�v�p

// 		�G���g���|�C���g	 //
//.�R���g���[���[�̏�����
void InitController()
{
	//.�R���g���[���[�̃��������������m��
	ZeroMemory(&Xstate, sizeof(XINPUT_STATE));
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	DWORD dwResult = XInputGetState(0, &Xstate);		//.�P�o�i�O�Ԗځj�̃n���h���l��
	DWORD dwResultV = XInputSetState(0, &Vibration);

	if (dwResult == ERROR_SUCCESS)					//.�R���g���[���[���ڑ�����Ă��邩�ǂ���
	{//.�R���g���[���[���ڑ�����Ă���ꍇ
		// Controller is connected
		SetControllerr(&Xstate);
		SetEnzineControllerr(&Xstate, &Vibration);
		SetTitleControllerr(&Xstate);
		SetCameraControllerr(&Xstate);
		SetKirimeControllerr(&Xstate, &Vibration);
		SetStageSelectControllerr(&Xstate);
		SetGameClearControllerr(&Xstate);
		SetGameOverControllerr(&Xstate);
		SetRushPlayerController(&Xstate);
		SetControllerrEnemyParts(&Xstate);
		SetPauseControllerr(&Xstate);
		SetGameControllerr(&Xstate);

	}
	else
	{//.�R���g���[���[���ڑ�����Ă��Ȃ��ꍇ
		MessageBox(NULL, _T("�R���g���[���[���ڑ�����Ă��܂���"), _T("WARNING"), MB_OK);
	}


}

//.�R���g���[���[�̏I��
void UnInitController()
{//.�R���g���[���[�̏I�������B
	XInputEnable(false);	//�o�C�u���[�V�������g�p����ꍇ�͕K�{�B
}

void UpdateController()
{
	DWORD dwResult = XInputGetState(0, &Xstate);
	DWORD dwResultV = XInputSetState(0, &Vibration);
}

XINPUT_STATE* GetXinputState(void)
{
	return &Xstate;
}

XINPUT_VIBRATION* GetXinputVibration(void)
{
	return  &Vibration; 
}

