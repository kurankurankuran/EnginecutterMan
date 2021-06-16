//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "particle.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "scene.h"
//#include "shadow.h"
//#include "model.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PARTICLE	L"data/texture/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	PARTICLE_SIZE_X		(10.0f)							// �r���{�[�h�̕�
#define	PARTICLE_SIZE_Y		(10.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_PARTICLE	(1.0f)							// �ړ����x

#define	MAX_PARTICLE		(100)							// �G�t�F�N�g�ő吔

//#define	DISP_SHADOW			// �e�̕\��
#undef DISP_SHADOW

#define MATERIAL_DIFFUSE	XMFLOAT4(1,1,1,1)
#define MATERIAL_AMBIENT	XMFLOAT4(1,1,1,1)
#define MATERIAL_SPECULAR	XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE	XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER		(0.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	XMFLOAT3 pos;			// �ʒu
	XMFLOAT3 rot;			// ��]
	XMFLOAT3 move;			// �ړ���
	XMFLOAT4 col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nIdxShadow;			// �eID
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
} PARTICLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticle(ID3D11Device* pDevice);
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY);
void SetColorParticle(int nIdxParticle, XMFLOAT4 col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_meshParticle;					// ���b�V�����
static MATERIAL			g_material;						// �}�e���A��

static PARTICLE			g_aParticle[MAX_PARTICLE];		// �p�[�e�B�N�����[�N
static XMFLOAT3			g_posBase;						// �r���{�[�h�����ʒu
static float			g_fWidthBase = 5.0f;			// ��̕�
static float			g_fHeightBase = 10.0f;			// ��̍���
static bool				g_bPause = false;				// �|�[�YON/OFF
static int              g_nScene;

static CPlayer*         g_pPlayer;
static CRushPlayer*     g_pRushPlayer;
static CCamera*         g_pCamera;

static XMMATRIX         g_RotNormtx;
XMVECTOR                g_vParticleRot;

static bool             g_bLoadMesh = false;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticle(void)
{
	g_nScene = GetScene();

	g_RotNormtx = XMMatrixIdentity();
	g_vParticleRot = XMVectorSet(0.0f, 0.0f, -70.0f, 1.0f);

	ID3D11Device* pDevice = GetDevice();

	//// ���_���̍쐬
	//MakeVertexParticle(pDevice);

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		// �e�N�X�`���̓ǂݍ���
		CreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PARTICLE,			// �t�@�C���̖��O
			&g_meshParticle.pTexture);	// �ǂݍ��ރ������[

		SaveMesh(&g_meshParticle);

		// ���_���̍쐬
		MakeVertexParticle(pDevice);

		g_bLoadMesh = true;
	}
	XMStoreFloat4x4(&g_meshParticle.mtxTexture, XMMatrixIdentity());

	// �}�e���A���̐ݒ�
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_meshParticle.pMaterial = &g_material;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		g_aParticle[nCntParticle].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fSizeX = PARTICLE_SIZE_X;
		g_aParticle[nCntParticle].fSizeY = PARTICLE_SIZE_Y;
		g_aParticle[nCntParticle].nIdxShadow = -1;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
#ifdef DISP_SHADOW
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (g_aParticle[nCntParticle].bUse) {
			// �e�폜
			ReleaseShadow(g_aParticle[nCntParticle].nIdxShadow);
			g_aParticle[nCntParticle].nIdxShadow = -1;
			g_aParticle[nCntParticle].bUse = false;
		}
	}
