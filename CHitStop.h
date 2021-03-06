//=============================================================================
//
// ヒットストップ処理 [CHitStop.h]
// Author : KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"

//******************************************************************************
// クラス定義
//****************************************************************************** 

class CHitStop
{
private:
	bool m_bStopflg;// ヒットストップフラグ
	int  m_nTimer;  // ヒットストップ時間
	
protected:

public:
	CHitStop();
	~CHitStop();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void HitStopOn(int); //int型で時間指定
	bool GetHitStopflg(void); // ヒットストップ中フラグ

	static CHitStop* Get();
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

