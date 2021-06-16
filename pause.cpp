//=============================================================================
//
// �ꎞ��~���� [pause.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_TUTORIAL L"data/texture/tutorial.png"


#define	NUM_PAUSE_MENU			(3)						// �|�[�Y���j���[��
#define	PAUSE_MENU_WIDTH		(180.0f)				// �|�[�Y���j���[��
#define	PAUSE_MENU_HEIGHT		(60.0f)				// �|�[�Y���j���[����
#define	PAUSE_MENU_POS_X			(0.0f)					// �|�[�Y���j���[�ʒu(X���W)
#define	PAUSE_MENU_POS_Y			(100.0f)				// �|�[�Y���j���[�ʒu(Y���W)
#define	PAUSE_MENU_INTERVAL	(100.0f)				// �|�[�Y���j���[�Ԋu
#define	PLATE_WIDTH					(360.0f)				// �v���[�g�̕�(��)
#define	PLATE_HEIGHT					(430.0f)				// �v���[�g�̕�(�c)
#define	PLATE_POS_X						(0.0f)					// �v���[�g�̈ʒu(X���W)
#define	PLATE_POS_Y						(0.0f)					// �v���[�g�̈ʒu(Y���W)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// (�Ȃ�)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = {nullptr};	// �e�N�X�`���ւ̃|�C���^
static ID3D11ShaderResourceView*	g_pTextureTutorial[1];	// �e�N�X�`���ւ̃|�C���^

static PAUSE_MENU g_nSelectMenu = PAUSE_MENU_CONTINUE;	// �����I�����Ă��
static float g_fCurve	= 1.0f;
static float g_fCol		= 0.0f;

static bool         g_bInputUpNow;
static bool         g_bInputUpOld;
static bool         g_bInputDownNow;
static bool         g_bInputDownOld;

static XINPUT_STATE* g_pXController;

static LPCWSTR c_aFileNamePauseMenu[NUM_PAUSE_MENU] =
{
	L"data/texture/tuzukerupause.png",	// ��������
	L"data/texture/startpause.png",			// �ŏ�����
	L"data/texture/titlepause.png",			// �^�C�g����
};

static bool         g_bLoadMesh = false;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPause(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	g_bInputUpNow = false;
	g_bInputUpOld = false;
	g_bInputDownNow = false;
	g_bInputDownOld = false;

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++)
		{
			// �e�N�X�`���̓ǂݍ���
			hr = CreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
				c_aFileNamePauseMenu[nCntPauseMenu],		// �t�@�C���̖��O
				&g_pTextures[nCntPauseMenu]);					// �ǂݍ��ރ������[

			// �e�N�X�`���\�̕ۑ�
			SaveTex(g_pTextures[nCntPauseMenu]);


		}
		// �e�N�X�`���̓ǂݍ���
		hr = CreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TUTORIAL,		// �t�@�C���̖��O
			&g_pTextureTutorial[0]);					// �ǂݍ��ރ������[
			// �e�N�X�`���\�̕ۑ�
		SaveTex(g_pTextureTutorial[0]);

		g_bLoadMesh = true;
	}
	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	g_fCurve = 0.0f;

	// ���ʉ�������
	/*g_pSE_Select = CreateSound(SE_SELECT_PATH);
*/
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{
#if MESH_RELEASE_ON
	// �e�N�X�`���̊J��
	SAFE_RELEASE(g_pTextureTutorial[0]);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++) 
	{
		
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{

	g_bInputUpOld = g_bInputUpNow;
	g_bInputDownOld = g_bInputDownNow;

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


	if (GetKeyTrigger(VK_W) || GetKeyTrigger(VK_UP)
		|| g_bInputUpNow && !g_bInputUpOld)
	{
		CSound::Play(SE_BUTTON);
		g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	} 
	else if (GetKeyTrigger(VK_S) || GetKeyTrigger(VK_DOWN)
		|| g_bInputDownNow && !g_bInputDownOld
		)
	{
		CSound::Play(SE_BUTTON);
		g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) 
	{
		g_fCurve -= XM_2PI;
	}

	// ���ˌ��̐ݒ�
	//g_fCol = cosf(g_fCurve) * 0.2f + 0.5f;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);								// Z�o�b�t�@�X�V���Ȃ�

	SetPolygonTexture(nullptr);
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X - 300.0f, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(1.0f, 0.2f, 0.2f/* g_fCol, g_fCol, g_fCol */);
	SetPolygonAlpha(0.4f);
	DrawPolygon(pDeviceContext);

	
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X - 300.0f, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
	SetPolygonTexture(g_pTextureTutorial[0]);
	DrawPolygon(pDeviceContext);


	//**************************************************************************



	SetPolygonTexture(nullptr);
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X+300, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(1.0f, 0.2f, 0.2f/* g_fCol, g_fCol, g_fCol */);
	SetPolygonAlpha(0.4f);
	DrawPolygon(pDeviceContext);



	SetPolygonSize(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++)
	{
		SetPolygonPos(PAUSE_MENU_POS_X+300, PAUSE_MENU_POS_Y - nCntPauseMenu * PAUSE_MENU_INTERVAL);
		if (nCntPauseMenu == g_nSelectMenu)
		{
			// �I������Ă���
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			// �I������Ă��Ȃ�
			SetPolygonColor(0.8f, 0.8f, 0.8f);
		}
		SetPolygonAlpha(1.0f);

		// �e�N�X�`���̐ݒ�
		SetPolygonTexture(g_pTextures[nCntPauseMenu]);

		// �|���S���̕`��
		DrawPolygon(pDeviceContext);
	}
	SetPolygonColor(0.0f, 0.0f, 0.0f);

	SetZWrite(true);							// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);		// �A���t�@�u�����h����
}


//=============================================================================
// �|�[�Y���j���[�̐ݒ�
//=============================================================================
void SetPauseMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// �|�[�Y���j���[�̎擾
//=============================================================================
PAUSE_MENU GetPauseMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// �|�[�Y���j���[�̃��Z�b�g
//=============================================================================
void ResetPauseMenu(void)
{
	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	CSound::Play(SE_BUTTON);
	SetPauseMenu();
}

//=============================================================================
// �R���g���[���[�I�u�W�F�N�g�擾
//=============================================================================
void SetPauseControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}