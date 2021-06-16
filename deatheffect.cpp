//=============================================================================
//
// ���S�G�t�F�N�g���� [deatheffect.cpp]
// Author : �O��@�G���Y
// ----------------------------------------------------------------------------
// 2020/12/23 DrawDeathEffect()�̍ŏ��ƍŌ�ɃI�t�ɂ�����I���ɂ��鏈����ǉ��B�O��
//
//=============================================================================
#include "deatheffect.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_DEATHEFFECT		L"data/texture/pipo-mapeffect005.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	DEATHEFFECT_SIZE_X		(5.0f)							// �r���{�[�h�̕�
#define	DEATHEFFECT_SIZE_Y		(5.0f)							// �r���{�[�h�̍���

#define	MAX_DEATHEFFECT			(128)							// �r���{�[�h�ő吔

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(0.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct DEATHEFFECT {
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
HRESULT MakeVertexDeathEffect(ID3D11Device* pDevice);
void SetVertexDeathEffect(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorDeathEffect(int nIdxDeathEffect, XMFLOAT4 col);
void SetTextureDeathEffect(int nIdxDeathEffect, int nPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_mesh;
static MATERIAL			g_material;
static DEATHEFFECT		g_deatheffect[MAX_DEATHEFFECT];	// �������[�N
static CPlayer*         g_pPlayer;
static CCamera*         g_pCamera;
static bool             g_bLoadMesh = false;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitDeathEffect(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	/*ZeroMemory(&g_mesh, sizeof(MESH));*/

	//// ���_���̍쐬
	//MakeVertexDeathEffect(pDevice);

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {

		ZeroMemory(&g_mesh, sizeof(MESH));

		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			TEXTURE_DEATHEFFECT,	// �t�@�C���̖��O
			&g_mesh.pTexture);	// �ǂݍ��ރ������[
		// �e�N�X�`���\�̕ۑ�
		SaveMesh(&g_mesh);

		// ���_���̍쐬
		MakeVertexDeathEffect(pDevice);

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

	DEATHEFFECT* pHE = g_deatheffect;
	for (int i = 0; i < MAX_DEATHEFFECT; ++i, ++pHE) {
		pHE->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pHE->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pHE->fSizeX = DEATHEFFECT_SIZE_X;
		pHE->fSizeY = DEATHEFFECT_SIZE_Y;
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
void UninitDeathEffect(void)
{
#if MESH_RELEASE_ON
		// ���b�V���̊J��
		ReleaseMesh(&g_mesh);
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateDeathEffect(void)
{
	DEATHEFFECT* pHE = g_deatheffect;
	for (int i = 0; i < MAX_DEATHEFFECT; ++i, ++pHE)
{
		if (pHE->bUse)
		{
			++pHE->nCounter;
			
			// �q�b�g�G�t�F�N�g�̑��x�ݒ�
			if (pHE->nCounter % 6 == 0)
			{
				pHE->nPattern++;
			}
			
			// �摜�̌����o��
			if (pHE->nPattern >= 10)
			{
				pHE->bUse = false;
				continue;
			}


			// ���_���W�̐ݒ�
			pHE->fSizeX += 0.5f;
			pHE->fSizeY += 0.5f;

			SetVertexDeathEffect(i, pHE->fSizeX, pHE->fSizeY);

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
				SetColorDeathEffect(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pHE->fAlpha));
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDeathEffect(void)
{
	// �����ŃI�t�ɂ���
	CLight::Get()->SetDisable();

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// �r���[�}�g���b�N�X���擾
	//XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	XMFLOAT4X4& mtxView = CCamera::Get()/*g_pCamera*/->GetViewMatrix();
	// ���Z����
	SetBlendState(BS_ADDITIVE);

	DEATHEFFECT* pHE = g_deatheffect;
	for (int i = 0; i < MAX_DEATHEFFECT; ++i, ++pHE) {
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
			SetTextureDeathEffect(i, pHE->nPattern);

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
HRESULT MakeVertexDeathEffect(ID3D11Device* pDevice)
{
	// �I�u�W�F�N�g�̒��_�z��𐶐�
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	// ���_���W�̐ݒ�
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[1].vtx = XMFLOAT3(-1.0f / 2, 1.0f  / 2, 0.0f);
	pVtx[2].vtx = XMFLOAT3(1.0f  / 2, -1.0f / 2, 0.0f);
	pVtx[3].vtx = XMFLOAT3(1.0f  / 2, 1.0f  / 2, 0.0f);

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
void SetVertexDeathEffect(int nIdxDeathEffect, float fSizeX, float fSizeY)
{
	if (nIdxDeathEffect >= 0 && nIdxDeathEffect < MAX_DEATHEFFECT) {
		DEATHEFFECT* pExp = &g_deatheffect[nIdxDeathEffect];
		pExp->fSizeX = fSizeX;
		pExp->fSizeY = fSizeY;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorDeathEffect(int nIdxDeathEffect, XMFLOAT4 col)
{
	if (nIdxDeathEffect >= 0 && nIdxDeathEffect < MAX_DEATHEFFECT) {
		g_deatheffect[nIdxDeathEffect].col = col;
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureDeathEffect(int nIdxDeathEffect, int nPattern)
{
	XMMATRIX mtxTexture, mtxScale, mtxTranslate;

	// �e�N�X�`���}�g���b�N�X�̏�����
	mtxTexture = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScale = XMMatrixScaling(0.1f, 1.0f, 1.0f);		  // X��1��������
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxScale);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(0.1f * nPattern, 0.0f, 0.0f); 
	mtxTexture = XMMatrixMultiply(mtxTexture, mtxTranslate);

	// �e�N�X�`���}�g���b�N�X�̐ݒ�
	XMStoreFloat4x4(&g_mesh.mtxTexture, mtxTexture);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetDeathEffect(XMFLOAT3 pos, float fSizeX, float fSizeY)
{
	int nIdxSetHitEffect = -1;


	DEATHEFFECT* pHE = g_deatheffect;
	for (int i = 0; i < MAX_DEATHEFFECT; ++i, ++pHE) {
		if (!pHE->bUse) {
			pHE->pos = pos;
			pHE->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			pHE->nCounter = 0;
			pHE->nPattern = 0;
			pHE->fAlpha = 1.0f;
			pHE->bUse = true;

			// ���_���W�̐ݒ�
			SetVertexDeathEffect(i, fSizeX, fSizeY);

			nIdxSetHitEffect = i;
			break;
		}
	}

	return nIdxSetHitEffect;
}

void SetDeathEffectCamera(CCamera* pCamera)
{
	g_pCamera = pCamera;

}