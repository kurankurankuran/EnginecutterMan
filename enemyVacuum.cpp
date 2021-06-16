//=============================================================================
//
// ���f������ [EnemyVacuum.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "enemyVacuum.h"
#include "AssimpModel.h"

#include "Light.h"
#include "Camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define MODEL_VACUUM		"data/model/EnemyArmor4.fbx"
#define MODEL_VACUUM			"data/model/tubeTest.fbx"
//#define MODEL_VACUUM			"data/model/enemy_stand2.fbx"
//#define MODEL_VACUUM			"data/model/standAnimation.fbx"



#define SPHER_POSX           (0.0f)
#define SPHER_POSY           (0.0f)
#define SPHER_POSZ           (760.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAssimpModel		g_enemyVacuum[MAX_VACUUMPARTS_NUM];
static CCamera*			g_pCamera;
static CLight*			g_pLight;

static bool  g_bModelLoad = false;

static LPCSTR c_aFileName[MAX_VACUUMPARTS_NUM] =
{
	//"data/model/tube_tubeAnimation3.fbx",	// �`���[�u
	//"data/model/baseAnimation.fbx",	        // �x�[�X
	//////"data/model/headAnimation.fbx",		// �w�b�h
	//////"data/model/bodyAnimation.fbx",		// �`���[�u�Ǝ��Ƃ���
	//"data/model/bodyheadAnimation2.fbx",	// �`���[�u

	"data/model/Enemy/tube_AttackAnimation.fbx",		// �`���[�u
	"data/model/Enemy/base_AttackAnimation.fbx",	        // �x�[�X
	"data/model/Enemy/bodyhead_AttackAnimation.fbx",	// �`���[�u
	/*"data/model/Enemy/EnemyBodyAnimation.fbx",*/	// �`���[�u
	"data/model/Enemy/EnemyArmor4.fbx",
};


