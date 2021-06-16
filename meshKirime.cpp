//=============================================================================
//
// ���b�V���n�ʏ��� [meshKirime.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "meshKirime.h"
//#include "Shader.h"
//#include "Texture.h"
//#include "input.h"
//#include "mesh.h"

#include "Collision.h"
//#include "model.h"
#include "explosion.h"

#include "enemySpher.h"
#include "AssimpModel.h"
#include "deatheffect.h"
#include "bilibili.h"

//
#include "Light.h"
#include "Camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	TEXTURE_FILENAME_FLOOR	L"data/texture/sky002.jpg"	// �e�N�X�`���t�@�C����
//#define	TEXTURE_FILENAME_WALL	L"data/texture/field001.jpg"	// �e�N�X�`���t�@�C����
//#define	TEXTURE_FILENAME_WALL1	L"data/texture/field002.jpg"	// �e�N�X�`���t�@�C����
//#define	TEXTURE_FILENAME_WALL2	L"data/texture/wall000.jpg"	// �e�N�X�`���t�@�C����
//#define	TEXTURE_FILENAME_WALL3	L"data/texture/wall001.jpg"	// �e�N�X�`���t�@�C����

#define MODEL_SPHERE		"data/model/Enemy/sphere.fbx"

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MODELONG     (1.0f)  �@ //�����蔻��p�i���j
#define MODELWIDTH     (1.0f)�@//�����蔻��p�i���j
#define MODELHIGH     (1.0f) �@//�����蔻��p�i���j
#define MODEL_STRENGTH		10.0f�@//����

#define	VALUE_MOVE			(5.0f)							// �ړ���
#define	VALUE_ROTATE		(0.2f)							// ��]��

#define MAX_KIRIME   (60)

#define SPHERE_RAD          (30.0f)                         // ���a
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************





//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//MESH						 m_posModel[MAX_KIRIME];					// �\����
//int                          m_Cnt[MAX_KIRIME];
//MATERIAL					 m_material;				// �}�e���A��
//static CCamera*			g_pCamera;
//static CLight*			g_pLight;

bool                         g_bDest[MAX_ENEMY_PARTS];  //�p�[�c�j��t���O
static CAssimpModel		 g_Kirime;
static XINPUT_STATE* g_pMXController;
static XINPUT_VIBRATION* g_pMVibration;

