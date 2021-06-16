//=============================================================================
//
// モデル処理 [changeSpace.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
 //クラスの前方宣言
class CPlayer;
class CEnemyVacuum;
//*****************************************************************************
// 列挙体
//*****************************************************************************
namespace {
	enum {
		NONE_CHANGE = 0,
		RECTANDCIRCLE,
		CIRCLEANDCYLINDER,

		MAX_CHANGE
	}CHANGEMOVE_TYPE;
}

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CChangeSpace
{
private:
	XMFLOAT3			m_pos;
	//XMFLOAT3			m_rot;
	XMFLOAT3			m_scl;
	int                 m_nChangeType;
	int                 m_nChangeCylinderName;
	int                 m_nChangeCircleName;
	
	//XMVECTOR            m_vUp;
	bool                m_bVaild; // 連続切替防止用
	bool                m_bUseflg;
protected:

public:
	CChangeSpace();
	~CChangeSpace();

	//void SetChangeSpace(XMFLOAT3 f3Pos, XMFLOAT3 f3Scl, int nChangeType, int nChangeCylinderName, int nChangeCircleName, bool bUseflg);
	void SetChangeSpace(XMFLOAT3 f3Pos, XMFLOAT3 f3Scl, int nChangeType, int nChangeCircleName, bool bUseflg);

	void SetPos(XMFLOAT3 f3Pos) { m_pos = f3Pos; }
	void SetScl(XMFLOAT3 f3Scale) { m_scl = f3Scale; }

	bool GetUseflg(void) { return m_bUseflg; }
	int GetChangeType(void) { return m_nChangeType; }
	int GetChangeCylinderName(void) { return m_nChangeCylinderName; }
	int GetChangeCircleName(void) { return m_nChangeCircleName; }
	XMFLOAT3 GetPos(void) { return m_pos; }
	XMFLOAT3 GetScl(void) { return m_scl; }

	/*int ChangeField(XMFLOAT3 f3Point);
	int ChangeFieldRectAndCircle(XMFLOAT3 f3Point);
	int ChangeFieldCircle(XMFLOAT3 f3Point);*/

	int ChangeFieldRectAndCircle(XMFLOAT3 f3Point, int nNowField);
	//int ChangeFieldCircleAndCylinder(XMFLOAT3 f3Point, int nNowField);
	int ChangeFieldCircle(XMFLOAT3 f3Point);

	

};

////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
void SetChangeSpacePlayer(CPlayer* pPlayer);
void SetChangeSpaceEnemyVacuum(CEnemyVacuum* pEnemyVavuum);