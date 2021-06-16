//=============================================================================
//
// ���f������ [enemyAttack.h]
// Author : RYOOSUKE�@YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
//#include "Camera.h"
#include "Light.h"

//#include "model.h"
#include "CFallAttack.h"
#include "CHomingAttack.h"

//*****************************************************************************
// �萔�錾
//*****************************************************************************
//class CCamera;
class CPlayer;

//******************************************************************************
// �N���X��`
//****************************************************************************** 

class EnemyAttack
{
private:
	CFallAttack     m_FallAtk;
	CHomingAttack   m_HomingAtk;
protected:

public:
	EnemyAttack();
	~EnemyAttack();

	HRESULT Init();
	void Update();
	void Uninit();
	void Draw();

	/*void SetCamera(CCamera* pCamera) { 
		m_FallAtk.SetCamera(pCamera); 
		m_HomingAtk.SetCamera(pCamera);
	}
	void SetLight(CLight* pLight) { 
		m_FallAtk.SetLight(pLight); 
		m_HomingAtk.SetLight(pLight);
	}*/
	void SetPlayer(CPlayer* pPlayer) { 
		m_FallAtk.SetPlayer(pPlayer); 
		m_HomingAtk.SetPlayer(pPlayer);
	}
};