//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemyVacuum::InitEnemy(void)
{
	// �ʒu�E��]�E�X�P�[���̏����ݒ�(�G)
	m_Pos[0] = XMFLOAT3(-85.0f, -132.0f, 0.0f);
	m_Pos[1] = XMFLOAT3(-85.0f, -132.0f, 0.0f);
	m_Pos[2] = XMFLOAT3(-85.0f, -132.0f, 0.0f);
	m_Pos[3] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_Rot = XMFLOAT3(90.0f, 0.0f, 0.0f);
	m_Scl[0] = XMFLOAT3(55.0f, 55.0f, 55.0f);
	m_Scl[1] = XMFLOAT3(55.0f, 55.0f, 55.0f);
	m_Scl[2] = XMFLOAT3(55.0f, 55.0f, 55.0f);
	m_Scl[3] = XMFLOAT3(160.0f, 160.0f, 160.0f);

	//m_Scl[i] = XMFLOAT3(20.0f, 20.0f, 20.0f);


	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	hr = S_OK;
	hr = m_SpherFront.InitEnemy();
	if (FAILED(hr))
		return hr;
	m_SpherFront.SetPos(XMFLOAT3(0.0f, 0.0f, 800.0f));
	SetChangeSpace(XMFLOAT3(0.0f, 0.0f, 800.0f+1000.0f/2),
		XMFLOAT3(10000.0f, 10000.0f, 1000.0f),
		CIRCLEANDCYLINDER, FRONTCUBE_NAME
	);

	hr = m_Cylinder.InitEnemy();
	if (FAILED(hr))
		return hr;
	m_Cylinder.SetPos(XMFLOAT3(0.0f, 0.0f, MODEL_POSZ));

    hr = m_Parts.InitEnemy();
	if (FAILED(hr))
		return hr;

	m_EnemyAttack.Init();
	if (FAILED(hr))
		return hr;
#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif
	if (!g_bModelLoad) {
		// ���f���f�[�^�̓ǂݍ���
		for (int i = 0; i < MAX_VACUUMPARTS_NUM; ++i) {
			if (!g_enemyVacuum[i].Load(pDevice, pDeviceContext, c_aFileName[i]/*MODEL_VACUUM*/)) {
				hr = E_FAIL;
				MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitVacuum", MB_OK);



				return hr;
			}
			SaveModel(&g_enemyVacuum[i]);
		}
		g_bModelLoad = true;
	}
	//==============================================================================================
	for (int i = 0; i < MAX_CHANGESPACE; ++i) {
		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_Scl[i].x, m_Scl[i].y, m_Scl[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rot.x),
			XMConvertToRadians(m_Rot.y), XMConvertToRadians(m_Rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		mtxTranslate = XMMatrixTranslation(m_Pos[i].x, m_Pos[i].y, m_Pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);
	}
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemyVacuum::UninitEnemy(void)
{
	m_SpherFront.UninitEnemy();
	
	m_Cylinder.UninitEnemy();
	m_Parts.UninitEnemy();
	m_EnemyAttack.Uninit();
	
	// ���f���̉��
#if MODEL_RELEASE_ON
		for (int i = 0; i < MAX_VACUUMPARTS_NUM; ++i) {
			g_enemyVacuum[i].Release();
		}
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyVacuum::UpdateEnemy(void)
{
	m_SpherFront.UpdateEnemy();
	
	m_Cylinder.UpdateEnemy();
	m_Parts.UpdateEnemy();

	if (CCamera::Get()->GetWorldMoveflg()) {
		m_EnemyAttack.Update();
	}
	//// �J�����̌����擾
	//XMFLOAT3 rotCamera = g_pCamera->GetAngle();
	

	//==============================================================================================
	for (int i = 0; i < MAX_CHANGESPACE; ++i) {
		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_Scl[i].x, m_Scl[i].y, m_Scl[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rot.x),
			XMConvertToRadians(m_Rot.y), XMConvertToRadians(m_Rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		mtxTranslate = XMMatrixTranslation(m_Pos[i].x, m_Pos[i].y, m_Pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);
	}
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyVacuum::DrawEnemy(void)
{
	// �J�����A�����ݒ�
	for (int i = 0; i < MAX_VACUUMPARTS_NUM; ++i) {
		g_enemyVacuum[i].SetCamera(CCamera::Get());
		g_enemyVacuum[i].SetLight(CLight::Get());
	}

	//***********************************************************************
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_VACUUMPARTS_NUM; ++i) {
		// �s����������`��
		g_enemyVacuum[i].Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

		g_enemyVacuum[i].Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);

		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
	//******************************************************************************************

	m_SpherFront.DrawEnemy();
	m_Cylinder.DrawEnemy();
	m_Parts.DrawEnemy();
	
	m_EnemyAttack.Draw();


}


////=============================================================================
//// �J�����̐ݒ�
////=============================================================================
//void CEnemyVacuum::SetCamera(CCamera* Camera)
//{
//	//m_SpherFront.SetCamera(Camera);
//	m_SpherFront.GetKirime()->SetmeshKirimeC(Camera);
//	//m_Cylinder.SetCamera(Camera);
//	m_Parts.SetCamera(Camera);
//	m_EnemyAttack.SetCamera(Camera);
//
//	g_pCamera = Camera;
//}
//
////=============================================================================
//// ���C�g�̐ݒ�
////=============================================================================
//void CEnemyVacuum::SetVacuumEnemyLight(CLight* Light)
//{
//	//m_SpherFront.SetEnemyLight(Light);
//	m_SpherFront.GetKirime()->SetmeshKirimeL(Light);
//	//m_Cylinder.SetEnemyLight(Light);
//	m_Parts.SetLight(Light);
//	m_EnemyAttack.SetLight(Light);
//
//	g_pLight = Light;
//
//}


void  CEnemyVacuum::SetChangeSpace(XMFLOAT3 f3Pos, XMFLOAT3 f3Scl, int nChangeType, int nChangeCircleName)
{
	for (int i = 0; i < MAX_CHANGESPACE; ++i) {
		if (m_changeSpace[i].GetUseflg())
			continue;
		m_changeSpace[i].SetChangeSpace(f3Pos, f3Scl, nChangeType, nChangeCircleName, true);
		break;
	}
}

int  CEnemyVacuum::ChangeField(XMFLOAT3 f3Point, int nNowOnFiled)
{
	int nChangeNum = nNowOnFiled;
	for (int i = 0; i < MAX_CHANGESPACE; ++i) {
		if (!m_changeSpace[i].GetUseflg())
			continue;
		
		switch (m_changeSpace[i].GetChangeType())
		{
		case RECTANDCIRCLE:
			nChangeNum = m_changeSpace[i].ChangeFieldRectAndCircle(f3Point, nNowOnFiled);
			break;
		case CIRCLEANDCYLINDER:
			nChangeNum = m_changeSpace[i].ChangeFieldCircle(f3Point);
			break;
		}
		
		if (nChangeNum != nNowOnFiled) 
			break;
		
	}

	return nChangeNum;
}

