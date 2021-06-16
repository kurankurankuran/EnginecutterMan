//=============================================================================
//
// ���f������ [CHomingAttack.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
//#include "main.h"
#include "AssimpModel.h"
#include "CHomingAttack.h"
#include "Collision.h"
#include "hiteffect.h"
#include "model.h"
#include "explosion.h"
#include "Sound.h"
#include "CHitStop.h"

#include "Light.h"
#include "Camera.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define HOMINGATTACK_MODEL			"data/model/car001.fbx"
#define MODEL_HOMINGATKSET		"data/model/Enemy/sphere.fbx"
#define MODEL_HOMINGATKBULLET		"data/model/EnemyCore/LeftHand_Attack.fbx"
#define HOMINGATTACK_MOVE		(1)			// �G�ǔ����x
#define HOMINGATTACK_DAMAGE      (5)        // �_���[�W
#define HOMINGATTACK_FALLTIME    (5*60)    // �ǔ�����
#define HOMINGATTACK_SETY        (5.0f)     // �U���̏����xY 
#define HOMINGATTACK_TIME        (4*60)     // �U�������Ԃ̊Ԋu 
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//CAssimpModel	g_homing[MAX_HOMINGATK];		        // ���f��
//CAssimpModel	g_homingset[MAX_HOMINGATKSETNUM];		// ���f��

CAssimpModel	g_homing;		        // ���f��
CAssimpModel	g_homingset;		// ���f��

static bool g_bModelLoad = false;

CHomingAttack::CHomingAttack()
{

}

