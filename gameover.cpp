//=============================================================================
//
// �Q�[���I�[�o�[��ʏ��� [gameover.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "gameover.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "scene.h"
#include "fade.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAMEOVEAR_TEXTURENAME		L"data/texture/failed.png"	// �e�N�X�`���t�@�C����
#define PUSH_BUTTON		L"data/texture/space_game.png"

#define GAMEOVEAR_POS_X		0					// �|���S���̕\���ʒu
#define GAMEOVEAR_POS_Y		0					// �|���S���̕\���ʒu
#define GAMEOVEAR_SIZE_X		SCREEN_WIDTH	// �|���S���̃T�C�Y
#define GAMEOVEAR_SIZE_Y		SCREEN_HEIGHT	// �|���S���̃T�C�Y
#define GAMEOVEAR_ANIM_SIZE_X		(5.0f)	// �|���S���̃T�C�Y
#define GAMEOVEAR_ANIM_SIZE_Y		(5.0f)	// �|���S���̃T�C�Y


#define GEMEOVER_TIMER		(60*5)		// �Q�[���I�[�o�[��ʂ���̈ڍs����

#define	START_POS_X			0			// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y			-230		// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	START_WIDTH			250			// �X�^�[�g�{�^���̕�
#define	START_HEIGHT		50			// �X�^�[�g�{�^���̍���

#define	COUNT_APPEAR		(60 * 0.5/* �b */)		// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP		(60 * 1	/* �b */)		// �X�^�[�g�{�^���_�ł̎���

#define MAX_GAMEOVER_ANIM	(128)	// �r���{�[�h�ő吔
//*****************************************************************************
// �萔
//*****************************************************************************
enum ETex
{
	TEX_GAMEOVEAR = 0,
	TEX_START,

	MAX_TEX
};

static LPCWSTR g_pszTexFName[] =
{
	GAMEOVEAR_TEXTURENAME,
	PUSH_BUTTON
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct GAMEOVER {
	XMFLOAT2	pos;		// �ʒu
	float		fSizeX;		// ��
	float		fSizeY;		// ����
	int			nCounter;	// �J�E���^�[
	int			nPattern;	// �p�^�[��No.
	bool		bUse;		// �g�p���Ă��邩�ǂ���
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
static GAMEOVER			g_GameOveranim[MAX_GAMEOVER_ANIM];

static bool         g_bLoadMesh = false;
//=============================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//=============================================================================
HRESULT InitGameOver()
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

	GAMEOVER* pGameOver = g_GameOveranim;
	for (int i = 0; i < MAX_GAMEOVER_ANIM; ++i, ++pGameOver)
	{
		pGameOver->pos = XMFLOAT2(0.0f, 0.0f);
		pGameOver->fSizeX = GAMEOVEAR_ANIM_SIZE_X;
		pGameOver->fSizeY = GAMEOVEAR_ANIM_SIZE_Y;
		pGameOver->nCounter = 0;
		pGameOver->nPattern = 0;
		pGameOver->bUse = false;
	}
	pGameOver = g_GameOveranim;
	pGameOver->bUse = true;

	// SE�Đ��J�n
	CSound::Play(SE_GAMEOVER);
	// BGM�Đ�
	/*CSound::Play(BGM_GAMEOVER);*/
	return hr;
}

//=============================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//=============================================================================
void UninitGameOver()
{
	//// BGM�I��
	//CSound::Stop(BGM_GAMEOVER);

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
void UpdateGameOver()
{
	// �Q�[���I�[�o�[�ŏo���A�j���[�V�����̏���
	GAMEOVER* pGameOver = g_GameOveranim;

	if (pGameOver->nPattern >= 27)
	{
		pGameOver->nPattern = 27;
}
	else {
		for (int i = 0; i < MAX_GAMEOVER_ANIM; ++i, ++pGameOver)
		{
			if (pGameOver->bUse)
			{
				++pGameOver->nCounter;
				// �Q�[���I�[�o�[�ŏo���A�j���[�V�����̑��x�ݒ�
				if (pGameOver->nCounter % 5 == 0)
				{
					pGameOver->nPattern++;

				}

				// ���_���W�̐ݒ�
				pGameOver->fSizeX += 0.5f;
				pGameOver->fSizeY += 0.5f;
			}
		}
	}
	// ���܂�

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
		CSound::Play(SE_KETTEI);		// SE�Đ�
		SetFadeOut(SCENE_TITLE);		// �^�C�g����ʂ�
		g_bStart = true;
	}
}
//=============================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//=============================================================================
void DrawGameOver()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);								// Z�o�b�t�@�X�V���Ȃ�

	// �w�i�`��
	SetPolygonPos(GAMEOVEAR_POS_X, GAMEOVEAR_POS_Y);
	SetPolygonSize(GAMEOVEAR_SIZE_X, GAMEOVEAR_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_GAMEOVEAR]);
	SetPolygonFrameSize(1.0f / 28, 1.0f / 1);
	int u = g_GameOveranim[0].nPattern % 28;
	int v = g_GameOveranim[0].nPattern / 28;
	SetPolygonUV((float)u / 28, (float)v / 1);

	DrawPolygon(pDeviceContext);

	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(1.0f, 1.0f);

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
void SetGameOverControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}