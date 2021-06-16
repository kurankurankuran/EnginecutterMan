//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "scene.h"
#include "fade.h"
#include "AssimpModel.h"
#include "model.h"
#include "mesh.h"
#include "Sound.h"
#include <stdlib.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TITLE_TEXTURENAME		L"data/texture/�^�C�g��1.png"	// �e�N�X�`���t�@�C����
#define	START_TEXTURENAME		L"data/texture/space_game.png"	// �e�N�X�`���t�@�C����
#define	LINE_TEXTURENAME		L"data/texture/title3.png"		// �e�N�X�`���t�@�C����(�w�i)
//#define TITLE_LOGO				L"data/texture/titlelogo.png"	// �e�N�X�`���t�@�C����(���^�C�g�����S)
#define TITLE_WHITELOGO			L"data/texture/titlelogowhite.png"	// �e�N�X�`���t�@�C����(���^�C�g�����S)
#define TITLE_EFFECT			L"data/texture/titleeffect3.png"

#define	TITLE_POS_X		0							// �|���S���̕\���ʒu
#define	TITLE_POS_Y		0							// �|���S���̕\���ʒu
#define	TITLE_LOGO_POS_X		25					// �^�C�g�����S�̕\���ʒu
#define	TITLE_LOGO_POS_Y		70					// �^�C�g�����S�̕\���ʒu
#define	TITLE_SIZE_X		SCREEN_WIDTH			// �|���S���̃T�C�Y
#define	TITLE_SIZE_Y		SCREEN_HEIGHT			// �|���S���̃T�C�Y
#define	TITLE_LOGO_SIZE_X	900						// �^�C�g�����S�̃T�C�Y
#define	TITLE_LOGO_SIZE_Y	200						// �^�C�g�����S�̃T�C�Y
#define TITLE_ANIM_SIZEX	(5.0f)
#define TITLE_ANIM_SIZEY	(5.0f)

#define	START_POS_X			0						// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y			-320					// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	START_WIDTH			250						// �X�^�[�g�{�^���̕�
#define	START_HEIGHT		50						// �X�^�[�g�{�^���̍���

#define	COUNT_APPEAR		(60 * 0.5f /* �b */)		// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP		(60 * 1		/* �b */)	// �X�^�[�g�{�^���_�ł̎���

#define	NUM_TITLE_MENU				(3)				// �^�C�g����ʂł̑I��
#define	TITLE_MENU_WIDTH			(180.0f)		// �^�C�g�����j���[��
#define	TITLE_MENU_HEIGHT			(60.0f)			// �^�C�g�����j���[����
#define	TITLE_MENU_POS_X			(0.0f)			// �^�C�g�����j���[�ʒu(X���W)
#define	TITLE_MENU_POS_Y			(-100.0f)		// �^�C�g�����j���[�ʒu(Y���W)
#define	TITLE_MENU_INTERVAL	(75.0f)			// �^�C�g�����j���[�Ԋu

#define MAX_TITLE_ANIM	(128)	// �r���{�[�h�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TITLE {
	//XMFLOAT4X4	mtxWorld;	// ���[���h�}�g���b�N�X
	XMFLOAT2	pos;		// �ʒu
	//XMFLOAT4	col;		// �F
	float		fSizeX;		// ��
	float		fSizeY;		// ����
	int			nCounter;	// �J�E���^�[
	int			nPattern;	// �p�^�[��No.
	//float		fAlpha;		// �A���t�@�l
	bool		bUse;		// �g�p���Ă��邩�ǂ���
};

//*****************************************************************************
// �萔
//*****************************************************************************
enum ETex
{
	TEX_TITLE = 0,
	TEX_START,
	TEX_ANIMTITLE,
	TEX_LOGO,
	TEX_TITLEEFFECT,


	MAX_TEX
};

