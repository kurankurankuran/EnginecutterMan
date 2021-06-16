//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "CLEAR.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
//#include "scene.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CLEAR_TEXTURENAME		L"data/texture/GAME_CLEAR.png"			// �e�N�X�`���t�@�C����
#define	START_TEXTURENAME		L"data/texture/space_game.png"	// �e�N�X�`���t�@�C����

#define	TITLE_POS_X		0								// �|���S���̕\���ʒu
#define	TITLE_POS_Y		0								// �|���S���̕\���ʒu
#define	TITLE_SIZE_X		SCREEN_WIDTH		// �|���S���̃T�C�Y
#define	TITLE_SIZE_Y		SCREEN_HEIGHT	// �|���S���̃T�C�Y

#define	START_POS_X			0					// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y			-170			// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	START_WIDTH		550				// �X�^�[�g�{�^���̕�
#define	START_HEIGHT		100				// �X�^�[�g�{�^���̍���

#define	COUNT_APPEAR		(60 * 0.5f /* �b */)	// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP		(60 * 1	 /* �b */)		// �X�^�[�g�{�^���_�ł̎���

//*****************************************************************************
// �萔
//*****************************************************************************
enum ETex
{
	TEX_TITLE = 0,
	TEX_START,

	MAX_TEX
};

static LPCWSTR g_pszTexFNameClear[] =
{
	CLEAR_TEXTURENAME,
	START_TEXTURENAME
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextureClear[MAX_TEX];	// �e�N�X�`��
static int			g_nCountAppear = 0;		//
static int			g_nCountDisp = 0;			//
static bool		g_bDispStart = false;		//
static bool		g_bStart = false;			// �X�^�[�g�{�^������
static int		g_stat;		// ���
static float    g_Alpha = 0.0f;
static bool		g_AStart = false;
static XINPUT_STATE* g_pXController;


static bool         g_bLoadMesh = false;
//=============================================================================
// �^�C�g����ʂ̏���������
//=============================================================================
HRESULT InitClear()
{
	HRESULT hr = S_OK;
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		// �e�N�X�`���ǂݍ���
		for (int i = 0; i < MAX_TEX; i++)
		{
			hr = CreateTextureFromFile(GetDevice(), g_pszTexFNameClear[i], &g_pTextureClear[i]);
			if (FAILED(hr))
			{
				MessageBoxW(GetMainWnd(), g_pszTexFNameClear[i],
					L"�e�N�X�`���ǂݍ��݃G���[", MB_ICONSTOP | MB_OK);
				return hr;
			}
			SaveTex(g_pTextureClear[i]);
		}
		g_bLoadMesh = true;
	}
	// �ϐ�������
	g_nCountAppear = COUNT_APPEAR;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_bStart = false;
	g_stat = 0;

	// BGM�Đ��J�n
	//CSound::Play(BGM_TITLE);

	return hr;
}

//=============================================================================
// �^�C�g����ʂ̏I������
//=============================================================================
void UninitClear()
{
	// �e�N�X�`���J��

#if MESH_RELEASE_ON
	for (int i = 0; i < MAX_TEX; ++i)
	{
		SAFE_RELEASE(g_pTextureClear[i]);
		if (g_stat)
		{
			g_stat = 0;
		}
	}
#endif
}

//=============================================================================
// �^�C�g����ʂ̍X�V����
//=============================================================================
void UpdateClear()
{
	
	if (g_AStart==true)
	{
		--g_nCountAppear;
		if (g_nCountAppear <= 0)
		{
			g_nCountDisp = (g_nCountDisp + 1) % 80;
			g_bDispStart = (g_nCountDisp <= INTERVAL_DISP);
		}

		if (g_bStart)
		{
			g_bDispStart = g_nCountAppear & 20;
		}


		g_bStart = true;
	}
		
	
}

//=============================================================================
// �^�C�g����ʂ̕`�揈��
//=============================================================================
void DrawClear()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	for (int i = 0; i < MAX_TEX; ++i)
	{
		if (!g_stat)
		{
			continue;
		}

		// �w�i�`��
		SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
		SetPolygonSize(TITLE_SIZE_X, TITLE_SIZE_Y);
		SetPolygonTexture(g_pTextureClear[TEX_TITLE]);
		SetPolygonAlpha(g_Alpha);
		DrawPolygon(pDeviceContext);

	}
	if (g_bDispStart&&g_AStart == true)
	{
		// �J�n�{�^���`��
		if (g_stat)
		{


			SetPolygonTexture(g_pTextureClear[TEX_START]);
			SetPolygonSize(START_WIDTH, START_HEIGHT);
			SetPolygonPos(START_POS_X, START_POS_Y);
			DrawPolygon(pDeviceContext);
		}
	}
	
	

	SetZWrite(true);
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����

}
//=============================================================================
// �R���g���[���[�I�u�W�F�N�g�擾
//=============================================================================
void SetClearControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}

void GetKey(void)
{
	for (int i = 0; i < MAX_TEX; ++i)
	{
		if (g_stat)
		{
			continue;
		}

		
		g_stat = 1;
		
	}
	g_Alpha += 0.004f;
	g_AStart = true;


}
