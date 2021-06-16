//=============================================================================
//
// �X�e�[�W�Z���N�g���� [StageSelect.cpp]
// Author : RYUSUKE	YAMADA
//-----------------------------------------------------------------------------
// �I���ƌ���̉��o��悤�ɂ��Ă����܂����B
//=============================================================================

#include "main.h"
#include "Texture.h"
#include "StageSelect.h"
#include "input.h"
#include "fade.h"
#include "scene.h"
#include "polygon.h"
#include "debugproc.h"
#include "Sound.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	SSELECT_POS_X						(0)			// �|���S���̕\���ʒu
#define	SSELECT_POS_Y						(0)			// �|���S���̕\���ʒu
#define	SSELECT_SIZE_X					(300)			// �|���S���̃T�C�Y
#define	SSELECT_SIZE_Y					(300)			// �|���S���̃T�C�Y

#define	NUM_SSELECT						(3)				// �X�e�[�W�Z���N�g��ʂł̑I��
#define	SSELECT_MENU_INTERVAL	(400.0f)				// �X�e�[�W�Z���N�g�Ԋu
#define	SSELECT_MENU_WIDTH		(180.0f)				// �X�e�[�W�Z���N�g��
#define	SSELECT_MENU_HEIGHT		(300.0f)				// �X�e�[�W�Z���N�g����
#define	SSELECT_MENU_POS_X		(-SSELECT_MENU_INTERVAL)// �X�e�[�W�Z���N�g�ʒu(X���W)
#define	SSELECT_MENU_POS_Y		(0.0f)					// �X�e�[�W�Z���N�g�ʒu(Y���W)

#define	SCALE_SPEED		(1)			// �I�����Ă���z�̊g��k���X�s�[�h
#define	COLOR_SPEED		(0.01f)			// �I�����Ă���z�̐F�ω��X�s�[�h

#define	PICK_UP			L"data/texture/white.png"			// �I���������
#define	YAZIRUSHI		L"data/texture/yazirusi.png"		// ���

#define	HAIKEI_TEXTURE	L"data/texture/BossCheck.png"	// �w�i�摜
#define MAX_HAIKAEI_ANIM (128)
#define HAIKAEI_ANIM_SIZE_X		(5.0f)	// �|���S���̃T�C�Y
#define HAIKAEI_ANIM_SIZE_Y		(5.0f)	// �|���S���̃T�C�Y
#define HAIKEI_POS_X 0
#define HAIKEI_POS_Y 0
#define HAIKEI_SIZE_X		SCREEN_WIDTH	// �|���S���̃T�C�Y
#define HAIKEI_SIZE_Y		SCREEN_HEIGHT	// �|���S���̃T�C�Y


