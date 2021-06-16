//=============================================================================
//
// �G�p�[�c���� [enemyParts.h]
// Author : �O��@�G���Y
//
//=============================================================================
#pragma once

#include "main.h"

#include "smoke.h"

//*****************************************************************************
// �萔�錾
//*****************************************************************************
#define MAX_ENEMY_PARTS		(4)			// �G�̃p�[�c��

// �N���X�̑O���錾
//class CCamera;
//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct {
	XMFLOAT3 f3Pos;
	XMFLOAT3 f3Scl;
}GravitySpace;


class EnemyParts
{
private:
	bool   m_bStat[MAX_ENEMY_PARTS];                           //�g�p���t���O
	bool   m_fall[MAX_ENEMY_PARTS];							  // �������t���O
	bool   m_fallenParts[MAX_ENEMY_PARTS];					  // ���ɗ������p�[�c�t���O
	XMFLOAT3 m_posEnemy[MAX_ENEMY_PARTS];     // ���݂̈ʒu
	XMFLOAT3 m_rotEnemy[MAX_ENEMY_PARTS];	  // ���݂̌���
	XMFLOAT3 m_sclEnemy[MAX_ENEMY_PARTS];	  // �傫��
	XMFLOAT3 m_moveEnemy[MAX_ENEMY_PARTS];    // �ړ���
	XMFLOAT4X4 m_mtxWorld[MAX_ENEMY_PARTS];   // ���[���h�}�g���b�N�X
	int m_nHp[MAX_ENEMY_PARTS];				  // �p�[�c�����t���O
	XMFLOAT3 m_fPower[MAX_ENEMY_PARTS];		  // ������ԗ͂̏���

	float m_rad;  //�n�ʔ���p
	float m_fAxisRot[MAX_ENEMY_PARTS]; // ���R��]�p�x
	GravitySpace m_GravitySpace[MAX_ENEMY_PARTS]; // �n�ʔ���p

	CSmoke m_Smoke;

	
protected:

public:
	
	EnemyParts();
	~EnemyParts();
	HRESULT InitEnemy();
	void UpdateEnemy();
	void UninitEnemy();
	void DrawEnemy();

	/*void SetCamera(CCamera* pCamera);
	void SetLight(CLight* pLight);*/
	
	float GetRad() { return m_rad; }
	
	int ColGravitySpace(XMFLOAT3);
};

void SetControllerrEnemyParts(XINPUT_STATE* pControlller);