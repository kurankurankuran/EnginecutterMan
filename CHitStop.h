//=============================================================================
//
// �q�b�g�X�g�b�v���� [CHitStop.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"

//******************************************************************************
// �N���X��`
//****************************************************************************** 

class CHitStop
{
private:
	bool m_bStopflg;// �q�b�g�X�g�b�v�t���O
	int  m_nTimer;  // �q�b�g�X�g�b�v����
	
protected:

public:
	CHitStop();
	~CHitStop();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void HitStopOn(int); //int�^�Ŏ��Ԏw��
	bool GetHitStopflg(void); // �q�b�g�X�g�b�v���t���O

	static CHitStop* Get();
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

