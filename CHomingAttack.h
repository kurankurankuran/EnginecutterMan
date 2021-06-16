//=============================================================================
//
// モデル処理 [CHomingAttack.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"
//#include "Camera.h"

#include "Enemylife.h"

//#include "model.h"

//*****************************************************************************
// 定数宣言
//*****************************************************************************
#define MAX_HOMINGATK (20)
#define MAX_HOMINGATKSETNUM        (10)       // 攻撃判定を設置する最大数

class CPlayer;
//class CCamera;

class CHomingAttack
{
private:
	bool   m_bUseAtk[MAX_HOMINGATK];              // 攻撃判定使っているかどうか
	int    m_nHomingTime[MAX_HOMINGATK]; //追尾時間
	XMFLOAT3 m_pos[MAX_HOMINGATK];   // 配置位置（反応範囲の頭上）
	float    m_rad[MAX_HOMINGATK];   // ダメージ球の半径
	XMFLOAT3 m_scl[MAX_HOMINGATK];   // 反応範囲の大きさ（横幅X、縦幅Y、奥行きZ）
	XMFLOAT3 m_rot[MAX_HOMINGATK];   // 回転角度 (斜めにするときだけ)
	XMFLOAT3 m_move[MAX_HOMINGATK];  // 移動量
	int    m_nDamage[MAX_HOMINGATK];// プレイヤーに与えるダメージ
	int m_nLife[MAX_HOMINGATKSETNUM];
	XMFLOAT4X4 m_mtxWorld[MAX_HOMINGATK]; // ワールドマトリックス



	bool     m_bUseSet[MAX_HOMINGATKSETNUM];  // 攻撃範囲使っているかどうか
	XMFLOAT3 m_posSet[MAX_HOMINGATKSETNUM];   // 配置位置（反応範囲の頭上）
	float    m_radSet[MAX_HOMINGATKSETNUM];   // ダメージ球の半径
	XMFLOAT3 m_sclSet[MAX_HOMINGATKSETNUM];   // 反応範囲の大きさ（横幅X、縦幅Y、奥行きZ）
	XMFLOAT3 m_rotSet[MAX_HOMINGATKSETNUM];   // 回転角度 (斜めにするときだけ)
	XMFLOAT4X4 m_mtxWorldSet[MAX_HOMINGATKSETNUM]; // ワールドマトリックス
	int      m_nAtkSetTime[MAX_HOMINGATKSETNUM];                   // 攻撃が出現する時間の間隔
	/*int      m_nLifeSet[MAX_HOMINGATKSETNUM];*/

	CEnemyLife* m_pLife[MAX_HOMINGATKSETNUM];  // ライフポインタ
	CEnemyLife m_Life[MAX_HOMINGATKSETNUM];  // ライフポインタ

	/*CCamera* m_pCamera;
	CLight*  m_pLight;*/
	CPlayer* m_pPlayer;
protected:

public:
	CHomingAttack();
	~CHomingAttack();

	HRESULT Init();
	void Update();
	void Uninit();
	void Draw();

	/*void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	void SetLight(CLight* pLight) { m_pLight = pLight; }*/
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }

	void SetAtkHoming(int);
};