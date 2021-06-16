//=============================================================================
//
// ���f������ [EnemyVacuum.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include "enemyRushWash.h"
#include "AssimpModel.h"
#include "deatheffect.h"
#include <stdio.h>
#include <time.h>
#include "scene.h"
#include "fade.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPHER_POSX           (0.0f)
#define SPHER_POSY           (0.0f)
#define SPHER_POSZ           (800.0f)

#define EFFECT_CNT           (10)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAssimpModel		g_enemyCore[MAX_COREPARTS_NUM];
static CCamera*			g_pCamera;
static CLight*			g_pLight;

static bool g_bModelLoad = false;

static LPCSTR c_aFileNameCore[MAX_COREPARTS_NUM] =
{
	"data/model/EnemyCore/Armor.fbx",
	"data/model/EnemyCore/Armor1.fbx",
	"data/model/EnemyCore/Armor2.fbx",
	"data/model/EnemyCore/Armor3.fbx",
	"data/model/EnemyCore/Armor4.fbx",
	"data/model/EnemyCore/Armor5.fbx",
	"data/model/EnemyCore/Armor6.fbx",
	"data/model/EnemyCore/Armor7.fbx",
	"data/model/EnemyCore/LeftHand_Stand.fbx",
	"data/model/EnemyCore/RightHand_Stand.fbx",
	"data/model/EnemyCore/Body.fbx",//
	//"data/model/EnemyCore/BodyCenter.fbx",//3
	//"data/model/EnemyCore/BodyGlass.fbx",//3
	
	/*"data/model/EnemyCore/WeakPoint.fbx",*/
};
//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemyRushWash::InitEnemy(void)
{
	m_nDeathEffectTime = 10 * 60;
	m_bDeathflg = false;
	m_nEffectCnt = EFFECT_CNT;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�(�G)
	for (int i = 0; i < MAX_COREPARTS_NUM; ++i) {
		m_Pos[i] = XMFLOAT3(0.0f, 0.0f, 600.0f);
		if (i == 8) {
			m_Pos[i] = XMFLOAT3(450.0f, 0.0f, 750.0f);
		}else if (i == 9) {
			m_Pos[i] = XMFLOAT3(-450.0f, 0.0f, 750.0f);
		}
		m_Rot[i] = XMFLOAT3(90.0f, 0.0f, 0.0f);
		if (i == 8) {
			m_Rot[i] = XMFLOAT3(90.0f, 0.0f,-75.0f);
		}
		else if (i == 9) {
			m_Rot[i] = XMFLOAT3(90.0f, 0.0f, 75.0f);
		}
		m_Scl[i] = XMFLOAT3(20.0f, 20.0f, 20.0f);
	}

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	hr = S_OK;
	hr = m_SpherFront.InitEnemy();
	if (FAILED(hr))
		return hr;
	m_SpherFront.SetPos(XMFLOAT3(0.0f, 0.0f, 800.0f));
	m_SpherFront.SetScl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_SpherFront.SetAlphaType(Clear);


	// ���f���f�[�^�̓ǂݍ���

#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif
	if (!g_bModelLoad) {
		for (int i = 0; i < MAX_COREPARTS_NUM; ++i) {
			if (!g_enemyCore[i].Load(pDevice, pDeviceContext, c_aFileNameCore[i]/*MODEL_VACUUM*/)) {
				hr = E_FAIL;
				MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitWash", MB_OK);
				return hr;
			}
			SaveModel(&g_enemyCore[i]);
		}
		g_bModelLoad = true;
	}
	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		
		/*m_pLife[i] = new CEnemyLife(XMFLOAT3(0.0f, 0.0f, 1400.0f - i * 25.0f), 100, false);*/
		m_Life[i].Init(XMFLOAT3(0.0f, 0.0f, 1400.0f - i * 25.0f), 100, false);
	}
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemyRushWash::UninitEnemy(void)
{
	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		/*delete m_pLife[i];*/
		m_Life[i].Uninit();
	}
	m_SpherFront.UninitEnemy();
	
	//// ���f���̉��
	//for (int i = 0; i < MAX_COREPARTS_NUM; ++i) {
	//	g_enemyCore[i].Release();
	//}
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyRushWash::UpdateEnemy(void)
{
	

	m_SpherFront.UpdateEnemy();
	
	if (!m_bDeathflg) {
		CheckClearflg();
	}
	else
	{
		m_SpherFront.SetScl(XMFLOAT3(m_SpherFront.GetScl().x + 2.0f, m_SpherFront.GetScl().y + 2.0f, m_SpherFront.GetScl().z + 2.0f));

		XMFLOAT3 vCameraPos = XMFLOAT3(-3000.0f, 3000.0f, 1000.0f);
		XMFLOAT3 vCameraTarget = XMFLOAT3( 0.0f,  0.0f, 800.0f);
		XMFLOAT3 vCameraUp = XMFLOAT3(0.0f, 0.0f, 1.0f);
		CCamera::Get()->SetPos(vCameraPos);
		CCamera::Get()->SetTarget(vCameraTarget);
		CCamera::Get()->SetUpVector(vCameraUp);

		--m_nDeathEffectTime;
		if (m_nDeathEffectTime < 0) {
			SetFadeOut(SCENE_GAMECLEAR);
		}

		m_nEffectCnt = 2;

		srand((unsigned int)time(NULL));
		int nVec = (rand() % 300);

		int nXYZ = nVec % 3;

		if (nVec % 2 == 0) {
			nVec = -1 * nVec;
		}

		XMFLOAT3 f3Vec;
		switch (nXYZ)
		{
		case 0:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 3), (float)nVec * (rand() % 5), (float)nVec * (rand() % 5));
			break;
		case 1:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 4), (float)nVec * (rand() % 4), (float)nVec * (rand() % 3));
			break;
		case 2:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 5), (float)nVec * (rand() % 3), (float)nVec * (rand() % 2));
			break;
		}
		SetDeathEffect(XMFLOAT3(f3Vec.x * 1.2f + 0.0f, f3Vec.y * 0.8f + 0.0f, f3Vec.z * 1.2f + 800.0f), 1000.0f, 1000.0f);

		SetDeathEffect(XMFLOAT3(f3Vec.x * 1.2f + 0.0f, f3Vec.y * 1.5f + 0.0f, f3Vec.z * 0.5f + 800.0f), 1000.0f, 1000.0f);

		SetDeathEffect(XMFLOAT3(f3Vec.x * 1.5f + 0.0f, f3Vec.y * 1.3f + 0.0f, f3Vec.z * 1.4f + 800.0f), 1000.0f, 1000.0f);

		SetDeathEffect(XMFLOAT3(f3Vec.x + 0.0f, f3Vec.y + 0.0f, f3Vec.z + 800.0f), 1000.0f, 1000.0f);

	}
	

	
	--m_nEffectCnt;
	if (m_nEffectCnt < 0) {
		
		m_nEffectCnt = EFFECT_CNT;
				
		srand((unsigned int)time(NULL));
		int nVec = (rand() % 300);
		
		int nXYZ = nVec % 3;

		if (nVec % 2 == 0) {
			nVec = -1 * nVec;
		}

		XMFLOAT3 f3Vec;
		switch (nXYZ)
		{
		case 0:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 5), (float)nVec * (rand() % 3), (float)nVec * (rand() % 2));
			break;
		case 1:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 3), (float)nVec * (rand() % 2), (float)nVec * (rand() % 5));
			break;
		case 2:
			f3Vec = XMFLOAT3((float)nVec * (rand() % 2), (float)nVec * (rand() % 5), (float)nVec * (rand() % 3));
			break;
		}
		SetDeathEffect(XMFLOAT3(f3Vec.x * 1.2f + 0.0f, f3Vec.y * 0.8f + 0.0f, f3Vec.z * 1.2f + 800.0f), 200.0f, 200.0f);

		SetDeathEffect(XMFLOAT3(f3Vec.x * 1.2f + 0.0f, f3Vec.y * 1.5f + 0.0f, f3Vec.z * 0.5f + 800.0f), 200.0f, 200.0f);

		SetDeathEffect(XMFLOAT3(f3Vec.x + 0.0f, f3Vec.y + 0.0f, f3Vec.z + 800.0f), 200.0f, 200.0f);
	}

	// �J�����̌����擾
	//XMFLOAT3 rotCamera = g_pCamera->GetAngle();

	//==============================================================================================
	for (int i = 0; i < MAX_COREPARTS_NUM; ++i) {
		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_Scl[i].x, m_Scl[i].y, m_Scl[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rot[i].x),
			XMConvertToRadians(m_Rot[i].y), XMConvertToRadians(m_Rot[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		mtxTranslate = XMMatrixTranslation(m_Pos[i].x, m_Pos[i].y, m_Pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);
	
	// �J�����A�����ݒ�
		g_enemyCore[i].SetCamera(CCamera::Get()/*g_pCamera*/);
		g_enemyCore[i].SetLight(CLight::Get()/*g_pLight*/);
		
	}

	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		/*m_pLife[i]->Update();*/
		m_Life[i].Update();
		//m_pLife[i]->SetCamera(g_pCamera);
	}

	
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyRushWash::DrawEnemy(void)
{
	//***********************************************************************
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_COREPARTS_NUM; ++i) {
		// �s����������`��
		g_enemyCore[i].Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

		g_enemyCore[i].Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);

		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
	//******************************************************************************************
	
	m_SpherFront.DrawEnemy();

	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		//m_Life[i].SetCamera(g_pCamera);
		m_Life[i].Draw();
		//m_pLife[i]->Draw();
	}
}


