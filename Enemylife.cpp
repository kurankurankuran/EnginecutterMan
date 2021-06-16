//=============================================================================
//
// ���C�t�\������ [EnemyLife.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include <string.h>
#include "EnemyLife.h"
#include "polygon.h"
#include "Texture.h"
#include "mesh.h"
#include "scene.h"
#include "Camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE         (11)
//#define FRAME_TEXTURENAME	L"data/texture/frame_life1.png" // �ǂݍ��ރe�N�X�`���t�@�C����
//#define LIFE_TEXTURENAME	L"data/texture/life0001.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_100_TEXTURENAME L"data/texture/ENEMYLIFE_MAX.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_90_TEXTURENAME	L"data/texture/ENEMYLIFE_90.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_80_TEXTURENAME	L"data/texture/ENEMYLIFE_80.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_70_TEXTURENAME	L"data/texture/ENEMYLIFE_70.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_60_TEXTURENAME	L"data/texture/ENEMYLIFE_60.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_50_TEXTURENAME	L"data/texture/ENEMYLIFE_50.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_40_TEXTURENAME	L"data/texture/ENEMYLIFE_40.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_30_TEXTURENAME	L"data/texture/ENEMYLIFE_30.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_20_TEXTURENAME	L"data/texture/ENEMYLIFE_20.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_10_TEXTURENAME	L"data/texture/ENEMYLIFE_10.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_00_TEXTURENAME	L"data/texture/ENEMYLIFE_00.png" // �ǂݍ��ރe�N�X�`���t�@�C����

//#define FRAME_TEXTURENAME2	L"data/texture/frame_life2.png" // �ǂݍ��ރe�N�X�`���t�@�C����
//#define LIFE_TEXTURENAME2	L"data/texture/life0002.png" // �ǂݍ��ރe�N�X�`���t�@�C����
//#define FRAME_TEXTURENAME3	L"data/texture/frame_life3.png" // �ǂݍ��ރe�N�X�`���t�@�C����
//#define LIFE_TEXTURENAME3	L"data/texture/life0003.png" // �ǂݍ��ރe�N�X�`���t�@�C����

//#define FRAME_SIZE_X		400							// �g�̃T�C�Y
//#define FRAME_SIZE_Y		15							// �g�̃T�C�Y
//#define FRAME_POS_X			(0.0f)	// �g�̕\���ʒu
//#define FRAME_POS_Y			(300.0f)	// �g�̕\���ʒu


//#define MAX_HP			10							// �\������

#define LIFE_SIZE_X			300.0f							// �����̃T�C�Y
#define LIFE_SIZE_Y			100.0f							// �����̃T�C�Y
//#define LIFE_POS_X			(-SCREEN_WIDTH/2+FRAME_SIZE_X/2)	// �����̕\���ʒu
//#define LIFE_POS_Y			(SCREEN_HEIGHT/2-FRAME_SIZE_Y/2)				// �����̕\���ʒu

//
//#define LIFE_POS_X			(-243.0f)	// �����̕\���ʒu
//#define LIFE_POS_Y			(299.0f)	// �����̕\���ʒu


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexLife(ID3D11Device* pDevice);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//static ID3D11ShaderResourceView*	g_pTexture[2];	// �e�N�X�`���ւ̃|�C���^

static MESH				g_meshEnemyLife[MAX_TEXTURE];
//static CCamera*         g_pCamera;
static bool             g_bLoadMesh = false;
//
//static EnemyHP						g_HP[1];	// �r���{�[�h�e

//=============================================================================
//  �R���X�g���N�^
//=============================================================================
CEnemyLife::CEnemyLife()
{
	/*Init(XMFLOAT3(0.0f, 0.0f, 0.0f), 0, true);*/
}

CEnemyLife::CEnemyLife(XMFLOAT3 pos, int nLife,bool bIraseflg)
{
	Init(pos, nLife, bIraseflg);
}

//=============================================================================
//  �f�X�g���N�^
//=============================================================================
CEnemyLife::~CEnemyLife()
{
	//Uninit();
}


