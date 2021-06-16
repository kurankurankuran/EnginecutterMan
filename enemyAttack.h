//=============================================================================
//
// モデル処理 [enemyAttack.h]
// Author : RYOOSUKE　YAMADA
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
// 定数宣言
//*****************************************************************************
//class CCamera;
class CPlayer;

//******************************************************************************
// クラス定義
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