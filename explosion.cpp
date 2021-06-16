//=============================================================================
//
// �������� [explosion.cpp]
// Author : HIROHIKO HAMAYA
//
// ----------------------------------------------------------------------------
// 2020/12/23 DrawExplosion()�̍ŏ��ƍŌ�ɃI�t�ɂ�����I���ɂ��鏈����ǉ��B�O��
//
//=============================================================================
#include "explosion.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EXPLOSION		L"data/texture/EX2.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EXPLOSION_SIZE_X		(5.0f)							// �r���{�[�h�̕�
#define	EXPLOSION_SIZE_Y		(5.0f)							// �r���{�[�h�̍���

#define	MAX_EXPLOSION			(128)							// �r���{�[�h�ő吔

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(0.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct EXPLOSION {
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
HRESULT MakeVertexExplosion(ID3D11Device* pDevice);
void SetVertexExplosion(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorExplosion(int nIdxExplosion, XMFLOAT4 col);
void SetTextureExplosion(int nIdxExplosion, int nPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_meshExplosion;
static MATERIAL			g_material;
static EXPLOSION		g_explosion[MAX_EXPLOSION];	// �������[�N
static CPlayer*         g_pPlayer;
static CCamera*         g_pCamera;

static bool             g_bLoadMesh = false;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitExplosion(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	//ZeroMemory(&g_meshExplosion, sizeof(MESH));

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {

		//ZeroMemory(&g_meshExplosion, sizeof(MESH));

		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EXPLOSION,	// �t�@�C���̖��O
			&g_meshExplosion.pTexture);	// �ǂݍ��ރ������[

		SaveMesh(&g_meshExplosion);

		// ���_���̍쐬
		MakeVertexExplosion(pDevice);

		g_bLoadMesh = true;
	}
	XMStoreFloat4x4(&g_meshExplosion.mtxTexture, XMMatrixIdentity());

	

	// �}�e���A���̐ݒ�
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_meshExplosion.pMaterial = &g_material;

	EXPLOSION* pExp = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExp) {
		pExp->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pExp->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pExp->fSizeX = EXPLOSION_SIZE_X;
		pExp->fSizeY = EXPLOSION_SIZE_Y;
		pExp->nCounter = 0;
		pExp->nPattern = 0;
		pExp->fAlpha = 1.0f;
		pExp->bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitExplosion(void)
{
#if MESH_RELEASE_ON
		// ���b�V���̊J��
		ReleaseMesh(&g_meshExplosion);
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateExplosion(void)
{
	EXPLOSION* pExp = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExp) 
{
		if (pExp->bUse) 
		{
			++pExp->nCounter;
			//pExp->nPattern++;

			
				if (pExp->nCounter % 2 == 0)
				{
					pExp->nPattern++;


				}
			
			
			if (pExp->nPattern >= 8)
			{
				pExp->bUse = false;
				continue;
			}


			// ���_���W�̐ݒ�
			pExp->fSizeX += 0.5f;
			pExp->fSizeY += 0.5f;

			SetVertexExplosion(i, pExp->fSizeX, pExp->fSizeY);

			if (pExp->nPattern >= 2)
			{
				// ���_�J���[�̐ݒ�
				pExp->fAlpha -= 0.02f;

				/*if (pExp->nCounter % 10 == 0)
				{
					pExp->nPattern++;

				}*/

				if (pExp->fAlpha < 0.0f) 
				{
					pExp->fAlpha = 0.0f;
				}
				SetColorExplosion(i, XMFLOAT4(1.0f, 1.0f, 1.0f, pExp->fAlpha));
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawExplosion(void)
{
	// �����ŃI�t�ɂ���
	CLight::Get()->SetDisable();

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// �r���[�}�g���b�N�X���擾
	//XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();
	XMFLOAT4X4& mtxView = g_pCamera->GetViewMatrix();
	// ���Z����
	SetBlendState(/*BS_SUBTRACTION*/BS_ADDITIVE);

	EXPLOSION* pExp = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExp) {
		if (pExp->bUse) {
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&pExp->mtxWorld, mtxWorld);

			pExp->mtxWorld._11 = mtxView._11;
			pExp->mtxWorld._12 = mtxView._21;
			pExp->mtxWorld._13 = mtxView._31;
			pExp->mtxWorld._21 = mtxView._12;
			pExp->mtxWorld._22 = mtxView._22;
			pExp->mtxWorld._23 = mtxView._32;
			pExp->mtxWorld._31 = mtxView._13;
			pExp->mtxWorld._32 = mtxView._23;
			pExp->mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&pExp->mtxWorld);

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(pExp->fSizeX, pExp->fSizeY, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(pExp->pos.x, pExp->pos.y, pExp->pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&pExp->mtxWorld, mtxWorld);
			g_meshExplosion.mtxWorld = pExp->mtxWorld;

			// �e�N�X�`�����W�̐ݒ�
			SetTextureExplosion(i, pExp->nPattern);

			// �J���[�̐ݒ�
			g_material.Diffuse = pExp->col;

			// ���b�V���̕`��
			DrawMesh(pDeviceContext, &g_meshExplosion);
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
HRESULT MakeVertexExplosion(ID3D11Device* pDevice)
{
	// �I�u�W�F�N�g�̒��_�z��𐶐�
	g_meshExplosion.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshExplosion.nNumVertex];

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
	g_meshExplosion.nNumIndex = 4;
	int* pIndexWk = new int[g_meshExplosion.nNumIndex];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_meshExplosion, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexExplosion(int nIdxExplosion, float fSizeX, float fSizeY)
{
	if (nIdxExplosion >= 0 && nIdxExplosion < MAX_EXPLOSION) {
		EXPLOSION* pExp = &g_explosion[nIdxExplosion];
		pExp->fSizeX = fSizeX;
		pExp->fSizeY = fSizeY;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorExplosion(int nIdxExplosion, XMFLOAT4 col)
{
	if (nIdxExplosion >= 0 && nIdxExplosion < MAX_EXPLOSION) {
		g_explosion[nIdxExplosion].col = col;
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureExplosion(int nIdxExplosion, int nPattern)
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
	XMStoreFloat4x4(&g_meshExplosion.mtxTexture, mtxTexture);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetExplosion(XMFLOAT3 pos, float fSizeX, float fSizeY)
{
	int nIdxExplosion = -1;


	EXPLOSION* pExp = g_explosion;
	for (int i = 0; i < MAX_EXPLOSION; ++i, ++pExp) {
		if (!pExp->bUse) {
			pExp->pos = pos;
			pExp->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			pExp->nCounter = 0;
			pExp->nPattern = 0;
			pExp->fAlpha = 1.0f;
			pExp->bUse = true;

			// ���_���W�̐ݒ�
			SetVertexExplosion(i, fSizeX, fSizeY);

			nIdxExplosion = i;
			break;
		}
	}

	return nIdxExplosion;
}

//void SetExplosionPlayer(CPlayer* pPlayer)
//{
//	g_pPlayer = pPlayer;
//
//
//
//}

void SetExplosionCamera(CCamera* pCamera)
{


	g_pCamera = pCamera;


}