//=============================================================================
//
// �t�F�[�h�C�� / �A�E�g���� [fade.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "fade.h"
#include "polygon.h"
#include "scene.h"
//#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FADE_RATE		0.02f			// �t�F�[�h�̍���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static float	g_fRed		= 0.0f;			// ��(�t�F�[�h�A�E�g�F)
static float	g_fGreen	= 0.0f;			// ��(�t�F�[�h�A�E�g�F)
static float	g_fBlue	= 0.0f;			    // ��(�t�F�[�h�A�E�g�F)
static float	g_fAlpha	= 1.0f;			// �s�����x
static int		g_nFade	= FADE_IN;		// ���݂̏��
static int		g_nNextScene = SCENE_TITLE;	// ���̃V�[��

//=============================================================================
// �t�F�[�h�C�� / �A�E�g�̏���������
//=============================================================================
HRESULT InitFade()
{
	g_fAlpha	= 1.0f;			// �s�����x
	g_nFade	= FADE_IN;			// ���
	g_nNextScene = SCENE_TITLE;	// ���̃V�[��

	return S_OK;
}

//=============================================================================
// �t�F�[�h�C�� / �A�E�g�̏I������
//=============================================================================
void UninitFade()
{
	// (�������Ȃ�)
}

//=============================================================================
// �t�F�[�h�C�� / �A�E�g�̍X�V����
//=============================================================================
void UpdateFade()
{
	// �������Ă��Ȃ�
	if (g_nFade == FADE_NONE)
	{
		return;
		
	}
	

	// --- �t�F�[�h�A�E�g���� ---
	if (g_nFade == FADE_OUT)
	{
		g_fAlpha += FADE_RATE;		// �s�����x�𑝉�
		if (g_fAlpha >= 1.0f)
		{
			// �t�F�[�h�C�������ɐ؂�ւ�
			g_fAlpha	= 1.0f;
			g_nFade	= FADE_IN;
			// �V�[���؂�ւ�
			SetScene(g_nNextScene);
		}
		// BGM���t�F�[�h�A�E�g
		//CSound::SetVolume(1.0f - g_fAlpha);
		return;
	}

	// --- �t�F�[�h�C������ ---
	g_fAlpha -= FADE_RATE;			// �s�����x��������
	if (g_fAlpha <= 0.0f)
	{
		// �t�F�[�h�A�E�g�����I��
		g_fAlpha = 0.0f;
		g_nFade = FADE_NONE;
	}
	// �{�����[�����t�F�[�h�C��
	//CSound::SetVolume(1.0f - g_fAlpha);

}

//=============================================================================
// �t�F�[�h�C�� / �A�E�g����
//=============================================================================
void DrawFade()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// �t�F�[�h�C�� / �A�E�g
		// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�

	SetPolygonPos(0.0f, 0.0f);
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetPolygonTexture(nullptr);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);

	SetPolygonColor(g_fRed, g_fGreen, g_fBlue);
	SetPolygonAlpha(g_fAlpha);

	DrawPolygon(GetDeviceContext());
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
	SetZWrite(true);
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}

//=============================================================================
// �t�F�[�h�A�E�g�J�n(���̃V�[��)
//=============================================================================
void SetFadeOut(int nNextScene)	
{
	if (g_nFade != FADE_OUT)
	{
		g_nFade = FADE_OUT;
		g_nNextScene = nNextScene;
	}
}
//=============================================================================
// �t�F�[�h�C�� / �A�E�g�̏�Ԃ̎擾
//=============================================================================
int GetFade()		
{
	return g_nFade;
}
//=============================================================================
// �t�F�[�h�A�E�g���̉�ʂ̐F��ݒ�
//=============================================================================
void SetFadeColor(float fR, float fG, float fB)	
{
	g_fRed	= fR;
	g_fGreen = fG;
	g_fBlue	= fB;
}