static LPCWSTR c_aFileNamePauseMenu[NUM_SSELECT] =
{
	L"data/texture/Vacuum.jpg",		// �X�e�[�W�P
	L"data/texture/Vacuum.jpg",		// �X�e�[�W�Q
	L"data/texture/Vacuum.jpg",		// �X�e�[�W�R
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct SELECT {
	XMFLOAT2	pos;		// �ʒu
	float		fSizeX;		// ��
	float		fSizeY;		// ����
	int			nCounter;	// �J�E���^�[
	int			nPattern;	// �p�^�[��No.
	bool		bUse;		// �g�p���Ă��邩�ǂ���
};

//*****************************************************************************
// �萔
//*****************************************************************************
enum ETex
{
	TEX_SELECT = 0,
	TEX_START,

	MAX_TEX
};
static LPCWSTR g_pszTexFName[] =
{
	HAIKEI_TEXTURE
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };			// �e�N�X�`���ւ̃|�C���^
static ID3D11ShaderResourceView*	g_pTexture;			// �e�N�X�`��
static ID3D11ShaderResourceView*	g_pBgTexture[MAX_TEX];			// �e�N�X�`��
static ID3D11ShaderResourceView*	g_yazirushi;			// �e�N�X�`��
static ID3D11ShaderResourceView*	g_haikei;			// �e�N�X�`��


static STAGE_SELECT_MENU g_nSelectMenu = STAGE_2;			// �I�����Ă��

static float	g_fCurve = 1.0f;

static int		timer = 0;
static bool	plus	= true;
static bool	minus = false;

static int		select_timer = 0;
static float	rgb_r	= 0.0f;
static float	rgb_g	= 0.0f;
static float	rgb_b	= 1.0f;

static bool	select_stage = true;
static bool	Enter_Use = false;
static int		Enter_time;

static XINPUT_STATE*	g_pXController;
static bool         g_bInputLeftNow;
static bool         g_bInputLeftOld;
static bool         g_bInputRightNow;
static bool         g_bInputRightOld;

static SELECT g_SelectAnim[MAX_HAIKAEI_ANIM];


static bool         g_bLoadMesh = false;

//=============================================================================
// �X�e�[�W�Z���N�g�̏���������
//=============================================================================
HRESULT InitStageSelect()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		// �e�N�X�`���ǂݍ���
		CreateTextureFromFile(GetDevice(), PICK_UP, &g_pTexture);
		if (FAILED(hr))
		{
			return hr;
		}
		SaveTex(g_pTexture);

		// �e�N�X�`���ǂݍ���
		CreateTextureFromFile(GetDevice(), YAZIRUSHI, &g_yazirushi);
		if (FAILED(hr))
		{
			return hr;
		}
		SaveTex(g_yazirushi);

		// �e�N�X�`���ǂݍ���
		/*for (int i = 0; i < MAX_TEX; i++)
		{*/
		hr = CreateTextureFromFile(GetDevice(), g_pszTexFName[0], &g_pBgTexture[0]);
		if (FAILED(hr))
		{
			MessageBoxW(GetMainWnd(), g_pszTexFName[0],
				L"�e�N�X�`���ǂݍ��݃G���[", MB_ICONSTOP | MB_OK);
			return hr;
		}
		//}
		SaveTex(g_pBgTexture[0]);

	
		// �^�C�g���I���̃e�N�X�`���ǂݍ���
		for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_SSELECT; nCntPauseMenu++)
		{
			hr = CreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
				c_aFileNamePauseMenu[nCntPauseMenu],		// �t�@�C���̖��O
				&g_pTextures[nCntPauseMenu]);					// �ǂݍ��ރ������[

			SaveTex(g_pTextures[nCntPauseMenu]);
		}
		g_bLoadMesh = true;
	}
	g_fCurve = 0.0f;

	timer = 0;
	plus = true;
	minus = false;

	select_timer = 0;	

	rgb_r = 0.0f;
	rgb_g = 0.0f;
	rgb_b = 1.0f;
	
	Enter_Use = false;
	select_stage = true;

	g_bInputLeftNow = false;
	g_bInputLeftOld = false;
	g_bInputRightNow = false;
	g_bInputRightOld = false;

	// �w�i�A�j���[�V����������
	SELECT* pSelect = g_SelectAnim;
	for (int i = 0; i < MAX_HAIKAEI_ANIM; ++i, ++pSelect)
	{
		pSelect->pos = XMFLOAT2(0.0f, 0.0f);
		pSelect->fSizeX = HAIKAEI_ANIM_SIZE_X;
		pSelect->fSizeY = HAIKAEI_ANIM_SIZE_Y;
		pSelect->nCounter = 0;
		pSelect->nPattern = 0;
		pSelect->bUse = false;
	}
	pSelect = g_SelectAnim;
	pSelect->bUse = true;

	return hr;
}


//=============================================================================
// �X�e�[�W�Z���N�g�̏I������
//=============================================================================
void UninitStageSelect()
{

#if  MESH_RELEASE_ON
	// �e�N�X�`���̊J��
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_SSELECT; nCntPauseMenu++)
	{
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
	// �e�N�X�`���J��
	for (int i = 0; i < MAX_TEX; i++)
	{
		SAFE_RELEASE(g_pBgTexture[i]);
	}
	SAFE_RELEASE(g_yazirushi);
	SAFE_RELEASE(g_haikei);
	SAFE_RELEASE(g_pTexture);
#endif
}


