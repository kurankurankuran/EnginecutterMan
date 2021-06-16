//=============================================================================
//
// ライフ表示処理 [EnemyEnemyLife.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"
//#include "Camera.h"

//class CCamera;
//*****************************************************************************
// マクロ宣言
//*****************************************************************************
//#define MAX_ENEMYLIFE			(100)							// ライフ
//#define ENEMYLIFE_NUM           (MAX_ENEMYLIFE/10)              // ライフ個数
#define MAX_ENEMYLIFE			(100)							// ライフ
#define ENEMYLIFE_NUM           (MAX_ENEMYLIFE/10)              // ライフ個数
//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CEnemyLife{
private:
	int     		m_nLife;
	/*int			    m_nStat[ENEMYLIFE_NUM];
	XMFLOAT3        m_nPos[ENEMYLIFE_NUM];*/
	int			    m_nStat;
	int             m_nNum;
	XMFLOAT3        m_Pos;
	XMFLOAT2        m_Scl;
	bool            m_bIraseflg;

public:
	CEnemyLife();
	CEnemyLife(XMFLOAT3 pos, int nLife,bool bIraseflg);
	~CEnemyLife();

	HRESULT Init(XMFLOAT3, int, bool);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void DelEnemyLife(int nValue);
	int GetLife(void);
	int GetStat(void) { return m_nStat; }
	//int GetEnemyLife(void);

	//int GetHP(void);

	//void SetCamera(CCamera*);
	
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
