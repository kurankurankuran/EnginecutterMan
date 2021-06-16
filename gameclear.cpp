//=============================================================================
//
// �Q�[���N���A��ʏ��� [gameclear.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "gameclear.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "scene.h"
#include "fade.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define GAMECLEAR_TEXTURENAME		L"data/texture/gameclear000.png"	// �e�N�X�`���t�@�C����
#define GAMECLEAR_TEXTURENAME		L"data/texture/gameclear.jpg"	// �e�N�X�`���t�@�C����
#define PUSH_BUTTON		L"data/texture/space_game.png"

#define GAMECLEAR_POS_X			0								// �|���S���̕\���ʒu
#define GAMECLEAR_POS_Y			0								// �|���S���̕\���ʒu
#define GAMECLEAR_SIZE_X		SCREEN_WIDTH		// �|���S���̃T�C�Y
#define GAMECLEAR_SIZE_Y		SCREEN_HEIGHT	// �|���S���̃T�C�Y

#define GEMECLEAR_TIMER		(60*5)						// �Q�[���I�[�o�[��ʂ���̈ڍs����

#define	START_POS_X			0				// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y			-230		// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	START_WIDTH		250			// �X�^�[�g�{�^���̕�
#define	START_HEIGHT		50			// �X�^�[�g�{�^���̍���

#define	COUNT_APPEAR		(60 * 0.5/* �b */)		// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP		(60 * 1		/* �b */)			// �X�^�[�g�{�^���_�ł̎���

//*****************************************************************************
// �萔
//*****************************************************************************
enum ETex
{
	TEX_GAMECLEAR = 0,
	TEX_START,

	MAX_TEX
};

static LPCWSTR g_pszTexFName[] =
{
	GAMECLEAR_TEXTURENAME,
	PUSH_BUTTON
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[MAX_TEX];	// �e�N�X�`��
static int			g_nCountAppear = 0;		//
static int			g_nCountDisp = 0;			//
static bool		g_bDispStart = false;		//
static bool		g_bStart = false;			// �X�^�[�g�{�^������
static XINPUT_STATE*	g_pXController;


static bool         g_bLoadMesh = false;
//=============================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//=============================================================================
HRESULT InitGameClear()
{
	HRESULT hr = S_OK;
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		// �e�N�X�`���ǂݍ���
		for (int i = 0; i < MAX_TEX; i++)
		{
			hr = CreateTextureFromFile(GetDevice(), g_pszTexFName[i], &g_pTexture[i]);
			if (FAILED(hr))
			{
				MessageBoxW(GetMainWnd(), g_pszTexFName[i],
					L"�e�N�X�`���ǂݍ��݃G���[", MB_ICONSTOP | MB_OK);
				return hr;
			}

			SaveTex(g_pTexture[i]);
		}
		g_bLoadMesh = true;
	}
	// �ϐ�������
	g_nCountAppear = COUNT_APPEAR;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_bStart = false;

	// BGM�Đ�
	CSound::Play(BGM_GAMECLEAR);

	return hr;
}

//=============================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//=============================================================================
void UninitGameClear()
{
	// BGM�I��
	CSound::Stop(BGM_GAMECLEAR);

#if MESH_RELEASE_ON
	// �e�N�X�`���J��
	for (int i = 0; i < MAX_TEX; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
#endif
}

//=============================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//=============================================================================
void UpdateGameClear()
{
	--g_nCountAppear;
	if (g_nCountAppear <= 0)
	{
		g_nCountDisp = (g_nCountDisp + 1) % 80;
		g_bDispStart = (g_nCountDisp <= INTERVAL_DISP);
	}

	if (g_bStart)
	{
		g_bDispStart = g_nCountAppear & 2;
	}

	// [Enter]�܂��̓X�y�[�X�L�[����������`���[�g���A����
	if (!g_bStart && (/*GetKeyRelease(VK_RETURN) || */GetKeyRelease(VK_SPACE))
#if SETCONTROLLER
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_B
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y
#endif
		)
	{
		//CSound::Play(SE_KETTEI);
		SetFadeOut(SCENE_TITLE);		// �^�C�g����ʂ�
		g_bStart = true;
	}
}

//=============================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//=============================================================================
void DrawGameClear()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);

	// �w�i�`��
	SetPolygonPos(GAMECLEAR_POS_X, GAMECLEAR_POS_Y);
	SetPolygonSize(GAMECLEAR_SIZE_X, GAMECLEAR_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_GAMECLEAR]);
	DrawPolygon(pDeviceContext);

	// �J�n�{�^���`��
	if (g_bDispStart)
	{
		SetPolygonTexture(g_pTexture[TEX_START]);
		SetPolygonSize(START_WIDTH, START_HEIGHT);
		SetPolygonPos(START_POS_X, START_POS_Y);
		DrawPolygon(pDeviceContext);
	}

	SetZWrite(true);							// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);		// �A���t�@�u�����h����
}

//=============================================================================
// �R���g���[���[�I�u�W�F�N�g�擾
//=============================================================================
void SetGameClearControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}