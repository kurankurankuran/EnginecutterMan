////=============================================================================
////
//// ���f������ [enemyRect.h]
//// Author : KAZUKI KURA
////
////=============================================================================
//#pragma once
//
//#include "main.h"
//#include "Camera.h"
//#include "Light.h"
//#include "enemy.h"
//#include "XYZBox.h"
//
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//#define MAX_XYZBOX (6)
//// �N���X�̑O���錾
//class CCamera;
//
////*****************************************************************************
//// �N���X��`
////*****************************************************************************
//class CEnemyRect:public CEnemy
//{
//private:
//	/*XMFLOAT4X4		m_mtxWorld;
//
//	XMFLOAT3			m_posModel;
//	XMFLOAT3			m_rotModel;
//	XMFLOAT3			m_sclModel;*/
//
//	// ���[�J��XYZ�x�N�g��
//	/*XMVECTOR         m_vEnemyX;
//	XMVECTOR         m_vEnemyY;
//	XMVECTOR         m_vEnemyZ;*/
//
//	// �傫��(����X�A����Y�A���s��Z)
//	XMFLOAT3         m_f3EnemyScl;
//
//	// �ʂ̏�������ʗp
//	CXYZBox          m_XYZBox[MAX_XYZBOX];
//protected:
//
//public:
//	HRESULT InitEnemy(void);
//	void UninitEnemy(void);
//	void UpdateEnemy(void);
//	void DrawEnemy(void);
//	XMFLOAT3* GetPosEnemy(void);
//
//	void SetEnemyCamera(CCamera*);
//	void SetEnemyLight(CLight*);
//	void SetEnemyX(XMFLOAT3 f3X);
//	void SetEnemyY(XMFLOAT3 f3Y);
//	void SetEnemyZ(XMFLOAT3 f3Z);
//
//	XMVECTOR GetXYZEnemy(int nXYZ);
//	
//
//
//	//*****************************************
//	void SetXYZBox(XMFLOAT3 f3Pos, XMFLOAT3 f3Scl, int nXYZBox);
//	int GetVecY(XMFLOAT3 f3Pos);
//	//*****************************************
//};
//
//////*****************************************************************************
////// �v���g�^�C�v�錾
//////*****************************************************************************
////HRESULT InitEnemy(void);
////void UninitEnemy(void);
////void UpdateEnemy(void);
////void DrawEnemy(void);
////void SetEnemyRectCamera(CCamera*);
////void SetEnemyRectLight(CLight*);
////
////XMFLOAT3* GetPosEnemy(void);
////XMVECTOR* GetXYZEnemy(int nXYZ);
