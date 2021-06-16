//=============================================================================
//
// モデル処理 [enemy.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"
//#include "Camera.h"
//#include "Light.h"
//#include "AssimpModel.h"
//#include "enemyParts.h"
#include "meshKirime.h"
//#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_RAD  (150.0f)
#define MAX_CHANGESPACE (5)


// クラスの前方宣言
//class CCamera;
//class EnemyParts;
class CPlayer;
//class CAssimpModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy
{
private:
	XMFLOAT4X4		m_mtxWorld;

	XMFLOAT3		m_posModel;
	XMFLOAT3		m_rotModel;
	XMFLOAT3		m_sclModel;
	//CChangeSpace  m_changeSpace[MAX_CHANGESPACE];

	CKirime         m_Kirime;
	
	/*CCamera*		m_pCamera;
	CLight*			m_pLight;*/
	//CAssimpModel	m_enemy;
	CPlayer*        m_pPlayer;

	int             m_nAlphaType;

protected:
	

public:
	CEnemy();
	//CEnemy(XMFLOAT3 f3Pos) { m_posModel = f3Pos; }
	~CEnemy();

	virtual HRESULT InitEnemy(void) = 0;
	virtual void UninitEnemy(void) = 0;
	virtual void UpdateEnemy(void) = 0;
	virtual void DrawEnemy(void) = 0;


	//************************************************************************************************
	void SetmtxWorld(XMFLOAT4X4 mtxWorld) { m_mtxWorld = mtxWorld; }
	void SetPos(XMFLOAT3 f3Pos) { m_posModel = f3Pos; }
	void SetRot(XMFLOAT3 f3Rot) { m_rotModel = f3Rot; }
	void SetScl(XMFLOAT3 f3Scl) { m_sclModel = f3Scl; }
	/*void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	void SetEnemyLight(CLight* pLight) { m_pLight = pLight; }*/
	void SetEnemyPlayer(CPlayer* pPlayer) { 
		m_pPlayer = pPlayer;
		//m_Kirime.SetmeshKirimePlayer(pPlayer);
	}
	void SetAlphaType(int nType) { m_nAlphaType = nType; }
	//*************************************************************************************************
	XMFLOAT4X4* GetmtxWorld() { return &m_mtxWorld; }
	XMFLOAT3 GetPos() { return m_posModel; }
	XMFLOAT3 GetRot() { return m_rotModel; }
	XMFLOAT3 GetScl() { return m_sclModel; }
	//CAssimpModel GetModel() { return	m_enemy; }
	/*CCamera* GetCamera(void) { return m_pCamera; }
	CLight*  GetLight(void) { return m_pLight; }*/
	CPlayer* GetPlayer(void) { return m_pPlayer; }
	int GetAlphaType(void) { return m_nAlphaType; }
	//************************************************************************************************
	HRESULT InitmeshKirime(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f)
	{
		return m_Kirime.InitmeshKirime();
		//return m_Kirime.InitmeshKirime(nNumBlockX =7, nNumBlockZ = 7, fSizeBlockX = 1.0f, fSizeBlockZ = 1.0f, fTexSizeX , fTexSizeZ );
	}
	void UninitmeshKirime(void) { m_Kirime.UninitmeshKirime(); }
	void UpdatemeshKirime(void) { m_Kirime.UpdatemeshKirime(); }
	void DrawmeshKirime(void) { m_Kirime.DrawmeshKirime(); }
	int CollisionKirime(DirectX::XMFLOAT3 vPos,XMFLOAT3 vPosPlayer, float Long,
		float Width, float High,
		int nDamage, DirectX::XMFLOAT3* pvHitPos)
	{
		return m_Kirime.CollisionKirime(vPos, vPosPlayer,Long, Width, High, nDamage, pvHitPos);
	}
	XMFLOAT3 GetVecXYZ() { return m_Kirime.GetVecXYZ(); }
	XMFLOAT3*GetKakunoPos() { return m_Kirime.GetKakunoPos(); }
	CKirime* GetKirime(void) { return &m_Kirime; }
	/*HRESULT MakeVertexKirime(ID3D11Device* pDevice,
		int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
		float fTexSizeX, float fTexSizeZ)
	{
		return m_Kirime.MakeVertexKirime(pDevice, nNumBlockX, nNumBlockZ, fSizeBlockX, fSizeBlockZ, fTexSizeX, fTexSizeZ);
	}*/

};
