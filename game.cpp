//=============================================================================
//
// �Q�[����� [game.cpp]
// Author : RYUSUKE	YAMADA
//-----------------------------------------------------------------------------
// 2020/12/23 SetMeshLight(&g_light);��SetMeshLight(CLight::Get());�ɕύX�����悤��
//			  (&g_light)�ɂȂ��Ă�Ƃ����(CLight::Get())�ɕύX���܂����B�O��
//=============================================================================

// ****** �C���N���[�h�� ******
#include "game.h"
#include "fade.h"
#include "enemyVacuum.h"
#include "mesh.h"
#include "scene.h"
#include "input.h"
#include "particle.h"
#include "model.h"
#include "changeSpace.h"
//#include "Enemylife.h"
#include "field.h"
#include "explosion.h"
#include "CLEAR.h"
#include "hiteffect.h"
#include "pause.h"
#include "Sound.h"
#include "deatheffect.h"
#include "CHitStop.h"
#include "bilibili.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int		        g_nFade = FADE_IN;
static bool	            g_bPause = false;				// �ꎞ��~��
static bool	            g_bBGM = false;				// BGM�Đ���

//static CCamera*				g_camera;				// �Q�[���J����
//static CLight					g_light;					// �Q�[������

CPlayer*				g_Player;					// �v���C���[
static CEnemyVacuum           g_EnemyVacuum;	// �G(�|���@�^)
//static CEnemyVacuum           g_InEnemyVacuum;	// �G(�|���@�^)
CField*                 g_pField;   // �t�B�[���h

static bool   g_bInstansGenerate = true;

static XINPUT_STATE* g_pXController;
static bool  g_bNowPress;
static bool  g_bOldPress;

//=============================================================================
// �Q�[����ʂ̏���������
//=============================================================================
HRESULT InitGame()
{
	g_bNowPress = false;
	g_bOldPress = false;
	HRESULT hr = S_OK;

	//// ���b�V��������
	//hr = InitMesh();
	//if (FAILED(hr))
	//	return hr;

	//g_camera = new CCamera;
	CCamera::Get()->Init();
   	g_Player = new CPlayer(30);
	
	
	
	SetMeshCamera(CCamera::Get());
	SetMeshLight(CLight::Get());

	if (g_bInstansGenerate) {
		
		g_pField = new CField;
		//g_EnemyVacuum = new CEnemyVacuum;
		g_EnemyVacuum.InitEnemy();
		//g_EnemyVacuum->InitEnemy();
		//g_InEnemyVacuum.InitEnemy();
	}
	else
	{
		g_pField->Init();
		//g_EnemyVacuum->InitEnemy();
	}
	//*************************************************
	/*g_EnemyVacuum.SetCamera(CCamera::Get());
	g_EnemyVacuum.SetVacuumEnemyLight(CLight::Get());*/
	g_EnemyVacuum.SetEnemyPlayer(g_Player);
	/*g_EnemyVacuum->SetCamera(CCamera::Get());
	g_EnemyVacuum->SetVacuumEnemyLight(CLight::Get());
	g_EnemyVacuum->SetEnemyPlayer(g_Player);*/
	/*g_InEnemyVacuum.SetCamera(CCamera::Get());
	g_InEnemyVacuum.SetVacuumEnemyLight(CLight::Get());
	g_InEnemyVacuum.SetEnemyPlayer(g_Player);*/
	//**************************************************
	//g_Player->SetPlayerCamera(CCamera::Get());
	//g_Player->SetPlayerLight(CLight::Get());
	g_Player->SetPlayerEnemy(&g_EnemyVacuum);
	/*g_Player->SetPlayerEnemy(g_EnemyVacuum);*/
	/*g_Player->SetPlayerEnemy(&g_InEnemyVacuum);*/

	

	//*************************************************:
	SetChangeSpacePlayer(g_Player);
	SetChangeSpaceEnemyVacuum(&g_EnemyVacuum);
	//SetChangeSpaceEnemyVacuum(g_EnemyVacuum);
	/*SetChangeSpaceEnemyVacuum(&g_InEnemyVacuum);*/
	//**************************************************
	//g_pField->SetCamera(CCamera::Get());
	//g_pField->SetLight(CLight::Get());
	
	//**************************************************
	InitExplosion();
	SetExplosionCamera(CCamera::Get());
	
	////******************************************************
	// �p�[�e�B�N��������
	hr = InitParticle();
	if (FAILED(hr))
		return hr;
	SetParticlePlayer(g_Player);
	SetParticleCamera(CCamera::Get());
	////******************************************************
	CCamera::Get()->SetCameraEnemy(&g_EnemyVacuum);
	/*CCamera::Get()->SetCameraEnemy(g_EnemyVacuum);*/
	//CCamera::Get()->SetCameraEnemy(&g_InEnemyVacuum);
	CCamera::Get()->SetCameraPlayer(g_Player);

	//InitClear();

	// �G�t�F�N�g������
	InitHitEffect();
	SetHitEffectCamera(CCamera::Get());

	// �r���r��������
	InitBilibili();
	SetBilibiliCamera(CCamera::Get());

	// ���S�G�t�F�N�g������
	InitDeathEffect();
	SetDeathEffectCamera(CCamera::Get());



	// �|�[�Y������
	hr = InitPause();
	if (FAILED(hr))
		return hr;
	g_bPause = false;


	// �q�b�g�X�g�b�v������
	CHitStop::Get()->Init();

	return hr;
}