//=============================================================================
// �X�e�[�W�Z���N�g�̍X�V����
//=============================================================================
void UpdateStageSelect()
{
	// �X�e�[�W�I���ŏo���A�j���[�V�����̏���
	SELECT* pSelect = g_SelectAnim;
	for (int i = 0; i < MAX_HAIKAEI_ANIM; ++i, ++pSelect)
	{
		if (pSelect->bUse)
		{
			++pSelect->nCounter;
			// �X�e�[�W�I���ŏo���A�j���[�V�����̑��x�ݒ�
			if (pSelect->nCounter % 5 == 0)
			{
				pSelect->nPattern++;

			}

			// ���_���W�̐ݒ�
			pSelect->fSizeX += 0.5f;
			pSelect->fSizeY += 0.5f;
		}
	}
	// ���܂�

	g_bInputLeftOld = g_bInputLeftNow;
	g_bInputRightOld = g_bInputRightNow;

#if SETCONTROLLER
	if (g_pXController->Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_LEFT
		) {
		g_bInputLeftNow = true;
	}
	else
	{
		g_bInputLeftNow = false;
	}

	if (g_pXController->Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_RIGHT
		) {
		g_bInputRightNow = true;
	}
	else
	{
		g_bInputRightNow = false;
	}
#endif


	// �I���J�[�\�����킹��
	// �J�[�\�������ړ�
	//if (GetKeyTrigger(VK_A) || GetKeyTrigger(VK_LEFT)
	//	|| g_bInputLeftNow && !g_bInputLeftOld
	//	)
	//{
	//	CSound::Play(SE_BUTTON); // SE�Đ��J�n
	//	g_nSelectMenu = (STAGE_SELECT_MENU)((g_nSelectMenu + NUM_SSELECT - 1) % NUM_SSELECT);
	//	SetStageSelect();
	//}
	//// �J�[�\�����E�ړ�
	//if (GetKeyTrigger(VK_D) || GetKeyTrigger(VK_RIGHT)
	//	|| g_bInputRightNow && !g_bInputRightOld
	//	)
	//{
	//	CSound::Play(SE_BUTTON); // SE�Đ��J�n
	//	g_nSelectMenu = (STAGE_SELECT_MENU)((g_nSelectMenu + 1) % NUM_SSELECT);
	//	SetStageSelect();
	//}


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
			switch (GetStageSelect())
			{
				/***** �X�e�[�W�I�� *****/
				// �X�e�[�W�P
			case STAGE_1:
				SetFadeOut(SCENE_GAME);
				//SetFadeOut(SCENE_STAGE1);
				CSound::Play(SE_KETTEI); // SE�Đ��J�n
				break;
				// �X�e�[�W�Q
			case STAGE_2:
				SetFadeOut(SCENE_GAME);
				//SetFadeOut(SCENE_STAGE2);
				CSound::Play(SE_KETTEI); // SE�Đ��J�n
				break;
				// �X�e�[�W�R
			case STAGE_3:
				SetFadeOut(SCENE_GAME);
				//SetFadeOut(SCENE_STAGE3);
				CSound::Play(SE_KETTEI); // SE�Đ��J�n
				break;
			}		
			Enter_Use = true;
		}
	}
	if (Enter_Use == true || Enter_time == 0)
	{
		Enter_time++;
		select_stage = true;
	}

	if (Enter_time >= 5)
	{
		select_stage = false;
		Enter_time = 0;
	}


	/***** �I�����ꂽ�X�e�[�W�摜�̊g��k�� *****/
	if (plus == true)
	{
		minus = false;
		timer += SCALE_SPEED;
		if (timer >= 50)
		{
			minus = true;
		}
	}
	if (minus == true)
	{
		plus = false;
		timer -= SCALE_SPEED;
		if (timer <= -10)
		{
			plus = true;
		}
	}

	/***** �I�����ꂽ�g(�w�i�摜)�̕\��*****/
	if (rgb_g <= 0.0f && rgb_b <= 1.0f)
	{
		rgb_b -= COLOR_SPEED;
		rgb_r += COLOR_SPEED;
		if (rgb_b <= 0.0f)
		{
			rgb_b = 0.0f;
		}
		if (rgb_r >= 1.0f)
		{
			rgb_r = 1.0f;
		}
	}
	if (rgb_r <= 1.0f && rgb_b <= 0.0f)
	{
		rgb_r -= COLOR_SPEED;
		rgb_g += COLOR_SPEED;
		if (rgb_r <= 0.0f)
		{
			rgb_r = 0.0f;
		}
		if (rgb_g >= 1.0f)
		{
			rgb_g = 1.0f;
		}
	}
	if (rgb_r <= 0.0f && rgb_g <= 1.0f)
	{
		rgb_g -= COLOR_SPEED;
		rgb_b += COLOR_SPEED;
		if (rgb_g <= 0.0f)
		{
			rgb_g = 0.0f;
		}
		if (rgb_b >= 1.0f)
		{
			rgb_b = 1.0f;
		}
	}
	/*StartDebugProc();
	PrintDebugProc("R(%f)\n", rgb_r);
	PrintDebugProc("G(%f)\n", rgb_g);
	PrintDebugProc("B(%f)\n", rgb_b);
	PrintDebugProc("��ϰ:%d", Enter_time);*/
}


