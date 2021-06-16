//=============================================================================
//
// ���f������ [model.cpp]
// Author :KAZUKI KURA
//
//=============================================================================
#include "model.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "Collision.h"
#include "meshKirime.h"
#include "Sound.h"
#include "scene.h"
#include "fade.h"
#include "deatheffect.h"
#include "CHitStop.h"
#include "Camera.h"
#include "Light.h"

//
//int main(void)
//{
//	XMStoreFloat4x4(XMMatrixIdentity(), XMMatrixIdentity());
//	return 0;
//}



static enum {
	NONE_PLAYER = 0,
	FRONT_CHARA,
	LEFT_CHARA,
	RIGHT_CHARA,
	
	FRONT_MACHINE,
	LEFT_MACHINE,
	RIGHT_MACHINE,

	MAX_PLAYER,

}MODEL_PLAYER;

static enum {
	NONE_DEL = 0,
	FRONT,
	LEFT,
	RIGHT,

	MAX_DEL,

}MODEL_DEL;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_CAR			        "data/model/Player/playCharaTest_02.fbx"

#define MODEL_CHARA_FRONT			"data/model/Player/chara_drive.fbx"
#define MODEL_CHARA_LEFT			"data/model/Player/chara_left.fbx"
#define MODEL_CHARA_RIGHT			"data/model/Player/chara_right.fbx"
#define MODEL_MACHINE_FRONT			"data/model/Player/machine_drive_07.fbx"
#define MODEL_MACHINE_LEFT			"data/model/Player/machine_left_07.fbx"
#define MODEL_MACHINE_RIGHT			"data/model/Player/machine_right_07.fbx"

//#define MODEL_CAR			"data/model/Player/chara_drive_1214.fbx"
//#define MODEL_CAR			"data/model/Player/chara_ranaway_1214.fbx"
//#define MODEL_CAR			"data/model/Player/chara_stop_02.fbx"

#define VALUE_MOVE_MODEL	(4.0f)
#define	RATE_MOVE_MODEL		(0.20f)		// �ړ������W��
#define	RATE_ROTATE_MODEL	(0.20f)		// ��]�����W��
#define	RATE_ROTATE_MODEL_BREAK	(0.02f)	// ��]�����W��
#define VALUE_ROT_MODEL    (VALUE_MOVE_MODEL / (2.0f* 3.14159265359f * ENEMY_RAD) * 360.0f) // ��]��
#define LEN_SEARCH_FIELD    (10.0f)     // ���̒n�`�`�����������͈�
#define PLAYER_INVTIME      (3*60)      // ���G����

#define SLOW_TIME           (2.5f)     // �q�b�g�X�g�b�v����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAssimpModel		g_model[MAX_PLAYER+10];            // �L����

//static CCamera*			g_pCamera;          // �J����
//static CLight*			g_pLight;           // ���C�g

//static CEnemy*          g_pEnemy;
CEnemyVacuum*          g_pEnemy;

static bool			  g_bBGM = false;

static XINPUT_STATE* g_pXController;
static XMVECTOR      g_NorVec;

static float        g_elapsedTime = 0.0f;
static bool         g_isSlowDown = false;
static XMFLOAT3     g_kakunoMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
static int			 g_Cnt = 0;
static int			 g_nAtkCnt;


static int g_nDeathtime;  // ���񂾂Ƃ��Ɏg���^�C�}
static bool g_bModelLoad = false;

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	XMMatrixIdentity();
	g_nAtkCnt = 0;
	m_nModelType = FRONT;
	g_nDeathtime = 60;
	m_bMove = false;
	m_f3JumpMoveUp = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_f3JumpMoveDown = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_bJumpflg = false;
	m_fRazianDest = 0.0f;
	m_fRazian = 0.0f;
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	m_posModel = XMFLOAT3(MODEL_POSX, MODEL_POSY, MODEL_POSZ);
	m_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	/*m_sclModel = XMFLOAT3(2.0f, 2.0f, 2.0f);*/
	m_sclModel = XMFLOAT3(2.0f, 2.0f, 2.0f);
	m_f3modelY = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_nInvTime = PLAYER_INVTIME;
	m_bInvflg = false;
	m_vecRotDest = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_vecNowRot = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_posCut = XMFLOAT3(MODEL_POSX, MODEL_POSY, MODEL_POSZ);         // �n���W�p
	m_vecCut = XMFLOAT3(0.0f, 0.0f, 70.0f);         // �n�x�N�g��

	

	m_nOnFieldType = CYLINDER_FIELD;
	m_nOnFieldName = BODY_NAME;

	
	//**************************************************************************************************
	//XMMATRIX mtxWorld, mtxScl, /*mtxRot,*/ mtxTranslate;

	//// ���[���h�}�g���b�N�X�̏�����
	//mtxWorld = XMMatrixIdentity();

	//// �X�P�[���𔽉f
	//mtxScl = XMMatrixScaling(m_sclModel.x, m_sclModel.y, m_sclModel.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//// ��]�𔽉f
	//mtxWorld = XMMatrixMultiply(mtxWorld, m_RotationY);
	//// �ړ��𔽉f
	//mtxTranslate = XMMatrixTranslation(m_posModel.x, m_posModel.y, m_posModel.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//// ���[���h�}�g���b�N�X�ݒ�
	//XMStoreFloat4x4(&m_mtxWorld, mtxWorld);

	//**************************************************************************************************


	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	m_bBreak = false;
	//*********************************************