#endif
#if MESH_RELEASE_ON
		// ���b�V���̊J��
		ReleaseMesh(&g_meshParticle);
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{
	//XMFLOAT3 rotCamera;

	// �J�����̉�]���擾
	//rotCamera = CCamera::Get()->GetAngle();

	
	if (!g_bPause) {
		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
			if (g_aParticle[nCntParticle].bUse) {
				// �g�p��

				g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
				g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
				
				g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z/2;
				if (g_aParticle[nCntParticle].pos.z <= g_aParticle[nCntParticle].fSizeY/* ����Z *// 2) {
					// ���n����
					g_aParticle[nCntParticle].nLife = 0;
				}

				g_aParticle[nCntParticle].move.x += (0.0f - g_aParticle[nCntParticle].move.x) * 0.015f;
				g_aParticle[nCntParticle].move.z -= 0.25f;
				g_aParticle[nCntParticle].move.y += (0.0f - g_aParticle[nCntParticle].move.y) * 0.015f;

#ifdef DISP_SHADOW
				float colA;
				if (g_aParticle[nCntParticle].nIdxShadow != -1) {
					// �e�g�p��
					//float colA;

					// �e�̈ʒu�ݒ�
					MoveShadow(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT3(g_aParticle[nCntParticle].pos.x, 0.1f, g_aParticle[nCntParticle].pos.z));

					// �e�̃��l�ݒ�
					if (g_aParticle[nCntParticle].col.w > 0.0f) {
						colA = (300.0f - (g_aParticle[nCntParticle].pos.y - 9.0f)) / (300.0f / g_aParticle[nCntParticle].col.w);
						if (colA < 0.0f) {
							colA = 0.0f;
						}
					} else {
						colA = 0.0f;
					}

					// �e�̐F�̐ݒ�
					SetShadowColor(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT4(0.15f, 0.15f, 0.15f, colA));
				}
#endif
				g_aParticle[nCntParticle].nLife--;
				if (g_aParticle[nCntParticle].nLife <= 0) {
					g_aParticle[nCntParticle].bUse = false;
					//ReleaseShadow(g_aParticle[nCntParticle].nIdxShadow);
					g_aParticle[nCntParticle].nIdxShadow = -1;
				} else {
					if (g_aParticle[nCntParticle].nLife <= 80) {
						g_aParticle[nCntParticle].col.x = 0.60f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.06f + 0.2f;
						g_aParticle[nCntParticle].col.y = 0.70f - (float)(80 - g_aParticle[nCntParticle].nLife) / 8.0f * 0.07f;
						g_aParticle[nCntParticle].col.z = 0.05f;
					}

					if (g_aParticle[nCntParticle].nLife <= 20) {
						// ���l�ݒ�
						g_aParticle[nCntParticle].col.w -= 0.05f;
						if (g_aParticle[nCntParticle].col.w < 0.0f) {
							g_aParticle[nCntParticle].col.w = 0.0f;
						}
					}

					// �F�̐ݒ�
					SetColorParticle(nCntParticle, g_aParticle[nCntParticle].col);
				}
			}
		}

		// �p�[�e�B�N������
//		if((rand() % 2) == 0)
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;



			/*XMFLOAT3 ModelPos = GetModelPos();
			pos = ModelPos;*/

			/*XMFLOAT3 ModelPos = g_pPlayer->GetPosModel();
			pos = ModelPos;*/



			fAngle = rand() % 360 - 180.0f;
			fLength = rand() % (int)(g_fWidthBase * 100) / 100.0f - g_fWidthBase;
			//			move.x = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;
			move.x = SinDeg(fAngle) * fLength;
			move.y = rand() % 300 / 100.0f + g_fHeightBase;
			//			move.z = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;
			move.z = CosDeg(fAngle) * fLength;

			XMVECTOR vParticleRot;
			switch (g_nScene)
			{
			case SCENE_GAME:
				pos.x = g_pPlayer->GetPosCut().x;
				pos.y = g_pPlayer->GetPosCut().y;
				pos.z = g_pPlayer->GetPosCut().z;

				vParticleRot = XMVectorSet(move.x, move.y, move.z, 1.0f);
				vParticleRot = XMVector3TransformNormal(vParticleRot, XMLoadFloat4x4(&g_pPlayer->GetTotateWorldMatrix()));
				
				XMStoreFloat3(&move, vParticleRot);
				//***************************************************************************

				nLife = rand() % 100 + 150;

				fSize = (float)(rand() % 30 + 20);

				if (g_pPlayer->GetBreakflg()) {
					// �r���{�[�h�̐ݒ�
					SetParticle(pos, move, XMFLOAT4(0.8f, 0.7f, 0.2f, 0.85f), fSize, fSize, nLife);
				}
				break;

			case SCENE_RUSH:
				pos.x = g_pRushPlayer->GetPosCut().x;
				pos.y = g_pRushPlayer->GetPosCut().y;
				pos.z = g_pRushPlayer->GetPosCut().z;

				vParticleRot = XMVectorSet(move.x, move.y, move.z, 1.0f);
				vParticleRot = XMVector3TransformNormal(vParticleRot, XMLoadFloat4x4(&g_pRushPlayer->GetTotateWorldMatrix()));

				XMStoreFloat3(&move, vParticleRot);
				//***************************************************************************

				nLife = rand() % 100 + 150;

				fSize = (float)(rand() % 30 + 20);

				if (g_pRushPlayer->GetBreakflg()) {
					// �r���{�[�h�̐ݒ�
					SetParticle(pos, move, XMFLOAT4(0.8f, 0.7f, 0.2f, 0.85f), fSize, fSize, nLife);
				}
				break;
			}

			
}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	// �����ŃI�t�ɂ���
	CLight::Get()->SetDisable();

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// ���Z����
	SetBlendState(BS_ADDITIVE);

	// �r���[�}�g���b�N�X���擾
	/*XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();*/
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix(); 

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (g_aParticle[nCntParticle].bUse) {
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&g_meshParticle.mtxWorld, mtxWorld);

			g_meshParticle.mtxWorld._11 = mtxView._11;
			g_meshParticle.mtxWorld._12 = mtxView._21;
			g_meshParticle.mtxWorld._13 = mtxView._31;
			g_meshParticle.mtxWorld._21 = mtxView._12;
			g_meshParticle.mtxWorld._22 = mtxView._22;
			g_meshParticle.mtxWorld._23 = mtxView._32;
			g_meshParticle.mtxWorld._31 = mtxView._13;
			g_meshParticle.mtxWorld._32 = mtxView._23;
			g_meshParticle.mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&g_meshParticle.mtxWorld);

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(g_aParticle[nCntParticle].fSizeX/3, g_aParticle[nCntParticle].fSizeY/3, 5.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			////�@�v���C���[�̉�]�𔽉f
			//mtxWorld = XMMatrixMultiply(mtxWorld, g_pPlayer->GetRotateNorMatrix());
			//mtxWorld = XMMatrixMultiply(mtxWorld, g_pPlayer->GetTotateWorldMatrix());

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&g_meshParticle.mtxWorld, mtxWorld);

			// �J���[�̐ݒ�
			g_material.Diffuse = g_aParticle[nCntParticle].col;

			// ���b�V���̕`��
			DrawMesh(pDeviceContext, &g_meshParticle);
		}
	}

	// ������������
	SetBlendState(BS_NONE);

	// �����ŃI���ɂ���
	CLight::Get()->SetEnable();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticle(ID3D11Device* pDevice)
{
	// �I�u�W�F�N�g�̒��_�z��𐶐�
	g_meshParticle.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshParticle.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	// ���_���W�̐ݒ�
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[1].vtx = XMFLOAT3( 1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[2].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2, -1.0f / 2, 0.0f);

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
	pVtx[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 1.0f);

	// �C���f�b�N�X�z��𐶐�
	g_meshParticle.nNumIndex = 4;
	int* pIndexWk = new int[g_meshParticle.nNumIndex];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_meshParticle, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY)
{
	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_aParticle[nIdxParticle].fSizeX = fSizeX;
		g_aParticle[nIdxParticle].fSizeY = fSizeY;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorParticle(int nIdxParticle, XMFLOAT4 col)
{
	if (nIdxParticle >= 0 && nIdxParticle < MAX_PARTICLE) {
		g_aParticle[nIdxParticle].col = col;
	}
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
int SetParticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; ++nCntParticle) {
		if (!g_aParticle[nCntParticle].bUse) {
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;

			// ���_���W�̐ݒ�
			SetVertexParticle(nCntParticle, fSizeX, fSizeY);

			// ���_�J���[�̐ݒ�
			SetColorParticle(nCntParticle, col);

			nIdxParticle = nCntParticle;

#ifdef DISP_SHADOW
			// �e�̐ݒ�
			g_aParticle[nCntParticle].nIdxShadow = CreateShadow(pos, fSizeX);
			if (g_aParticle[nCntParticle].nIdxShadow != -1) {
				SetShadowColor(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxParticle;
}

//�p�[�e�B�N�������Ăяo��
int SpawnParticle(XMFLOAT3 pos, XMFLOAT3 dir)
{
	//XMFLOAT3 ModelPos = GetModelPos();
	XMFLOAT3 ModelPos;

	switch (g_nScene) {
	case SCENE_GAME:
		ModelPos = g_pPlayer->GetPosModel();
		break;

	case SCENE_RUSH:
		ModelPos = g_pRushPlayer->GetPosModel();
		break;
	}
	
	PARTICLE* pParticle = g_aParticle;
	for (int i = 0; i < MAX_PARTICLE; ++i, ++pParticle) {
		if (pParticle->bUse) continue;

		pos = ModelPos;

		pParticle->pos = pos;

		//pParticle->nIdxShadow = CreateShadow(pos, PARTICLE_SIZE_X);
		pParticle->bUse = 1;
		return i;
	}
	return -1;
}

void SetParticlePlayer(CPlayer* pPlayer)
{
	g_pPlayer = pPlayer;
}

void SetParticleRushPlayer(CRushPlayer* pPlayer)
{
	g_pRushPlayer = pPlayer;
}

void SetParticleCamera(CCamera* pCamera)
{
	g_pCamera = pCamera;
}