static bool g_bModelLoad = false;
//CPlayer*              g_pPlayer;
//=============================================================================
// ����������
//=============================================================================
//HRESULT CKirime::InitmeshKirime(int nNumBlockX, int nNumBlockZ,
//	float fSizeBlockX, float fSizeBlockZ, float fTexSizeX, float fTexSizeZ)
HRESULT CKirime::InitmeshKirime(void)
{
	m_Life1 = 0;
	m_Life2 = 0;
	m_Life3 = 0;
	m_Life4 = 0;

	//SetmeshKirimePlayer();

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	

	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{
		g_bDest[i] = false;
	}
	
	/*ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;*/

	// ���f���f�[�^�̓ǂݍ���

	for (int i = 0; i < MAX_KIRIME; ++i)
	{
		m_Cnt[i] = 0;
		m_SclModel[i] = XMFLOAT3(7.0f, 7.0f, 7.0f);
		
	}

#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif
	if(!g_bModelLoad){
		if (!g_Kirime.Load(pDevice, pDeviceContext, MODEL_SPHERE))
		{
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_Kirime);

		g_bModelLoad = true;
	}


	//�[�[�[�����蔻��񐔂̏�����



	// �ʒu�E��]�̏����ݒ�
	//**************************************************************
	// ��O
	m_posModel[0] = XMFLOAT3(0.0f, 585.0f, 920.0f);
	m_rotModel[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[1] = XMFLOAT3(0.0f, 566.0f, 994.0f);
	m_rotModel[1] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[2] = XMFLOAT3(0.0f, 547.0f, 1073.0f);
	m_rotModel[2] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[3] = XMFLOAT3(0.0f, 528.0f, 1190.0f);
	m_rotModel[3] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[4] = XMFLOAT3(0.0f, 420.0f, 1200.0f);
	m_rotModel[4] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[5] = XMFLOAT3(0.0f, 380.0f, 1260.0f);
	m_rotModel[5] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[6] = XMFLOAT3(0.0f, 340.0f, 1300.0f);
	m_rotModel[6] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[7] = XMFLOAT3(0.0f, 300.0f, 1360.0f);
	m_rotModel[7] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[8] = XMFLOAT3(0.0f, 240.0f, 1390.0f);
	m_rotModel[8] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[9] = XMFLOAT3(0.0f, 200.0f, 1390.0f);
	m_rotModel[9] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[10] = XMFLOAT3(0.0f, 160.0f, 1390.0f);
	m_rotModel[10] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[11] = XMFLOAT3(0.0f, 120.0f, 1400.0f);
	m_rotModel[11] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[12] = XMFLOAT3(0.0f, 80.0f, 1400.0f);
	m_rotModel[12] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[13] = XMFLOAT3(0.0f, 80.0f, 1400.0f);
	m_rotModel[13] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//*******************************************************************
	//// ��
	m_posModel[14] = XMFLOAT3(0.0f, -530.0f, 1130.0f);
	m_rotModel[14] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[15] = XMFLOAT3(0.0f, -500.0f, 1150.0f);
	m_rotModel[15] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[16] = XMFLOAT3(0.0f, -480.0f, 1170.0f);
	m_rotModel[16] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[17] = XMFLOAT3(0.0f, -460.0f, 1190.0f);
	m_rotModel[17] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[18] = XMFLOAT3(0.0f, -440.0f, 1220.0f);
	m_rotModel[18] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[18] = XMFLOAT3(0.0f, -400.0f, 1250.0f);
	m_rotModel[18] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[19] = XMFLOAT3(0.0f, -360.0f, 1280.0f);
	m_rotModel[19] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[20] = XMFLOAT3(0.0f, -320.0f, 1280.0f);
	m_rotModel[20] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[21] = XMFLOAT3(0.0f, -280.0f, 1340.0f);
	m_rotModel[21] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[22] = XMFLOAT3(0.0f, -240.0f, 1370.0f);
	m_rotModel[22] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[23] = XMFLOAT3(0.0f, -200.0f, 1370.0f);
	m_rotModel[23] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[24] = XMFLOAT3(0.0f, -160.0f, 1380.0f);
	m_rotModel[24] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[25] = XMFLOAT3(0.0f, -120.0f, 1380.0f);
	m_rotModel[25] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[26] = XMFLOAT3(0.0f, -60.0f, 1380.0f);
	m_rotModel[26] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//*******************************************************************
	// �E
	m_posModel[27] = XMFLOAT3(520.0f, 20.0f, 1060.0f);
	m_rotModel[27] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[28] = XMFLOAT3(510.0f, 20.0f, 1090.0f);
	m_rotModel[28] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[29] = XMFLOAT3(480.0f, 20.0f, 1160.0f);
	m_rotModel[29] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[30] = XMFLOAT3(450.0f, 20.0f, 1190.0f);
	m_rotModel[30] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[31] = XMFLOAT3(410.0f, 20.0f, 1230.0f);
	m_rotModel[31] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[32] = XMFLOAT3(370.0f, 20.0f, 1250.0f);
	m_rotModel[32] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[33] = XMFLOAT3(340.0f, 20.0f, 1310.0f);
	m_rotModel[33] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[34] = XMFLOAT3(330.0f, 20.0f, 1310.0f);
	m_rotModel[34] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[35] = XMFLOAT3(310.0f, 20.0f, 1350.0f);
	m_rotModel[35] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[36] = XMFLOAT3(280.0f, 20.0f, 1370.0f);
	m_rotModel[36] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[37] = XMFLOAT3(230.0f, 20.0f, 1370.0f);
	m_rotModel[37] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[38] = XMFLOAT3(190.0f, 20.0f, 1370.0f);
	m_rotModel[38] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[39] = XMFLOAT3(160.0f, 20.0f, 1390.0f);
	m_rotModel[39] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[40] = XMFLOAT3(120.0f, 20.0f, 1400.0f);
	m_rotModel[40] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//*******************************************************************
	// ��
	m_posModel[41] = XMFLOAT3(-520.0f, 20.0f, 1130.0f);
	m_rotModel[41] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[42] = XMFLOAT3(-480.0f, 20.0f, 1090.0f);
	m_rotModel[42] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[43] = XMFLOAT3(-450.0f, 20.0f, 1150.0f);
	m_rotModel[43] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[44] = XMFLOAT3(-430.0f, 20.0f, 1190.0f);
	m_rotModel[44] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[45] = XMFLOAT3(-390.0f, 20.0f, 1230.0f);
	m_rotModel[45] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[46] = XMFLOAT3(-350.0f, 20.0f, 1270.0f);
	m_rotModel[46] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[47] = XMFLOAT3(-270.0f, 20.0f, 1310.0f);
	m_rotModel[47] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[48] = XMFLOAT3(-230.0f, 20.0f, 1340.0f);
	m_rotModel[48] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[49] = XMFLOAT3(-190.0f, 20.0f, 1340.0f);
	m_rotModel[49] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[50] = XMFLOAT3(-160.0f, 20.0f, 1350.0f);
	m_rotModel[50] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[51] = XMFLOAT3(-130.0f, 20.0f, 1360.0f);
	m_rotModel[51] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[52] = XMFLOAT3(-100.0f, 20.0f, 1370.0f);
	m_rotModel[52] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[53] = XMFLOAT3(-60.0f, 20.0f, 1380.0f);
	m_rotModel[53] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_posModel[54] = XMFLOAT3(-20.0f, 20.0f, 1400.0f);
	m_rotModel[54] = XMFLOAT3(0.0f, 0.0f, 0.0f);


	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CKirime::UninitmeshKirime(void)
{
	

#if MODEL_RELEASE_ON
	// ���f���̉��
		g_Kirime.Release();
#endif

}

//=============================================================================
// �X�V����
//=============================================================================
void CKirime::UpdatemeshKirime(void)
{
	//==============================================================================================
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;



	for (int i = 0; i < MAX_KIRIME; ++i)
	{

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_SclModel[i].x, m_SclModel[i].y, m_SclModel[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);


		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_posModel[i].x, m_posModel[i].y, m_posModel[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotModel[i].x),
			XMConvertToRadians(m_rotModel[i].y), XMConvertToRadians(m_rotModel[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);




		if (!g_bDest[0]) {

			if( 0 <= i && i <= 13 && m_Cnt[i] > 3) {
				if (i == 13)
				{
					for (int j = 0; j <= 13; ++j) {
						SetBilibili(m_posModel[j], 200.0f, 200.0f);
					}
					g_bDest[0] = true;
				}
			}
		}

		if (!g_bDest[1]) {

			if (14 <= i && i <= 26 && m_Cnt[i] > 3) {
				if (i == 26)
				{
					for (int j = 14; j <= 26; ++j) {
						SetBilibili(m_posModel[j], 200.0f, 200.0f);
					}
					g_bDest[1] = true;
				}
			}
		}

		if (!g_bDest[2]) {

			if (27 <= i && i <= 40 && m_Cnt[i] > 3) {
				if (i == 40)
				{
					for (int j = 27; j <= 40; ++j) {
						SetBilibili(m_posModel[j], 200.0f, 200.0f);
					}
					g_bDest[2] = true;
				}
			}
		}

		if (!g_bDest[3]) {

			if (41 <= i && i <= 54 && m_Cnt[i] > 3) {
				if (i == 54)
				{
					for (int j = 41; j <= 54; ++j) {
						SetBilibili(m_posModel[j], 200.0f, 200.0f);
					}
					g_bDest[3] = true;
				}
			}
		}
	}


	/*if (!g_bDest[0]) {

		for (int i = 0; i <= 13; ++i) {
			if (m_Cnt[i] <= 3)
				break;
			if (i == 13)
			{
				for (int j = 0; j <= 13; ++j) {
					SetBilibili(m_posModel[j], 200.0f, 200.0f);
				}
				g_bDest[0] = true;
			}
		}
	}*/
	


	/*if (!g_bDest[1]) {

		for (int i = 14; i <= 26; ++i) {
			if (m_Cnt[i] <= 3)
				break;
			if (i == 26) {
				for (int j = 14; j <= 26; ++j) {
					SetBilibili(m_posModel[j], 200.0f, 200.0f);
				}
				g_bDest[1] = true;
			}
		}
	}*/


	/*if (!g_bDest[2]) {
		for (int i = 27; i <= 40; ++i) {
			if (m_Cnt[i] <= 3)
				break;
			if (i == 40) {
				for (int j = 27; j <= 40; ++j) {
					SetBilibili(m_posModel[j], 200.0f, 200.0f);
				}
				g_bDest[2] = true;
			}
		}
	}*/

	
	/*if (!g_bDest[3]) {
		for (int i = 41; i <= 54; ++i) {
			if (m_Cnt[i] <= 3)
				break;
			if (i == 54) {
				for (int j = 41; j <= 54; ++j) {
					SetBilibili(m_posModel[j], 200.0f, 200.0f);
				}
				g_bDest[3] = true;
			}
		}
	}*/
	
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void CKirime::DrawmeshKirime(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	ID3D11DeviceContext* pDC = GetDeviceContext();

	if (!g_bDest[0])
	{


		// �J�����A�����ݒ�
		g_Kirime.SetCamera(CCamera::Get());
		g_Kirime.SetLight(CLight::Get());

		// ���b�V���̕`��
		for (int i = 0; i <= 13; ++i)
		{


			g_Kirime.Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);
			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

			g_Kirime.Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);
			
			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����
		}

	}
	if (!g_bDest[1]) {

		// �J�����A�����ݒ�
		g_Kirime.SetCamera(CCamera::Get());
		g_Kirime.SetLight(CLight::Get());


		// ���b�V���̕`��
		for (int i = 14; i <= 26; ++i)
		{
			g_Kirime.Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);
			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

			g_Kirime.Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);
			
			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����
		}

	}
	if (!g_bDest[2]) {

		// �J�����A�����ݒ�
		g_Kirime.SetCamera(CCamera::Get());
		g_Kirime.SetLight(CLight::Get());


		// ���b�V���̕`��
		for (int i = 27; i <= 40; ++i)
		{
			g_Kirime.Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);
			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

			g_Kirime.Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);
		
			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����
		}

	}
	if (!g_bDest[3]) {

		// �J�����A�����ݒ�
		g_Kirime.SetCamera(CCamera::Get());
		g_Kirime.SetLight(CLight::Get());


		// ���b�V���̕`��
		for (int i = 41; i <= 54; ++i)
		{
			g_Kirime.Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);
			// ������������`��
			SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
			SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

			g_Kirime.Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);
		
			SetZWrite(true);				// Z�o�b�t�@�X�V����
			SetBlendState(BS_NONE);			// �A���t�@�u�����h����
		}

	}

	
}