//=============================================================================
// �Q�[����ʂ̏I������
//=============================================================================
void UninitGame()
{
	// �q�b�g�X�g�b�v�I������
	CHitStop::Get()->Uninit();

	// �ꎞ��~�I������
	UninitPause();

	if (g_bBGM)
	{
		// BGM��~
		g_bBGM = false;
		CSound::Stop(BGM_GAME1);
	}

	// ���S�G�t�F�N�g�I��
	UninitDeathEffect();

	// �r���r���I��
	UninitBilibili();

	// �q�b�g�G�t�F�N�g�I��
	UninitHitEffect();
	
	// �p�[�e�B�N���I������
	UninitParticle();

	

	UninitExplosion();

	if (g_bInstansGenerate) {
		delete g_pField;
		//delete g_EnemyVacuum;
		g_EnemyVacuum.UninitEnemy();
		/*g_EnemyVacuum->UninitEnemy();*/
		//g_InEnemyVacuum.UninitEnemy();
		
	}
	else {
		g_pField->Uninit();
		//g_EnemyVacuum->UninitEnemy();
	}
	delete g_Player;
	//delete CCamera::Get();
 
	//// ���b�V���I������
	//UninitMesh();
}

//=============================================================================
// �Q�[����ʂ̍X�V����
//=============================================================================
void UpdateGame()
{
	g_bOldPress = g_bNowPress;



	if (!g_bBGM)
	{
		// �t�F�[�h�C��������������BGM�Đ�
		int nFade = GetFade();
		if (g_nFade == FADE_IN && nFade == FADE_NONE)
		{
			// BGM�Đ��J�n
			g_bBGM = true;
			CSound::Play(BGM_GAME1);
		}
		g_nFade = nFade;
	}

	if (g_bPause)
	{
		// �ꎞ��~�X�V
		UpdatePause();
	}
	else
	{
		if (!(CHitStop::Get()->GetHitStopflg())) {

			// �t�B�[���h�X�V
			g_pField->Update();

			if (GetWorldMoveflg()) {
				// ���f���X�V
				g_Player->Update();

				// �G�X�V	
				g_EnemyVacuum.UpdateEnemy();
				/*g_EnemyVacuum->UpdateEnemy();*/
				/*g_InEnemyVacuum.UpdateEnemy();*/

				// �G�N�X�v���[�V�����X�V
				UpdateExplosion();

				// �p�[�e�B�N���X�V
				UpdateParticle();

				// �q�b�g�G�t�F�N�g�X�V
				UpdateHitEffect();

				// �r���r���G�t�F�N�g�X�V
				UpdateBilibili();

				// ���S�G�t�F�N�g�X�V
				UpdateDeathEffect();
			}
			else
			{
				switch (CCamera::Get()->GetUpdateIns())
				{
				case INS_CENEMY:
					g_EnemyVacuum.UpdateEnemy();
					//g_EnemyVacuum->UpdateEnemy();
					//g_InEnemyVacuum.UpdateEnemy();
					break;
				}
			}


			// �J�����̍X�V
			CCamera::Get()->Update();
		}

		CHitStop::Get()->Update();
	}

	// �|�[�YON/OFF
#if SETCONTROLLER
	if (g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y) {
		g_bNowPress = true;
	}
	else
	{
		g_bNowPress = false;
	}
#endif
		if ((!g_bNowPress&&g_bOldPress||GetKeyTrigger(VK_P) || GetKeyTrigger(VK_PAUSE)
			))
		{
			if (GetFade() == FADE_NONE)
			{
				g_bPause = !g_bPause;
				if (g_bPause)
				{
					CSound::Pause();
					//CSound::Play(SE_DECIDE);	// ���ʉ��Đ�
					ResetPauseMenu();
				}
				else
				{
					//CSound::Play(SE_CANCEL);	// ���ʉ��Đ�
					CSound::Resume();
				}
			}
		}

	// �|�[�Y��
	if (g_bPause)
	{
		if (GetFade() == FADE_NONE)
		{
			if (GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE)
#if SETCONTROLLER
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_B
#endif
				)
			{
				switch (GetPauseMenu())
				{
				case PAUSE_MENU_CONTINUE:
					g_bPause = !g_bPause;
					//CSound::Play(SE_CANCEL);	// ���ʉ��Đ�
					CSound::Resume();
					break;
				case PAUSE_MENU_RETRY:
					SetFadeOut(SCENE_GAME);
					break;
				case PAUSE_MENU_QUIT:
					SetFadeOut(SCENE_TITLE);
					break;
				}
			}
		}
	}
}

//=============================================================================
// �Q�[����ʂ̕`�揈��
//=============================================================================
void DrawGame()
{
	//�t�B�[���h�`��
	g_pField->Draw();

	// �G�`��
	g_EnemyVacuum.DrawEnemy();
	//g_EnemyVacuum->DrawEnemy();
	//g_InEnemyVacuum.DrawEnemy();

	// ���f���`��
	g_Player->Draw();

	DrawExplosion();

	// �p�[�e�B�N���`��
	DrawParticle();

	// �q�b�g�G�t�F�N�g�`��
	DrawHitEffect();

	// �r���r���G�t�F�N�g�`��
	DrawBilibili();

	// ���S�G�t�F�N�g�`��
	DrawDeathEffect();

	// �|�[�Y�����̕`��
	if (g_bPause) {
		DrawPause();
	}
}


//=============================================================================
// �R���g���[���[�I�u�W�F�N�g�擾
//=============================================================================
void SetGameControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}