CHomingAttack::~CHomingAttack()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CHomingAttack::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���e�͂����Ō��߂Ă悢


	for (int i = 0; i <MAX_HOMINGATKSETNUM; ++i) {
		m_bUseSet[i] = false;
		m_nAtkSetTime[i] = 0;
		m_bUseSet[i] = false;
		m_posSet[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);   // �z�u�ʒu�i�����͈͂̓���j
		m_radSet[i] = 0.0f;   // �_���[�W���̔��a
		m_sclSet[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);   // �����͈͂̑傫���i����X�A�c��Y�A���s��Z�j
		m_rotSet[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);   // ��]�p�x (�΂߂ɂ���Ƃ�����)
		/*m_nLifeSet[i] = 100;*/

		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(m_sclSet[i].x, m_sclSet[i].y, m_sclSet[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotSet[i].x),
			XMConvertToRadians(m_rotSet[i].y), XMConvertToRadians(m_rotSet[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_posSet[i].x, m_posSet[i].y, m_posSet[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorldSet[i], mtxWorld);

		//// ���f���f�[�^�̓ǂݍ���
		//if (!g_homingset.Load(pDevice, pDeviceContext, MODEL_HOMINGATK)) {
		//	hr = E_FAIL;
		//	MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		//	return hr;
		//}
	}
	
	m_bUseSet[0] = true;
	m_posSet[0] = XMFLOAT3(0.0f, 530.0f, 750.0f);
	m_radSet[0] = 40.0f * 2;
	m_sclSet[0] = XMFLOAT3(m_radSet[0] / 2, m_radSet[0] / 2, m_radSet[0] / 2);   // �����͈͂̑傫���i����X�A�c��Y�A���s��Z�j

	m_bUseSet[1] = true;
	m_posSet[1] = XMFLOAT3(0.0f, -530.0f, 750.0f);
	m_radSet[1] = 40.0f * 2;
	m_sclSet[1] = XMFLOAT3(m_radSet[1] / 2, m_radSet[1] / 2, m_radSet[1] / 2);   // �����͈͂̑傫���i����X�A�c��Y�A���s��Z�j

	//m_pLife[0] = new CEnemyLife(m_posSet[0], 100/*m_nLifeSet[0]*/,true);
	//m_pLife[1] = new CEnemyLife(m_posSet[1], 100/*m_nLifeSet[1]*/,true);

	m_Life[0].Init(m_posSet[0], 100/*m_nLifeSet[1]*/, true);
	m_Life[1].Init(m_posSet[1], 100/*m_nLifeSet[1]*/, true);

	//************************************************************************************************:

	for (int i = 0; i < MAX_FALLATK; ++i) {
		m_bUseAtk[i] = false;
		m_pos[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rad[i] = 30.0f;
		m_scl[i] = XMFLOAT3(20.0f, 20.0f, 20.0f);
		m_rot[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_move[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_nDamage[i] = HOMINGATTACK_DAMAGE;
		m_nHomingTime[i] = 20 * 60;
		m_nLife[i] = 1;

		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(m_scl[i].x, m_scl[i].y, m_scl[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rot[i].x),
			XMConvertToRadians(m_rot[i].y), XMConvertToRadians(m_rot[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_pos[i].x, m_pos[i].y, m_pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);


		//// ���f���f�[�^�̓ǂݍ���
		//if (!g_homing.Load(pDevice, pDeviceContext, MODEL_HOMINGATK)) {
		//	hr = E_FAIL;
		//	MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		//	return hr;
		//}
	}
#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif
	if (!g_bModelLoad) {
		// ���f���f�[�^�̓ǂݍ���
		if (!g_homingset.Load(pDevice, pDeviceContext, MODEL_HOMINGATKSET)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_homingset);

		// ���f���f�[�^�̓ǂݍ���
		if (!g_homing.Load(pDevice, pDeviceContext, MODEL_HOMINGATKBULLET)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_homing);

		g_bModelLoad = true;
	}
	//// �ۉe�̐���
	//for (int i = 0; i < MAX_HOMING_PARTS; i++)
	//{
	//	g_nShadow[i] = CreateShadow(m_posEnemy[i], 20.0f);
	//}

	return hr;
}


//=============================================================================
// �I������
//=============================================================================

void CHomingAttack::Uninit()
{
	//for (int i = 0; i < MAX_HOMING_PARTS; i++)
	//{
	//	// �ۉe�̉��
	//	ReleaseShadow(g_nShadow[i]);
	//}

	/*delete m_pLife[1];
	delete m_pLife[0];*/
	m_Life[1].Uninit();
	m_Life[0].Uninit();


	// ���f���̉��
#if MODEL_RELEASE_ON
		for (int i = 0; i < MAX_HOMINGATKSETNUM; ++i) {
			g_homingset.Release();
		}
		for (int i = 0; i < MAX_FALLATK; ++i) {
			g_homing.Release();
		}
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void CHomingAttack::Update()
{

	// ���ˑ���ɂ��Ă̔��ˎ��ԊǗ�
	for (int i = 0; i <MAX_HOMINGATKSETNUM; ++i) {
		if (!m_bUseSet[i])continue;

		--m_nAtkSetTime[i];

		if (m_nAtkSetTime[i] < 0) {
			if (GetWorldMoveflg()) {
				SetAtkHoming(i);
				m_nAtkSetTime[i] = HOMINGATTACK_TIME;
			}
		}
		if (CollisionCircleToCircle(m_posSet[i], m_pPlayer->GetPosCut(), m_radSet[i], 50.0f)) {
			
			if (m_pPlayer->GetBreakflg()) {
				/*--m_nLifeSet[i];*/
				//m_pLife[i]->DelEnemyLife(10);
				m_Life[i].DelEnemyLife(10);
				SetExplosion(m_posSet[i], 700.0f, 700.0f);

				CSound::Play(SE_PATK);	// ���ʉ��Đ�
				CSound::SetVolume(SE_PATK, 3.0f);

				CHitStop::Get()->HitStopOn(2);
			}
				
				
			if (m_Life[i].GetLife() <= 0/*m_pLife[i]->GetLife() <= 0*/) {
				m_bUseSet[i] = false;
				SetExplosion(XMFLOAT3(m_posSet[i].x+50.0f, m_posSet[i].y + 50.0f,m_posSet[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_posSet[i].x + (-50.0f), m_posSet[i].y + 50.0f, m_posSet[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_posSet[i].x + (-50.0f), m_posSet[i].y + (-50.0f), m_posSet[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_posSet[i].x + (-50.0f), m_posSet[i].y + (-50.0f), m_posSet[i].z + (-50.0f)),
					700.0f, 700.0f);

			}
		}

		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(m_sclSet[i].x, m_sclSet[i].y, m_sclSet[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotSet[i].x),
			XMConvertToRadians(m_rotSet[i].y), XMConvertToRadians(m_rotSet[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_posSet[i].x, m_posSet[i].y, m_posSet[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorldSet[i], mtxWorld);

		
	}


	// �ǔ������Ƃ��ꂪ�v���C���[�Ɠ�����Α̗͂����炷����
	for (int i = 0; i < MAX_FALLATK; ++i) {
		if (!m_bUseAtk[i]) continue;
		--m_nHomingTime[i];
		if (m_nHomingTime[i] > 0) {
			m_move[i].x += 0.0f;
			m_move[i].y += 0.0f;
			m_move[i].z += 0.0f;
			m_move[i].x = (m_pPlayer->GetPosModel().x - m_pos[i].x) / 30.0f/*(200.0f * 60.0f)*/;
			m_move[i].y = (m_pPlayer->GetPosModel().y - m_pos[i].y) / 30.0f/*(200.0f * 60.0f)*/;
			m_move[i].z = (m_pPlayer->GetPosModel().z - m_pos[i].z) / 30.0f/*(200.0f * 60.0f)*/;

			m_pos[i].x += m_move[i].x/**HOMINGATTACK_MOVE*/;
			m_pos[i].y += m_move[i].y/**HOMINGATTACK_MOVE*/;
			m_pos[i].z += m_move[i].z/**HOMINGATTACK_MOVE*/;

			if (m_pos[i].z < 800.0f) {
				if (CollisionCircleToCircle(m_pos[i], XMFLOAT3(0.0f, 0.0f, m_pos[i].z), m_rad[i], MODEL_POSY)) {
					XMVECTOR vUnGravity = XMVector3Normalize(
						XMVectorSet(
							m_pos[i].x - 0.0f,
							m_pos[i].y - 0.0f,
							0.0f, 1.0f)
					);

					m_pos[i].x += XMVectorGetX(vUnGravity) * 10;
					m_pos[i].y += XMVectorGetY(vUnGravity) * 10;
					m_pos[i].z += XMVectorGetZ(vUnGravity) * 10;
				}
			}
			else
			{
				if (CollisionCircleToCircle(m_pos[i], XMFLOAT3(0.0f, 0.0f, 800.0f), m_rad[i], MODEL_POSY)) {
					XMVECTOR vUnGravity = XMVector3Normalize(
						XMVectorSet(
							m_pos[i].x - 0.0f,
							m_pos[i].y - 0.0f,
							0.0f, 1.0f)
					);

					m_pos[i].x += XMVectorGetX(vUnGravity) * 10;
					m_pos[i].y += XMVectorGetY(vUnGravity) * 10;
					m_pos[i].z += XMVectorGetZ(vUnGravity) * 10;
				}
			}

			if (CollisionRectToPoint(XMFLOAT3(0.0f, 530.0f, 380.0f), XMFLOAT3(400.0f, 400.0f, 400.0f), m_pos[i]))
			{
				m_bUseAtk[i] = false;
				SetExplosion(XMFLOAT3(m_pos[i].x + 50.0f, m_pos[i].y + 50.0f, m_pos[i].z + 50.0f),
					300.0f, 300.0f);
			}


			if (CollisionCircleToCircle(m_pos[i], m_pPlayer->GetPosModel(), m_rad[i], 50.0f)) {
				// �G�ɓ���������G�t�F�N�g�o����HP���炵��
				// �_�ł��Ă�Ƃ��ɂ̓G�t�F�N�g�o�����AHP�����炳�Ȃ��B
				if (!m_pPlayer->GetInvflg())
				{
					m_pPlayer->GetLife()->DelLife(m_nDamage[i]);
					SetHitEffect(m_pPlayer->GetPosModel(), 400.0f, 400.0f);


					CSound::Play(SE_PATK);	// ���ʉ��Đ�
					CSound::SetVolume(SE_PATK, 3.0f);

					CHitStop::Get()->HitStopOn(2);

					// SE�Đ��J�n
					//CSound::Play(SE_HIT);
				}

				m_bUseAtk[i] = false;
				m_pPlayer->SetInvflg(true);
			}
		}
		//else if( -200<m_pos[i].x)
		else{
			m_bUseAtk[i] = false;
			SetExplosion(XMFLOAT3(m_pos[i].x + 50.0f, m_pos[i].y + 50.0f, m_pos[i].z + 50.0f),
				700.0f, 700.0f);
			SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + 50.0f, m_pos[i].z + 50.0f),
				700.0f, 700.0f);
			SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + (-50.0f), m_pos[i].z + 50.0f),
				700.0f, 700.0f);
			SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + (-50.0f), m_pos[i].z + (-50.0f)),
				700.0f, 700.0f);
		}

		if (CollisionCircleToCircle(m_pos[i], m_pPlayer->GetPosCut(), m_radSet[i], 5.0f)) {
			if (m_pPlayer->GetBreakflg()) {
				--m_nLife[i];
				SetExplosion(m_pos[i], 700.0f, 700.0f);
			}
			if (m_nLife[i] <= 0) {
				m_bUseAtk[i] = false;
				SetExplosion(XMFLOAT3(m_pos[i].x + 50.0f, m_pos[i].y + 50.0f, m_pos[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + 50.0f, m_pos[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + (-50.0f), m_pos[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_pos[i].x + (-50.0f), m_pos[i].y + (-50.0f), m_pos[i].z + (-50.0f)),
					700.0f, 700.0f);

			}
		}


		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(m_scl[i].x, m_scl[i].y, m_scl[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rot[i].x),
			XMConvertToRadians(m_rot[i].y), XMConvertToRadians(m_rot[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_pos[i].x, m_pos[i].y, m_pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);

		//// �ۉe�̈ړ�
		//MoveShadow(g_nShadow[i], m_posEnemy[i]);

	   

	}

	/*m_pLife[0]->Update();
	m_pLife[1]->Update();*/

	m_Life[0].Update();
	m_Life[1].Update(); 
}



//=============================================================================
// �`�揈��
//=============================================================================
void CHomingAttack::Draw()
{
	/*m_pLife[0]->SetCamera(m_pCamera);
	m_pLife[1]->SetCamera(m_pCamera);*/
	/*m_Life[0].SetCamera(m_pCamera);
	m_Life[1].SetCamera(m_pCamera);*/

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_FALLATK; ++i) {
		if (!m_bUseAtk[i])continue;

		// �J�����A�����ݒ�
		g_homing.SetCamera(CCamera::Get());
		g_homing.SetLight(CLight::Get());


		// �s����������`��
		g_homing.Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_homing.Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);

		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}

	for (int i = 0; i <MAX_HOMINGATKSETNUM; ++i) {
		if (!m_bUseSet[i])continue;

		
		// �J�����A�����ݒ�
		g_homingset.SetCamera(CCamera::Get());
		g_homingset.SetLight(CLight::Get());
		
		// �s����������`��
		g_homingset.Draw(pDeviceContext, m_mtxWorldSet[i], eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_homingset.Draw(pDeviceContext, m_mtxWorldSet[i], eTransparentOnly);

		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
		
	}

	/*m_pLife[0]->Draw();
	m_pLife[1]->Draw();*/
	m_Life[0].Draw();
	m_Life[1].Draw();
}

void CHomingAttack::SetAtkHoming(int nNum)
{
	for (int i = 0; i < MAX_FALLATK; ++i) {
		if (m_bUseAtk[i])continue;
		m_bUseAtk[i] = true;
		m_nHomingTime[i] = HOMINGATTACK_FALLTIME;
		m_pos[i].x = m_posSet[nNum].x;
		m_pos[i].y = m_posSet[nNum].y;
		m_pos[i].z = m_posSet[nNum].z;
		m_move[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		/*m_move[i] = XMFLOAT3(
			m_pPlayer->GetModelY().x * HOMINGATTACK_SETY, 
			m_pPlayer->GetModelY().y * HOMINGATTACK_SETY,
			m_pPlayer->GetModelY().z * HOMINGATTACK_SETY
		);*/

		break;
	}
}
