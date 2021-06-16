//=============================================================================
//
// モデル処理 [enemyVacuum.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"
#include "enemy.h"
//#include "enemySpher.h"
#include "enemyRushWash.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

// クラスの前方宣言
//class CCamera;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemyRushVacuum/*:public CEnemy*/
{
private:
	//CEnemySpher     m_SpherFront;
	CEnemyRushWash  m_Wash;
	
	XMFLOAT3        m_Pos;
	XMFLOAT3        m_Scl;
	XMFLOAT3        m_Rot;
	XMFLOAT4X4      m_mtxWorld;

protected:

public:
	HRESULT InitEnemy(void);
	void UninitEnemy(void);
	void UpdateEnemy(void);
	void DrawEnemy(void);

	CEnemyRushVacuum()
	{
		InitEnemy();
	}
	~CEnemyRushVacuum()
	{
		UninitEnemy();
	}

	/*void SetCamera(CCamera*);
	void SetVacuumEnemyLight(CLight*);*/

	//CEnemySpher* GetSpherFrontP(void) { return &m_SpherFront; }
	CEnemyRushWash* GetWash(void) { return &m_Wash; }
};

