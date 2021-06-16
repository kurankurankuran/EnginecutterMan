//=============================================================================
//
// モデル処理 [CFallAttack.h]
// Author : RYOOSUKE　YAMADA
//
//=============================================================================
#pragma once

#include "main.h"
//#include "Camera.h"
#include "Light.h"
#include "Enemylife.h"


//*****************************************************************************
// 定数宣言
//*****************************************************************************
#define MAX_FALLATK (10)
#define MAX_ATKLENGENUM        (10)       // 攻撃判定を設置する最大数

class CPlayer;
class CCamera;
//******************************************************************************
// クラス定義
//****************************************************************************** 

class CFallAttack
{
private:
	bool   m_bUseAtk[MAX_FALLATK];              // 攻撃判定使っているかどうか
	bool   m_fall[MAX_FALLATK];    // 反応範囲に入るとtrueにする
	int    m_nFallTime[MAX_FALLATK]; //落ちる時間
	XMFLOAT3 m_pos[MAX_FALLATK];   // 配置位置（反応範囲の頭上）
	float    m_rad[MAX_FALLATK];   // ダメージ球の半径
	XMFLOAT3 m_scl[MAX_FALLATK];   // 反応範囲の大きさ（横幅X、縦幅Y、奥行きZ）
	XMFLOAT3 m_rot[MAX_FALLATK];   // 回転角度 (斜めにするときだけ)
	XMFLOAT3 m_move[MAX_FALLATK];  // 移動量
	int    m_nDamage[MAX_FALLATK];// プレイヤーに与えるダメージ
	XMFLOAT4X4 m_mtxWorld[MAX_FALLATK]; // ワールドマトリックス



	bool     m_bUseLenge[MAX_ATKLENGENUM];  // 攻撃範囲使っているかどうか
	XMFLOAT3 m_posLenge[MAX_ATKLENGENUM];   // 配置位置（反応範囲の頭上）
	float    m_radLenge[MAX_ATKLENGENUM];   // ダメージ球の半径
	XMFLOAT3 m_sclLenge[MAX_ATKLENGENUM];   // 反応範囲の大きさ（横幅X、縦幅Y、奥行きZ）
	XMFLOAT3 m_rotLenge[MAX_ATKLENGENUM];   // 回転角度 (斜めにするときだけ)
	XMFLOAT4X4 m_mtxWorldLenge[MAX_ATKLENGENUM]; // ワールドマトリックス
	int      m_nAtkSetTime[MAX_ATKLENGENUM];                   // 攻撃が降ってくる時間の間隔
	bool     m_bLenge[MAX_ATKLENGENUM];                       // プレイヤーが攻撃範囲内に入ったかどうか
	//int      m_nLife[MAX_ATKLENGENUM];
	//CEnemyLife* m_pLife[MAX_ATKLENGENUM];  // ライフポインタ
	CEnemyLife  m_Life[MAX_ATKLENGENUM];  // ライフポインタ


	/*CCamera* m_pCamera;
	CLight*  m_pLight;*/
	CPlayer* m_pPlayer;
protected:

public:
	CFallAttack();
	~CFallAttack();

	HRESULT Init();
	void Update();
	void Uninit();
	void Draw();

	/*void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	void SetLight(CLight* pLight) { m_pLight = pLight; }*/
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }

	void SetAtkFall(void);
};