#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif
	if (!g_bModelLoad) {
		/*for (int i = 1; i < MAX_PLAYER; ++i) {
			if (!g_model[i].Load(pDevice, pDeviceContext, MODEL_CAR)) {
				hr = E_FAIL;
				MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
				return hr;
			}
		}
		g_bModelLoad = true;*/

		//**********************************************
		for (int i = 1; i < MAX_PLAYER; ++i) {
			// ���f���f�[�^�̓ǂݍ���
			switch (i) {
			case FRONT_CHARA:
				if (!g_model[i].Load(pDevice, pDeviceContext, MODEL_CHARA_FRONT)) {
					hr = E_FAIL;
					MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
					return hr;
				}
				SaveModel(&g_model[i]);

				break;

			case LEFT_CHARA:
				if (!g_model[i].Load(pDevice, pDeviceContext, MODEL_CHARA_LEFT)) {
					hr = E_FAIL;
					MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
					return hr;
				}
				SaveModel(&g_model[i]);

				break;
			case RIGHT_CHARA:
				if (!g_model[i].Load(pDevice, pDeviceContext, MODEL_CHARA_RIGHT)) {
					hr = E_FAIL;
					MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
					return hr;
				}
				SaveModel(&g_model[i]);

				break;
			case FRONT_MACHINE:
				if (!g_model[i].Load(pDevice, pDeviceContext, MODEL_MACHINE_FRONT)) {
					hr = E_FAIL;
					MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
					return hr;
				}
				SaveModel(&g_model[i]);

				break;

			case LEFT_MACHINE:
				if (!g_model[i].Load(pDevice, pDeviceContext, MODEL_MACHINE_LEFT)) {
					hr = E_FAIL;
					MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
					return hr;
				}
				SaveModel(&g_model[i]);

				break;
			case RIGHT_MACHINE:
				if (!g_model[i].Load(pDevice, pDeviceContext, MODEL_MACHINE_RIGHT)) {
					hr = E_FAIL;
					MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
					return hr;
				}
				SaveModel(&g_model[i]);

				break;
			}
		}
		g_bModelLoad = true;
	}
	//************************************************************************************
	//********************
	XMStoreFloat4x4(&m_VRotToNextMat4x4, XMMatrixIdentity());
	XMStoreFloat4x4(&m_RotMat4x4, XMMatrixIdentity());
	XMStoreFloat4x4(&m_RotationY4x4, XMMatrixIdentity());
	XMStoreFloat4x4(&m_RotationNor4x4, XMMatrixIdentity());
	//********************
	XMMATRIX        m_RotationY = XMLoadFloat4x4(&m_RotationY4x4);
	m_RotationY = XMMatrixMultiply(m_RotationY, XMMatrixRotationY(3.14159265359f)); // ���[���h�s��Ɋ|����p�s��̏C��
	XMStoreFloat4x4(&m_RotationY4x4, m_RotationY);


	XMMATRIX        m_VRotToNextMat = XMLoadFloat4x4(&m_VRotToNextMat4x4);    // �����t���[�����̉^���ʕ��v���C���[���W����]������s��
	XMMATRIX        m_RotMat = XMLoadFloat4x4(&m_RotMat4x4);           // ��ɋ��̂̒��S�����ɂ��邽�߂̉�]�s��
	XMVECTOR Vpm;
	XMVECTOR Vs;
	XMVECTOR Vn;

	XMFLOAT3 f3V1, f3V2;

	Vpm = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

	// ���̒��S���玩�n�_�����x�N�g�����ړ���n�_�܂ŉ�]������p�̉�]�s��
	m_VRotToNextMat = XMMatrixRotationAxis(Vpm, 3.14159265359f / 2.0f);

	// ���[���h���W�ړ��𔽉f������p�̍s����쐬
	m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

	Vs = XMVectorSet(
		m_posModel.x - 0.0f,
		m_posModel.y - 0.0f,
		m_posModel.z - MODEL_POSZ,
		1.0f);

	Vn = XMVector3TransformNormal(Vs, m_VRotToNextMat);

	// ���ݒn�_�x�N�g���ƈړ���n�_�x�N�g���̍�����ړ��悪���̕\�ʏ�ɂȂ�悤�ŏI�I�ȉ^���ʂ��v�Z
	XMStoreFloat3(&f3V1, Vs);
	XMStoreFloat3(&f3V2, Vn);

	m_posModel.x += f3V2.x - f3V1.x;
	m_posModel.y += f3V2.y - f3V1.y;
	m_posModel.z += f3V2.z - f3V1.z;


	XMMATRIX mtxWorld, mtxScl, /*mtxRot,*/ mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(m_sclModel.x, m_sclModel.y, m_sclModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxWorld = XMMatrixMultiply(mtxWorld, m_RotationY);
	mtxWorld = XMMatrixMultiply(mtxWorld, m_RotMat);
	//mtxWorld = XMMatrixMultiply(mtxWorld, BreakRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(m_posModel.x, m_posModel.y, m_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&m_mtxWorld, mtxWorld);

	//*********************************************************************
	XMStoreFloat4x4(&m_VRotToNextMat4x4, m_VRotToNextMat);
	XMStoreFloat4x4(&m_RotMat4x4, m_RotMat);
	//************************************************************************************

	// BGM������
	g_bBGM = false;

	// �G���W��������
	hr = m_Enzine.Initengine();
	if (FAILED(hr))
		return hr;

	hr = m_Life.InitLife();
	if (FAILED(hr))
		return hr;

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	m_Life.UninitLife();

	// �G���W���I������
	m_Enzine.Uninitengine();

	// BGM��~
	if (g_bBGM) {
		CSound::Stop(BGM_GAME1);
		g_bBGM = false;
	}
	// SE��~
	CSound::Stop(SE_DEATH);


	//���f���̉��
#if MODEL_RELEASE_ON
		for (int i = 0; i < MAX_PLAYER; ++i) {
			g_model[i].Release();
		}
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// BGM�J�n
	CSound::Play(BGM_GAME1);
	g_bBGM = true;

	// ���G�t���O�Ǘ�
	if (m_bInvflg) {
		--m_nInvTime;
		if (m_nInvTime < 0) {
			m_bInvflg = false;
			m_nInvTime = PLAYER_INVTIME;
		}
	}

	

	// �v���C���[���C�t��0�ɂȂ����玀�S�G�t�F�N�g���������ăv���C���[�������B
	// ���̌�J�E���g�����ăV�[���؂�ւ�������
	if (m_Life.GetLife() <= 0)
	{
		g_nDeathtime--;
		SetDeathEffect(m_posModel, 400.0f, 400.0f);

		CSound::Play(SE_DEATH);

		if (g_nDeathtime % 4 == 0) {
			SetDeathEffect(XMFLOAT3(m_posModel.x + 30.0f, m_posModel.y + 60.0f, m_posModel.z + 60.0f), 400.0f, 400.0f);
		}
		if (g_nDeathtime % 3 == 0) {
			SetDeathEffect(XMFLOAT3(m_posModel.x - 30.0f, m_posModel.y + 60.0f, m_posModel.z + 60.0f), 400.0f, 400.0f);
		}
		if (g_nDeathtime % 5 == 0) {
			SetDeathEffect(XMFLOAT3(m_posModel.x + 70.0f, m_posModel.y + 60.0f, m_posModel.z + 60.0f), 400.0f, 400.0f);
		}


		if (g_nDeathtime < 0)
			SetFadeOut(SCENE_GAMEOVER);
	}

#ifdef _DEBUG
	if (GetKeyPress(VK_5)) {
		SetFadeOut(SCENE_RUSH);
	}
	if (GetKeyPress(VK_6)) {
		SetFadeOut(SCENE_GAMECLEAR);
	}
#endif
	//*********************************************
	XMMATRIX        m_VRotToNextMat = XMLoadFloat4x4(&m_VRotToNextMat4x4);    // �����t���[�����̉^���ʕ��v���C���[���W����]������s��
	XMMATRIX        m_RotMat = XMLoadFloat4x4(&m_RotMat4x4);           // ��ɋ��̂̒��S�����ɂ��邽�߂̉�]�s��
	XMMATRIX        m_RotationY = XMLoadFloat4x4(&m_RotationY4x4);                         // �v���C���[�̏�x�N�g������]�s��
	XMMATRIX        m_RotationNor = XMLoadFloat4x4(&m_RotationNor4x4);                       // �v���C���[�̌�����ς����]�s��

	XMVECTOR        m_vRotDest = XMVectorSet(m_vecRotDest.x, m_vecRotDest.y, m_vecRotDest.z, 1.0f);// �ړI�����x�N�g��
	XMVECTOR        m_vNowRot = XMVectorSet(m_vecNowRot.x, m_vecNowRot.y, m_vecNowRot.z, 1.0f);                           // ���݂̌��������x�N�g�� 
																								   //*********************************************
	//*********************************************
	// �v���C���[�̃��[�J��XYZ�����쐬
	XMVECTOR vMoveX = *CCamera::Get()->GetCameraXYZ(0);
	XMFLOAT3 f3MoveX;
	XMStoreFloat3(&f3MoveX, vMoveX);

	XMVECTOR vMoveY;
	XMFLOAT3 f3MoveY;

	XMVECTOR vMoveZ;
	XMFLOAT3 f3MoveZ;

	bool bWorldMove = GetWorldMoveflg();
	if (bWorldMove)
	{
		/*m_VRotToNextMat = XMMatrixIdentity();
		XMStoreFloat4x4(&m_VRotToNextMat4x4, m_VRotToNextMat);
		*/

		if (g_Cnt >= 60) {
			m_bMove = true;
		}
		else {
			g_Cnt++;
			m_RotationNor = XMMatrixIdentity();
			XMStoreFloat4x4(&m_RotationNor4x4, m_RotationNor);
		}
	}
	else
	{
		m_bMove = false;
		g_Cnt = 0;
	}
	if (m_bMove)
	{
		switch (m_nOnFieldType)
		{
		case RECT_FIELD:

			vMoveY = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);

			XMStoreFloat3(&f3MoveY, vMoveY);

			vMoveZ = XMVector3Cross(vMoveX, vMoveY);
			f3MoveZ;
			XMStoreFloat3(&f3MoveZ, vMoveZ);

			break;
		case CIRCLE_FIELD:
			switch (m_nOnFieldName)
			{
			case BODY_NAME:
				vMoveY = XMVector3Normalize(XMVectorSet(
					m_posModel.x - g_pEnemy->GetCylinderP()->GetPos().x,
					m_posModel.y - g_pEnemy->GetCylinderP()->GetPos().y,
					m_posModel.z - g_pEnemy->GetCylinderP()->GetPos().z,
					1.0f));
				break;
			case FRONTCUBE_NAME:
				vMoveY = XMVector3Normalize(XMVectorSet(
					m_posModel.x - g_pEnemy->GetSpherFrontP()->GetPos().x,
					m_posModel.y - g_pEnemy->GetSpherFrontP()->GetPos().y,
					m_posModel.z - g_pEnemy->GetSpherFrontP()->GetPos().z,
					1.0f));
				break;
			/*case BACKCUBE_NAME:
				vMoveY = XMVector3Normalize(XMVectorSet(
					m_posModel.x - g_pEnemy->GetSpherBackP()->GetPos().x,
					m_posModel.y - g_pEnemy->GetSpherBackP()->GetPos().y,
					m_posModel.z - g_pEnemy->GetSpherBackP()->GetPos().z,
					1.0f));
				break;*/
			}


			XMStoreFloat3(&f3MoveY, vMoveY);

			vMoveZ = XMVector3Cross(vMoveX, vMoveY);
			f3MoveZ;
			XMStoreFloat3(&f3MoveZ, vMoveZ);

			break;

		case CYLINDER_FIELD:
			// �~���̒��S���W�̓v���C���[�ɍ��킹�ē������Ƃ��A(X,Y,Z)�̂���Z�̂ݕϐ��Ƃ���
			// �~���̒��S���̒����v���C���[�ɍ��킹�ē����_�̍��W����v���C���[�܂ł̃x�N�g��
			vMoveY = XMVector3Normalize(XMVectorSet(
				m_posModel.x - g_pEnemy->GetCylinderP()->GetPos().x,
				m_posModel.y - g_pEnemy->GetCylinderP()->GetPos().y,
				m_posModel.z - g_pEnemy->GetCylinderP()->GetPos().z,
				1.0f));
			XMStoreFloat3(&f3MoveY, vMoveY);

			vMoveZ = XMVector3Cross(vMoveX, vMoveY);
			f3MoveZ;
			XMStoreFloat3(&f3MoveZ, vMoveZ);

			break;
		}

		// �v���C���[�̏�����x�N�g��������ۑ�
		m_f3modelY = f3MoveY;

		// �^���ʃx�N�g�����쐬
		XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMFLOAT3 f3Move;
		//*********************************************


		XMVECTOR vMoveLen2 = XMVector3Length(XMVectorSet(m_moveModel.x, m_moveModel.y, m_moveModel.z, 1.0f));              // �^���ʃx�N�g���̒���
		float fMoveLen2 = XMVectorGetX(vMoveLen2);

#if SETCONTROLLER
		///////////////////////////////////////////////////////////////////////////////////
				//--------------		�k�X�e�B�b�N����		-----------------
				//�EXbox�̃R���g���[���[�ɂ͗V�сiDEADZONE�j�̕��������邽�߁A
				//	�V�т̕����ɃX�e�B�b�N������ꍇ�͓��͂���Ă��Ȃ��Ƃ���
				//�@�������K�v�B�V�ѕ����̒l�͈̔͂͒萔���p�ӁiTHUMB_DEADZONE�j
				//�@����Ă��邽�߁A��������g�p�B
		if (g_pXController->Gamepad.sThumbLX	<  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&
			g_pXController->Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			g_pXController->Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&
			g_pXController->Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{//.�V�ѕ����ɂk�X�e�B�b�N������ꍇ�͖���������B
			g_pXController->Gamepad.sThumbLX = 0;
			g_pXController->Gamepad.sThumbLY = 0;
		}
		else {
#endif 

			// �ړ��L�[�ɂ���ĉ�]�̖ړI�����ƈړ����������߂�
			if (GetKeyPress(VK_LEFT)
#if SETCONTROLLER
				|| g_pXController->Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
#endif
				) {
				m_nModelType = LEFT;
				//CSound::Play(SE_PMBE);
				if (GetKeyPress(VK_UP)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
#endif
					) {
					// ���O�ړ�					
					
					m_vRotDest = XMVectorSet(-f3MoveX.x + f3MoveZ.x, -f3MoveX.y + f3MoveZ.y, -f3MoveX.z + f3MoveZ.z, 1.0f);
					m_vRotDest = XMVector3Normalize(m_vRotDest);
				}
				else if (GetKeyPress(VK_DOWN)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
#endif
					) {

					// ����ړ�				

					m_vRotDest = XMVectorSet(-f3MoveX.x - f3MoveZ.x, -f3MoveX.y - f3MoveZ.y, -f3MoveX.z - f3MoveZ.z, 1.0f);
					m_vRotDest = XMVector3Normalize(m_vRotDest);
				}
				else {
					// ���ړ�
					//m_nModelType = LEFT;
					m_vRotDest = XMVectorSet(-f3MoveX.x, -f3MoveX.y, -f3MoveX.z, 1.0f);
					m_vRotDest = XMVector3Normalize(m_vRotDest);
				}
			}
			else if (GetKeyPress(VK_RIGHT)
#if SETCONTROLLER
				|| g_pXController->Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
#endif
				) {
				m_nModelType = RIGHT;
				//CSound::Play(SE_PMBE);
				if (GetKeyPress(VK_UP)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
#endif
					) {
					// �E�O�ړ�					

					m_vRotDest = XMVectorSet(f3MoveX.x + f3MoveZ.x, f3MoveX.y + f3MoveZ.y, f3MoveX.z + f3MoveZ.z, 1.0f);
					m_vRotDest = XMVector3Normalize(m_vRotDest);
				}
				else if (GetKeyPress(VK_DOWN)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
#endif
					) {
					// �E��ړ�			

					m_vRotDest = XMVectorSet(f3MoveX.x - f3MoveZ.x, f3MoveX.y - f3MoveZ.y, f3MoveX.z - f3MoveZ.z, 1.0f);
					m_vRotDest = XMVector3Normalize(m_vRotDest);
				}
				else {
					// �E�ړ�
					//m_nModelType = RIGHT;
					m_vRotDest = XMVectorSet(f3MoveX.x, f3MoveX.y, f3MoveX.z, 1.0f);
					m_vRotDest = XMVector3Normalize(m_vRotDest);

				}
			}
			else if (GetKeyPress(VK_UP)
#if SETCONTROLLER
				|| g_pXController->Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
#endif
				) {
				m_nModelType = FRONT;
				m_vRotDest = XMVectorSet(f3MoveZ.x, f3MoveZ.y, f3MoveZ.z, 1.0f);
				m_vRotDest = XMVector3Normalize(m_vRotDest);
			}
			else if (GetKeyPress(VK_DOWN)
#if SETCONTROLLER
				|| g_pXController->Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
#endif
				) {
				//CSound::Play(SE_PMBE);

			}
			else {
				m_nModelType = FRONT;
			}


#if SETCONTROLLER
		}
#endif
		if (GetKeyPress(VK_2)
#if SETCONTROLLER
			|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_BACK
#endif
			)
		{
			SetFadeOut(SCENE_TITLE);			// �V�[���J��
		}


		f3Move.x = XMVectorGetX(m_vNowRot/*m_vRotDest*/) / VAL_MAXROT * VALUE_MOVE_MODEL * m_Enzine.GetRotSpeed();
		f3Move.y = XMVectorGetY(m_vNowRot/*m_vRotDest*/) / VAL_MAXROT * VALUE_MOVE_MODEL * m_Enzine.GetRotSpeed();
		f3Move.z = XMVectorGetZ(m_vNowRot/*m_vRotDest*/) / VAL_MAXROT * VALUE_MOVE_MODEL * m_Enzine.GetRotSpeed();

		//**********************************************************************************
		// ��]�������ȏ�ɂȂ�Ɛn�𒆐S�ɉ�]����i�\���j
		/*XMMATRIX BreakRot;*/
		if (m_Enzine.GetRotSpeed()/*m_RotSpeed*/ - BREAK_ROT > 0) {
			// �i�s�����x�N�g���Ǝ��n�_�x�N�g���̊O�ςŉ�]�����Ƃ�
			// 90�t���[���ň��]�����]�s����쐬
			//BreakRot = XMMatrixRotationAxis(XMVector3Cross(m_vNowRot, XMVectorSet(
			//	m_posModel.x - g_pEnemy->GetCylinderP()->GetPos().x,
			//	m_posModel.y - g_pEnemy->GetCylinderP()->GetPos().y,
			//	m_posModel.z - g_pEnemy->GetCylinderP()->GetPos().z/*0.0f*/,
			//	1.0f)), -((2.0f * 3.14159265359f) / 90.0f *(m_Enzine.GetRotSpeed()/*m_RotSpeed*/ - BREAK_ROT)));
			m_bBreak = true;
		}
		else {
			/*BreakRot = XMMatrixIdentity();*/
			m_bBreak = false;
		}

		//****************************************************************************************************
		// �؂�ڂƂ̓����蔻��ƃA�V�X�g�֘A����

		if (m_bBreak) {
			XMFLOAT3* pColPos = new XMFLOAT3;

			if (g_pEnemy->GetSpherFrontP()->CollisionKirime(m_posCut, m_posModel, 10.0f,
				20.0f, 20.0f, 20, pColPos))
			{

				CSound::Play(SE_PATK);	// ���ʉ��Đ�
				CSound::SetVolume(SE_PATK, 3.0f);

				CHitStop::Get()->HitStopOn(2);

			}
			/*if (!g_pEnemy->GetSpherFrontP()->CollisionKirime(m_posCut, m_posModel, 10.0f,
				20.0f, 20.0f, 20, pColPos))
			{
				CSound::Stop(SE_PATK);
			}*/
			delete pColPos;
		}
		//****************************************************************************************************



	//****************************************************************************************************

		//*****************************************************************************************************
		// ���f���̌�����i�s�����Ɍ����鏈��
		XMVECTOR R;
		float fSita = 0.0f;
		XMMATRIX DiffRotationY = XMMatrixIdentity();                   // Y�����������Ƃ����]�s��̏�����
		float fLRSita;


		R = XMVector3AngleBetweenVectors(m_vNowRot, m_vRotDest);      // ���݌����ƖړI�����̊p�x��
		//********************************************************

		//********************************************************
																		  // �x�N�g������]������p�x
		fSita = XMVectorGetX(R);                                           // ���W�A���p�x��float�Ɋi�[


		XMVECTOR vCut = XMVectorSet(m_vecCut.x, m_vecCut.y, m_vecCut.z, 1.0f);


		if (!(fSita == 0.0f)) {
			// �ړI�����x�N�g���ƃv���C���[�̃��[�J��Y�������x�N�g���̊O�ςŊ�x�N�g�����쐬
			// ��x�N�g���ƌ��ݕ����x�N�g���̊p�x����|90��|�ȏ�Ȃ��]�������t�ɂ���

			R = XMVector3AngleBetweenVectors(XMVector3Normalize(XMVector3Cross(m_vRotDest, vMoveY)), m_vNowRot);

			fLRSita = XMVectorGetX(R);

			if (fLRSita > 3.14159265359f / 2) {
				fSita = fSita * (-1);
			}
			if (m_bBreak) {
				fSita = fSita * RATE_ROTATE_MODEL_BREAK;
			}
			else
			{
				fSita = fSita * RATE_ROTATE_MODEL_BREAK;
				//fSita = fSita * RATE_ROTATE_MODEL;
			}

			DiffRotationY = XMMatrixRotationY(fSita); // Y����]�s����쐬

			m_RotationY = XMMatrixMultiply(m_RotationY, DiffRotationY); // ���[���h�s��Ɋ|����p�s��̏C��

			m_RotationNor = XMMatrixRotationAxis(vMoveY, fSita);

			m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_RotationNor); // ���݌���������ړI�����ɏC��
			g_NorVec = m_vNowRot;


			vCut = XMVector3TransformNormal(vCut, XMLoadFloat4x4(&m_RotationNor4x4));

			m_vRotDest = m_vNowRot;

		}
		else {
			m_RotationNor = XMMatrixIdentity();
		}


		//*****************************************************************************************************

		//****************************************************************************************************
		if (m_bBreak) {
			vMove = XMVectorSet(f3Move.x * 1.3f, f3Move.y * 1.3f, f3Move.z * 1.3f, 1.0f);
		}
		else {
			vMove = XMVectorSet(f3Move.x, f3Move.y, f3Move.z, 1.0f); // �^���ʃx�N�g��
		}

		XMVECTOR vMoveLen = XMVector3Length(vMove);              // �^���ʃx�N�g���̒���
		float fMoveLen = XMVectorGetX(vMoveLen);

		if (!fMoveLen == 0.0f || !fMoveLen2 == 0.0f || m_bJumpflg || GetKeyPress(VK_J)
#if SETCONTROLLER
			|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
#endif
			)
		{
			XMVECTOR Vpm;
			XMVECTOR Vs;
			XMVECTOR Vn;

			XMFLOAT3 f3V1, f3V2;
			//XMVECTOR vGravity;

			switch (m_nOnFieldType) {
			case RECT_FIELD:
				/*m_moveModel.x = f3Move.x;
				m_moveModel.y = f3Move.y;
				m_moveModel.z = f3Move.z;*/
				break;
			case CIRCLE_FIELD:


				Vpm = XMVector3Cross(/*vMove*/m_vNowRot, vMoveY);  // �^���ʃx�N�g���ƃ��[�J��Y�������x�N�g���̊O�ςŉ�]���x�N�g�����쐬

				// �ʂ̒����i�ړ������j�Ɣ��a���璆�S�p���v�Z
				m_fRazianDest = (fMoveLen  * 360.0f / (2.0f * 3.14159265359f * (ENEMY_RAD))) * 3.14159265359f / 180.0f;

				m_fRazian += (m_fRazianDest - m_fRazian) * 0.1f;

				// ���̒��S���玩�n�_�����x�N�g�����ړ���n�_�܂ŉ�]������p�̉�]�s��
				m_VRotToNextMat = XMMatrixRotationAxis(Vpm, -m_fRazian);

				// ���[���h���W�ړ��𔽉f������p�̍s����쐬
				m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

				// ���ݕ����x�N�g���ƖړI�����x�N�g�������̏�̈ړ�����]
				m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_VRotToNextMat);
				m_vRotDest = XMVector3TransformNormal(m_vRotDest, m_VRotToNextMat);

				// ���ݒn�_�x�N�g���ƈړ���n�_�x�N�g��
				switch (m_nOnFieldName)
				{
				case BODY_NAME:
					Vs = XMVectorSet(
						m_posModel.x - g_pEnemy->GetCylinderP()->GetPos().x,
						m_posModel.y - g_pEnemy->GetCylinderP()->GetPos().y,
						m_posModel.z - g_pEnemy->GetCylinderP()->GetPos().z,
						1.0f);
					break;
				case FRONTCUBE_NAME:
					Vs = XMVectorSet(
						m_posModel.x - g_pEnemy->GetSpherFrontP()->GetPos().x,
						m_posModel.y - g_pEnemy->GetSpherFrontP()->GetPos().y,
						m_posModel.z - g_pEnemy->GetSpherFrontP()->GetPos().z,
						1.0f);
					break;
				/*case BACKCUBE_NAME:
					Vs = XMVectorSet(
						m_posModel.x - g_pEnemy->GetSpherBackP()->GetPos().x,
						m_posModel.y - g_pEnemy->GetSpherBackP()->GetPos().y,
						m_posModel.z - g_pEnemy->GetSpherBackP()->GetPos().z,
						1.0f);
					break;*/
				}


				Vn = XMVector3TransformNormal(Vs, m_VRotToNextMat);

				// ���ݒn�_�x�N�g���ƈړ���n�_�x�N�g���̍�����ړ��悪���̕\�ʏ�ɂȂ�悤�ŏI�I�ȉ^���ʂ��v�Z
				XMStoreFloat3(&f3V1, Vs);
				XMStoreFloat3(&f3V2, Vn);

				m_moveModel.x = f3V2.x - f3V1.x;
				m_moveModel.y = f3V2.y - f3V1.y;
				m_moveModel.z = f3V2.z - f3V1.z;

				// �W�����v����
				if (GetKeyPress(VK_J)
#if SETCONTROLLER
					|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
#endif
					&& !m_bJumpflg) {
					m_bJumpflg = true;
					XMVECTOR vJumpMove = XMVector3Normalize(XMVectorSet(
						(m_posModel.x - g_pEnemy->GetSpherFrontP()->GetPos().x),
						(m_posModel.y - g_pEnemy->GetSpherFrontP()->GetPos().y),
						(m_posModel.z - g_pEnemy->GetSpherFrontP()->GetPos().z),
						1.0f));

					m_f3JumpMoveUp = XMFLOAT3(
						XMVectorGetX(vJumpMove) * 4,
						XMVectorGetY(vJumpMove) * 4,
						XMVectorGetZ(vJumpMove) * 4
					);

					m_f3JumpMoveDown.x = -m_f3JumpMoveUp.x;
					m_f3JumpMoveDown.y = -m_f3JumpMoveUp.y;
					m_f3JumpMoveDown.z = -m_f3JumpMoveUp.z;
				}
				if (m_bJumpflg) {
					m_f3JumpMoveUp.x = m_f3JumpMoveUp.x - m_f3JumpMoveUp.x * 0.02f;
					m_f3JumpMoveUp.y = m_f3JumpMoveUp.y - m_f3JumpMoveUp.y * 0.02f;
					m_f3JumpMoveUp.z = m_f3JumpMoveUp.z - m_f3JumpMoveUp.z * 0.02f;

					XMVECTOR vJumpMoveUp = XMVectorSet(m_f3JumpMoveUp.x, m_f3JumpMoveUp.y, m_f3JumpMoveUp.z, 1.0f);
					vJumpMoveUp = XMVector3TransformNormal(vJumpMoveUp, m_VRotToNextMat);

					XMVECTOR vJumpMoveDown = XMVectorSet(m_f3JumpMoveDown.x, m_f3JumpMoveDown.y, m_f3JumpMoveDown.z, 1.0f);
					vJumpMoveDown = XMVector3TransformNormal(vJumpMoveDown, m_VRotToNextMat);

					XMStoreFloat3(&m_f3JumpMoveUp, vJumpMoveUp);
					XMStoreFloat3(&m_f3JumpMoveDown, vJumpMoveDown);

					if ((int)XMVectorGetX(XMVector3Length(vJumpMoveUp)) == 0) {

					}
					else
					{
						m_moveModel.x += m_f3JumpMoveUp.x;
						m_moveModel.y += m_f3JumpMoveUp.y;
						m_moveModel.z += m_f3JumpMoveUp.z;
					}
				}
				//*********************************************************************


				break;

			case CYLINDER_FIELD:
				fMoveLen = XMVectorGetX(XMVector3Length(XMVectorSet(f3Move.x, f3Move.y, 0.0f, 1.0f)));

				//XMVECTOR Vlr = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f), vMoveY));

				// ���̒��S���玩�n�_�����x�N�g�����ړ���n�_�܂ŉ�]������p�̉�]�s��
				if (XMVectorGetX(XMVector3AngleBetweenVectors(XMVector3Normalize(XMVector3Cross(m_vNowRot, vMoveY)), XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f))) > 3.14159265359f / 2)
				{
					fSita = (fMoveLen  * 360.0f / (2.0f * 3.14159265359f * (ENEMY_RAD))) * 3.14159265359f / 180.0f;
				}
				else
				{
					fSita = -(fMoveLen  * 360.0f / (2.0f * 3.14159265359f * (ENEMY_RAD))) * 3.14159265359f / 180.0f;
				}

				XMMATRIX RotZmtx = XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f), fSita);
				Vs = XMVectorSet(
					m_posModel.x - g_pEnemy->GetCylinderP()->GetPos().x,
					m_posModel.y - g_pEnemy->GetCylinderP()->GetPos().y,
					m_posModel.z - g_pEnemy->GetCylinderP()->GetPos().z,
					1.0f);
				Vn = XMVector3TransformNormal(Vs, RotZmtx);
				XMStoreFloat3(&f3V1, Vs);
				XMStoreFloat3(&f3V2, Vn);

				m_moveModel.x = f3V2.x - f3V1.x;
				m_moveModel.y = f3V2.y - f3V1.y;
				m_moveModel.z = f3V2.z - f3V1.z;

				m_moveModel.z += f3Move.z * 4.0f;

				fSita = XMVectorGetX(XMVector3AngleBetweenVectors(Vs, Vn));
				if (!XMVector3Equal(Vs, Vn)/*fSita == 0.0f*/) {
					// ���̒��S���玩�n�_�����x�N�g�����ړ���n�_�܂ŉ�]������p�̉�]�s��
					m_VRotToNextMat = XMMatrixRotationAxis(XMVector3Cross(Vs, Vn), fSita);

					// ���[���h���W�ړ��𔽉f������p�̍s����쐬
					m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

					// ���ݕ����x�N�g���ƖړI�����x�N�g�������̏�̈ړ�����]
					m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_VRotToNextMat);
					m_vRotDest = XMVector3TransformNormal(m_vRotDest, m_VRotToNextMat);
				}
				else {
					m_VRotToNextMat = XMMatrixIdentity();
				}
				//************************************************************************************************************************



				// �W�����v����
				if (GetKeyPress(VK_J))
				
