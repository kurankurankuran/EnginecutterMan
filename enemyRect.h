////=============================================================================
////
//// モデル処理 [enemyRect.h]
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
//// マクロ定義
////*****************************************************************************
//#define MAX_XYZBOX (6)
//// クラスの前方宣言
//class CCamera;
//
////*****************************************************************************
//// クラス定義
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
//	// ローカルXYZベクトル
//	/*XMVECTOR         m_vEnemyX;
//	XMVECTOR         m_vEnemyY;
//	XMVECTOR         m_vEnemyZ;*/
//
//	// 大きさ(横幅X、高さY、奥行きZ)
//	XMFLOAT3         m_f3EnemyScl;
//
//	// 面の上方向識別用
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
////// プロトタイプ宣言
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
