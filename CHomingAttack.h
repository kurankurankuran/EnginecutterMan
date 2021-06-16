//=============================================================================
//
// ���f������ [CHomingAttack.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"
//#include "Camera.h"

#include "Enemylife.h"

//#include "model.h"

//*****************************************************************************
// �萔�錾
//*****************************************************************************
#define MAX_HOMINGATK (20)
#define MAX_HOMINGATKSETNUM        (10)       // �U�������ݒu����ő吔

class CPlayer;
//class CCamera;

class CHomingAttack
{
private:
	bool   m_bUseAtk[MAX_HOMINGATK];              // �U������g���Ă��邩�ǂ���
	int    m_nHomingTime[MAX_HOMINGATK]; //�ǔ�����
	XMFLOAT3 m_pos[MAX_HOMINGATK];   // �z�u�ʒu�i�����͈͂̓���j
	float    m_rad[MAX_HOMINGATK];   // �_���[�W���̔��a
	XMFLOAT3 m_scl[MAX_HOMINGATK];   // �����͈͂̑傫���i����X�A�c��Y�A���s��Z�j
	XMFLOAT3 m_rot[MAX_HOMINGATK];   // ��]�p�x (�΂߂ɂ���Ƃ�����)
	XMFLOAT3 m_move[MAX_HOMINGATK];  // �ړ���
	int    m_nDamage[MAX_HOMINGATK];// �v���C���[�ɗ^����_���[�W
	int m_nLife[MAX_HOMINGATKSETNUM];
	XMFLOAT4X4 m_mtxWorld[MAX_HOMINGATK]; // ���[���h�}�g���b�N�X



	bool     m_bUseSet[MAX_HOMINGATKSETNUM];  // �U���͈͎g���Ă��邩�ǂ���
	XMFLOAT3 m_posSet[MAX_HOMINGATKSETNUM];   // �z�u�ʒu�i�����͈͂̓���j
	float    m_radSet[MAX_HOMINGATKSETNUM];   // �_���[�W���̔��a
	XMFLOAT3 m_sclSet[MAX_HOMINGATKSETNUM];   // �����͈͂̑傫���i����X�A�c��Y�A���s��Z�j
	XMFLOAT3 m_rotSet[MAX_HOMINGATKSETNUM];   // ��]�p�x (�΂߂ɂ���Ƃ�����)
	XMFLOAT4X4 m_mtxWorldSet[MAX_HOMINGATKSETNUM]; // ���[���h�}�g���b�N�X
	int      m_nAtkSetTime[MAX_HOMINGATKSETNUM];                   // �U�����o�����鎞�Ԃ̊Ԋu
	/*int      m_nLifeSet[MAX_HOMINGATKSETNUM];*/

	CEnemyLife* m_pLife[MAX_HOMINGATKSETNUM];  // ���C�t�|�C���^
	CEnemyLife m_Life[MAX_HOMINGATKSETNUM];  // ���C�t�|�C���^

	/*CCamera* m_pCamera;
	CLight*  m_pLight;*/
	CPlayer* m_pPlayer;
protected:

public:
	CHomingAttack();
	~CHomingAttack();

	HRESULT Init();
	void Update();
	void Uninit();
	void Draw();

	/*void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	void SetLight(CLight* pLight) { m_pLight = pLight; }*/
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }

	void SetAtkHoming(int);
};