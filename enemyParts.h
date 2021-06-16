//=============================================================================
//
// 敵パーツ処理 [enemyParts.h]
// Author : 三宅　秀太郎
//
//=============================================================================
#pragma once

#include "main.h"

#include "smoke.h"

//*****************************************************************************
// 定数宣言
//*****************************************************************************
#define MAX_ENEMY_PARTS		(4)			// 敵のパーツ数

// クラスの前方宣言
//class CCamera;
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct {
	XMFLOAT3 f3Pos;
	XMFLOAT3 f3Scl;
}GravitySpace;


class EnemyParts
{
private:
	bool   m_bStat[MAX_ENEMY_PARTS];                           //使用中フラグ
	bool   m_fall[MAX_ENEMY_PARTS];							  // 落下中フラグ
	bool   m_fallenParts[MAX_ENEMY_PARTS];					  // 床に落ちたパーツフラグ
	XMFLOAT3 m_posEnemy[MAX_ENEMY_PARTS];     // 現在の位置
	XMFLOAT3 m_rotEnemy[MAX_ENEMY_PARTS];	  // 現在の向き
	XMFLOAT3 m_sclEnemy[MAX_ENEMY_PARTS];	  // 大きさ
	XMFLOAT3 m_moveEnemy[MAX_ENEMY_PARTS];    // 移動量
	XMFLOAT4X4 m_mtxWorld[MAX_ENEMY_PARTS];   // ワールドマトリックス
	int m_nHp[MAX_ENEMY_PARTS];				  // パーツ落下フラグ
	XMFLOAT3 m_fPower[MAX_ENEMY_PARTS];		  // 吹き飛ぶ力の初速

	float m_rad;  //地面判定用
	float m_fAxisRot[MAX_ENEMY_PARTS]; // 自由回転角度
	GravitySpace m_GravitySpace[MAX_ENEMY_PARTS]; // 地面判定用

	CSmoke m_Smoke;

	
protected:

public:
	
	EnemyParts();
	~EnemyParts();
	HRESULT InitEnemy();
	void UpdateEnemy();
	void UninitEnemy();
	void DrawEnemy();

	/*void SetCamera(CCamera* pCamera);
	void SetLight(CLight* pLight);*/
	
	float GetRad() { return m_rad; }
	
	int ColGravitySpace(XMFLOAT3);
};

void SetControllerrEnemyParts(XINPUT_STATE* pControlller);