#if SETCONTROLLER
					|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
#endif
				{
						 XMVECTOR vJumpMove = XMVector3Normalize(XMVectorSet(
						(m_posModel.x - g_pEnemy->GetCylinderP()->GetPos().x),
						(m_posModel.y - g_pEnemy->GetCylinderP()->GetPos().y),
						(m_posModel.z - g_pEnemy->GetCylinderP()->GetPos().z)
						,1.0f));

					m_f3JumpMoveUp = XMFLOAT3(
						XMVectorGetX(vJumpMove) * 4,
						XMVectorGetY(vJumpMove) * 4,
						XMVectorGetZ(vJumpMove) * 4
					);

					m_f3JumpMoveDown.x = -m_f3JumpMoveUp.x;
					m_f3JumpMoveDown.y = -m_f3JumpMoveUp.y;
					m_f3JumpMoveDown.z = -m_f3JumpMoveUp.z;
				}
				if (m_bJumpflg) {
					m_f3JumpMoveUp.x = m_f3JumpMoveUp.x - m_f3JumpMoveUp.x * 0.02f;
					m_f3JumpMoveUp.y = m_f3JumpMoveUp.y - m_f3JumpMoveUp.y * 0.02f;
					m_f3JumpMoveUp.z = m_f3JumpMoveUp.z - m_f3JumpMoveUp.z * 0.02f;

					XMVECTOR vJumpMoveUp = XMVectorSet(m_f3JumpMoveUp.x, m_f3JumpMoveUp.y, m_f3JumpMoveUp.z, 1.0f);
					vJumpMoveUp = XMVector3TransformNormal(vJumpMoveUp, m_VRotToNextMat);

					XMVECTOR vJumpMoveDown = XMVectorSet(m_f3JumpMoveDown.x, m_f3JumpMoveDown.y, m_f3JumpMoveDown.z, 1.0f);
					vJumpMoveDown = XMVector3TransformNormal(vJumpMoveDown, m_VRotToNextMat);

					XMStoreFloat3(&m_f3JumpMoveUp, vJumpMoveUp);
					XMStoreFloat3(&m_f3JumpMoveDown, vJumpMoveDown);

					if ((int)XMVectorGetX(XMVector3Length(vJumpMoveUp)) == 0) {

					}
					else
					{
						m_moveModel.x += m_f3JumpMoveUp.x;
						m_moveModel.y += m_f3JumpMoveUp.y;
						m_moveModel.z += m_f3JumpMoveUp.z;
					}
				}


				//*********************************************************************



				break;
			}


		}
		else
		{
			m_VRotToNextMat = XMMatrixIdentity();

		}


		//********************************************************************

		//*********************************************************************

		// �ʒu�ړ�
		m_posModel.x += m_moveModel.x;
		m_posModel.y += m_moveModel.y;
		m_posModel.z += m_moveModel.z;

		XMVECTOR vGravity;


		switch (m_nOnFieldType) {
		case RECT_FIELD:
			/*m_moveModel.x = f3Move.x;
			m_moveModel.y = f3Move.y;
			m_moveModel.z = f3Move.z;*/
			break;
		case CIRCLE_FIELD:


			if (CollisionCircleToCircle(m_posModel, g_pEnemy->GetSpherFrontP()->GetPos(), 0.0f, /*MODEL_POSY*/560.0f + 35.0f)) {
				XMVECTOR Vs;
				XMVECTOR Vn;
				float fSita;

				XMFLOAT3 f3V1, f3V2;

				Vn = XMVectorSet(
					m_posModel.x - g_pEnemy->GetSpherFrontP()->GetPos().x,
					m_posModel.y - g_pEnemy->GetSpherFrontP()->GetPos().y,
					m_posModel.z - g_pEnemy->GetSpherFrontP()->GetPos().z,
					1.0f);



				Vs = XMVectorSet(
					(m_posModel.x - m_moveModel.x) - g_pEnemy->GetSpherFrontP()->GetPos().x,
					(m_posModel.y - m_moveModel.y) - g_pEnemy->GetSpherFrontP()->GetPos().y,
					(m_posModel.z - m_moveModel.z) - g_pEnemy->GetSpherFrontP()->GetPos().z,
					1.0f);

				//Z�����̕�
				if (m_posModel.z < 800.0f) {

					m_posModel.x -= m_moveModel.x;
					m_posModel.y -= m_moveModel.y;
					m_posModel.z -= m_moveModel.z;

					// �t�s��ŉ�]�𖳌��ɂ���
					m_VRotToNextMat = XMMatrixInverse(nullptr, m_VRotToNextMat);
					// ���[���h���W�ړ��𔽉f������p�̍s����쐬
					m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

					// ���ݕ����x�N�g���ƖړI�����x�N�g�������̏�̈ړ�����]
					m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_VRotToNextMat);
					m_vRotDest = XMVector3TransformNormal(m_vRotDest, m_VRotToNextMat);
					m_VRotToNextMat = XMMatrixIdentity();

					//********************************************************************

					Vs = XMVectorSet(
						XMVectorGetX(Vs),
						XMVectorGetY(Vs),
						0.0f,
						1.0f);
					Vn = XMVectorSet(
						XMVectorGetX(Vn),
						XMVectorGetY(Vn),
						0.0f,
						1.0f);


					// ���ݒn�_�x�N�g���ƈړ���n�_�x�N�g���̍�����ړ��悪���̕\�ʏ�ɂȂ�悤�ŏI�I�ȉ^���ʂ��v�Z
					XMStoreFloat3(&f3V1, Vs);
					XMStoreFloat3(&f3V2, Vn);

					m_moveModel.x = f3V2.x - f3V1.x;
					m_moveModel.y = f3V2.y - f3V1.y;
					m_moveModel.z = 0.0f;

					// �ʒu�ړ�
					m_posModel.x += m_moveModel.x;
					m_posModel.y += m_moveModel.y;
					m_posModel.z += m_moveModel.z;

					//fSita = XMVectorGetX(XMVector3AngleBetweenNormalsEst(XMVector3Normalize(Vs), XMVector3Normalize(Vn)));
					fSita = XMVectorGetX(XMVector3AngleBetweenVectors(Vs, Vn));
					if (!(fSita == 0.0f)) {
						m_VRotToNextMat = XMMatrixRotationAxis(XMVector3Cross(Vs, Vn), fSita);

						// ���[���h���W�ړ��𔽉f������p�̍s����쐬
						m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

						// ���ݕ����x�N�g���ƖړI�����x�N�g�������̏�̈ړ�����]
						m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_VRotToNextMat);
						m_vRotDest = XMVector3TransformNormal(m_vRotDest, m_VRotToNextMat);
					}

				}


				if (
					g_pEnemy->GetEnemyParts()->ColGravitySpace(
						XMFLOAT3(
							m_posModel.x - m_moveModel.x,
							m_posModel.y - m_moveModel.y,
							m_posModel.z - m_moveModel.z)) < 0
					&&
					g_pEnemy->GetEnemyParts()->ColGravitySpace(
						XMFLOAT3(
							m_posModel.x,
							m_posModel.y,
							m_posModel.z)) > 0)
				{
					// X�����̕�
					if (
						(XMVectorGetX(Vs)/* + m_posModel.x*/ <= 0.0f && 0.0f <= XMVectorGetX(Vn) /*+ m_posModel.x*/) ||
						(XMVectorGetX(Vn)/* + m_posModel.x*/ <= 0.0f && 0.0f <= XMVectorGetX(Vs) /*+ m_posModel.x*/)
						) {
						m_posModel.x -= m_moveModel.x;
						m_posModel.y -= m_moveModel.y;
						m_posModel.z -= m_moveModel.z;

						// �t�s��ŉ�]�𖳌��ɂ���
						m_VRotToNextMat = XMMatrixInverse(nullptr, m_VRotToNextMat);
						// ���[���h���W�ړ��𔽉f������p�̍s����쐬
						m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

						// ���ݕ����x�N�g���ƖړI�����x�N�g�������̏�̈ړ�����]
						m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_VRotToNextMat);
						m_vRotDest = XMVector3TransformNormal(m_vRotDest, m_VRotToNextMat);
						m_VRotToNextMat = XMMatrixIdentity();
						//************************************************************************************:
						Vs = XMVectorSet(
							0.0f,
							XMVectorGetY(Vs),
							XMVectorGetZ(Vs),
							1.0f);
						Vn = XMVectorSet(
							0.0f,
							XMVectorGetY(Vn),
							XMVectorGetZ(Vn),
							1.0f);

						// ���ݒn�_�x�N�g���ƈړ���n�_�x�N�g���̍�����ړ��悪���̕\�ʏ�ɂȂ�悤�ŏI�I�ȉ^���ʂ��v�Z
						XMStoreFloat3(&f3V1, Vs);
						XMStoreFloat3(&f3V2, Vn);
						/*XMStoreFloat3(&f3V1, Va);
						XMStoreFloat3(&f3V2, Vb);*/

						m_moveModel.x = f3V2.x - f3V1.x;
						m_moveModel.y = f3V2.y - f3V1.y;
						m_moveModel.z = f3V2.z - f3V1.z;

						// �ʒu�ړ�
						m_posModel.x += m_moveModel.x;
						m_posModel.y += m_moveModel.y;
						m_posModel.z += m_moveModel.z;


						//fSita = XMVectorGetX(XMVector3AngleBetweenVectors(Va, Vb));
						fSita = XMVectorGetX(XMVector3AngleBetweenVectors(Vs, Vn));
						if (!(fSita == 0.0f)) {
							if (!(0.0f == XMVectorGetX(XMVector3Length(XMVector3Cross(Vs, Vn))))) {
								m_VRotToNextMat = XMMatrixRotationAxis(XMVector3Cross(Vs, Vn), fSita);
								/*if (!(0.0f == XMVectorGetX(XMVector3Length(XMVector3Cross(Va, Vb))))) {
									m_VRotToNextMat = XMMatrixRotationAxis(XMVector3Cross(Va, Vb), fSita);*/
									// ���[���h���W�ړ��𔽉f������p�̍s����쐬
								m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

								// ���ݕ����x�N�g���ƖړI�����x�N�g�������̏�̈ړ�����]
								m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_VRotToNextMat);
								m_vRotDest = XMVector3TransformNormal(m_vRotDest, m_VRotToNextMat);
							}
						}
					}

					// Y�����̕�
					if ((XMVectorGetY(Vs) <= 0.0f && 0.0f <= XMVectorGetY(Vn)) ||
						(XMVectorGetY(Vn) <= 0.0f && 0.0f <= XMVectorGetY(Vs))
						) {
						m_posModel.x -= m_moveModel.x;
						m_posModel.y -= m_moveModel.y;
						m_posModel.z -= m_moveModel.z;

						// �t�s��ŉ�]�𖳌��ɂ���
						m_VRotToNextMat = XMMatrixInverse(nullptr, m_VRotToNextMat);
						// ���[���h���W�ړ��𔽉f������p�̍s����쐬
						m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

						// ���ݕ����x�N�g���ƖړI�����x�N�g�������̏�̈ړ�����]
						m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_VRotToNextMat);
						m_vRotDest = XMVector3TransformNormal(m_vRotDest, m_VRotToNextMat);
						m_VRotToNextMat = XMMatrixIdentity();

						//******************************************************
						Vs = XMVectorSet(
							XMVectorGetX(Vs),
							0.0f,
							XMVectorGetZ(Vs),
							1.0f);
						Vn = XMVectorSet(
							XMVectorGetX(Vn),
							0.0f,
							XMVectorGetZ(Vn),
							1.0f);



						XMStoreFloat3(&f3V1, Vs);
						XMStoreFloat3(&f3V2, Vn);

						m_moveModel.x = f3V2.x - f3V1.x;
						m_moveModel.y = f3V2.y - f3V1.y;
						m_moveModel.z = f3V2.z - f3V1.z;

						// �ʒu�ړ�
						m_posModel.x += m_moveModel.x;
						m_posModel.y += m_moveModel.y;
						m_posModel.z += m_moveModel.z;

						//fSita = XMVectorGetX(XMVector3AngleBetweenVectors(Va, Vb));
						fSita = XMVectorGetX(XMVector3AngleBetweenVectors(Vs, Vn));
						if (!(fSita == 0.0f)) {
							if (!(0.0f == XMVectorGetX(XMVector3Length(XMVector3Cross(Vs, Vn))))) {
								m_VRotToNextMat = XMMatrixRotationAxis(XMVector3Cross(Vs, Vn), fSita);
								/*if (!(0.0f == XMVectorGetX(XMVector3Length(XMVector3Cross(Va, Vb))))) {
									m_VRotToNextMat = XMMatrixRotationAxis(XMVector3Cross(Va, Vb), fSita);*/
									// ���[���h���W�ړ��𔽉f������p�̍s����쐬
								m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

								// ���ݕ����x�N�g���ƖړI�����x�N�g�������̏�̈ړ�����]
								m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_VRotToNextMat);
								m_vRotDest = XMVector3TransformNormal(m_vRotDest, m_VRotToNextMat);
							}
						}
					}
				}
			}
			// �d�͏���
			vGravity = XMVector3Normalize(XMVectorSet(
				(g_pEnemy->GetSpherFrontP()->GetPos().x - m_posModel.x),
				(g_pEnemy->GetSpherFrontP()->GetPos().y - m_posModel.y),
				(g_pEnemy->GetSpherFrontP()->GetPos().z - m_posModel.z),
				1.0f));

			if (g_pEnemy->GetEnemyParts()->ColGravitySpace(m_posModel) > 0) {
				if (CollisionCircleToCircle(m_posModel, g_pEnemy->GetSpherFrontP()->GetPos(),
					0.0f, 560.0f + 40.0f)) {
					m_bJumpflg = false;
					m_posModel.x -= XMVectorGetX(vGravity);
					m_posModel.y -= XMVectorGetY(vGravity);
					m_posModel.z -= XMVectorGetZ(vGravity);
				}

			}
			else
			{
				if (CollisionCircleToCircle(m_posModel, g_pEnemy->GetSpherFrontP()->GetPos(),
					0.0f, 560.0f)) {
					m_bJumpflg = false;
					m_posModel.x -= XMVectorGetX(vGravity);
					m_posModel.y -= XMVectorGetY(vGravity);
					m_posModel.z -= XMVectorGetZ(vGravity);
				}
			}

			if (0 == CollisionCircleToCircle(m_posModel, g_pEnemy->GetSpherFrontP()->GetPos(),
				0.0f, 560.0f)) {
				m_posModel.x += XMVectorGetX(vGravity);
				m_posModel.y += XMVectorGetY(vGravity);
				m_posModel.z += XMVectorGetZ(vGravity);
			}

			break;

		case CYLINDER_FIELD:
			//********************************************************************
				// �d�͏���
			vGravity = XMVector3Normalize(XMVectorSet(
				(g_pEnemy->GetCylinderP()->GetPos().x - m_posModel.x),
				(g_pEnemy->GetCylinderP()->GetPos().y - m_posModel.y),
				(g_pEnemy->GetCylinderP()->GetPos().z - m_posModel.z),
				1.0f));


			m_posModel.x += XMVectorGetX(vGravity);
			m_posModel.y += XMVectorGetY(vGravity);
			m_posModel.z += XMVectorGetZ(vGravity);

			if (g_pEnemy->GetEnemyParts()->ColGravitySpace(m_posModel) < 0) {
				if (CollisionCircleToCircle(m_posModel, g_pEnemy->GetCylinderP()->GetPos(), 0.0f, MODEL_POSY)) {
					m_bJumpflg = false;


					m_posModel.x -= XMVectorGetX(vGravity);
					m_posModel.y -= XMVectorGetY(vGravity);
					m_posModel.z -= XMVectorGetZ(vGravity);

				}
			}
			else {
				if (CollisionCircleToCircle(m_posModel, g_pEnemy->GetCylinderP()->GetPos(), 0.0f, MODEL_POSY + 70.0f)) {
					m_bJumpflg = false;

					m_posModel.x -= XMVectorGetX(vGravity);
					m_posModel.y -= XMVectorGetY(vGravity);
					m_posModel.z -= XMVectorGetZ(vGravity);

				}
			}


			break;
		}


		if (CollisionRectToPoint(XMFLOAT3(0.0f, 530.0f, 380.0f), XMFLOAT3(400.0f, 400.0f, 400.0f), m_posModel)) {
			XMVECTOR Vs;
			XMVECTOR Vn;
			float fSita;

			XMFLOAT3 f3V1, f3V2;

			Vn = XMVectorSet(
				m_posModel.x - g_pEnemy->GetCylinderP()->GetPos().x,
				m_posModel.y - g_pEnemy->GetCylinderP()->GetPos().y,
				m_posModel.z - g_pEnemy->GetCylinderP()->GetPos().z,
				1.0f);

			m_posModel.x -= m_moveModel.x;
			m_posModel.y -= m_moveModel.y;
			m_posModel.z -= m_moveModel.z;

			// �t�s��ŉ�]�𖳌��ɂ���
			m_VRotToNextMat = XMMatrixInverse(nullptr, m_VRotToNextMat);
			// ���[���h���W�ړ��𔽉f������p�̍s����쐬
			m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

			// ���ݕ����x�N�g���ƖړI�����x�N�g�������̏�̈ړ�����]
			m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_VRotToNextMat);
			m_vRotDest = XMVector3TransformNormal(m_vRotDest, m_VRotToNextMat);
			m_VRotToNextMat = XMMatrixIdentity();

			Vs = XMVectorSet(
				m_posModel.x - g_pEnemy->GetCylinderP()->GetPos().x,
				m_posModel.y - g_pEnemy->GetCylinderP()->GetPos().y,
				m_posModel.z - g_pEnemy->GetCylinderP()->GetPos().z,
				1.0f);

			if ((m_posModel.z - m_moveModel.z) < 380.0f - 400.0f / 2.0f || 380.0f + 400.0f / 2.0f < (m_posModel.z - m_moveModel.z)) {
				Vs = XMVectorSet(
					XMVectorGetX(Vs),
					XMVectorGetY(Vs),
					0.0f,
					1.0f);
				Vn = XMVectorSet(
					XMVectorGetX(Vn),
					XMVectorGetY(Vn),
					0.0f,
					1.0f);


				// ���ݒn�_�x�N�g���ƈړ���n�_�x�N�g���̍�����ړ��悪���̕\�ʏ�ɂȂ�悤�ŏI�I�ȉ^���ʂ��v�Z
				XMStoreFloat3(&f3V1, Vs);
				XMStoreFloat3(&f3V2, Vn);

				m_moveModel.x = f3V2.x - f3V1.x;
				m_moveModel.y = f3V2.y - f3V1.y;
				m_moveModel.z = 0.0f;

				// �ʒu�ړ�
				m_posModel.x += m_moveModel.x;
				m_posModel.y += m_moveModel.y;
				m_posModel.z += m_moveModel.z;

				fSita = XMVectorGetX(XMVector3AngleBetweenNormalsEst(XMVector3Normalize(Vs), XMVector3Normalize(Vn)));
				if (!(fSita == 0.0f)) {
					m_VRotToNextMat = XMMatrixRotationAxis(XMVector3Cross(Vs, Vn), fSita);

					// ���[���h���W�ړ��𔽉f������p�̍s����쐬
					m_RotMat = XMMatrixMultiply(m_RotMat, m_VRotToNextMat);

					// ���ݕ����x�N�g���ƖړI�����x�N�g�������̏�̈ړ�����]
					m_vNowRot = XMVector3TransformNormal(m_vNowRot, m_VRotToNextMat);
					m_vRotDest = XMVector3TransformNormal(m_vRotDest, m_VRotToNextMat);
				}
			}
			else {
				Vs = XMVectorSet(
					0.0f,
					0.0f/*XMVectorGetY(Vs)*/,
					XMVectorGetZ(Vs),
					1.0f);
				Vn = XMVectorSet(
					0.0f/*XMVectorGetX(Vs)*/,
					0.0f/*XMVectorGetY(Vn)*/,
					XMVectorGetZ(Vn),
					1.0f);

				// ���ݒn�_�x�N�g���ƈړ���n�_�x�N�g���̍�����ړ��悪���̕\�ʏ�ɂȂ�悤�ŏI�I�ȉ^���ʂ��v�Z
				XMStoreFloat3(&f3V1, Vs);
				XMStoreFloat3(&f3V2, Vn);

				m_moveModel.x = 0.0f/*f3V2.x - f3V1.x*/;
				m_moveModel.y = 0.0f/*f3V2.y - f3V1.y*/;
				m_moveModel.z = f3V2.z - f3V1.z;

				// �ʒu�ړ�
				m_posModel.x += m_moveModel.x;
				m_posModel.y += m_moveModel.y;
				m_posModel.z += m_moveModel.z;


			}


		}

		if (m_posModel.z < 50.0f) {
			m_posModel.z = 50.0f;
		}


		if (!XMMatrixIsIdentity(XMLoadFloat4x4(&m_VRotToNextMat4x4))) {
			vCut = XMVector3TransformNormal(vCut, XMLoadFloat4x4(&m_VRotToNextMat4x4));
		}
		XMStoreFloat3(&m_vecCut, vCut);
		m_posCut.x = m_posModel.x + m_vecCut.x;
		m_posCut.y = m_posModel.y + m_vecCut.y;
		m_posCut.z = m_posModel.z + m_vecCut.z;




		// �����Ă���n�ʐؑփt�B�[���h����
		m_nOnFieldType = g_pEnemy->ChangeField(m_posModel, m_nOnFieldType);


		PrintDebugProc("[��� �� : (%f, %f, %f)]\n", m_posModel.x, m_posModel.y, m_posModel.z);
		//PrintDebugProc("[��� ѷ : (%f)]\n", g_rotModel.y);
		PrintDebugProc("\n");

		PrintDebugProc("*** ��� ��� ***\n");
		PrintDebugProc("ϴ   ��޳ : \x1e\n");//��
		PrintDebugProc("���  ��޳ : \x1f\n");//��
		PrintDebugProc("���� ��޳ : \x1d\n");//��
		PrintDebugProc("з�  ��޳ : \x1c\n");//��
		PrintDebugProc("���� �ݶ� : LSHIFT\n");
		PrintDebugProc("з�  �ݶ� : RSHIFT\n");
		PrintDebugProc("\n");
	}
	XMMATRIX mtxWorld, mtxScl, /*mtxRot,*/ mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(m_sclModel.x, m_sclModel.y, m_sclModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxWorld = XMMatrixMultiply(mtxWorld, m_RotationY);
	mtxWorld = XMMatrixMultiply(mtxWorld, m_RotMat);
	//mtxWorld = XMMatrixMultiply(mtxWorld, BreakRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(m_posModel.x, m_posModel.y, m_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&m_mtxWorld, mtxWorld);

	//*********************************************************************
	XMStoreFloat4x4(&m_VRotToNextMat4x4, m_VRotToNextMat);
	XMStoreFloat4x4(&m_RotMat4x4, m_RotMat);
	XMStoreFloat4x4(&m_RotationY4x4, m_RotationY);
	XMStoreFloat4x4(&m_RotationNor4x4, m_RotationNor);

	m_vecRotDest = XMFLOAT3(XMVectorGetX(m_vRotDest), XMVectorGetY(m_vRotDest), XMVectorGetZ(m_vRotDest));
	m_vecNowRot = XMFLOAT3(XMVectorGetX(m_vNowRot), XMVectorGetY(m_vNowRot), XMVectorGetZ(m_vNowRot));
	//*********************************************************************
	//==============================================================================================

	
	// �G���W���X�V
	m_Enzine.Updateengine();

	m_Life.UpdateLife();
}


