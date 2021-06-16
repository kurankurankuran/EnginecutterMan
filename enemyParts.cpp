//=============================================================================
//
// �G�p�[�c���� [enemyParts.cpp]
// Author : �O��@�G���Y
//
//=============================================================================
//#include "enemyParts.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "meshKirime.h"
#include "fade.h"
#include "scene.h"
#include "Collision.h"
#include "Camera.h"
#include "Light.h"
#include "model.h"

//#include "bilibili.h"
//#include "shadow.h"
//#include "control.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMYPARTS_0			"data/model/Enemy/EnemyArmor.fbx"
#define ENEMYPARTS_1			"data/model/Enemy/EnemyArmor1.fbx"
#define ENEMYPARTS_2			"data/model/Enemy/EnemyArmor2.fbx"
#define ENEMYPARTS_3			"data/model/Enemy/EnemyArmor3.fbx"

//#define ENEMYPARTS_0			"data/model/armor1test.fbx"
//#define ENEMYPARTS_1			"data/model/armor2test.fbx"
//#define ENEMYPARTS_2			"data/model/armor3test.fbx"
//#define ENEMYPARTS_3			"data/model/armor4test.fbx"



#define	VALUE_MOVE_ENEMYPARTS	(2.0f)		// �ړ����x
#define	RATE_MOVE_ENEMYPARTS	(0.020f)	// �ړ������W��
#define	VALUE_ROTATE_ENEMYPARTS	(9.0f)		// ��]���x
#define	RATE_ROTATE_ENEMYPARTS	(0.20f)		// ��]�����W��

#define ENEMY_LIFE			(4)			 // �G�̗�
#define ENEMY_GRAVITY		(0.015f)     // �G�������x

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAssimpModel	g_enemyParts[MAX_ENEMY_PARTS];		// ���f��

//static CCamera*		g_pCamera;		// �J����
//static CLight*		g_pLight;		// ���C�g
//static int			g_nShadow[MAX_ENEMY_PARTS];		// �ۉe�ԍ�

static float		g_fGravity;		// �d��

static int          g_nKillCnt;

static XMFLOAT3     g_vPosCamera; // �J�������W�ۑ��p
static XMFLOAT3     g_vTargetCamera; // �J�����^�[�Q�b�g���W�ۑ��p
static XMFLOAT3     g_vUpCamera; // �J���������W�ۑ��p

//static XMFLOAT3     m_pos_bilibili[55];

static bool g_bModelLoad = false;


static XINPUT_STATE* g_pXController;


// �R���X�g���N�^
EnemyParts::EnemyParts()
{

}

TAssimpMaterial g_DefMat;

