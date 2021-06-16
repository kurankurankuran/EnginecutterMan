//=============================================================================
//
// ���f������ [model.cpp]
// Author :KAZUKI KURA
//
//=============================================================================
#include "enemySpher.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "Collision.h"
#include "model.h"
#include "scene.h"
#include "Light.h"
#include "Camera.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_SPHERE		"data/model/Enemy/sphere.fbx"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//static CCamera*			g_pCamera;
//static CLight*			g_pLight;
CAssimpModel		g_enemySphere;
static int              g_nScene;

static bool g_bModelLoad = false;

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemySpher::InitEnemy(void)
{
	SetAlphaType(eNoAffect);
	g_nScene = GetScene();

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	
	//// �ʒu�E��]�E�X�P�[���̏����ݒ�(�G)
	//SetPos(XMFLOAT3(200.0f,220.0f, 75.0f));
	SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetScl(XMFLOAT3(ENEMY_RAD, ENEMY_RAD, ENEMY_RAD));

	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	mtxScl = XMMatrixScaling(GetScl().x, GetScl().y, GetScl().z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(GetRot().x),
		XMConvertToRadians(GetRot().y), XMConvertToRadians(GetRot().z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	mtxTranslate = XMMatrixTranslation(GetPos().x, GetPos().y, GetPos().z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(GetmtxWorld(), mtxWorld);
#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif
	if (!g_bModelLoad) {
		// ���f���f�[�^�̓ǂݍ���
		if (!g_enemySphere.Load(pDevice, pDeviceContext, MODEL_SPHERE)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			
			
			return hr;
		}

		SaveModel(&g_enemySphere);

		g_bModelLoad = true;
	}
	/*if (!GetModel().Load(pDevice, pDeviceContext, MODEL_SPHERE)) {
		hr = E_FAIL;
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
		return hr;
	}*/
	if (!(g_nScene == SCENE_RUSH)) {
		hr = InitmeshKirime(10, 10, 10.0f, 10.0f, 1.0f, 1.0f);
		if (FAILED(hr))
			return hr;
	}

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemySpher::UninitEnemy(void)
{
	if (!(g_nScene == SCENE_RUSH)) {
		UninitmeshKirime();
	}
	
	// ���f���̉��
#if MODEL_RELEASE_ON
		g_enemySphere.Release();
#endif
	
	//GetModel().Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemySpher::UpdateEnemy(void)
{
	
	//==============================================================================================
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	mtxScl = XMMatrixScaling(GetScl().x, GetScl().y, GetScl().z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(GetRot().x),
		XMConvertToRadians(GetRot().y), XMConvertToRadians(GetRot().z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	mtxTranslate = XMMatrixTranslation(GetPos().x, GetPos().y, GetPos().z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(GetmtxWorld(), mtxWorld);



	if (!(g_nScene == SCENE_RUSH)) {
		UpdatemeshKirime();
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemySpher::DrawEnemy(void)
{
	// �J�����A�����ݒ�

	g_enemySphere.SetCamera(CCamera::Get());
	g_enemySphere.SetLight(CLight::Get());

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	if (GetAlphaType() == Clear) {
		// �s����������`��
		g_enemySphere.Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

		g_enemySphere.Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eTransparentOnly);

		g_enemySphere.Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), Clear);
	}
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	
	if (!(g_nScene == SCENE_RUSH)) {
		DrawmeshKirime();
	}
}