//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	switch (m_nModelType) {
	case FRONT:
		// �J�����A�����ݒ�
		g_model[FRONT_CHARA].SetCamera(CCamera::Get());
		g_model[FRONT_CHARA].SetLight(CLight::Get());
		g_model[FRONT_MACHINE].SetCamera(CCamera::Get());
		g_model[FRONT_MACHINE].SetLight(CLight::Get());
		break;
	case LEFT:
		// �J�����A�����ݒ�
		g_model[LEFT_CHARA].SetCamera(CCamera::Get());
		g_model[LEFT_CHARA].SetLight(CLight::Get());
		g_model[LEFT_MACHINE].SetCamera(CCamera::Get());
		g_model[LEFT_MACHINE].SetLight(CLight::Get());
		break;
	case RIGHT:
		// �J�����A�����ݒ�
		g_model[RIGHT_CHARA].SetCamera(CCamera::Get());
		g_model[RIGHT_CHARA].SetLight(CLight::Get());
		g_model[RIGHT_MACHINE].SetCamera(CCamera::Get());
		g_model[RIGHT_MACHINE].SetLight(CLight::Get());
		break;

	}


	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	if (m_nInvTime % 4 == 0) {
		switch (m_nModelType) {
		case FRONT:
			// �s����������`��
			g_model[FRONT_CHARA].Draw(pDeviceContext, m_mtxWorld, eOpacityOnly);
			g_model[FRONT_MACHINE].Draw(pDeviceContext, m_mtxWorld, eOpacityOnly);
			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

			if (m_Life.GetLife() <= 0)
			{
				g_model[FRONT_CHARA].Draw(pDeviceContext, m_mtxWorld, DanDan);
				g_model[FRONT_MACHINE].Draw(pDeviceContext, m_mtxWorld, DanDan);
			}

			//g_model.Draw(pDeviceContext, m_mtxWorld, eTransparentOnly);


			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����
			break;
		case LEFT:
			// �s����������`��
			g_model[LEFT_CHARA].Draw(pDeviceContext, m_mtxWorld, eOpacityOnly);
			g_model[LEFT_MACHINE].Draw(pDeviceContext, m_mtxWorld, eOpacityOnly);
			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

			if (m_Life.GetLife() <= 0)
			{
				g_model[LEFT_CHARA].Draw(pDeviceContext, m_mtxWorld, DanDan);
				g_model[LEFT_MACHINE].Draw(pDeviceContext, m_mtxWorld, DanDan);
			}

			//g_model.Draw(pDeviceContext, m_mtxWorld, eTransparentOnly);


			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����
			break;
		case RIGHT:
			// �s����������`��
			g_model[RIGHT_CHARA].Draw(pDeviceContext, m_mtxWorld, eOpacityOnly);
			g_model[RIGHT_MACHINE].Draw(pDeviceContext, m_mtxWorld, eOpacityOnly);
			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

			if (m_Life.GetLife() <= 0)
			{
				g_model[RIGHT_CHARA].Draw(pDeviceContext, m_mtxWorld, DanDan);
				g_model[RIGHT_MACHINE].Draw(pDeviceContext, m_mtxWorld, DanDan);
			}

			//g_model.Draw(pDeviceContext, m_mtxWorld, eTransparentOnly);


			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����
			break;
		}
	}
	// ���C�t�`��
	m_Life.DrawLife();
	// �G���W���`��
	m_Enzine.Drawengine();

}
//
////=============================================================================
//// �J�����̐ݒ�
////=============================================================================
//void CPlayer::SetPlayerCamera(CCamera* pCamera)
//{
//	g_pCamera = pCamera;
//}
//
////=============================================================================
//// ���C�g�̐ݒ�
////=============================================================================
//void CPlayer::SetPlayerLight(CLight* pLight)
//{
//	g_pLight = pLight;
//}

//=============================================================================
// �G�I�u�W�F�N�g�擾
//=============================================================================
void CPlayer::SetPlayerEnemy(CEnemyVacuum* pEnemy)
{
	g_pEnemy = pEnemy;
}
//=============================================================================
// �R���g���[���[�I�u�W�F�N�g�擾
//=============================================================================
void SetControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}

//�@���ݕ����x�N�g�����擾
XMVECTOR  GetModelNor(void)
{
	return g_NorVec;
}

CAssimpModel* GetPlayerModel(void)
{
	return g_model;
}