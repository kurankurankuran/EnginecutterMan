//=============================================================================
//
// モデル処理 [model.cpp]
// Author :KAZUKI KURA
//
//=============================================================================
#include "enemy.h"
//#include "main.h"
//#include "input.h"
//
//#include "debugproc.h"
//#include "Collision.h"
//#include "model.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define MODEL_SPHERE		"data/model/sphere.fbx"
//#define ENEMY_CAR			"data/model/car001.fbx"
//#define MODEL_SPHERE		"data/model/CircleEnemyFBXtype.fbx"


//#define VALUE_MOVE_MODEL	(1.0f)
//#define	RATE_MOVE_MODEL		(0.20f)		// 移動慣性係数
//#define VALUE_ROTATE_MODEL	(3.6f)
//#define	RATE_ROTATE_MODEL	(0.20f)		// 回転慣性係数
//*****************************************************************************
// グローバル変数
//*****************************************************************************
//static CCamera*			g_pCamera;
//static CLight*			g_pLight;
//CAssimpModel		g_enemy;


CEnemy::CEnemy()
{
	//InitEnemy();
	
}

CEnemy::~CEnemy()
{

}
//
//void CEnemy::SetChangeSpace(XMFLOAT3 f3Pos, XMFLOAT3 f3Scl, int nChangeType)
//{
//	for (int i = 0; i < MAX_CHANGESPACE; ++i) {
//		if (m_changeSpace[i].GetUseflg())
//			continue;
//		m_changeSpace[i].SetChangeSpace(f3Pos, f3Scl, nChangeType, true);
//		break;
//	}
//}
//
//int CEnemy::ChangeField(XMFLOAT3 f3Point, int nNowOnFiled)
//{
//	int nChangeNum = nNowOnFiled;
//	for (int i = 0; i < MAX_CHANGESPACE; ++i) {
//		if (!m_changeSpace[i].GetUseflg())
//			continue;
//		/*if (m_changeSpace[i].GetChangeType() == nNowOnFiled)
//			continue;*/
//			/*if (!CollisionRectToPoint(m_changeSpace[i].GetPos(), m_changeSpace[i].GetScl(), f3Point))
//				continue;*/
//		switch (m_changeSpace[i].GetChangeType())
//		{
//		case RECTANDCIRCLE:
//			nChangeNum = m_changeSpace[i].ChangeFieldRectAndCircle(f3Point, nNowOnFiled);
//			break;
//		case CIRCLEANDCYLINDER:
//			nChangeNum = m_changeSpace[i].ChangeFieldCircleAndCylinder(f3Point, nNowOnFiled);
//			break;
//		}
//		break;
//	}
//
//	return nChangeNum;
//}