////=============================================================================
//// �J�����̐ݒ�
////=============================================================================
//void CEnemyRushWash::SetCamera(CCamera* Camera)
//{
//	m_SpherFront.SetCamera(Camera);
//	
//	g_pCamera = Camera;
//}

////=============================================================================
//// ���C�g�̐ݒ�
////=============================================================================
//void CEnemyRushWash::SetLight(CLight* Light)
//{
//	m_SpherFront.SetEnemyLight(Light);
//	
//	g_pLight = Light;
//
//}


//=============================================================================
// �R�A�G�l�~�[���C�t�C���X�^���X�擾
//=============================================================================
CEnemyLife* CEnemyRushWash::GetInstansEnemyLife()
{
	int nNum = 0;
	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		/*if (m_pLife[i]->GetStat() == 0) continue;*/
		if (m_Life[i].GetStat() == 0) continue;

		nNum = i;
		break;
	}

	return /*m_pLife[nNum];*/ &m_Life[nNum];
}

//=============================================================================
// ���C�t���Q�Ƃ��A�S�ĂO�Ȃ甚���t���OON
//=============================================================================
void CEnemyRushWash::CheckClearflg(void)
{
	bool bClearflg = true;
	for (int i = 0; i < MAX_CORELIFE_BER; ++i) {
		//if (m_pLife[i]->GetStat() == 0) continue;
		if (m_Life[i].GetStat() == 0) continue;
		bClearflg = false;

		;
		break;
	}


	if (bClearflg) {
		m_bDeathflg = true;
		//SetFadeOut(SCENE_GAMECLEAR);
	}
}