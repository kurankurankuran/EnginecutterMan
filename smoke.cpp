//=============================================================================
//
// �X���[�N���� [smoke.cpp]
// Author : ���{�@�ّ��Y
//
//=============================================================================
//�`�X�V�����`
// 12/08  �X���[�N�̔����ꏊ�����߂Ĉ�莞�ԏo������悤��
//=============================================================================
#include "smoke.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
//#include "camera.h"
//#include "light.h"
//#include "shadow.h"
//#include "model.h"
//#include "enemyParts.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SMOKE	L"data/texture/smoke.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	SMOKE_SIZE_X		(500.0f)					// �r���{�[�h�̕�
#define	SMOKE_SIZE_Y		(500.0f)					// �r���{�[�h�̍���
#define	VALUE_MOVE_SMOKE	(1.0f)						// �ړ����x

#define SMOKE_TIMER			(1)							// �X���[�N���������鎞��

//#define	DISP_SHADOW			// �e�̕\��
#undef DISP_SHADOW

#define MATERIAL_DIFFUSE	XMFLOAT4(1,1,1,1)
#define MATERIAL_AMBIENT	XMFLOAT4(1,1,1,1)
#define MATERIAL_SPECULAR	XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE	XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER		(0.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexSmoke(ID3D11Device* pDevice);
//void SetVertexSmoke(int nIdxSmoke, float fSizeX, float fSizeY);
//void SetColorSmoke(int nIdxSmoke, XMFLOAT4 col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH				g_meshSmoke;					// ���b�V�����
static MATERIAL			g_material;						// �}�e���A��
static bool             g_bLoadMesh = false;
//=============================================================================
// ����������
//=============================================================================
HRESULT CSmoke::InitSmoke(void)
{
	/*g_RotNormtx = XMMatrixIdentity();
	g_vSmokeRot = XMVectorSet(0.0f, 0.0f, -70.0f, 1.0f);*/

	m_posBase = XMFLOAT3(0.0f, 0.0f, 400.0f);
	m_nTimer = SMOKE_TIMER * 60;
	m_bGenerate = false;

	ID3D11Device* pDevice = GetDevice();

	

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		// �e�N�X�`���̓ǂݍ���
		CreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SMOKE,			// �t�@�C���̖��O
			&g_meshSmoke.pTexture);	// �ǂݍ��ރ������[
		/*g_bLoadMesh = true;*/

		// ���_���̍쐬
		MakeVertexSmoke(pDevice);

		SaveMesh(&g_meshSmoke);

		

		g_bLoadMesh = true;
	}
	

	XMStoreFloat4x4(&g_meshSmoke.mtxTexture, XMMatrixIdentity());

	// �}�e���A���̐ݒ�
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_meshSmoke.pMaterial = &g_material;

	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; ++nCntSmoke) {
		m_aSmoke[nCntSmoke].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_aSmoke[nCntSmoke].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_aSmoke[nCntSmoke].move = XMFLOAT3(1.0f, 1.0f, 1.0f);
		m_aSmoke[nCntSmoke].fSizeX = SMOKE_SIZE_X;
		m_aSmoke[nCntSmoke].fSizeY = SMOKE_SIZE_Y;
		m_aSmoke[nCntSmoke].nIdxShadow = -1;
		m_aSmoke[nCntSmoke].nLife = 10;
		m_aSmoke[nCntSmoke].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSmoke::UninitSmoke(void)
{
#ifdef DISP_SHADOW
	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; ++nCntSmoke) {
		if (m_aSmoke[nCntSmoke].bUse) {
			// �e�폜
			ReleaseShadow(m_aSmoke[nCntSmoke].nIdxShadow);
			m_aSmoke[nCntSmoke].nIdxShadow = -1;
			m_aSmoke[nCntSmoke].bUse = false;
		}
	}
#endif
#if MESH_RELEASE_ON
		// ���b�V���̊J��
		ReleaseMesh(&g_meshSmoke);
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void CSmoke::UpdateSmoke(void)
{
	//XMFLOAT3 rotCamera;

	// �J�����̉�]���擾
	//rotCamera = CCamera::Get()->GetAngle();

	
	if (!m_bPause) {


		for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; ++nCntSmoke) {
			if (m_aSmoke[nCntSmoke].bUse) {
				// �g�p��

				
				m_aSmoke[nCntSmoke].pos.x += m_aSmoke[nCntSmoke].move.x;
				m_aSmoke[nCntSmoke].pos.y += m_aSmoke[nCntSmoke].move.y;
				
				m_aSmoke[nCntSmoke].pos.z += m_aSmoke[nCntSmoke].move.z/2;
				if (m_aSmoke[nCntSmoke].pos.z <= m_aSmoke[nCntSmoke].fSizeY/* ����Z *// 2) {
					// ���n����
					/*m_aSmoke[nCntSmoke].pos.y = m_aSmoke[nCntSmoke].fSizeY / 2;
					m_aSmoke[nCntSmoke].move.y = -m_aSmoke[nCntSmoke].move.y * 0.75f;*/
					m_aSmoke[nCntSmoke].nLife = 0;
				}

				/*if (m_aSmoke[nCntSmoke].pos.y > 1)
				{
					m_aSmoke[nCntSmoke].nLife = 0;
				}*/
			
				/*m_aSmoke[nCntSmoke].move.x += (0.0f - m_aSmoke[nCntSmoke].move.x) * 0.015f;
				m_aSmoke[nCntSmoke].move.z -= 0.25f;
				m_aSmoke[nCntSmoke].move.y += (0.0f - m_aSmoke[nCntSmoke].move.y) * 0.015f;*/



#ifdef DISP_SHADOW
				float colA;
				if (m_aSmoke[nCntSmoke].nIdxShadow != -1) {
					// �e�g�p��
					//float colA;

					// �e�̈ʒu�ݒ�
					MoveShadow(m_aSmoke[nCntSmoke].nIdxShadow, XMFLOAT3(m_aSmoke[nCntSmoke].pos.x, 0.1f, m_aSmoke[nCntSmoke].pos.z));

					// �e�̃��l�ݒ�
					if (m_aSmoke[nCntSmoke].col.w > 0.0f) {
						colA = (300.0f - (m_aSmoke[nCntSmoke].pos.y - 9.0f)) / (300.0f / m_aSmoke[nCntSmoke].col.w);
						if (colA < 0.0f) {
							colA = 0.0f;
						}
					} else {
						colA = 0.0f;
					}

					// �e�̐F�̐ݒ�
					SetShadowColor(m_aSmoke[nCntSmoke].nIdxShadow, XMFLOAT4(0.15f, 0.15f, 0.15f, colA));
				}
#endif
				m_aSmoke[nCntSmoke].nLife--;
				if (m_aSmoke[nCntSmoke].nLife <= 0) {
					m_aSmoke[nCntSmoke].bUse = false;
					//ReleaseShadow(m_aSmoke[nCntSmoke].nIdxShadow);
					m_aSmoke[nCntSmoke].nIdxShadow = -1;
				} else {
					/*if (m_aSmoke[nCntSmoke].nLife <= 80) {
						m_aSmoke[nCntSmoke].col.x = 1.0f - (float)(80 - m_aSmoke[nCntSmoke].nLife) / 8.0f * 0.06f + 0.2f;
						m_aSmoke[nCntSmoke].col.y = 1.0f - (float)(80 - m_aSmoke[nCntSmoke].nLife) / 8.0f * 0.07f;
						m_aSmoke[nCntSmoke].col.z = 1.0f;
					}*/
					/*if (m_aSmoke[nCntSmoke].nLife <= 80) {
						m_aSmoke[nCntSmoke].col.x = 0.60f - (float)(80 - m_aSmoke[nCntSmoke].nLife) / 8.0f * 0.06f + 0.2f;
						m_aSmoke[nCntSmoke].col.y = 0.70f - (float)(80 - m_aSmoke[nCntSmoke].nLife) / 8.0f * 0.07f;
						m_aSmoke[nCntSmoke].col.z = 0.05f;
					}*/

					//if (m_aSmoke[nCntSmoke].nLife <= 10) {
					//	// ���l�ݒ�
					//	m_aSmoke[nCntSmoke].col.w -= 0.05f;
					//	if (m_aSmoke[nCntSmoke].col.w < 0.0f) {
					//		m_aSmoke[nCntSmoke].col.w = 1.0f;

					if (m_aSmoke[nCntSmoke].bUse) {
						// ���l�ݒ�
						m_aSmoke[nCntSmoke].col.w -= 0.05f;
						if (m_aSmoke[nCntSmoke].col.w < 0.0f) {
							m_aSmoke[nCntSmoke].col.w = 0.0f;
						}
					}

					// �F�̐ݒ�
					SetColorSmoke(nCntSmoke, m_aSmoke[nCntSmoke].col);
				}
			}
		}

		if (m_bGenerate) {
			--m_nTimer;

			if (m_nTimer > 0)
			{

				// �p�[�e�B�N������
		//		if((rand() % 2) == 0)
				{
					XMFLOAT3 pos;
					XMFLOAT3 move;
					float fAngle, fLength;
					int nLife;
					float fSize;
					float lenX;
					float lenY;


					/*XMFLOAT3 ModelPos = GetModelPos();
					pos = ModelPos;*/

					//XMFLOAT3 ModelPos = m_pEnemyParts->GetPosModel();
					pos = m_posBase/*ModelPos*/;

					// m_lenX,m_lenY���Ń����_���Ȓl�����AlenX,lenY�ɓ����
					m_lenX = (float)(rand() % 300);
					m_lenY = (float)(rand() % 100);

					lenX = m_lenX;
					lenY = m_lenY;

					pos.x += lenX;
					pos.y += lenY;

					fAngle = rand() % 360 - 180.0f;
					fLength = rand() % (int)(m_fWidthBase * 100) / 100.0f - m_fWidthBase;
					//			move.x = rand() % (int)(m_fWidthBase * 200) / 100.0f - m_fWidthBase;
					move.x = SinDeg(fAngle) * fLength;
					move.y = rand() % 300 / 100.0f + m_fHeightBase;
					//			move.z = rand() % (int)(m_fWidthBase * 200) / 100.0f - m_fWidthBase;
					move.z = CosDeg(fAngle) * fLength;

					//***************************************************************************
					//XMFLOAT3 vVecSmoke;
					//m_vSmokeRot = XMVector3TransformNormal(m_vSmokeRot, m_pEnemyParts->GetRotateNorMatrix());
					//if (!XMMatrixIsIdentity(m_pEnemyParts->GetRotateMatrix())) {
					//	m_vSmokeRot = XMVector3TransformNormal(m_vSmokeRot, m_pEnemyParts->GetRotateMatrix());
					//}
					//
					////m_RotNormtx = XMMatrixMultiply(m_RotNormtx, m_pPlayer->GetRotateNorMatrix());
					////vSmokeRot = XMVector3TransformNormal(vSmokeRot, m_RotNormtx);
					//
					//XMStoreFloat3(&vVecSmoke, m_vSmokeRot);



					//pos.x += vVecSmoke.x;
					//pos.y += vVecSmoke.y;
					//pos.z += vVecSmoke.z;

					//XMVECTOR vSmokeRot;
					//vSmokeRot = XMVectorSet(move.x, move.y, move.z, 1.0f);
					//vSmokeRot = XMVector3TransformNormal(vSmokeRot, m_pEnemyParts->GetTotateWorldMatrix());
					////vSmokeRot = XMVector3TransformNormal(vSmokeRot, m_pPlayer->GetRotateMatrix());
					///*vSmokeRot = XMVector3TransformNormal(vSmokeRot, XMMatrixRotationX(-3.14159265359f / 3.5f));
					//vSmokeRot = XMVector3TransformNormal(vSmokeRot, m_pPlayer->GetRotateNorMatrix());*/

					//XMStoreFloat3(&move, vSmokeRot);
					//***************************************************************************

					nLife = rand() % 40 + 10;

					fSize = (float)(rand() % 500 + 20);

					//pos.y += fSize / 2;

					// �r���{�[�h�̐ݒ�
					SetSmoke(pos, move/*f3ParMove*/, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife);
				}
			}
			else
			{
				m_nTimer = SMOKE_TIMER * 60;
				m_bGenerate = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSmoke::DrawSmoke(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// ���Z����
	SetBlendState(BS_ADDITIVE);

	// �r���[�}�g���b�N�X���擾
	/*XMFLOAT4X4& mtxView = CSmoke::Get()->GetViewMatrix();*/
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();/*m_pCamera->GetViewMatrix(); */

	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; ++nCntSmoke) {
		if (m_aSmoke[nCntSmoke].bUse) {
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&g_meshSmoke.mtxWorld, mtxWorld);

			g_meshSmoke.mtxWorld._11 = mtxView._11;
			g_meshSmoke.mtxWorld._12 = mtxView._21;
			g_meshSmoke.mtxWorld._13 = mtxView._31;
			g_meshSmoke.mtxWorld._21 = mtxView._12;
			g_meshSmoke.mtxWorld._22 = mtxView._22;
			g_meshSmoke.mtxWorld._23 = mtxView._32;
			g_meshSmoke.mtxWorld._31 = mtxView._13;
			g_meshSmoke.mtxWorld._32 = mtxView._23;
			g_meshSmoke.mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&g_meshSmoke.mtxWorld);

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(m_aSmoke[nCntSmoke].fSizeX, m_aSmoke[nCntSmoke].fSizeY, 5.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(m_aSmoke[nCntSmoke].pos.x, m_aSmoke[nCntSmoke].pos.y, m_aSmoke[nCntSmoke].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			//�@�v���C���[�̉�]�𔽉f
			//mtxWorld = XMMatrixMultiply(mtxWorld, m_pPlayer->GetRotateNorMatrix());
			//mtxWorld = XMMatrixMultiply(mtxWorld, m_pPlayer->GetTotateWorldMatrix());

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&g_meshSmoke.mtxWorld, mtxWorld);

			// �J���[�̐ݒ�
			g_material.Diffuse = m_aSmoke[nCntSmoke].col;

			// ���b�V���̕`��
			DrawMesh(pDeviceContext, &g_meshSmoke);
		}
	}

	 //������������
	SetBlendState(BS_NONE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexSmoke(ID3D11Device* pDevice)
{
	// �I�u�W�F�N�g�̒��_�z��𐶐�
	g_meshSmoke.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshSmoke.nNumVertex];

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
	g_meshSmoke.nNumIndex = 4;
	int* pIndexWk = new int[g_meshSmoke.nNumIndex];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_meshSmoke, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void CSmoke::SetVertexSmoke(int nIdxSmoke, float fSizeX, float fSizeY)
{
	if (nIdxSmoke >= 0 && nIdxSmoke < MAX_SMOKE) {
		m_aSmoke[nIdxSmoke].fSizeX = fSizeX;
		m_aSmoke[nIdxSmoke].fSizeY = fSizeY;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void CSmoke::SetColorSmoke(int nIdxSmoke, XMFLOAT4 col)
{
	if (nIdxSmoke >= 0 && nIdxSmoke < MAX_SMOKE) {
		m_aSmoke[nIdxSmoke].col = col;
	}
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
int CSmoke::SetSmoke(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxSmoke = -1;

	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; ++nCntSmoke) {
		if (!m_aSmoke[nCntSmoke].bUse) {
			m_aSmoke[nCntSmoke].pos = pos;
			m_aSmoke[nCntSmoke].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_aSmoke[nCntSmoke].move = move;
			m_aSmoke[nCntSmoke].nLife = nLife;
			m_aSmoke[nCntSmoke].bUse = true;

			// ���_���W�̐ݒ�
			SetVertexSmoke(nCntSmoke, fSizeX, fSizeY);

			// ���_�J���[�̐ݒ�
			SetColorSmoke(nCntSmoke, col);

			nIdxSmoke = nCntSmoke;

#ifdef DISP_SHADOW
			// �e�̐ݒ�
			m_aSmoke[nCntSmoke].nIdxShadow = CreateShadow(pos, fSizeX);
			if (m_aSmoke[nCntSmoke].nIdxShadow != -1) {
				SetShadowColor(m_aSmoke[nCntSmoke].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxSmoke;
}

////�p�[�e�B�N�������Ăяo��
//int SpawnSmoke(XMFLOAT3 pos, XMFLOAT3 dir)
//{
//	//XMFLOAT3 ModelPos = GetModelPos();
//	XMFLOAT3 ModelPos = m_pEnemyParts->GetPosModel();
//
//
//	
//	SMOKE* pSmoke = m_aSmoke;
//	for (int i = 0; i < MAX_SMOKE; ++i, ++pSmoke) {
//		if (pSmoke->bUse) continue;
//
//		pos = ModelPos;
//
//		pSmoke->pos = pos;
//
//		//pSmoke->nIdxShadow = CreateShadow(pos, SMOKE_SIZE_X);
//		pSmoke->bUse = 1;
//		return i;
//	}
//	return -1;
//}

//void SetSmokeEnemyParts(EnemyParts* pEnemyParts)
//{
//	m_pEnemyParts = pEnemyParts;
//}

//void CSmoke::SetCamera(CCamera* pCamera)
//{
//	m_pCamera = pCamera;
//}

//=============================================================================
// �X���[�N�Ăяo���֐�
//=============================================================================
void CSmoke::SetCallSmoke(XMFLOAT3 pos, float lenX,float lenY)
{
	m_posBase = pos;
	m_lenX = lenX;
	m_lenY = lenY;

	m_bGenerate = true;
}