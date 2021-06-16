//=============================================================================
//
// 弾処理 [engine.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>
//#include "model.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENGINE             (16)  //エフェクトの数
#define VAL_MAXROT           (300)   // 単位時間回転数
#define BREAK_ROT           (270)         // 単位時間回転数の限界


#define ADD_VIBRATION	50		//.バイブレーションの強さの上昇値（自由値）
#define MAX_VIBRATION	(ADD_VIBRATION*1240)	//.バイブレーションの最大値

//*****************************************************************************
// 構造体定義
//*****************************************************************************
//**********構造体**********
typedef struct
{

	XMFLOAT2 pos;            //中心座標
	XMFLOAT2 posTexCoord;    //テクスチャ座標（左上）
	XMFLOAT2 sizeTexCoord;   //テクスチャサイズ（右下）（０．０－１．０）
	XMFLOAT4 Color;          //カラー
	int frame;             //アニメーション管理用フレーム
	int currentAnimNo;     //アニメーションのコマ番号（左上から０－）

}ST_ENGINE_PARAM;       //爆発を管理するパラメータの構造体


class CEnzine {
private:
	

	ST_ENGINE_PARAM 	g_engine[MAX_ENGINE];	//情報
	int g_nTimer;

	int             m_RotSpeed;          // 刃の回転速度

public:
	HRESULT Initengine();
	void Uninitengine();
	void Updateengine();
	void Drawengine();
	//int Fireengine(XMFLOAT2* pPos, int nDir);

	int GetRotSpeed(void) { return m_RotSpeed; }
	void SetRotSpeed(int nRotSpeed) {  m_RotSpeed = nRotSpeed; }

	CEnzine() { m_RotSpeed = 0; }
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//HRESULT Initengine();
//void Uninitengine();
//void Updateengine();
//void Drawengine();
////int Fireengine(XMFLOAT2* pPos, int nDir);


void SetEnzineControllerr(XINPUT_STATE* pControlller, XINPUT_VIBRATION *pVibration);
//void SetEnzinePlayer(CPlayer* pPlayer);
