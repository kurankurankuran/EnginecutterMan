//=============================================================================
//
// ���f������ [enemyCylinder.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"
//#include "Camera.h"
//#include "Light.h"
#include "enemy.h"
//#include "changeSpace.h"
//#include "model.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************


// �N���X�̑O���錾
//class CCamera;
//class CPlayer;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemyCylinder:public CEnemy
{
private:
	
protected:

public:
	HRESULT InitEnemy(void);
	void UninitEnemy(void);
	void UpdateEnemy(void);
	void DrawEnemy(void);

	//void SetEnemyCamera(CCamera*);
	//void SetEnemyLight(CLight*);
	//
	////********************************************************************
	//void SetEnemyCylinderPlayer(CPlayer* pPlayer);
	
};

////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//HRESULT InitEnemy(void);
//void UninitEnemy(void);
//void UpdateEnemy(void);
//void DrawEnemy(void);
//void SetEnemyRectCamera(CCamera*);
//void SetEnemyRectLight(CLight*);
//
//XMFLOAT3* GetPosEnemy(void);
//XMVECTOR* GetXYZEnemy(int nXYZ);