static LPCWSTR g_pszTexFName[] =
{
	TITLE_TEXTURENAME,
	START_TEXTURENAME,
	LINE_TEXTURENAME,
	//TITLE_LOGO,
	TITLE_WHITELOGO,
	TITLE_EFFECT
};

static LPCWSTR c_aFileNamePauseMenu[NUM_TITLE_MENU] =
{
	L"data/texture/title_game_start.png",		// �Q�[���X�^�[�g
	L"data/texture/title_tutorial.png",			// �`���[�g���A��
	L"data/texture/quit_game.png",				// �Q�[���I��
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[MAX_TEX];	// �e�N�X�`��
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// �e�N�X�`���ւ̃|�C���^

static TITLE_MENU g_nSelectMenu = GAME_START;		// �����I�����Ă��

static MESH				g_mesh;
static MATERIAL			g_material;
static TITLE			g_titleanim[MAX_TITLE_ANIM];	// �������[�N
static TITLE			g_titleeffect[MAX_TITLE_ANIM];

static int		g_nCountAppear = 0;
static int			g_nCountDisp = 0;
static bool			g_bDispStart = false;
static bool			g_bStart = false;			// �X�^�[�g�{�^������

//CAssimpModel		g_modelTitle;			// ���f��
static XINPUT_STATE*	g_pXController;

static float g_fCurve = 1.0f;
static bool         g_bInputUpNow;
static bool         g_bInputUpOld;
static bool         g_bInputDownNow;
static bool         g_bInputDownOld;

static bool			  g_bBGM;

static bool         g_bLoadMesh = false;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetVertexTitleAnim(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorTitleAnim(int nIdxHitEffect, XMFLOAT4 col);

//=============================================================================
// �^�C�g����ʂ̏���������
//=============================================================================
HRESULT InitTitle()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	g_bInputUpNow = false;
	g_bInputUpOld = false;
	g_bInputDownNow = false;
	g_bInputDownOld = false;
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
	
		// �^�C�g���I���̃e�N�X�`���ǂݍ���
		for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_TITLE_MENU; nCntPauseMenu++)
		{

			hr = CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
				c_aFileNamePauseMenu[nCntPauseMenu],		// �t�@�C���̖��O
				&g_pTextures[nCntPauseMenu]);				// �ǂݍ��ރ������[

			SaveTex(g_pTextures[nCntPauseMenu]);
		}
		g_bLoadMesh = true;
	}

	// �ϐ�������
	g_nCountAppear = COUNT_APPEAR;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_bStart = false;

	g_fCurve = 0.0f;

	TITLE* pTA = g_titleanim;
	for (int i = 0; i < MAX_TITLE_ANIM; ++i, ++pTA)
	{
		//pTA->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pTA->pos = XMFLOAT2(0.0f, 0.0f);
		//pTA->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pTA->fSizeX = TITLE_ANIM_SIZEX;
		pTA->fSizeY = TITLE_ANIM_SIZEY;
		pTA->nCounter = 0;
		pTA->nPattern = 0;
		//pTA->fAlpha = 1.0f;
		pTA->bUse = false;
	}
	pTA = g_titleanim;
	pTA->bUse = true;

	TITLE* pTE = g_titleeffect;
	for (int i = 0; i < MAX_TITLE_ANIM; ++i, ++pTE)
	{
		pTE->pos = XMFLOAT2(0.0f, 0.0f);
		pTE->fSizeX = TITLE_ANIM_SIZEX;
		pTE->fSizeY = TITLE_ANIM_SIZEY;
		pTE->nCounter = 0;
		pTE->nPattern = 0;
		pTE->bUse = false;
	}
	pTE = g_titleeffect;
	pTE->bUse = true;
	// BGM�Đ��J�n
	CSound::Play(BGM_TITLE);
	g_bBGM = true;

	return hr;
}

//=============================================================================
// �^�C�g����ʂ̏I������
//=============================================================================
void UninitTitle()
{
#if MESH_RELEASE_ON
		// �e�N�X�`���J��
		for (int i = 0; i < MAX_TEX; ++i)
		{
			SAFE_RELEASE(g_pTexture[i]);
		}

		// �e�N�X�`���̊J��
		for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_TITLE_MENU; nCntPauseMenu++)
		{
			SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
		}
