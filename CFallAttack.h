//=============================================================================
//
// ���f������ [CFallAttack.h]
// Author : RYOOSUKE�@YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
//#include "Camera.h"
#include "Light.h"
#include "Enemylife.h"


//*****************************************************************************
// �萔�錾
//*****************************************************************************
#define MAX_FALLATK (10)
#define MAX_ATKLENGENUM        (10)       // �U�������ݒu����ő吔

class CPlayer;
class CCamera;
//******************************************************************************
// �N���X��`
//****************************************************************************** 

class CFallAttack
{
private:
	bool   m_bUseAtk[MAX_FALLATK];              // �U������g���Ă��邩�ǂ���
	bool   m_fall[MAX_FALLATK];    // �����͈͂ɓ����true�ɂ���
	int    m_nFallTime[MAX_FALLATK]; //�����鎞��
	XMFLOAT3 m_pos[MAX_FALLATK];   // �z�u�ʒu�i�����͈͂̓���j
	float    m_rad[MAX_FALLATK];   // �_���[�W���̔��a
	XMFLOAT3 m_scl[MAX_FALLATK];   // �����͈͂̑傫���i����X�A�c��Y�A���s��Z�j
	XMFLOAT3 m_rot[MAX_FALLATK];   // ��]�p�x (�΂߂ɂ���Ƃ�����)
	XMFLOAT3 m_move[MAX_FALLATK];  // �ړ���
	int    m_nDamage[MAX_FALLATK];// �v���C���[�ɗ^����_���[�W
	XMFLOAT4X4 m_mtxWorld[MAX_FALLATK]; // ���[���h�}�g���b�N�X



	bool     m_bUseLenge[MAX_ATKLENGENUM];  // �U���͈͎g���Ă��邩�ǂ���
	XMFLOAT3 m_posLenge[MAX_ATKLENGENUM];   // �z�u�ʒu�i�����͈͂̓���j
	float    m_radLenge[MAX_ATKLENGENUM];   // �_���[�W���̔��a
	XMFLOAT3 m_sclLenge[MAX_ATKLENGENUM];   // �����͈͂̑傫���i����X�A�c��Y�A���s��Z�j
	XMFLOAT3 m_rotLenge[MAX_ATKLENGENUM];   // ��]�p�x (�΂߂ɂ���Ƃ�����)
	XMFLOAT4X4 m_mtxWorldLenge[MAX_ATKLENGENUM]; // ���[���h�}�g���b�N�X
	int      m_nAtkSetTime[MAX_ATKLENGENUM];                   // �U�����~���Ă��鎞�Ԃ̊Ԋu
	bool     m_bLenge[MAX_ATKLENGENUM];                       // �v���C���[���U���͈͓��ɓ��������ǂ���
	//int      m_nLife[MAX_ATKLENGENUM];
	//CEnemyLife* m_pLife[MAX_ATKLENGENUM];  // ���C�t�|�C���^
	CEnemyLife  m_Life[MAX_ATKLENGENUM];  // ���C�t�|�C���^


	/*CCamera* m_pCamera;
	CLight*  m_pLight;*/
	CPlayer* m_pPlayer;
protected:

public:
	CFallAttack();
	~CFallAttack();

	HRESULT Init();
	void Update();
	void Uninit();
	void Draw();

	/*void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	void SetLight(CLight* pLight) { m_pLight = pLight; }*/
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }

	void SetAtkFall(void);
};