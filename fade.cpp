//=============================================================================
//
// フェードイン / アウト処理 [fade.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "fade.h"
#include "polygon.h"
#include "scene.h"
//#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_RATE		0.02f			// フェードの刻み

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static float	g_fRed		= 0.0f;			// 赤(フェードアウト色)
static float	g_fGreen	= 0.0f;			// 緑(フェードアウト色)
static float	g_fBlue	= 0.0f;			    // 青(フェードアウト色)
static float	g_fAlpha	= 1.0f;			// 不透明度
static int		g_nFade	= FADE_IN;		// 現在の状態
static int		g_nNextScene = SCENE_TITLE;	// 次のシーン

//=============================================================================
// フェードイン / アウトの初期化処理
//=============================================================================
HRESULT InitFade()
{
	g_fAlpha	= 1.0f;			// 不透明度
	g_nFade	= FADE_IN;			// 状態
	g_nNextScene = SCENE_TITLE;	// 次のシーン

	return S_OK;
}

//=============================================================================
// フェードイン / アウトの終了処理
//=============================================================================
void UninitFade()
{
	// (何もしない)
}

//=============================================================================
// フェードイン / アウトの更新処理
//=============================================================================
void UpdateFade()
{
	// 何もしていない
	if (g_nFade == FADE_NONE)
	{
		return;
		
	}
	

	// --- フェードアウト処理 ---
	if (g_nFade == FADE_OUT)
	{
		g_fAlpha += FADE_RATE;		// 不透明度を増加
		if (g_fAlpha >= 1.0f)
		{
			// フェードイン処理に切り替え
			g_fAlpha	= 1.0f;
			g_nFade	= FADE_IN;
			// シーン切り替え
			SetScene(g_nNextScene);
		}
		// BGMもフェードアウト
		//CSound::SetVolume(1.0f - g_fAlpha);
		return;
	}

	// --- フェードイン処理 ---
	g_fAlpha -= FADE_RATE;			// 不透明度を下げる
	if (g_fAlpha <= 0.0f)
	{
		// フェードアウト処理終了
		g_fAlpha = 0.0f;
		g_nFade = FADE_NONE;
	}
	// ボリュームもフェードイン
	//CSound::SetVolume(1.0f - g_fAlpha);

}

//=============================================================================
// フェードイン / アウト処理
//=============================================================================
void DrawFade()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// フェードイン / アウト
		// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない

	SetPolygonPos(0.0f, 0.0f);
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetPolygonTexture(nullptr);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);

	SetPolygonColor(g_fRed, g_fGreen, g_fBlue);
	SetPolygonAlpha(g_fAlpha);

	DrawPolygon(GetDeviceContext());
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
	SetZWrite(true);
	SetBlendState(BS_NONE);			// アルファブレンド無効
}

//=============================================================================
// フェードアウト開始(次のシーン)
//=============================================================================
void SetFadeOut(int nNextScene)	
{
	if (g_nFade != FADE_OUT)
	{
		g_nFade = FADE_OUT;
		g_nNextScene = nNextScene;
	}
}
//=============================================================================
// フェードイン / アウトの状態の取得
//=============================================================================
int GetFade()		
{
	return g_nFade;
}
//=============================================================================
// フェードアウト時の画面の色を設定
//=============================================================================
void SetFadeColor(float fR, float fG, float fB)	
{
	g_fRed	= fR;
	g_fGreen = fG;
	g_fBlue	= fB;
}