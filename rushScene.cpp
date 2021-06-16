//=============================================================================
//
// �Q�[����� [game.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================

// ****** �C���N���[�h�� ******
#include "game.h"
#include "fade.h"
#include "enemyRushVacuum.h"
#include "mesh.h"
#include "scene.h"
#include "input.h"
#include "particle.h"
#include "rushPlayer.h"
#include "changeSpace.h"
//#include "Enemylife.h"
#include "field.h"
#include "explosion.h"
#include "CHitStop.h"
#include "deatheffect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int		                   g_nFade = FADE_IN;
static bool	                       g_bPause = false;				// �ꎞ��~��
static bool	                       g_bBGM = false;				// BGM�Đ���

//static CCamera*				       g_camera;				// �Q�[���J����
//static CLight					   g_light;					// �Q�[������

static CRushPlayer*	     		   g_Player;					// �v���C���[
static CEnemyRushVacuum*           g_EnemyRushVacuum;	// �G(�|���@�^)
CField*                     g_Field;   // �t�B�[���h

//=============================================================================
// �Q�[����ʂ̏���������
//=============================================================================
HRESULT InitRush()
{
	HRESULT hr = S_OK;

	//// ���b�V��������
	//	hr = InitMesh();
	//if (FAILED(hr))
	//	return hr;

	//g_camera = new CCamera;
	g_Player = new CRushPlayer(30);
	g_Field = new CField;

	CCamera::Get()->Init();

	SetMeshCamera(CCamera::Get()/*g_camera*/);
	SetMeshLight(CLight::Get()/*&g_light*/);

	g_EnemyRushVacuum = new CEnemyRushVacuum;

	//*************************************************
	//g_EnemyRushVacuum->SetCamera(g_camera);
	//g_EnemyRushVacuum->SetVacuumEnemyLight(CLight::Get()/*&g_light*/);

	//**************************************************
	g_Player->SetPlayerCamera(CCamera::Get()/*g_camera*/);
	g_Player->SetPlayerLight(CLight::Get()/*&g_light*/);
	g_Player->SetPlayerEnemy(g_EnemyRushVacuum);

	//**************************************************
	InitExplosion();
	SetExplosionCamera(CCamera::Get()/*g_camera*/);
	//**************************************************
	//g_Field->SetCamera(g_camera);
	//g_Field->SetLight(CLight::Get()/*&g_light*/);
	//**************************************************
	
	////******************************************************
	// �p�[�e�B�N��������
	hr = InitParticle();
	if (FAILED(hr))
		return hr;
	SetParticleRushPlayer(g_Player);
	SetParticleCamera(CCamera::Get()/*g_camera*/);
	////******************************************************
	CCamera::Get()->SetCameraRushEnemy(g_EnemyRushVacuum);
	CCamera::Get()->SetCameraRushPlayer(g_Player);

	//********************************************************
	// �q�b�g�X�g�b�v������
	CHitStop::Get()->Init();

	// ���S�G�t�F�N�g������
	InitDeathEffect();
	SetDeathEffectCamera(CCamera::Get());

	return hr;
}


//=============================================================================
// �Q�[����ʂ̏I������
//=============================================================================
void UninitRush()
{
	// ���S�G�t�F�N�g�I��
	UninitDeathEffect();

	// �q�b�g�X�g�b�v�I������
	CHitStop::Get()->Uninit();
	
	// �p�[�e�B�N���I������
	UninitParticle();

	delete g_Field;

	UninitExplosion();

	delete g_EnemyRushVacuum;	
	delete g_Player;
	//delete g_camera;
	//// ���b�V���I������
	//UninitMesh();
}

//=============================================================================
// �Q�[����ʂ̍X�V����
//=============================================================================
void UpdateRush()
{
	if (!(CHitStop::Get()->GetHitStopflg())) {
		// �t�B�[���h�X�V
		g_Field->Update();

		// ���f���X�V
		g_Player->Update();

		// �G�X�V	
		g_EnemyRushVacuum->UpdateEnemy();

		UpdateExplosion();

		// ���S�G�t�F�N�g�X�V
		UpdateDeathEffect();

		// �p�[�e�B�N���X�V
		UpdateParticle();

		// �J�����̍��X�V
		CCamera::Get()->Update();
	}
	CHitStop::Get()->Update();
	if (GetKeyTrigger(VK_P))
	{
		SetFadeOut(SCENE_TITLE);
	}
}

//=============================================================================
// �Q�[����ʂ̕`�揈��
//=============================================================================
void DrawRush()
{
	//�t�B�[���h�`��
	g_Field->Draw();

	// �G�`��
	g_EnemyRushVacuum->DrawEnemy();

	// ���f���`��
	g_Player->Draw();

	DrawExplosion();

	// �p�[�e�B�N���`��
	DrawParticle();

	// ���S�G�t�F�N�g�`��
	DrawDeathEffect();

	//// �G���C�t�`��
	//DrawEnemyLife();	
}

