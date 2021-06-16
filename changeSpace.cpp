//=============================================================================
//
// モデル処理 [changeSpace.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
//#include "main.h"
#include "changeSpace.h"
#include "Collision.h"
#include "enemyVacuum.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CPlayer*      g_pPlayer;
static CEnemyVacuum* g_pEnemyVacuum;

CChangeSpace::CChangeSpace()
{
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scl = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_nChangeType = NONE_CHANGE;
	m_bUseflg = false;
	m_bVaild = true;
}

CChangeSpace::~CChangeSpace()
{

}

void CChangeSpace::SetChangeSpace(XMFLOAT3 f3Pos, XMFLOAT3 f3Scl, int nChangeType, int nChangeCircleName, bool bUseflg)
{
	m_pos = f3Pos;
	m_scl = f3Scl;
	m_nChangeType = nChangeType;
	//m_nChangeCylinderName = nChangeCylinderName;
	m_nChangeCircleName = nChangeCircleName;
	m_bUseflg = bUseflg;
}



int CChangeSpace::ChangeFieldRectAndCircle(XMFLOAT3 f3Point ,int nNowField)
{
	int nNextFieldType = nNowField;
	if (CollisionRectToPoint(m_pos, m_scl, f3Point)) {
		if (m_bVaild)
		{
			switch (nNowField)
			{
			case RECT_FIELD:
				nNextFieldType = CIRCLE_FIELD;
				break;
			case CIRCLE_FIELD:
				nNextFieldType = RECT_FIELD;
				break;
			}
		}
		m_bVaild = false;
	}
	else
	{
		m_bVaild = true;
	}

	return nNextFieldType;
}


int CChangeSpace::ChangeFieldCircle(XMFLOAT3 f3Point)
{
	int nNextFieldType;
	if (CollisionRectToPoint(m_pos, m_scl, f3Point)) {
		nNextFieldType = CIRCLE_FIELD;
		
			g_pPlayer->SetOnFieldType(CIRCLE_FIELD);
			g_pPlayer->SetOnFieldName(m_nChangeCircleName);
			
	}
	else
	{
		m_bVaild = true;
		nNextFieldType = CYLINDER_FIELD;
		
		g_pPlayer->SetOnFieldType(CYLINDER_FIELD);
		g_pPlayer->SetOnFieldName(m_nChangeCylinderName);
	}

	return nNextFieldType;
}
//=============================================================================
// プレイヤーオブジェクト取得
//=============================================================================
void SetChangeSpacePlayer(CPlayer* pPlayer)
{
	g_pPlayer = pPlayer;
}

//=============================================================================
// エネミーオブジェクト取得
//=============================================================================
void SetChangeSpaceEnemyVacuum(CEnemyVacuum* pEnemyVavuum)
{
	g_pEnemyVacuum = pEnemyVavuum;
}
