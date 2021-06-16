//=============================================================================
//
// フェード処理 [fade.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#pragma once

#include "main.h"

// フェード処理
enum EFade
{
	FADE_NONE = 0,		// 何もしていない
	FADE_IN,						// フェードイン処理
	FADE_OUT,					// フェードアウト処理
	
	MAX_FADE
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFade();
void UninitFade();
void UpdateFade();
void DrawFade();

void SetFadeOut(int nNextScene);						// フェードアウト開始(次のシーン)
int	GetFade();														// フェードイン / アウトの状態
void SetFadeColor(float fR, float fG, float fB);	// フェードアウト時の画面の色