//====================================================================
// �Ăяo�������蔻��֐����悤�I�I�I
//====================================================================
int CKirime::CollisionKirime(DirectX::XMFLOAT3 vPos,XMFLOAT3 vPosPlayer, float Long,
	float Width, float High,
	int nDamage, DirectX::XMFLOAT3* pvHitPos) 
{
	ID3D11Device* pDevice = GetDevice();
	//HRESULT hr;


	//---kirimi���ŉ�---

	if (!g_bDest[0]) {
		for (int i = 0; i <= 13; i++)
		{
			//---�����蔻��ϋv�x---
			if (m_Cnt[i] <= 3)
			{
				if (!CollisionRect(m_posModel[i].x, m_posModel[i].y,
					m_posModel[i].z, 30.0f, 30.0f, 30.0f, vPos.x,
					vPos.y, vPos.z, Long, Width, High))
				{
#if SETCONTROLLER
					//�o�C�u���[�V������]������
					if (g_pMVibration->wLeftMotorSpeed > 0)g_pMVibration->wLeftMotorSpeed -= 10000;
					if (g_pMVibration->wRightMotorSpeed > 0)g_pMVibration->wRightMotorSpeed -= 10000;

#endif

				}
				//==================================================Kirimi(�O)=============================================
				// Kirimi(�P)�ɓ���������A�̏���
				//=========================================================================================================
				if (CollisionRect(m_posModel[i].x, m_posModel[i].y,
					m_posModel[i].z, 30.0f, 30.0f, 30.0f, vPos.x,
					vPos.y, vPos.z, Long, Width, High))
				{

					m_KakunoPos = m_posModel[i];
					SetExplosion(m_posModel[i], 400.0f, 400.0f);
					m_SclModel[i] = XMFLOAT3(13.f, 13.0f, 13.0f);
#if SETCONTROLLER
					//�o�C�u���[�V������]������
					if (g_pMVibration->wLeftMotorSpeed < MAX_VIBRATION)g_pMVibration->wLeftMotorSpeed = 60000;
					if (g_pMVibration->wRightMotorSpeed < MAX_VIBRATION)g_pMVibration->wRightMotorSpeed = 60000;

#endif

					GetModelNor();
					if (XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), GetModelNor())) < 3.14159265359f / 2.0f)
					{
						if (i == 0) {
							m_Cnt[i]++;
						}
						else {


							//�����蔻��ϋv�x�{�{
							m_Cnt[i]++;

						}
					}
					else if (XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), GetModelNor())) > 3.14159265359f / 2.0f)
					{
						if (i == 13) {
							m_Cnt[i]++;
						}
						else {

							//�����蔻��ϋv�x�{�{
							m_Cnt[i]++;

						}
					}

					if (m_Cnt[i] > 3) {

					}
					return 1;


				}
			}
		}
	}

	if (!g_bDest[1]) {
		for (int i = 14; i <= 26; i++)
		{
			//---�����蔻��ϋv�x---
			if (m_Cnt[i] <= 3)
			{
				if (!CollisionRect(m_posModel[i].x, m_posModel[i].y,
					m_posModel[i].z, 30.0f, 30.0f, 30.0f, vPos.x,
					vPos.y, vPos.z, Long, Width, High))
				{
#if SETCONTROLLER
					//�o�C�u���[�V������]������
					if (g_pMVibration->wLeftMotorSpeed > 0)g_pMVibration->wLeftMotorSpeed -= 10000;
					if (g_pMVibration->wRightMotorSpeed > 0)g_pMVibration->wRightMotorSpeed -= 10000;
#endif

				}
				//==================================================Kirimi(�O)=============================================
				// Kirimi(�P)�ɓ���������A�̏���
				//=========================================================================================================
				if (CollisionRect(m_posModel[i].x, m_posModel[i].y,
					m_posModel[i].z, 30.0f, 30.0f, 30.0f, vPos.x,
					vPos.y, vPos.z, Long, Width, High))
				{
					m_KakunoPos = m_posModel[i];
					SetExplosion(m_posModel[i], 400.0f, 400.0f);
					m_SclModel[i] = XMFLOAT3(13.f, 13.0f, 13.0f);
#if SETCONTROLLER
					//�o�C�u���[�V������]������
					if (g_pMVibration->wLeftMotorSpeed < MAX_VIBRATION)g_pMVibration->wLeftMotorSpeed = 60000;
					if (g_pMVibration->wRightMotorSpeed < MAX_VIBRATION)g_pMVibration->wRightMotorSpeed = 60000;
#endif

					if (XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(0.0f, -1.0f, 0.0f, 1.0f), GetModelNor())) < 3.14159265359f / 2.0f)
					{
						if (i == 14) {
							m_Cnt[i]++;

						}
						else {

							//�����蔻��ϋv�x�{�{
							m_Cnt[i]++;
						}
					}

					else if (XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(0.0f, -1.0f, 0.0f, 1.0f), GetModelNor())) > 3.14159265359f / 2.0f)
					{
						if (i == 26) {
							m_Cnt[i]++;

						}
						else {

							//�����蔻��ϋv�x�{�{
							m_Cnt[i]++;
						}
					}

					if (m_Cnt[i] > 3) {

					}
					return 1;


				}
			}
		}
	}

	if (!g_bDest[2]) {
		for (int i = 27; i <= 40; i++)
		{
			//---�����蔻��ϋv�x---
			if (m_Cnt[i] <= 3)
			{
				if (!CollisionRect(m_posModel[i].x, m_posModel[i].y,
					m_posModel[i].z, 30.0f, 30.0f, 30.0f, vPos.x,
					vPos.y, vPos.z, Long, Width, High))
				{
#if SETCONTROLLER
					//�o�C�u���[�V������]������
					if (g_pMVibration->wLeftMotorSpeed > 0)g_pMVibration->wLeftMotorSpeed -= 10000;
					if (g_pMVibration->wRightMotorSpeed > 0)g_pMVibration->wRightMotorSpeed -= 10000;

#endif
				}
				//==================================================Kirimi(�O)=============================================
				// Kirimi(�P)�ɓ���������A�̏���
				//=========================================================================================================
				if (CollisionRect(m_posModel[i].x, m_posModel[i].y,
					m_posModel[i].z, 30.0f, 30.0f, 30.0f, vPos.x,
					vPos.y, vPos.z, Long, Width, High))
				{
					m_KakunoPos = m_posModel[i];
					SetExplosion(m_posModel[i], 400.0f, 400.0f);
					m_SclModel[i] = XMFLOAT3(13.f, 13.0f, 13.0f);
#if SETCONTROLLER
					//�o�C�u���[�V������]������
					if (g_pMVibration->wLeftMotorSpeed < MAX_VIBRATION)g_pMVibration->wLeftMotorSpeed = 60000;
					if (g_pMVibration->wRightMotorSpeed < MAX_VIBRATION)g_pMVibration->wRightMotorSpeed = 60000;
#endif



					if (XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), GetModelNor())) < 3.14159265359f / 2.0f)
					{
						if (i == 27) {
							m_Cnt[i]++;
						}
						else
						{

							m_Cnt[i]++;

						}
					}
					else if (XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), GetModelNor())) > 3.14159265359f / 2.0f)
					{
						if (i == 40) {
							m_Cnt[i]++;
						}
						else
						{

							//�����蔻��ϋv�x�{�{
							m_Cnt[i]++;
						}
					}


					if (m_Cnt[i] > 3) {

					}
					return 1;


				}
			}
		}
	}

	if (!g_bDest[3]) {
		for (int i = 41; i <= 54; i++)
		{
			//---�����蔻��ϋv�x---
			if (m_Cnt[i] <= 3)
			{
				if (!CollisionRect(m_posModel[i].x, m_posModel[i].y,
					m_posModel[i].z, 30.0f, 30.0f, 30.0f, vPos.x,
					vPos.y, vPos.z, Long, Width, High))
				{
#if SETCONTROLLER

					//�o�C�u���[�V������]������
					if (g_pMVibration->wLeftMotorSpeed > 0)g_pMVibration->wLeftMotorSpeed -= 10000;
					if (g_pMVibration->wRightMotorSpeed > 0)g_pMVibration->wRightMotorSpeed -= 10000;
#endif
				}
				//==================================================Kirimi(�O)=============================================
				// Kirimi(�P)�ɓ���������A�̏���
				//=========================================================================================================
				if (CollisionRect(m_posModel[i].x, m_posModel[i].y,
					m_posModel[i].z, 30.0f, 30.0f, 30.0f, vPos.x,
					vPos.y, vPos.z, Long, Width, High))
				{
					m_KakunoPos = m_posModel[i];
					SetExplosion(m_posModel[i], 400.0f, 400.0f);
					m_SclModel[i] = XMFLOAT3(13.f, 13.0f, 13.0f);
#if SETCONTROLLER
					if (g_pMVibration->wLeftMotorSpeed < MAX_VIBRATION)g_pMVibration->wLeftMotorSpeed = 60000;
					if (g_pMVibration->wRightMotorSpeed < MAX_VIBRATION)g_pMVibration->wRightMotorSpeed = 60000;
#endif


					if (XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(-1.0f, 0.0f, 0.0f, 1.0f), GetModelNor())) < 3.14159265359f / 2.0f)
					{
						if (i == 41) {
							m_Cnt[i]++;
						}
						else
						{


							//�����蔻��ϋv�x�{�{
							m_Cnt[i]++;
						}
					}
					else if (XMVectorGetX(XMVector3AngleBetweenVectors(XMVectorSet(-1.0f, 0.0f, 0.0f, 1.0f), GetModelNor())) > 3.14159265359f / 2.0f)
					{
						if (i == 54) {
							m_Cnt[i]++;
						}
						else
						{

							//�����蔻��ϋv�x�{�{
							m_Cnt[i]++;
						}
					}

					if (m_Cnt[i] > 3) {

					}
					return 1;


				}
			}
		}
	}
	//********************************************************************************************************

	return 0;
}
//=====================
// ���߂�ꂽ������n��
//=====================
XMFLOAT3 CKirime::GetVecXYZ()
{
	
	return m_NorPos;

}

XMFLOAT3* CKirime::GetKakunoPos()
{

	return &m_KakunoPos;


}

////=============================================================================
//// �J�����̐ݒ�
////=============================================================================
//void CKirime::SetmeshKirimeC(CCamera* pCamera)
//{
//	g_pCamera = pCamera;
//}
//
////=============================================================================
//// ���C�g�̐ݒ�
////=============================================================================
//void CKirime::SetmeshKirimeL(CLight* pLight)
//{
//	g_pLight = pLight;
//}

//=============================================================================
// �R���g���[���[�I�u�W�F�N�g�擾
//=============================================================================
void SetKirimeControllerr(XINPUT_STATE* pControlller, XINPUT_VIBRATION *pVibration)
{
	g_pMXController = pControlller;
	g_pMVibration = pVibration;
}


bool GetDestflg(int nPartsNum) {
	return g_bDest[nPartsNum];
}

