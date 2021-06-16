//=============================================================================
//
// モデル処理 [model.h]
// Author :KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"
#include "Camera.h"
#include "Light.h"
#include "enemyRushVacuum.h"
#include "life.h"
#include "engine.h"
//#include "enemyVacuum.h"
#include <Xinput.h>
//*****************************************************************************
// マクロ宣言
//*****************************************************************************
#define RUSHPLAYER_POSX (0.0f)
#define RUSHPLAYER_POSY (275.0f)
#define RUSHPLAYER_POSZ (800.0f)
#define BREAK_ROT           (270)         // 単位時間回転数の限界
//*****************************************************************************
// 列挙体
//*****************************************************************************


// クラスの前方宣言
class CCamera;
//class Enemy;
class CEnemyVacuum;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRushPlayer
{
private:
	//int             m_nLife;            // ライフ

	XMFLOAT4X4		m_mtxWorld;         // ワールド行列

	XMFLOAT3		m_posModel;         // 座標用
	XMFLOAT3		m_posCut;         // 刃座標用
	XMFLOAT3        m_vecCut;         // 刃ベクトル

	XMFLOAT3		m_sclModel;         // 大きさ調整用

	XMFLOAT3		m_moveModel;	    // 移動量

	XMFLOAT3        m_f3modelY;         // プレイヤーの上方向ベクトル
	
 	XMFLOAT3        m_vecRotDest;                    // 目的方向ベクトル
	XMFLOAT3        m_vecNowRot;
	
	
	XMFLOAT4X4      m_VRotToNextMat4x4;    // ワンフレーム毎の運動量分プレイヤー座標を回転させる行列
	XMFLOAT4X4      m_RotMat4x4;           // 常に球体の中心を下にするための回転行列
	XMFLOAT4X4      m_RotationY4x4;                         // プレイヤーの上ベクトル軸回転行列
	XMFLOAT4X4      m_RotationNor4x4;                       // プレイヤーの向きを変える回転行列


	int             m_nOnFieldType;                    // 乗っている地形の形状
	int             m_nOnFieldName;                    // 乗っている地形の名称

	bool            m_bBreak;            // 暴走フラグ

	CLife           m_Life;
	int             m_nInvTime;         // 無敵時間
	bool            m_bInvflg;          // 無敵フラグ
	CEnzine         m_Enzine;

	float           m_fRazianDest;
	float           m_fRazian; 

	bool            m_bJumpflg;
	//XMFLOAT3        m_f3JumpMoveDest;
	XMFLOAT3        m_f3JumpMoveUp;
	XMFLOAT3        m_f3JumpMoveDown;
	bool            m_bMove;

	int             m_nModelType;

protected:

public:
	
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CRushPlayer() {}
	CRushPlayer(int nLife) :m_Life(nLife)
	{
		Init();
	}
	~CRushPlayer() 
	{
		Uninit();
	}


	XMFLOAT3 GetModelY(void) { return m_f3modelY; }
	XMFLOAT3 GetPosModel(void) { return m_posModel; }
	XMFLOAT3 GetPosCut(void) { return m_posCut; }
	XMFLOAT4X4/*XMMATRIX */GetRotateMatrix(void){ return m_VRotToNextMat4x4;}
	XMFLOAT4X4/*XMMATRIX */ GetTotateWorldMatrix(void){ return m_RotMat4x4; }
	int GetOnField(void) { return m_nOnFieldType; }
	int GetOnFieldName(void) { return m_nOnFieldName; }
	bool GetBreakflg(void) { return m_bBreak; }
	XMFLOAT3 GetMove(void) { return m_moveModel; }
	XMFLOAT4X4/*XMMATRIX */ GetRotateNorMatrix(void){ return m_RotationNor4x4; }
	CEnzine* GetEnzine(void) { return &m_Enzine; }
	CLife* GetLife(void) { return &m_Life; }
	void SetPlayerCamera(CCamera*);
	void SetPlayerLight(CLight*);
	void SetPlayerEnemy(CEnemyRushVacuum* pEnemy);
	void SetOnFieldType(int nType) { m_nOnFieldType = nType; }
	void SetOnFieldName(int nName) { m_nOnFieldName = nName; }
	void SetInvflg(bool bInv) { m_bInvflg = bInv; }
	void SetMoveflg(bool bMove) { m_bMove = bMove; }
	//void DamageLife(int nDamage) { m_nLife -= nDamage; }
};


void SetRushPlayerController(XINPUT_STATE* pControlller);
//XMVECTOR  GetModelNor(void);