#endif

	// BGM��~
	if (g_bBGM) {
		CSound::Stop(BGM_TITLE);
		g_bBGM = false;
	}
}

//=============================================================================
// �^�C�g����ʂ̍X�V����
//=============================================================================
void UpdateTitle()
{
	// �^�C�g���ŏo���A�j���[�V�����̏���
	TITLE* pTA = g_titleanim;
	for (int i = 0; i < MAX_TITLE_ANIM; ++i, ++pTA)
	{
		if (pTA->bUse)
		{
			++pTA->nCounter;
			// �^�C�g���ŏo���A�j���[�V�����̑��x�ݒ�
			if (pTA->nCounter % 3 == 0)
			{
				pTA->nPattern++;
			}

			// ���_���W�̐ݒ�
			pTA->fSizeX += 0.5f;
			pTA->fSizeY += 0.5f;

			
		}
	}
	// ���܂�
	// �^�C�g���G�t�F�N�g�A�j���[�V��������
	TITLE* pTE = g_titleeffect;
	for (int i = 0; i < MAX_TITLE_ANIM; ++i, ++pTE)
	{
		if (pTE->bUse)
		{
			++pTE->nCounter;
			// �^�C�g���G�t�F�N�g�̑��x�ݒ�
			if (pTE->nCounter % 3 == 0)
			{
				pTE->nPattern++;
			}

			// ���_���W�̐ݒ�
			pTE->fSizeX += 0.5f;
			pTE->fSizeY += 0.5f;
		}
	}
	// ���܂�
	g_bInputUpOld = g_bInputUpNow;
	g_bInputDownOld = g_bInputDownNow;

	--g_nCountAppear;
	if (g_nCountAppear <= 0)
	{
		g_nCountDisp = (g_nCountDisp + 1) % 80;
		g_bDispStart = (g_nCountDisp <= INTERVAL_DISP);
	}

	if (g_bStart == true)
	{
		g_bDispStart = g_nCountAppear & 2;
	}

#if SETCONTROLLER
	if (g_pXController->Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_UP
		) {
		g_bInputUpNow = true;
	}
	else
	{
		g_bInputUpNow = false;
	}

	if (g_pXController->Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_DOWN
		) {
		g_bInputDownNow = true;
	}
	else
	{
		g_bInputDownNow = false;
	}
#endif




	// �I���J�[�\�����킹��
	// �J�[�\������Ɉړ�
	if (GetKeyTrigger(VK_W) || GetKeyTrigger(VK_UP)
		|| g_bInputUpNow && !g_bInputUpOld
		)
	{
		CSound::Play(SE_BUTTON); // SE�Đ��J�n
		g_nSelectMenu = (TITLE_MENU)((g_nSelectMenu + NUM_TITLE_MENU - 1) % NUM_TITLE_MENU);
		SetTitleMenu();
	}
	// �J�[�\�������ړ�
	else if (GetKeyTrigger(VK_S) || GetKeyTrigger(VK_DOWN)
		|| g_bInputDownNow && !g_bInputDownOld
		)
	{
		CSound::Play(SE_BUTTON); // SE�Đ��J�n
		g_nSelectMenu = (TITLE_MENU)((g_nSelectMenu + 1) % NUM_TITLE_MENU);
		SetTitleMenu();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI)
	{
		g_fCurve -= XM_2PI;
	}

	// �I�������V�[���Ɉړ�
	if (GetFade() == FADE_NONE)
	{
		// �X�y�[�X�L�[����Enter�Ō���
		if (GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE)
#if SETCONTROLLER
			|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
			|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_B
			|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
			|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y
#endif
			)
		{
			switch (GetTitleMenu())
			{
				/***** �V�[���I�� *****/
				// �Q�[���X�^�[�g
			case GAME_START:
				SetFadeOut(SCENE_STAGE_SELECT);
				CSound::Play(SE_KETTEI); // SE�Đ��J�n
				break;
				// �`���[�g���A��
			case TUTORIAL_START:
				SetFadeOut(SCENE_TITLE);
				break;
				// �Q�[���I��
			case GAME_END:
				exit(1);
				break;
			}
			g_bStart = true;		// �_��
		}
	}
}

