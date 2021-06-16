//=============================================================================
//
// �q�b�g�G�t�F�N�g���� [hiteffect.cpp]
// Author : �O��@�G���Y
// ----------------------------------------------------------------------------
// 2020/12/23 DrawHitEffect()�̍ŏ��ƍŌ�ɃI�t�ɂ�����I���ɂ��鏈����ǉ��B�O��
//
//=============================================================================
#include "hiteffect.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "light.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EXPLOSION		L"data/texture/pipo-btleffect003.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	HITEFFECT_SIZE_X		(5.0f)							// �r���{�[�h�̕�
#define	HITEFFECT_SIZE_Y		(5.0f)							// �r���{�[�h�̍���

#define	MAX_HITEFFECT			(128)							// �r���{�[�h�ő吔

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(0.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct HITEFFECT {
	XMFLOAT4X4	mtxWorld;	// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;		// �ʒu
	XMFLOAT4	col;		// �F
	float		fSizeX;		// ��
	float		fSizeY;		// ����
	int			nCounter;	// �J�E���^�[
	int			nPattern;	// �p�^�[��No.
	float		fAlpha;		// �A���t�@�l
	bool		bUse;		// �g�p���Ă��邩�ǂ���
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexHitEffect(ID3D11Device* pDevice);
void SetVertexHitEffect(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorHitEffect(int nIdxHitEffect, XMFLOAT4 col);
void SetTextureHitEffect(int nIdxHitEffect, int nPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_mesh;
static MATERIAL			g_material;
static HITEFFECT		g_hiteffect[MAX_HITEFFECT];	// �������[�N
static CPlayer*         g_pPlayer;
static CCamera*         g_pCamera;
static bool             g_bLoadMesh = false;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitHitEffect(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	/*ZeroMemory(&g_mesh, sizeof(MESH));
*/
	//// ���_���̍쐬
	//MakeVertexHitEffect(pDevice);

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {

		//ZeroMemory(&g_mesh, sizeof(MESH));

		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EXPLOSION,	// �t�@�C���̖��O
			&g_mesh.pTexture);	// �ǂݍ��ރ������[

		// �e�N�X�`���\�̕ۑ�
		SaveMesh(&g_mesh);

		// ���_���̍쐬
		MakeVertexHitEffect(pDevice);

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

	HITEFFECT* pHE = g_hiteffect;
	for (int i = 0; i < MAX_HITEFFECT; ++i, ++pHE) {
		pHE->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pHE->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pHE->fSizeX = HITEFFECT_SIZE_X;
		pHE->fSizeY = HITEFFECT_SIZE_Y;
		pHE->nCounter = 0;
		pHE->nPattern = 0;
		pHE->fAlpha = 1.0f;
		pHE->bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHitEffect(void)
{
#if MESH_RELEASE_ON
		// ���b�V���̊J��
		ReleaseMesh(&g_mesh);
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHitEffect(void)
{
	HITEFFECT* pHE = g_hiteffect;
	for (int i = 0; i < MAX_HITEFFECT; ++i, ++pHE)
{
		if (pHE->bUse)
		{
			++pHE->nCounter;
			
			// �q�b�g�G�t�F�N�g�̑��x�ݒ�
			if (pHE->nCounter % 2 == 0)
			{
				pHE->nPattern++;
			}
			
			// �摜�̌����o��
			if (pHE->nPattern >= 5)
			{
				pHE->bUse = false;
				continue;
			}


			// ���_���W�̐ݒ�
			pHE->fSizeX += 0.5f;
			pHE->fSizeY += 0.5f;

			SetVertexHitEffect(i, pHE->fSizeX, pHE->fSizeY);

			if (pHE->nPattern >= 2)
			{
				// ���_�J���[�̐ݒ�
				pHE->fAlpha -= 0.02f;

				if (pHE->nCounter % 10 == 0)
				{
					pHE->nPattern++;

				}

				if (pHE->fAlpha < 0.0f)
				{
					pHE->fAlpha = 0.0f;
				}
				SetColorHitEffect(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pHE->fAlpha));
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHitEffect(void)
{
	// �����ŃI�t�ɂ���
	CLight::Get()->SetDisable();

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// �r���[�}�g���b�N�X���擾
	//XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	// ���Z����
	SetBlendState(BS_ADDITIVE);

	HITEFFECT* pHE = g_hiteffect;
	for (int i = 0; i < MAX_HITEFFECT; ++i, ++pHE) {
		if (pHE->bUse) {
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&pHE->mtxWorld, mtxWorld);

			pHE->mtxWorld._11 = mtxView._11;
			pHE->mtxWorld._12 = mtxView._21;
			pHE->mtxWorld._13 = mtxView._31;
			pHE->mtxWorld._21 = mtxView._12;
			pHE->mtxWorld._22 = mtxView._22;
			pHE->mtxWorld._23 = mtxView._32;
			pHE->mtxWorld._31 = mtxView._13;
			pHE->mtxWorld._32 = mtxView._23;
			pHE->mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&pHE->mtxWorld);

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(pHE->fSizeX, pHE->fSizeY, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(pHE->pos.x, pHE->pos.y, pHE->pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&pHE->mtxWorld, mtxWorld);
			g_mesh.mtxWorld = pHE->mtxWorld;

			// �e�N�X�`�����W�̐ݒ�
			SetTextureHitEffect(i, pHE->nPattern);

			// �J���[�̐ݒ�
			g_material.Diffuse = pHE->col;

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
HRESULT MakeVertexHitEffect(ID3D11Device* pDevice)
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
void SetVertexHitEffect(int nIdxHitEffect, float fSizeX, float fSizeY)
{
	if (nIdxHitEffect >= 0 && nIdxHitEffect < MAX_HITEFFECT) {
		HITEFFECT* pExp = &g_hiteffect[nIdxHitEffect];
		pExp->fSizeX = fSizeX;
		pExp->fSizeY = fSizeY;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorHitEffect(int nIdxHitEffect, XMFLOAT4 col)
{
	if (nIdxHitEffect >= 0 && nIdxHitEffect < MAX_HITEFFECT) {
		g_hiteffect[nIdxHitEffect].col = col;
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureHitEffect(int nIdxHitEffect, int nPattern)
{
	XMMATRIX mtxTexture, mtxScale, mtxTranslate;

	// �e�N�X�`���}�g���b�N�X�̏�����
	mtxTexture = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScale = XMMatrixScaling(0.2f, 1.0f, 1.0f);		  // X��1��������
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxScale);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(0.2f * nPattern, 0.0f, 0.0f); 
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxTranslate);

	// �e�N�X�`���}�g���b�N�X�̐ݒ�
	XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTexture);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetHitEffect(XMFLOAT3 pos, float fSizeX, float fSizeY)
{
	int nIdxSetHitEffect = -1;


	HITEFFECT* pHE = g_hiteffect;
	for (int i = 0; i < MAX_HITEFFECT; ++i, ++pHE) {
		if (!pHE->bUse) {
			pHE->pos = pos;
			pHE->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			pHE->nCounter = 0;
			pHE->nPattern = 0;
			pHE->fAlpha = 1.0f;
			pHE->bUse = true;

			// ���_���W�̐ݒ�
			SetVertexHitEffect(i, fSizeX, fSizeY);

			nIdxSetHitEffect = i;
			break;
		}
	}

	return nIdxSetHitEffect;
}

void SetHitEffectCamera(CCamera* pCamera)
{
	g_pCamera = pCamera;


}