//=============================================================================
//
// �������� [bilibili.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "bilibili.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include <stdio.h>
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BILIBILI		L"data/texture/bilibili.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BILIBILI_SIZE_X		(5.0f)							// �r���{�[�h�̕�
#define	BILIBILI_SIZE_Y		(5.0f)							// �r���{�[�h�̍���

#define	MAX_BILIBILI		(128)							// �r���{�[�h�ő吔

#define MAX_BILIBILI_LIFE		(600)

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(0.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct BILIBILI {
	XMFLOAT4X4	mtxWorld;	// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;		// �ʒu
	XMFLOAT4	col;		// �F
	float		fSizeX;		// ��
	float		fSizeY;		// ����
	int			nCounter;	// �J�E���^�[
	int			nPattern;	// �p�^�[��No.
	float		fAlpha;		// �A���t�@�l
	int			nLife;		// ��������
	bool		bUse;		// �g�p���Ă��邩�ǂ���
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBilibili(ID3D11Device* pDevice);
void SetVertexBilibili(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorBilibili(int nIdxBilibili, XMFLOAT4 col);
void SetTextureBilibili(int nIdxBilibili, int nPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_mesh;
static MATERIAL			g_material;
static BILIBILI		g_bilibili[MAX_BILIBILI];	// �������[�N
static CPlayer*         g_pPlayer;
static CCamera*         g_pCamera;
static bool             g_bLoadMesh = false;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBilibili(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		ZeroMemory(&g_mesh, sizeof(MESH));

		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BILIBILI,	// �t�@�C���̖��O
			&g_mesh.pTexture);	// �ǂݍ��ރ������[
		// �e�N�X�`���\�̕ۑ�
		SaveMesh(&g_mesh);
		// ���_���̍쐬
		MakeVertexBilibili(pDevice);

		g_bLoadMesh = true;
	}
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// �}�e���A���̐ݒ�
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_mesh.pMaterial = &g_material;

	srand((unsigned int)time(NULL));

	BILIBILI* pBili = g_bilibili;
	for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili) {
		pBili->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pBili->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pBili->fSizeX = BILIBILI_SIZE_X;
		pBili->fSizeY = BILIBILI_SIZE_Y;
		pBili->nCounter = 0;
		pBili->nPattern = 0;/*((rand() % 8) == 1);*/
		pBili->fAlpha = 1.0f;
		pBili->nLife = 0/*60*MAX_BILIBILI_LIFE*/;
		pBili->bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBilibili(void)
{
#if MESH_RELEASE_ON
		// ���b�V���̊J��
		ReleaseMesh(&g_mesh);
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBilibili(void)
{
	//BILIBILI* pBili = g_bilibili;
	//for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili)
	//{
	//	if (pBili->bUse)
	//	{
	//		++pBili->nCounter;
	//		//pBili->nPattern++;

	//		//�����Ԋu
	//		if (pBili->nCounter % 30 == 0)
	//		{
	//			pBili->nPattern++;
	//		}


	//		/*if (pBili->nPattern >= 8)
	//		{
	//			pBili->bUse = false;
	//			continue;
	//		}*/


	//		// ���_���W�̐ݒ�
	//		pBili->fSizeX += 0.5f;
	//		pBili->fSizeY += 0.5f;

	//		SetVertexBilibili(i, pBili->fSizeX, pBili->fSizeY);

	//		if (pBili->nPattern >= 2)
	//		{
	//			// ���_�J���[�̐ݒ�
	//			pBili->fAlpha -= 0.02f;

	//			if (pBili->nCounter % 30 == 0)
	//			{
	//				pBili->nPattern++;
	//				pBili->nPattern = (rand() % 9);
	//				if (pBili->nPattern == 8)
	//				{
	//					pBili->bUse = false;
	//				}
	//				
	//				/*if (pBili->nLife > 0)
	//				{
	//					if (pBili->nPattern > 8)
	//					{
	//						pBili->nPattern == 0;
	//					}
	//				}
	//				else
	//				{
	//					pBili->bUse = false;
	//				}
	//				pBili->nLife -= 1;*/
	//			}

	//			if (pBili->fAlpha < 0.0f)
	//			{
	//				pBili->fAlpha = 0.0f;
	//			}
	//			SetColorBilibili(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pBili->fAlpha));
	//		}
	//	}
	//	/*pBili->nLife -= 1;
	//	if (pBili->nLife > 0)
	//	{
	//		if (pBili->nPattern > 8)
	//		{
	//			pBili->nPattern == 0;
	//		}
	//	}
	//	else
	//	{
	//		pBili->bUse = false;
	//	}*/
	//}
	BILIBILI* pBili = g_bilibili;
	for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili)
	{
		if (pBili->bUse)
		{
			++pBili->nCounter;
			//pBili->nPattern++;
			pBili->nLife++;

			////�����Ԋu
			if (pBili->nCounter % 30 == 0)
			{
				pBili->nPattern++;
				pBili->nPattern = (rand() % 9);

			}
			//                   
			if (pBili->nLife >= MAX_BILIBILI_LIFE)
			{

				pBili->bUse = false;
				pBili->nLife = 0;
				continue;

			}

			///*if (pBili->nPattern >= 8)
			//{
				//pBili->bUse = false;
			continue;
			//}*/


			// ���_���W�̐ݒ�
			pBili->fSizeX += 0.5f;
			pBili->fSizeY += 0.5f;

			SetVertexBilibili(i, pBili->fSizeX, pBili->fSizeY);

			if (pBili->nPattern >= 2)
			{
				// ���_�J���[�̐ݒ�
				//pBili->fAlpha -= 0.02f;

				/*if (pBili->nCounter % 30 == 0)
				{
					pBili->nPattern++;
					pBili->nPattern = (rand() % 9);
					if (pBili->nPattern == 8)
					{
						pBili->bUse = false;
					}*/

					/*if (pBili->nLife > 0)
					{
						if (pBili->nPattern > 8)
						{
							pBili->nPattern == 0;
						}
					}
					else
					{
						pBili->bUse = false;
					}
					pBili->nLife -= 1;*/
					//}

					/*if (pBili->fAlpha < 0.0f)
					{
						pBili->fAlpha = 0.0f;
					}*/
				SetColorBilibili(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pBili->fAlpha));
			}
		}
		/*pBili->nLife -= 1;
		if (pBili->nLife > 0)
		{
			if (pBili->nPattern > 8)
			{
				pBili->nPattern == 0;
			}
		}
		else
		{
			pBili->bUse = false;
		}*/
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBilibili(void)
{
	//ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	//XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	//// �r���[�}�g���b�N�X���擾
	////XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	//XMFLOAT4X4& mtxView = g_pCamera->GetViewMatrix();
	//// ���Z����
	//SetBlendState(BS_ADDITIVE);

	//BILIBILI* pBili = g_bilibili;
	//for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili) {
	//	if (pBili->bUse) {
	//		// ���[���h�}�g���b�N�X�̏�����
	//		mtxWorld = XMMatrixIdentity();
	//		XMStoreFloat4x4(&pBili->mtxWorld, mtxWorld);

	//		pBili->mtxWorld._11 = mtxView._11;
	//		pBili->mtxWorld._12 = mtxView._21;
	//		pBili->mtxWorld._13 = mtxView._31;
	//		pBili->mtxWorld._21 = mtxView._12;
	//		pBili->mtxWorld._22 = mtxView._22;
	//		pBili->mtxWorld._23 = mtxView._32;
	//		pBili->mtxWorld._31 = mtxView._13;
	//		pBili->mtxWorld._32 = mtxView._23;
	//		pBili->mtxWorld._33 = mtxView._33;

	//		mtxWorld = XMLoadFloat4x4(&pBili->mtxWorld);

	//		// �X�P�[���𔽉f
	//		mtxScale = XMMatrixScaling(pBili->fSizeX, pBili->fSizeY, 1.0f);
	//		mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

	//		// �ړ��𔽉f
	//		mtxTranslate = XMMatrixTranslation(pBili->pos.x, pBili->pos.y, pBili->pos.z);
	//		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//		// ���[���h�}�g���b�N�X�̐ݒ�
	//		XMStoreFloat4x4(&pBili->mtxWorld, mtxWorld);
	//		g_mesh.mtxWorld = pBili->mtxWorld;

	//		// �e�N�X�`�����W�̐ݒ�
	//		SetTextureBilibili(i, pBili->nPattern);

	//		// �J���[�̐ݒ�
	//		g_material.Diffuse = pBili->col;

	//		// ���b�V���̕`��
	//		DrawMesh(pDeviceContext, &g_mesh);
	//	}
	//}

	//// ��������������
	//SetBlendState(BS_NONE);

	// �����ŃI�t�ɂ���
	CLight::Get()->SetDisable();

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// �r���[�}�g���b�N�X���擾
	//XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	XMFLOAT4X4& mtxView = g_pCamera->GetViewMatrix();
	// ���Z����
	SetBlendState(BS_ADDITIVE);

	BILIBILI* pBili = g_bilibili;
	for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili) {
		if (pBili->bUse) {
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&pBili->mtxWorld, mtxWorld);

			pBili->mtxWorld._11 = mtxView._11;
			pBili->mtxWorld._12 = mtxView._21;
			pBili->mtxWorld._13 = mtxView._31;
			pBili->mtxWorld._21 = mtxView._12;
			pBili->mtxWorld._22 = mtxView._22;
			pBili->mtxWorld._23 = mtxView._32;
			pBili->mtxWorld._31 = mtxView._13;
			pBili->mtxWorld._32 = mtxView._23;
			pBili->mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&pBili->mtxWorld);

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(pBili->fSizeX, pBili->fSizeY, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(pBili->pos.x, pBili->pos.y, pBili->pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&pBili->mtxWorld, mtxWorld);
			g_mesh.mtxWorld = pBili->mtxWorld;

			// �e�N�X�`�����W�̐ݒ�
			SetTextureBilibili(i, pBili->nPattern);

			// �J���[�̐ݒ�
			g_material.Diffuse = pBili->col;

			// ���b�V���̕`��
			DrawMesh(pDeviceContext, &g_mesh);
		}
	}

	// ��������������
	SetBlendState(BS_NONE);

	// �����ŃI���ɂ���
	CLight::Get()->SetEnable();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBilibili(ID3D11Device* pDevice)
{
	// �I�u�W�F�N�g�̒��_�z��𐶐�
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

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
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[g_mesh.nNumIndex];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBilibili(int nIdxBilibili, float fSizeX, float fSizeY)
{
	if (nIdxBilibili >= 0 && nIdxBilibili < MAX_BILIBILI) {
		BILIBILI* pBili = &g_bilibili[nIdxBilibili];
		pBili->fSizeX = fSizeX;
		pBili->fSizeY = fSizeY;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorBilibili(int nIdxBilibili, XMFLOAT4 col)
{
	if (nIdxBilibili >= 0 && nIdxBilibili < MAX_BILIBILI) {
		g_bilibili[nIdxBilibili].col = col;
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureBilibili(int nIdxBilibili, int nPattern)
{
	XMMATRIX mtxTexture, mtxScale, mtxTranslate;

	// �e�N�X�`���}�g���b�N�X�̏�����
	mtxTexture = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScale = XMMatrixScaling(0.125f, 1.0f, 1.0f);
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxScale);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(0.125f * nPattern, 0.0f, 0.0f);
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxTranslate);

	// �e�N�X�`���}�g���b�N�X�̐ݒ�
	XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTexture);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetBilibili(XMFLOAT3 pos, float fSizeX, float fSizeY)
{
	int nIdxBilibili = -1;


	BILIBILI* pBili = g_bilibili;
	for (int i = 0; i < MAX_BILIBILI; ++i, ++pBili) {
		if (!pBili->bUse) {
			pBili->pos = pos;
			pBili->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			pBili->nCounter = 0;
			pBili->nPattern = 0;/*((rand() % 8) == 1);*/
			pBili->nLife = 0;
			pBili->fAlpha = 1.0f;
			pBili->bUse = true;

			// ���_���W�̐ݒ�
			SetVertexBilibili(i, fSizeX, fSizeY);

			nIdxBilibili = i;
			break;
		}
	}

	return nIdxBilibili;
}

//void SetBilibiliPlayer(CPlayer* pPlayer)
//{
//	g_pPlayer = pPlayer;
//
//
//
//}

void SetBilibiliCamera(CCamera* pCamera)
{


	g_pCamera = pCamera;


}