//=============================================================================
// ����������
//=============================================================================
HRESULT EnemyParts::InitEnemy()
{
	m_GravitySpace[0].f3Pos = XMFLOAT3(500.0f, 500.0f, 800.0f + 1000.0f / 2.0f);
	m_GravitySpace[1].f3Pos = XMFLOAT3(-500.0f, 500.0f, 800.0f + 1000.0f / 2.0f);
	m_GravitySpace[2].f3Pos = XMFLOAT3(-500.0f, -500.0f, 800.0f + 1000.0f / 2.0f);
	m_GravitySpace[3].f3Pos = XMFLOAT3(500.0f, -500.0f, 800.0f + 1000.0f / 2.0f);

	m_GravitySpace[0].f3Scl = XMFLOAT3(1000.0f, 1000.0f, 1000.0f);
	m_GravitySpace[1].f3Scl = XMFLOAT3(1000.0f, 1000.0f, 1000.0f);
	m_GravitySpace[2].f3Scl = XMFLOAT3(1000.0f, 1000.0f, 1000.0f);
	m_GravitySpace[3].f3Scl = XMFLOAT3(1000.0f, 1000.0f, 1000.0f);


	m_rad = MODEL_POSY + 50.0f;
	g_nKillCnt = 0;


	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	g_fGravity = ENEMY_GRAVITY;

	// ������ԗ͂̑��x
	m_fPower[0] = XMFLOAT3(6.0f, 6.0f, 2.0f);
	m_fPower[1] = XMFLOAT3(-6.0f, 6.0f, 2.0f);
	m_fPower[2] = XMFLOAT3(-6.0f, -6.0f, 2.0f); //�@����
	m_fPower[3] = XMFLOAT3(6.0f, -6.0f, 2.0f);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	m_posEnemy[0] = XMFLOAT3(0.0f, 0.0f, 50.0f); // �������W
	m_posEnemy[1] = XMFLOAT3(0.0f, 0.0f, 50.0f);
	m_posEnemy[2] = XMFLOAT3(0.0f, 0.0f, 50.0f); // �������W
	m_posEnemy[3] = XMFLOAT3(0.0f, 0.0f, 50.0f);

	m_sclEnemy[0] = XMFLOAT3(155.0f, 155.0f, 155.0f);
	m_sclEnemy[1] = XMFLOAT3(155.0f, 155.0f, 155.0f);
	m_sclEnemy[2] = XMFLOAT3(155.0f, 155.0f, 155.0f);
	m_sclEnemy[3] = XMFLOAT3(155.0f, 155.0f, 155.0f);

	m_moveEnemy[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_moveEnemy[1] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_moveEnemy[2] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_moveEnemy[3] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_rotEnemy[0] = XMFLOAT3(90.0f, 0.0f, 0.0f);
	m_rotEnemy[1] = XMFLOAT3(90.0f, 0.0f, 0.0f);
	m_rotEnemy[2] = XMFLOAT3(90.0f, 0.0f, 0.0f);
	m_rotEnemy[3] = XMFLOAT3(90.0f, 0.0f, 0.0f);
	/*g_rotDestEnemy = XMFLOAT3(0.0f, 0.0f, 0.0f);*/

	for (int i = 0; i < MAX_ENEMY_PARTS; ++i) {

		m_fallenParts[i] = false;
		m_fall[i] = false;
		m_nHp[i] = 1;
		m_bStat[i] = true;

		g_DefMat.Kd = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		if (g_bModelLoad) {
			g_enemyParts[i].SetMaterial(&g_DefMat);
		}
		//g_enemyParts[i].GetMaterial()->Kd.w = 1.0f;


		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �傫���𔽉f
		mtxScl = XMMatrixScaling(m_sclEnemy[i].x, m_sclEnemy[i].y, m_sclEnemy[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotEnemy[i].x),
			XMConvertToRadians(m_rotEnemy[i].y), XMConvertToRadians(m_rotEnemy[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		//// ���R����]�s��𔽉f
		//if (m_fall[i]) {
		//	mtxWorld = XMMatrixMultiply(mtxWorld, AxisRotmtx);
		//}

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_posEnemy[i].x, m_posEnemy[i].y, m_posEnemy[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);
	}
#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif

	if (!g_bModelLoad) {
		// ���f���f�[�^�̓ǂݍ���
		if (!g_enemyParts[0].Load(pDevice, pDeviceContext, ENEMYPARTS_0)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_enemyParts[0]);

		if (!g_enemyParts[1].Load(pDevice, pDeviceContext, ENEMYPARTS_1)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_enemyParts[1]);

		if (!g_enemyParts[2].Load(pDevice, pDeviceContext, ENEMYPARTS_2)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_enemyParts[2]);

		if (!g_enemyParts[3].Load(pDevice, pDeviceContext, ENEMYPARTS_3)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_enemyParts[3]);
		
		g_bModelLoad = true;

	}

	

	m_Smoke.InitSmoke();



	return hr;
}


//=============================================================================
// �I������
//=============================================================================
EnemyParts::~EnemyParts()
{
	m_Smoke.UninitSmoke();
	//UninitEnemy();
}

void EnemyParts::UninitEnemy()
{
	
	// ���f���̉��
#if MODEL_RELEASE_ON
		for (int i = 0; i < MAX_ENEMY_PARTS; ++i) {
			g_enemyParts[i].Release();
		}
#endif

	

}

//=============================================================================
// �X�V����
//=============================================================================
void EnemyParts::UpdateEnemy()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	bool bCameraMove = false;

	XMMATRIX AxisRotmtx;

	// �G�̗̑͂�0�ɂȂ����痎��

	for (int i = 0; i < MAX_ENEMY_PARTS; ++i) {

		switch (i) {
		case 0:// �E���p�[�c
			if (GetDestflg(0) && GetDestflg(2))
				--m_nHp[0];
			
			break;
		case 1:// �����p�[�c
			if (GetDestflg(0) && GetDestflg(3))
				--m_nHp[1];

		
			break;
		case 2:// ����p�[�c
			if (GetDestflg(1) && GetDestflg(3))
				--m_nHp[2];

			break;
		case 3:// �E��p�[�c
			if (GetDestflg(1) && GetDestflg(2))
				--m_nHp[3];

			break;
		}

		if (m_bStat[i]) {
			if (m_nHp[i] <= 0)
			{
				if (!m_fall[i]) {
					if (!bCameraMove) {
						
						bCameraMove = true;
					}
				}

				m_fall[i] = true;
				// �G�̗̑͂�0�ł���x���W��0.0���傫���Ȃ�E�����ɐ������
				if (m_nHp[i] == 0 /*&& m_posEnemy[i].x > 0.0f*/)
				{
					m_moveEnemy[i].x = m_fPower[i].x;
					m_moveEnemy[i].y = m_fPower[i].y;
					m_moveEnemy[i].z = m_fPower[i].z;

					if (GetWorldMoveflg()) {
						g_vPosCamera = CCamera::Get()->GetPos();
						g_vTargetCamera = CCamera::Get()->GetTarget();
						g_vUpCamera = *(CCamera::Get()->GetCameraUp());

						SetWorldMoveflg(false);
						CCamera::Get()->SetUpdateIns(INS_CENEMY);
					}
				}

				if (m_fall[i]) {

					XMFLOAT3 vCameraPos;

					switch (i) {
					case 0:// �E���p�[�c
						vCameraPos = XMFLOAT3(2000.0f, 2000.0f, 800.0f);
						// ���R����]�s����쐬
						m_fAxisRot[i] += (2 * 3.14159265359f) / (60 * 2.0f);
						AxisRotmtx = XMMatrixRotationAxis(XMVectorSet(-1.0f, -1.0f, 0.0f, 1.0f), m_fAxisRot[i]);


						break;
					case 1:// �����p�[�c
						vCameraPos = XMFLOAT3(-2000.0f, 2000.0f, 800.0f);

						// ���R����]�s����쐬
						m_fAxisRot[i] += (2 * 3.14159265359f) / (60 * 2.0f);
						AxisRotmtx = XMMatrixRotationAxis(XMVectorSet(-1.0f, -1.0f, 0.0f, 1.0f), m_fAxisRot[i]);

						break;
					case 2:// ����p�[�c
						vCameraPos = XMFLOAT3(-2000.0f, -2000.0f, 800.0f);

						// ���R����]�s����쐬
						m_fAxisRot[i] += -(2 * 3.14159265359f) / (60 * 2.0f);
						AxisRotmtx = XMMatrixRotationAxis(XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f), m_fAxisRot[i]);

						break;
					case 3:// �E��p�[�c
						vCameraPos = XMFLOAT3(2000.0f, -2000.0f, 800.0f);

						// ���R����]�s����쐬
						m_fAxisRot[i] += -(2 * 3.14159265359f) / (60 * 2.0f);
						AxisRotmtx = XMMatrixRotationAxis(XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f), m_fAxisRot[i]);

						break;

					}

					if (!GetWorldMoveflg()) {

						XMFLOAT3 vTargetPos = XMFLOAT3(0.0f, 0.0f, 400.0f);
						CCamera::Get()->SetPos(vCameraPos);
 						CCamera::Get()->SetTarget(vTargetPos);

						XMFLOAT3 vCameraUp = XMFLOAT3(0.0f, 0.0f, 1.0f);
						CCamera::Get()->SetUpVector(vCameraUp);
					}
					// ���ꂼ��̍��W�̈ړ��ʐݒ�
					m_moveEnemy[i].x += (0.0f - m_moveEnemy[i].x) * RATE_MOVE_ENEMYPARTS;
					m_moveEnemy[i].z = m_moveEnemy[i].z - g_fGravity;
					m_moveEnemy[i].y += (0.0f - m_moveEnemy[i].y) * RATE_MOVE_ENEMYPARTS;



				}
			}


		
			// �ʒu�ړ�
			m_posEnemy[i].x += m_moveEnemy[i].x;
			m_posEnemy[i].y += m_moveEnemy[i].y;
			m_posEnemy[i].z += m_moveEnemy[i].z;

			if (m_fall[i]) {
				// �������p�[�c�����ɂ�����X���[�N�o���ĕ\��������
				if (m_posEnemy[i].z < -1000.0f)
				{
					m_bStat[i] = false;
					m_posEnemy[i].z = -1000.0f;
					m_moveEnemy[i].x = 0.0f;
					m_moveEnemy[i].y = 0.0f;
					m_moveEnemy[i].z = 0.0f;
					m_fall[i] = false;
					m_fallenParts[i] = true;

					switch (i) {
					case 0:// �E���p�[�c
						m_Smoke.SetCallSmoke(XMFLOAT3(100.0f, 500.0f, 0.0f), 100.0f, 100.0f);
						break;
					case 1:// �����p�[�c
						m_Smoke.SetCallSmoke(XMFLOAT3(-500.0f, 500.0f, 0.0f), 100.0f, 100.0f);
						break;
					case 2:// ����p�[�c
						m_Smoke.SetCallSmoke(XMFLOAT3(-500.0f, -100.0f, 0.0f), 100.0f, 100.0f);
						break;
					case 3:// �E��p�[�c
						m_Smoke.SetCallSmoke(XMFLOAT3(-50.0f, -100.0f, 0.0f), 100.0f, 100.0f);
						break;
					}


					if (!GetWorldMoveflg()) {
						SetWorldMoveflg(true);
						CCamera::Get()->SetPos(g_vPosCamera);
						CCamera::Get()->SetTarget(g_vTargetCamera);
						CCamera::Get()->SetUpVector(g_vUpCamera);
					}

				}

				if (!GetWorldMoveflg()) {
					// �X�y�[�X�L�[����Enter�Ō���
					if (GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE)
#if SETCONTROLLER
						|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
						|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_B
						|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
						|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y
#endif
						)
					{
						SetWorldMoveflg(true);
						CCamera::Get()->SetPos(g_vPosCamera);
						CCamera::Get()->SetTarget(g_vTargetCamera);
						CCamera::Get()->SetUpVector(g_vUpCamera);
					}
				}
			}
		}
		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �傫���𔽉f
		mtxScl = XMMatrixScaling(m_sclEnemy[i].x, m_sclEnemy[i].y, m_sclEnemy[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotEnemy[i].x),
			XMConvertToRadians(m_rotEnemy[i].y), XMConvertToRadians(m_rotEnemy[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		//// ���R����]�s��𔽉f
		//if (m_fall[i]) {
		//	mtxWorld = XMMatrixMultiply(mtxWorld, AxisRotmtx);
		//}

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_posEnemy[i].x, m_posEnemy[i].y, m_posEnemy[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);

		//// �ۉe�̈ړ�
		//MoveShadow(g_nShadow[i], m_posEnemy[i]);



	}

	for (int i = 0; i <= MAX_ENEMY_PARTS; ++i) {
		if (m_bStat[i]) {
			g_nKillCnt = 0;
			break;
		}
		g_nKillCnt++;
		if (g_nKillCnt == MAX_ENEMY_PARTS) {
			SetTime2(true);
		}
	}



	m_Smoke.UpdateSmoke();



}



//=============================================================================
// �`�揈��
//=============================================================================
void EnemyParts::DrawEnemy()
{
	// �J�����A�����ݒ�
	for (int i = 0; i < MAX_ENEMY_PARTS; ++i) {
		g_enemyParts[i].SetCamera(CCamera::Get());
		g_enemyParts[i].SetLight(CLight::Get());
	}


	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{
		// �s����������`��
		g_enemyParts[i].Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);
		// ���ɗ������ăX���[�N�o��Ɠ����ɒi�X���������ď���
		if (m_fallenParts[i])
		{
			g_enemyParts[i].Draw(pDeviceContext, m_mtxWorld[i], DanDan);
		}

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_enemyParts[i].Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);

		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����


	}

	m_Smoke.DrawSmoke();
}



//=============================================================================
// �p�[�c���a�ݒ�p
//=============================================================================
int EnemyParts::ColGravitySpace(XMFLOAT3 f3Pos)
{
	for (int i = 0; i < MAX_ENEMY_PARTS; ++i) {
		if (!m_bStat[i] || m_fall[i])
			continue;

		if (CollisionRectToPoint(m_GravitySpace[i].f3Pos, m_GravitySpace[i].f3Scl, f3Pos)) {
			return 1;
		}
	}
	return -1;
}

//=============================================================================
// �R���g���[���[�I�u�W�F�N�g�擾
//=============================================================================
void SetControllerrEnemyParts(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}