//=============================================================================
// ���C�t�\���̏���������
//=============================================================================
HRESULT CEnemyLife::Init(XMFLOAT3 pos,int nLife,bool bIraseflg)
{
	ID3D11Device* pDevice = GetDevice();
	
	m_nLife = 0;

	//// ���_���̍쐬
	//MakeVertexLife(pDevice);

	HRESULT hr = S_OK;

	// �e�N�X�`���̓ǂݍ���
	LPCWSTR pszFName[11] = { /*FRAME_TEXTURENAME, */
		LIFE_00_TEXTURENAME,
		LIFE_10_TEXTURENAME,
		LIFE_20_TEXTURENAME,
		LIFE_30_TEXTURENAME,
		LIFE_40_TEXTURENAME,
		LIFE_50_TEXTURENAME,
		LIFE_60_TEXTURENAME,
		LIFE_70_TEXTURENAME,
		LIFE_80_TEXTURENAME,
		LIFE_90_TEXTURENAME,
		LIFE_100_TEXTURENAME,
	};

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {


	
		for (int i = 0; i < MAX_TEXTURE; ++i)
		{
			ZeroMemory(&g_meshEnemyLife[i], sizeof(MESH));
			hr = CreateTextureFromFile(pDevice, pszFName[i], &(g_meshEnemyLife[i].pTexture)/*&g_pTexture[i]*/);

			if (FAILED(hr))
			{
				MessageBoxW(NULL, pszFName[i], L"�e�N�X�`���ǂݍ��݃G���[", MB_OK);
				break;
			}
			SaveMesh(&g_meshEnemyLife[i]);

			XMStoreFloat4x4(&g_meshEnemyLife[i].mtxTexture, XMMatrixIdentity());

			// ���_���̍쐬
			MakeVertexLife(pDevice);

		}
		g_bLoadMesh = true;
	}
 	m_bIraseflg = bIraseflg;

	// ���C�t������
	m_nLife = nLife;
	
	m_nStat = 1;
	XMVECTOR vRocalY = XMVector3Normalize(XMVectorSet(pos.x - 0.0f, pos.y - 0.0f, 0.0f, 1.0f));
	m_nNum = (MAX_ENEMYLIFE / 10);
	if (GetScene() == SCENE_GAME) {
		m_Pos = XMFLOAT3(pos.x + XMVectorGetX(vRocalY)*200.0f, pos.y + XMVectorGetY(vRocalY)*200.0f, pos.z + XMVectorGetZ(vRocalY)*200.0f);
	}
	else if (GetScene() == SCENE_RUSH) {
		m_Pos = XMFLOAT3(pos.x, pos.y, pos.z);
	}
	m_Scl = XMFLOAT2(/*LIFE_SIZE_X*/nLife * 3, LIFE_SIZE_Y);

	return hr;
}

//=============================================================================
// ���C�t�\���̏I������
//=============================================================================
void CEnemyLife::Uninit(void)
{
	 //�e�N�X�`�����
#if MESH_RELEASE_ON
	for (int i = 0; i < MAX_TEXTURE; ++i) {
		SAFE_RELEASE(g_meshEnemyLife[i].pTexture/*g_pTexture[i]*/);
}
#endif
	
}

//=============================================================================
// ���C�t�\���̍X�V����
//=============================================================================
void CEnemyLife::Update(void)
{

	//ID3D11Device* pDevice = GetDevice();

	if (m_nLife <= 0) {
		m_nLife = 0;
		m_nStat = 0;
		m_nNum = 0;
	}
	if (m_nLife > 0) {
		m_nNum = m_nLife / 10;
	}
	/*if (GetScene() == SCENE_GAME) {*/
		/*if (m_nNum <= 0) {
			m_nStat = 0;
		}*/
	//}

}

