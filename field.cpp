//=============================================================================
//
// �n�ʏ��� [field.cpp]
// Author :KAZUKI KURA
//
//=============================================================================
#include "field.h"
#include "AssimpModel.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"
#include "Camera.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_CHAIR		 "data/model/chair.fbx"
#define MODEL_TELEVISION "data/model/television.fbx"
#define MODEL_TABLE		 "data/model/table.fbx"

#define	TEXTURE_FILENAME	L"data/texture/field002.jpg"	// �e�N�X�`���t�@�C����

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define EN					(3.14159265359f)	// �~����
#define ROOM_SIZE	(1000.0f)					// �����傫��


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAssimpModel		g_field[3+10];
//static CCamera*			g_pCamera;
//static CLight*			g_pLight;

//static MESH							g_mesh;					// �\����
static MATERIAL						g_material;				// �}�e���A��

static bool g_bModelLoad = false;
CField::CField()
{
	Init();
}

CField::~CField()
{
	Uninit();
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CField::Init(void)
{
	// �ʒu�E��]�E�X�P�[���̏����ݒ�(�t�B�[���h)
	m_Pos[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Pos[1] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Pos[2] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rot = XMFLOAT3(90.0f, 0.0f, 0.0f);
	m_Scl = XMFLOAT3(50.0f, 50.0f, 50.0f);

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif

	if (!g_bModelLoad) {
		// ���f���f�[�^�̓ǂݍ���
		if (!g_field[0].Load(pDevice, pDeviceContext, MODEL_CHAIR)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitField", MB_OK);
			return hr;
		}
		SaveModel(&g_field[0]);

		if (!g_field[1].Load(pDevice, pDeviceContext, MODEL_TELEVISION)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitField", MB_OK);
			return hr;
		}
		SaveModel(&g_field[1]);

		if (!g_field[2].Load(pDevice, pDeviceContext, MODEL_TABLE)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitField", MB_OK);
			return hr;
		}
		SaveModel(&g_field[2]);

		g_bModelLoad = true;
	}

	// �}�e���A���̏����ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 50.0f;
	g_material.Emissive = M_EMISSIVE;
	
	//// �t�B�[���h������((���W), (�p�x), (�@���x�N�g��), (�\���������))
 // 	m_FieldMesh.InitField(XMFLOAT3(0.0f, -ROOM_SIZE / 1.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), FLOOR);			// ��
	//m_FieldMesh.InitField(XMFLOAT3(0.0f, ROOM_SIZE *  1.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, EN), XMFLOAT3(0.0f, -1.0f, 0.0f), CEILING);	// �V��
	//m_FieldMesh.InitField(XMFLOAT3(ROOM_SIZE, 0.0f, 0.0f), XMFLOAT3(-EN / 2, EN / 2, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), WALL1);			// �E
	//m_FieldMesh.InitField(XMFLOAT3(-ROOM_SIZE, 0.0f, 0.0f), XMFLOAT3(EN / 2, EN / 2, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), WALL2);				// ��
	//m_FieldMesh.InitField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(EN / 2, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), WALL3);								// ��O
	//m_FieldMesh.InitField(XMFLOAT3(0.0f, 0.0f, ROOM_SIZE), XMFLOAT3(-EN / 2, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), WALL4);			// ��

	// ************* �t�B�[���h������ (���W), (�p�x), (�@���x�N�g��), (�傫��), (�\���������) *************
	// ----------------------------------------- ���@ ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(EN / 2, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), FLOOR);
	// ----------------------------------------- �V�� ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(0.0f, 0.0f, 4000.0f/*ROOM_SIZE * 4*/), XMFLOAT3(-EN / 2, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), CEILING);
	// ----------------------------------------- ���@ ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(0.0f, ROOM_SIZE * 2, ROOM_SIZE * 2), XMFLOAT3(EN, EN / 2, 0.0f),
		XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), WALL1);
	// ----------------------------------------- ��O ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(0.0f, -ROOM_SIZE * 2, ROOM_SIZE * 2), XMFLOAT3(0.0f, EN / 2, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), WALL2);
	// ----------------------------------------- �E�@ ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(ROOM_SIZE * 2, 0.0f, ROOM_SIZE * 2), XMFLOAT3(-EN / 2, EN / 2, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), WALL3);
	// ----------------------------------------- ���@ ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(-ROOM_SIZE * 2, 0.0f, ROOM_SIZE * 2), XMFLOAT3(EN / 2, EN / 2, 0.0f),
		XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), WALL4);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CField::Uninit(void)
{
	//// ���b�V�����
	//ReleaseMesh(&g_mesh);
	m_FieldMesh.UninitField();

	// ���f���̉��
#if MODEL_RELEASE_ON
		for (int i = 0; i < 3; ++i) {
			g_field[i].Release();
		}
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void CField::Update(void)
{
	// �J�����̌����擾
	//XMFLOAT3 rotCamera = g_pCamera->GetAngle();

	//==============================================================================================
	for (int i = 0; i < 3; ++i) {
		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_Scl.x, m_Scl.y, m_Scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rot.x),
			XMConvertToRadians(m_Rot.y), XMConvertToRadians(m_Rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		mtxTranslate = XMMatrixTranslation(m_Pos[i].x, m_Pos[i].y, m_Pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);

		
	}

	//// ���b�V���X�V
	//UpdateMesh(&g_mesh);
	m_FieldMesh.UpdateField();

}

//=============================================================================
// �`�揈��
//=============================================================================
void CField::Draw(void)
{
	
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	for (int i = 0; i < 3; ++i) {
		// �J�����A�����ݒ�
		g_field[i].SetCamera(CCamera::Get());
		g_field[i].SetLight(CLight::Get()/*g_pLight*/);

		// �s����������`��
		g_field[i].Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);


		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

		g_field[i].Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);

		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}

	
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//// ���b�V���̕`��
	//DrawMesh(pDC, &g_mesh);
	m_FieldMesh.DrawField();
}

////=============================================================================
//// �J�����̐ݒ�
////=============================================================================
//void CField::SetCamera(CCamera* Camera)
//{
//	g_pCamera = Camera;
//}

////=============================================================================
//// ���C�g�̐ݒ�
////=============================================================================
//void CField::SetLight(CLight* Light)
//{
//	g_pLight = Light;
//}