//=============================================================================
// �X�e�[�W�Z���N�g�̕`�揈��
//=============================================================================
void DrawStageSelect()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);

	int nCntPauseMenu;

	/*SetPolygonSize(SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3);
	SetPolygonColor(0.0f, 0.0f, 0.0f);
	SetPolygonTexture(g_haikei);
	DrawPolygon(pDeviceContext);*/
	// �w�i�`��
	SetPolygonPos(HAIKEI_POS_X, HAIKEI_POS_Y);
	SetPolygonSize(HAIKEI_SIZE_X, HAIKEI_SIZE_Y);
	SetPolygonTexture(g_pBgTexture[TEX_SELECT]);
	SetPolygonFrameSize(1.0f / 6, 1.0f / 1);
	int u = g_SelectAnim[0].nPattern % 6;
	int v = g_SelectAnim[0].nPattern / 6;
	SetPolygonUV((float)u / 6, (float)v / 1);

	DrawPolygon(pDeviceContext);

	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(1.0f, 1.0f);

	// �I����ʕ`��
	for (nCntPauseMenu = 1; nCntPauseMenu < 2/*NUM_SSELECT*/; nCntPauseMenu++)
	{
		SetPolygonPos(SSELECT_MENU_POS_X + nCntPauseMenu * SSELECT_MENU_INTERVAL, SSELECT_MENU_POS_Y);
		if (nCntPauseMenu == g_nSelectMenu)
		{
			// �I�����Ă�
			if (select_stage == true)
			{
				// �g(�w�i�摜)�\��
				SetPolygonSize(SSELECT_MENU_WIDTH + timer + 20, SSELECT_MENU_HEIGHT + timer + 20);
				SetPolygonColor(rgb_r, rgb_g, rgb_b);
				SetPolygonAlpha(1.0f);
				SetPolygonTexture(g_pTexture);
				DrawPolygon(pDeviceContext);

				SetPolygonSize(SSELECT_MENU_WIDTH + timer, SSELECT_MENU_HEIGHT + timer);
				SetPolygonColor(1.0f, 1.0f, 1.0f);
				SetPolygonAlpha(1.0f);
				SetPolygonTexture(g_pTextures[nCntPauseMenu]);
				DrawPolygon(pDeviceContext);
			}
		}
		else
		{
			// �I�����ĂȂ�
			SetPolygonSize(SSELECT_MENU_WIDTH, SSELECT_MENU_HEIGHT);
			SetPolygonColor(0.5f, 0.5f, 0.5f);
			SetPolygonAlpha(1.0f);
			SetPolygonTexture(g_pTextures[nCntPauseMenu]);
			DrawPolygon(pDeviceContext);
		}
	}

	

	SetZWrite(true);							// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);		// �A���t�@�u�����h����
}


//=============================================================================
// �X�e�[�W�Z���N�g�̐ݒ�
//=============================================================================
void SetStageSelect(void)
{
	g_fCurve = 0.0f;
}


//=============================================================================
// �X�e�[�W�Z���N�g�̎擾
//=============================================================================
STAGE_SELECT_MENU GetStageSelect(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// �R���g���[���[�I�u�W�F�N�g�擾
//=============================================================================
void SetStageSelectControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}