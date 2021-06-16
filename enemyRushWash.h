//=============================================================================
//
// ���f������ [enemyVacuum.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"
#include "enemy.h"
#include "enemySpher.h"
#include "Enemylife.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_COREPARTS_NUM               (11)
#define MAX_CORELIFE_BER                (3)

// �N���X�̑O���錾
//class CCamera;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemyRushWash/*:public CEnemy*/
{
private:
	CEnemySpher     m_SpherFront;
	
	bool            m_bDeathflg;
	int             m_nDeathEffectTime;

	XMFLOAT3        m_Pos[MAX_COREPARTS_NUM];
	XMFLOAT3        m_Scl[MAX_COREPARTS_NUM];
	XMFLOAT3        m_Rot[MAX_COREPARTS_NUM];
	XMFLOAT4X4      m_mtxWorld[MAX_COREPARTS_NUM];
	int             m_nEffectCnt; // ���������Ԋu�J�E���g

	//CEnemyLife*      m_pLife[MAX_CORELIFE_BER];
	CEnemyLife      m_Life[MAX_CORELIFE_BER];

protected:

public:
	HRESULT InitEnemy(void);
	void UninitEnemy(void);
	void UpdateEnemy(void);
	void DrawEnemy(void);

	/*CEnemyRushWash()
	{
		InitEnemy();
	}
	~CEnemyRushWash()
	{
		UninitEnemy();
	}*/

	//void SetCamera(CCamera*);
	//void SetLight(CLight*);

	CEnemySpher* GetSpherFrontP(void) { return &m_SpherFront; }
	CEnemyLife* GetInstansEnemyLife(void); 

	bool GetDeathflg(void) { return m_bDeathflg; }

	void CheckClearflg(void);
};

