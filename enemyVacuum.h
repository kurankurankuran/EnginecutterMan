//=============================================================================
//
// モデル処理 [enemyVacuum.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"
#include "enemy.h"
#include "enemySpher.h"
//#include "enemyRect.h"
#include "enemyCylinder.h"
#include "enemyParts.h"
#include "enemyAttack.h"
#include "changeSpace.h"
#include "meshKirime.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define MAX_VACUUMPARTS_NUM     (4)


// クラスの前方宣言
//class CCamera;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemyVacuum/*:public CEnemy*/
{
private:
	/*XMFLOAT3 c = XMFLOAT3(200.0f, 220.0f, 75.0f);*/

	CEnemySpher     m_SpherFront;
	//CEnemySpher     m_SpherBack;
	//CEnemyRect      m_Rect;
	CEnemyCylinder  m_Cylinder;
	EnemyParts      m_Parts;
	
	EnemyAttack     m_EnemyAttack;

	CChangeSpace    m_changeSpace[MAX_CHANGESPACE];

	
	XMFLOAT3        m_Pos[MAX_VACUUMPARTS_NUM];
	XMFLOAT3        m_Scl[MAX_VACUUMPARTS_NUM];
	XMFLOAT3        m_Rot;
	XMFLOAT4X4      m_mtxWorld[MAX_VACUUMPARTS_NUM];

	//CPlayer*        m_pPlayer;

protected:

public:
	HRESULT InitEnemy(void);
	void UninitEnemy(void);
	void UpdateEnemy(void);
	void DrawEnemy(void);

	CEnemyVacuum()
	{
		/*InitEnemy();*/
	}
	~CEnemyVacuum()
	{
		/*UninitEnemy();*/
	}

	/*void SetCamera(CCamera*);
	void SetVacuumEnemyLight(CLight*);*/

	
	void SetEnemyPlayer(CPlayer* pPlayer)
	{ 
		m_EnemyAttack.SetPlayer(pPlayer); 
		m_Cylinder.SetEnemyPlayer(pPlayer);
		m_SpherFront.SetEnemyPlayer(pPlayer);
		//m_SpherBack.SetEnemyPlayer(pPlayer);
		m_EnemyAttack.SetPlayer(pPlayer);
		
		//m_pPlayer = pPlayer;
	}
	
	CEnemySpher* GetSpherFrontP(void) { return &m_SpherFront; }
	//CEnemySpher* GetSpherBackP(void) { return &m_SpherBack; }

	CEnemyCylinder* GetCylinderP(void) { return &m_Cylinder; }
	
	EnemyParts* GetEnemyParts(void) { return &m_Parts; }

	int ChangeField(XMFLOAT3 f3Point, int nNowOnField);
	void SetChangeSpace(XMFLOAT3 f3Pos, XMFLOAT3 f3Scl, int nChangeType , int nChangeCircleName);
	

};

