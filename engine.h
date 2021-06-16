//=============================================================================
//
// �e���� [engine.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>
//#include "model.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENGINE             (16)  //�G�t�F�N�g�̐�
#define VAL_MAXROT           (300)   // �P�ʎ��ԉ�]��
#define BREAK_ROT           (270)         // �P�ʎ��ԉ�]���̌��E


#define ADD_VIBRATION	50		//.�o�C�u���[�V�����̋����̏㏸�l�i���R�l�j
#define MAX_VIBRATION	(ADD_VIBRATION*1240)	//.�o�C�u���[�V�����̍ő�l

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
//**********�\����**********
typedef struct
{

	XMFLOAT2 pos;            //���S���W
	XMFLOAT2 posTexCoord;    //�e�N�X�`�����W�i����j
	XMFLOAT2 sizeTexCoord;   //�e�N�X�`���T�C�Y�i�E���j�i�O�D�O�|�P�D�O�j
	XMFLOAT4 Color;          //�J���[
	int frame;             //�A�j���[�V�����Ǘ��p�t���[��
	int currentAnimNo;     //�A�j���[�V�����̃R�}�ԍ��i���ォ��O�|�j

}ST_ENGINE_PARAM;       //�������Ǘ�����p�����[�^�̍\����


class CEnzine {
private:
	

	ST_ENGINE_PARAM 	g_engine[MAX_ENGINE];	//���
	int g_nTimer;

	int             m_RotSpeed;          // �n�̉�]���x

public:
	HRESULT Initengine();
	void Uninitengine();
	void Updateengine();
	void Drawengine();
	//int Fireengine(XMFLOAT2* pPos, int nDir);

	int GetRotSpeed(void) { return m_RotSpeed; }
	void SetRotSpeed(int nRotSpeed) {  m_RotSpeed = nRotSpeed; }

	CEnzine() { m_RotSpeed = 0; }
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//HRESULT Initengine();
//void Uninitengine();
//void Updateengine();
//void Drawengine();
////int Fireengine(XMFLOAT2* pPos, int nDir);


void SetEnzineControllerr(XINPUT_STATE* pControlller, XINPUT_VIBRATION *pVibration);
//void SetEnzinePlayer(CPlayer* pPlayer);