//=============================================================================
// ���C�t�\������
//=============================================================================
void CEnemyLife::Draw(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	
	// �r���[�}�g���b�N�X���擾
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	SetBlendState(BS_ADDITIVE);

	// ���C�e�B���O�𖳌���
	CLight::Get()->SetDisable();


	//for(int i=0;i<m_nNum;++i){	
	//	if (*(m_pLifeTipStat + i) <= 0)continue;

	//	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	//	//���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();
	//	XMStoreFloat4x4(&g_meshEnemyLife[i].mtxWorld, mtxWorld);

	//	g_meshEnemyLife[i].mtxWorld._11 = mtxView._11;
	//	g_meshEnemyLife[i].mtxWorld._12 = mtxView._21;
	//	g_meshEnemyLife[i].mtxWorld._13 = mtxView._31;
	//	g_meshEnemyLife[i].mtxWorld._21 = mtxView._12;
	//	g_meshEnemyLife[i].mtxWorld._22 = mtxView._22;
	//	g_meshEnemyLife[i].mtxWorld._23 = mtxView._32;
	//	g_meshEnemyLife[i].mtxWorld._31 = mtxView._13;
	//	g_meshEnemyLife[i].mtxWorld._32 = mtxView._23;
	//	g_meshEnemyLife[i].mtxWorld._33 = mtxView._33;

	//	mtxWorld = XMLoadFloat4x4(&g_meshEnemyLife[i].mtxWorld);

	//	// �X�P�[���𔽉f
	//	mtxScale = XMMatrixScaling(m_Scl.x,
	//		m_Scl.y, 1.0f);
	//	mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(/*m_Pos.x*/m_fLifeTipPosX[i],
	//		m_Pos.y,
	//		m_Pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ���[���h�}�g���b�N�X�̐ݒ�
	//	XMStoreFloat4x4(&g_meshEnemyLife[i].mtxWorld, mtxWorld);

	//	// ���b�V���̕`��
	//	DrawMesh(pDeviceContext, &g_meshEnemyLife[i]);


	//}


	//// ���C�e�B���O��L����
	//CLight::Get()->SetEnable();

	//// ��������������
	//SetBlendState(BS_NONE);
	//******************************************************************************
	//ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;
	//// �r���[�}�g���b�N�X���擾
	//XMFLOAT4X4& mtxView = g_pCamera->GetViewMatrix();

	//SetBlendState(BS_ADDITIVE);

	// ���C�e�B���O�𖳌���
	CLight::Get()->SetDisable();
	if (m_bIraseflg) {
		if (m_nStat > 0) {
			//���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld, mtxWorld);

			g_meshEnemyLife[m_nNum].mtxWorld._11 = mtxView._11;
			g_meshEnemyLife[m_nNum].mtxWorld._12 = mtxView._21;
			g_meshEnemyLife[m_nNum].mtxWorld._13 = mtxView._31;
			g_meshEnemyLife[m_nNum].mtxWorld._21 = mtxView._12;
			g_meshEnemyLife[m_nNum].mtxWorld._22 = mtxView._22;
			g_meshEnemyLife[m_nNum].mtxWorld._23 = mtxView._32;
			g_meshEnemyLife[m_nNum].mtxWorld._31 = mtxView._13;
			g_meshEnemyLife[m_nNum].mtxWorld._32 = mtxView._23;
			g_meshEnemyLife[m_nNum].mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld);

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(m_Scl.x,
				m_Scl.y, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(m_Pos.x,
				m_Pos.y,
				m_Pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld, mtxWorld);

			// ���b�V���̕`��
			DrawMesh(pDeviceContext, &g_meshEnemyLife[m_nNum]);
		}
		//****************************************************************************
	}
	else {
		//���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();
		XMStoreFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld, mtxWorld);

		g_meshEnemyLife[m_nNum].mtxWorld._11 = mtxView._11;
		g_meshEnemyLife[m_nNum].mtxWorld._12 = mtxView._21;
		g_meshEnemyLife[m_nNum].mtxWorld._13 = mtxView._31;
		g_meshEnemyLife[m_nNum].mtxWorld._21 = mtxView._12;
		g_meshEnemyLife[m_nNum].mtxWorld._22 = mtxView._22;
		g_meshEnemyLife[m_nNum].mtxWorld._23 = mtxView._32;
		g_meshEnemyLife[m_nNum].mtxWorld._31 = mtxView._13;
		g_meshEnemyLife[m_nNum].mtxWorld._32 = mtxView._23;
		g_meshEnemyLife[m_nNum].mtxWorld._33 = mtxView._33;

		mtxWorld = XMLoadFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld);

		// �X�P�[���𔽉f
		mtxScale = XMMatrixScaling(m_Scl.x,
			m_Scl.y, 1.0f);
		mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_Pos.x,
			m_Pos.y,
			m_Pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		XMStoreFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld, mtxWorld);

		// ���b�V���̕`��
		DrawMesh(pDeviceContext, &g_meshEnemyLife[m_nNum]);
	}

	// ���C�e�B���O��L����
	CLight::Get()->SetEnable();

	// ��������������
	SetBlendState(BS_NONE);
}

//=============================================================================
// ���C�t���Z����
//=============================================================================
void CEnemyLife::DelEnemyLife(int nValue)
{
	m_nLife -= nValue;

	
}

//=============================================================================
// ���C�t�擾����
//=============================================================================
int CEnemyLife::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexLife(ID3D11Device* pDevice)
{
	HRESULT hr;
	for (int i = 0; i < MAX_TEXTURE; ++i) {
		// �I�u�W�F�N�g�̒��_�z��𐶐�
		g_meshEnemyLife[i].nNumVertex = 4;
		VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshEnemyLife[i].nNumVertex];

		// ���_�z��̒��g�𖄂߂�
		VERTEX_3D* pVtx = pVertexWk;

		// ���_���W�̐ݒ�
		pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[1].vtx = XMFLOAT3(-1.0f / 2, 1.0f / 2, 0.0f);
		pVtx[2].vtx = XMFLOAT3(1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[3].vtx = XMFLOAT3(1.0f / 2, 1.0f / 2, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
		pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
		pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
		pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

		// �C���f�b�N�X�z��𐶐�
		g_meshEnemyLife[i].nNumIndex = 4;
		int* pIndexWk = new int[g_meshEnemyLife[i].nNumIndex];

		// �C���f�b�N�X�z��̒��g�𖄂߂�
		pIndexWk[0] = 0;
		pIndexWk[1] = 1;
		pIndexWk[2] = 2;
		pIndexWk[3] = 3;

		// ���_�o�b�t�@����
		hr = MakeMeshVertex(pDevice, &g_meshEnemyLife[i], pVertexWk, pIndexWk);

		// �ꎞ�z��̉��
		delete[] pIndexWk;
		delete[] pVertexWk;
	}
	return hr;
}


//void CEnemyLife::SetCamera(CCamera* pCamera)
//{
//	g_pCamera = pCamera;
//}
