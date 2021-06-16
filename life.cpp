//=============================================================================
//
// ���C�t�\������ [Life.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <string.h>
#include "Life.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FRAME_TEXTURENAME	L"data/texture/frame_Life.png" // �ǂݍ��ރe�N�X�`���t�@�C����
#define LIFE_TEXTURENAME	L"data/texture/life0001.png" // �ǂݍ��ރe�N�X�`���t�@�C����

#define FRAME_SIZE_X		316							// �g�̃T�C�Y
#define FRAME_SIZE_Y		20							// �g�̃T�C�Y
#define FRAME_POS_X			(350.0f)	// �g�̕\���ʒu
#define FRAME_POS_Y			(-300.0f)	// �g�̕\���ʒu



#define LIFE_SIZE_X			10							// �����̃T�C�Y
#define LIFE_SIZE_Y			10							// �����̃T�C�Y
//#define LIFE_POS_X			(-SCREEN_WIDTH/2+FRAME_SIZE_X/2)	// �����̕\���ʒu
//#define LIFE_POS_Y			(SCREEN_HEIGHT/2-FRAME_SIZE_Y/2)				// �����̕\���ʒu


#define LIFE_POS_X			(200.0f)	// �����̕\���ʒu
#define LIFE_POS_Y			(-300.0f)				// �����̕\���ʒu
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static bool         g_bLoadMesh = false;
static ID3D11ShaderResourceView*	g_pTexture[2];	// �e�N�X�`���ւ̃|�C���^
//=============================================================================
// ���C�t�\���̏���������
//=============================================================================
HRESULT CLife::InitLife(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// �e�N�X�`���̓ǂݍ���
	LPCWSTR pszFName[] = { FRAME_TEXTURENAME, LIFE_TEXTURENAME };
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {

		for (int i = 0; i < _countof(g_pTexture); ++i)
		{
			hr = CreateTextureFromFile(pDevice, pszFName[i], &g_pTexture[i]);

			if (FAILED(hr))
			{
				MessageBoxW(NULL, pszFName[i], L"�e�N�X�`���ǂݍ��݃G���[", MB_OK);
				break;
			}

			SaveTex(g_pTexture[i]);
		}

		g_bLoadMesh = true;
	}

	return hr;
}

//=============================================================================
// ���C�t�\���̏I������
//=============================================================================
void CLife::UninitLife(void)
{
#if MESH_RELEASE_ON
	// �e�N�X�`�����
	for (int i = 0; i < _countof(g_pTexture); ++i) {
		SAFE_RELEASE(g_pTexture[i]);
	}
#endif
}

//=============================================================================
// ���C�t�\���̍X�V����
//=============================================================================
void CLife::UpdateLife(void)
{
	
	
}

//=============================================================================
// ���C�t�\������
//=============================================================================
void CLife::DrawLife(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	
	//SetBlendState(BS_ADDITIVE);
	// ��������������
	SetBlendState(BS_NONE);
	SetPolygonTexture(g_pTexture[1]);

	SetPolygonSize(LIFE_SIZE_X, LIFE_SIZE_Y);




	for (int i = 0; i < m_nLife; ++i) {
	
		
		SetPolygonPos(LIFE_POS_X + i * LIFE_SIZE_X, LIFE_POS_Y);

		
		DrawPolygon(pDeviceContext);

	}

	
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonColor(0.929412f, 0.152941f, 1.0f/*0.564706f*/);
	SetPolygonTexture(g_pTexture[0]);
	SetPolygonSize(FRAME_SIZE_X, FRAME_SIZE_Y);
	SetPolygonPos(FRAME_POS_X, FRAME_POS_Y);
	DrawPolygon(pDeviceContext);
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	
	
}

//=============================================================================
// ���C�t���Z����
//=============================================================================
void CLife::DelLife(int nValue)
{
	m_nLife -= nValue;

	if (m_nLife < 0)
		m_nLife = 0;
}

//=============================================================================
// ���C�t�擾����
//=============================================================================
int CLife::GetLife(void)
{
	return m_nLife;
}
