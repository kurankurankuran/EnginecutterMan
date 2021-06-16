//=============================================================================
//
// �e���� [engine.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "engine.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENGINE_TEXTURENAME	L"data/texture/Engine.png"	// �e�N�X�`���t�@�C����
//#define SE_ENGINE1_PATH      "data/sound/se_maoudamashii_retro19.mp3"

//#define SE_ENGINE2_PATH      "data/sound/se_maoudamashii_element_thunder05.mp3"


#define ENGINE_SIZE_X		174								// �|���S���̃T�C�Y
#define ENGINE_SIZE_Y		174								// �|���S���̃T�C�Y
#define ENGINE_FRAME_X		4								// ���t���[����
#define ENGINE_FRAME_Y		4								// �c�t���[����




#define ANIM_DEATHSHOW_FRAME      (BREAK_ROT/15)   //�A�j���[�V�����̂P�R�}�̃t���[����
#define ANIM_DEATHSHOW_SPLIT_X    (4)   //�摜�̕������i���j
#define ANIM_DEATHSHOW_SPLIT_Y    (4)   //�摜�̕������i�c�j




//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static XINPUT_STATE* g_pXController;
XINPUT_VIBRATION* g_pVibration;

static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��

static bool         g_bLoadMesh = false;
//=============================================================================
// ����������
//=============================================================================
HRESULT CEnzine::Initengine()
{
	HRESULT hr = S_OK;
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {	
		// �e�N�X�`���ǂݍ���
	    hr = CreateTextureFromFile(GetDevice(),
			ENGINE_TEXTURENAME, &g_pTexture);
		SaveTex(g_pTexture);

		g_bLoadMesh = true;
	}
	m_RotSpeed = 0;

	// ��񏉊���
	for (int i = 0; i < MAX_ENGINE; ++i)
	{
		
		g_engine[i].pos = XMFLOAT2(500.0f, -250.0f);

		g_engine[i].posTexCoord = XMFLOAT2(0.0f, 0.0f);

		//�����͈�ԉ摜�̈�ԍ���̃R�}
		g_engine[i].sizeTexCoord = XMFLOAT2(1.0f / (float)ENGINE_FRAME_X,
			1.0f / (float)ENGINE_FRAME_Y);

		//�e�N�X�`���T�C�Y�͌�ŕ������̒萔��ς��邾���ł����悤�ɂ��Ă���
		g_engine[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//�G�t�F�N�g�̐F������ς�������΂����ł������̕ҏW��
		g_engine[i].frame = 0;          //���̃J�E���^�[���X�V���ɑ��₵�Ă���
		g_engine[i].currentAnimNo = 0;  //�摜�̃R�}���肵�����R�}�ڂ��u�O�X�^�[�g�v

	}
	
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CEnzine::Uninitengine()
{
	CSound::Stop(BGM_PRAAY);

#if SETCONTROLLER
	g_pVibration->wLeftMotorSpeed = 0;
	g_pVibration->wRightMotorSpeed = 0;
#endif

#if  MESH_RELEASE_ON
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnzine::Updateengine()
{
	if (GetKeyPress(VK_SPACE)
#if SETCONTROLLER
		|| g_pXController->Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD
#endif
		)
	{
		++m_RotSpeed;
		if (m_RotSpeed > VAL_MAXROT) {
			m_RotSpeed = VAL_MAXROT;
		}
	}
	else {
		--m_RotSpeed;
		if (m_RotSpeed < 0) {
			m_RotSpeed = 0;
		}
	}

	// �u���[�L
	if (GetKeyPress(VK_B)
#if SETCONTROLLER
		|| g_pXController->Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD
#endif
		) {
		m_RotSpeed -= VAL_MAXROT / (2 * 60);
		if (m_RotSpeed < 0) {
			m_RotSpeed = 0;
		}
	}


	// �X�V
	ST_ENGINE_PARAM* pengine = g_engine;

	for (int i = 0; i < MAX_ENGINE; ++i, ++pengine)
	{

		g_engine[i].frame = m_RotSpeed/*g_pPlayer->GetRotSpeed()*/;
		/*if (g_engine[i].frame > BREAK_ROT)
			g_engine[i].frame = BREAK_ROT;
		else if (g_engine[i].frame < 0)
			g_engine[i].frame = 0;*/

		if (GetKeyPress(VK_SPACE) 
#if SETCONTROLLER
			|| g_pXController->Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD
#endif
			)
		{
#if SETCONTROLLER
			//�o�C�u���[�V������]������
			if (g_pVibration->wLeftMotorSpeed < MAX_VIBRATION)g_pVibration->wLeftMotorSpeed += ADD_VIBRATION;
			if (g_pVibration->wRightMotorSpeed < MAX_VIBRATION)g_pVibration->wRightMotorSpeed += ADD_VIBRATION;
#endif

			if (!(g_engine[i].frame > BREAK_ROT))
			{
				//�A�j���[�V�����i�t���[���j�X�V
				if (0 == g_engine[i].frame%ANIM_DEATHSHOW_FRAME)
				{
					g_engine[i].currentAnimNo++;
				}
				//�A�j���[�V�����ԍ��X�V�i���̃A�j���[�V�����֕ύX��������)
				if (16 == g_engine[i].currentAnimNo)
				{
					g_engine[i].currentAnimNo = 15;  //���݂̃R�}�ԍ����X�V����

				}
			}
		}
		else
		{
#if SETCONTROLLER
			//�o�C�u���[�V������]������
			if (g_pVibration->wLeftMotorSpeed > 0)g_pVibration->wLeftMotorSpeed -= ADD_VIBRATION * 2;
			if (g_pVibration->wRightMotorSpeed > 0)g_pVibration->wRightMotorSpeed -= ADD_VIBRATION * 2;
#endif

			if (!(g_engine[i].frame >= BREAK_ROT))
			{
				if (0 == g_engine[i].frame%ANIM_DEATHSHOW_FRAME)
				{
					g_engine[i].currentAnimNo--;
				}
				//�A�j���[�V�����ԍ��X�V�i���̃A�j���[�V�����֕ύX��������)
				if (1 >= g_engine[i].currentAnimNo)
				{
					g_engine[i].currentAnimNo = 0;

				}
			}

			CSound::Stop(BGM_PRAAY);
			


		}

		if (15 == g_engine[i].currentAnimNo)
		{
			
			CSound::Play(BGM_PRAAY);	// ���ʉ��Đ�
			CSound::SetVolume(BGM_PRAAY, 1.0f);

		}

		
		

	}

	
}

//=============================================================================
//�`�揈��
//=============================================================================
void CEnzine::Drawengine()
{
	// �`��
	// �����������L��

	//SetPolygonColor(1.0f, 1.0f, 1.0f);
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	SetBlendState(BS_ALPHABLEND);
	SetPolygonSize(ENGINE_SIZE_X, ENGINE_SIZE_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonFrameSize(1.0f / ENGINE_FRAME_X, 1.0f / ENGINE_FRAME_Y);
	
	

	ST_ENGINE_PARAM* pengine = g_engine;
	for (int i = 0; i < MAX_ENGINE; ++i, ++pengine) {
		
		SetPolygonPos(pengine->pos.x, pengine->pos.y);
		int u =g_engine->currentAnimNo % ENGINE_FRAME_X;
		int v = g_engine->currentAnimNo / ENGINE_FRAME_X;
		SetPolygonUV(u / (float)ENGINE_FRAME_X, v / (float)ENGINE_FRAME_Y);

		ST_ENGINE_PARAM* pBullet = g_engine;
	
	}
	DrawPolygon(pDeviceContext);
	// �t���[����߂��Ă���
	//
	//SetPolygonColor(0.0f, 0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);

	// ��������������
	SetBlendState(BS_NONE);
}

//=============================================================================
// �R���g���[���[�I�u�W�F�N�g�擾
//=============================================================================
void SetEnzineControllerr(XINPUT_STATE* pControlller, XINPUT_VIBRATION *pVibration)
{
	g_pXController = pControlller;
	g_pVibration = pVibration;
}

