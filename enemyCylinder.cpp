//=============================================================================
//
// ���f������ [enemyCylinder.cpp]
// Author :KAZUKI KURA
//
//=============================================================================
#include "enemyCylinder.h"
//#include "main.h"
//#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "Collision.h"
#include "Camera.h"
#include "Light.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_CYLINDER		"data/model/Enemy/sphere.fbx"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//static CCamera*			g_pCamera;
//static CLight*			g_pLight;
CAssimpModel		g_enemyCylinder;
//CPlayer*         g_pPlayer;
static bool g_bModelLoad = false;
//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemyCylinder::InitEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	
	//// �ʒu�E��]�E�X�P�[���̏����ݒ�(�G)
	//SetPos(XMFLOAT3(200.0f, MODEL_POSY/2, 100.0f));
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
		if (!g_enemyCylinder/*GetModel()*/.Load(pDevice, pDeviceContext, MODEL_CYLINDER)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_enemyCylinder);
		g_bModelLoad = true;
	}

	hr = InitmeshKirime(2, 2, 30.0f, 30.0f, 1.0f, 1.0f);
	if (FAILED(hr))
		return hr;

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemyCylinder::UninitEnemy(void)
{
	UninitmeshKirime();

	// ���f���̉��
#if MODEL_RELEASE_ON
		g_enemyCylinder[0].Release();
#endif

}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyCylinder::UpdateEnemy(void)
{
	
	//XMFLOAT3 rotCamera = GetCamera()->GetAngle();

	SetPos(XMFLOAT3(GetPos().x, GetPos().y, GetPlayer()->GetPosModel().z/*GetPos().z*/));
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



	UpdatemeshKirime();

}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyCylinder::DrawEnemy(void)
{
	// �J�����A�����ݒ�
	g_enemyCylinder.SetCamera(CCamera::Get());
	g_enemyCylinder.SetLight(CLight::Get());

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �s����������`��
	//g_enemyCylinder[0].Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eOpacityOnly);


	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

	//g_enemyCylinder[0].Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eTransparentOnly);

	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����

	
	DrawmeshKirime();
}