//=============================================================================
// �^�C�g����ʂ̕`�揈��
//=============================================================================
void DrawTitle()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);								// Z�o�b�t�@�X�V���Ȃ�

	// �w�i�`��
	SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
	SetPolygonSize(TITLE_SIZE_X, TITLE_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_ANIMTITLE]);
	SetPolygonFrameSize(1.0f / 6, 1.0f / 1);
	int u = g_titleanim[0].nPattern % 6;
	int v = g_titleanim[0].nPattern / 6;
	SetPolygonUV((float)u/6, (float)v/1);
	
	DrawPolygon(pDeviceContext);

	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(1.0f, 1.0f);


	// �w�i�G�t�F�N�g�`��
	SetPolygonPos(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y);
	SetPolygonSize(TITLE_SIZE_X, TITLE_SIZE_Y / 1.5f);
	SetPolygonTexture(g_pTexture[TEX_TITLEEFFECT]);
	SetPolygonFrameSize(1.0f / 5, 1.0f / 3);
	int x = g_titleeffect[0].nPattern % 15;
	int y = g_titleeffect[0].nPattern / 15;
	SetPolygonUV((float)x / 5, (float)y / 3);

	DrawPolygon(pDeviceContext);

	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(1.0f, 1.0f);

	// �^�C�g�����S�`��
	SetPolygonPos(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y);
	SetPolygonSize(TITLE_LOGO_SIZE_X, TITLE_LOGO_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_LOGO]);
	DrawPolygon(pDeviceContext);

	// �J�n�{�^���`��
	if (g_bDispStart == true)
	{
		SetPolygonTexture(g_pTexture[TEX_START]);
		SetPolygonSize(START_WIDTH, START_HEIGHT);
		SetPolygonPos(START_POS_X, START_POS_Y);
		DrawPolygon(pDeviceContext);
	}

	// �I����ʕ`��
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_TITLE_MENU; nCntPauseMenu++)
	{
		SetPolygonPos(TITLE_MENU_POS_X, TITLE_MENU_POS_Y - nCntPauseMenu * TITLE_MENU_INTERVAL);
		if (nCntPauseMenu == g_nSelectMenu)
		{
			SetPolygonSize(TITLE_MENU_WIDTH + 20, TITLE_MENU_HEIGHT + 20);
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonSize(TITLE_MENU_WIDTH, TITLE_MENU_HEIGHT);
			SetPolygonColor(0.5f, 0.5f, 0.5f);
		}
		SetPolygonAlpha(1.0f);

		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[nCntPauseMenu]);

		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}

	SetZWrite(true);							// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);		// �A���t�@�u�����h����
}

//=============================================================================
// �R���g���[���[�I�u�W�F�N�g�擾
//=============================================================================
void SetTitleControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}

//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetTitleMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
TITLE_MENU GetTitleMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexTitleAnim(int nIdxHitEffect, float fSizeX, float fSizeY)
{
	if (nIdxHitEffect >= 0 && nIdxHitEffect < MAX_TITLE_ANIM) {
		TITLE* pExp = &g_titleanim[nIdxHitEffect];
		pExp->fSizeX = fSizeX;
		pExp->fSizeY = fSizeY;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorTitleAnim(int nIdxHitEffect, XMFLOAT4 col)
{
	if (nIdxHitEffect >= 0 && nIdxHitEffect < MAX_TITLE_ANIM) {
		//g_titleanim[nIdxHitEffect].col = col;
	}
}