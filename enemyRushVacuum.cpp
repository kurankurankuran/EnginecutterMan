//=============================================================================
//
// ���f������ [EnemyVacuum.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "enemyRushVacuum.h"
#include "AssimpModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_RUSHVACUUM			"data/model/tubeTest.fbx"



#define SPHER_POSX           (0.0f)
#define SPHER_POSY           (0.0f)
#define SPHER_POSZ           (760.0f)

#define MAX_RUSHVACUUMPARTS_NUM     (3)


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAssimpModel		g_enemyRushVacuum[MAX_RUSHVACUUMPARTS_NUM];

static CCamera*			g_pCamera;
static CLight*			g_pLight;


static LPCSTR c_aFileName[MAX_RUSHVACUUMPARTS_NUM] =
{
	"data/model/Enemy/tube_AttackAnimation.fbx",		// �`���[�u
	"data/model/Enemy/base_AttackAnimation.fbx",	        // �x�[�X
	"data/model/Enemy/bodyhead_AttackAnimation.fbx",	// �`���[�u
};


static bool g_bModelLoad = false;

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemyRushVacuum::InitEnemy(void)
{
	// �ʒu�E��]�E�X�P�[���̏����ݒ�(�G)
	m_Pos = XMFLOAT3(-85.0f, -132.0f, 0.0f);
	m_Rot = XMFLOAT3(90.0f, 0.0f, 0.0f);
	m_Scl = XMFLOAT3(55.0f, 55.0f, 55.0f);
	//m_Scl = XMFLOAT3(20.0f, 20.0f, 20.0f);


	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	hr = S_OK;
	//hr = m_SpherFront.InitEnemy();
	hr = m_Wash.InitEnemy();

	if (FAILED(hr))
		return hr;
	//m_SpherFront.SetPos(XMFLOAT3(0.0f, 0.0f, 800.0f));


#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif

	if (!g_bModelLoad) {
		// ���f���f�[�^�̓ǂݍ���
		for (int i = 0; i < MAX_RUSHVACUUMPARTS_NUM; ++i) {
			if (!g_enemyRushVacuum[i].Load(pDevice, pDeviceContext, c_aFileName[i]/*MODEL_VACUUM*/)) {
				hr = E_FAIL;
				MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitVacuum", MB_OK);
				return hr;
			}

			SaveModel(&g_enemyRushVacuum[i]);
		}

		g_bModelLoad = true;
	}
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemyRushVacuum::UninitEnemy(void)
{
	//m_SpherFront.UninitEnemy();
	m_Wash.UninitEnemy();

	// ���f���̉��
	for (int i = 0; i < MAX_RUSHVACUUMPARTS_NUM; ++i) {
		g_enemyRushVacuum[i].Release();
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyRushVacuum::UpdateEnemy(void)
{
	//m_SpherFront.UpdateEnemy();
	m_Wash.UpdateEnemy();

	// �J�����̌����擾
	//XMFLOAT3 rotCamera = g_pCamera->GetAngle();

	//==============================================================================================
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	mtxScl = XMMatrixScaling(m_Scl.x, m_Scl.y, m_Scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rot.x),
		XMConvertToRadians(m_Rot.y), XMConvertToRadians(m_Rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	mtxTranslate = XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&m_mtxWorld, mtxWorld);

	//// �J�����A�����ݒ�
	//for (int i = 0; i < MAX_RUSHVACUUMPARTS_NUM; ++i) {
	//	g_enemyRushVacuum[i].SetCamera(g_pCamera);
	//	g_enemyRushVacuum[i].SetLight(g_pLight);
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyRushVacuum::DrawEnemy(void)
{
	//***********************************************************************
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_RUSHVACUUMPARTS_NUM; ++i) {
		// �J�����A�����ݒ�
		
		g_enemyRushVacuum[i].SetCamera(CCamera::Get()/*g_pCamera*/);
		g_enemyRushVacuum[i].SetLight(CLight::Get()/*g_pLight*/);
		


		// �s����������`��
		g_enemyRushVacuum[i].Draw(pDeviceContext, m_mtxWorld, eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

		g_enemyRushVacuum[i].Draw(pDeviceContext, m_mtxWorld, eTransparentOnly);

		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����

		g_enemyRushVacuum[i].Draw(pDeviceContext, m_mtxWorld, Clear);
	}

	//******************************************************************************************

	//m_SpherFront.DrawEnemy();
	m_Wash.DrawEnemy();

}


////=============================================================================
//// �J�����̐ݒ�
////=============================================================================
//void CEnemyRushVacuum::SetCamera(CCamera* Camera)
//{
//	//m_SpherFront.SetCamera(Camera);
//	//m_Wash.SetCamera(Camera);
//
//	g_pCamera = Camera;
//}
//
////=============================================================================
//// ���C�g�̐ݒ�
////=============================================================================
//void CEnemyRushVacuum::SetVacuumEnemyLight(CLight* Light)
//{
//	//m_SpherFront.SetEnemyLight(Light);
//	//m_Wash.SetLight(Light);
//
//	g_pLight = Light;
//
//}


