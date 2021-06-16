//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "CLEAR.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
//#include "scene.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CLEAR_TEXTURENAME		L"data/texture/GAME_CLEAR.png"			// テクスチャファイル名
#define	START_TEXTURENAME		L"data/texture/space_game.png"	// テクスチャファイル名

#define	TITLE_POS_X		0								// ポリゴンの表示位置
#define	TITLE_POS_Y		0								// ポリゴンの表示位置
#define	TITLE_SIZE_X		SCREEN_WIDTH		// ポリゴンのサイズ
#define	TITLE_SIZE_Y		SCREEN_HEIGHT	// ポリゴンのサイズ

#define	START_POS_X			0					// スタートボタンの位置(X座標)
#define	START_POS_Y			-170			// スタートボタンの位置(Y座標)
#define	START_WIDTH		550				// スタートボタンの幅
#define	START_HEIGHT		100				// スタートボタンの高さ

#define	COUNT_APPEAR		(60 * 0.5f /* 秒 */)	// スタートボタン出現までの時間
#define	INTERVAL_DISP		(60 * 1	 /* 秒 */)		// スタートボタン点滅の時間

//*****************************************************************************
// 定数
//*****************************************************************************
enum ETex
{
	TEX_TITLE = 0,
	TEX_START,

	MAX_TEX
};

static LPCWSTR g_pszTexFNameClear[] =
{
	CLEAR_TEXTURENAME,
	START_TEXTURENAME
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextureClear[MAX_TEX];	// テクスチャ
static int			g_nCountAppear = 0;		//
static int			g_nCountDisp = 0;			//
static bool		g_bDispStart = false;		//
static bool		g_bStart = false;			// スタートボタン押下
static int		g_stat;		// 状態
static float    g_Alpha = 0.0f;
static bool		g_AStart = false;
static XINPUT_STATE* g_pXController;


static bool         g_bLoadMesh = false;
//=============================================================================
// タイトル画面の初期化処理
//=============================================================================
HRESULT InitClear()
{
	HRESULT hr = S_OK;
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		// テクスチャ読み込み
		for (int i = 0; i < MAX_TEX; i++)
		{
			hr = CreateTextureFromFile(GetDevice(), g_pszTexFNameClear[i], &g_pTextureClear[i]);
			if (FAILED(hr))
			{
				MessageBoxW(GetMainWnd(), g_pszTexFNameClear[i],
					L"テクスチャ読み込みエラー", MB_ICONSTOP | MB_OK);
				return hr;
			}
			SaveTex(g_pTextureClear[i]);
		}
		g_bLoadMesh = true;
	}
	// 変数初期化
	g_nCountAppear = COUNT_APPEAR;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_bStart = false;
	g_stat = 0;

	// BGM再生開始
	//CSound::Play(BGM_TITLE);

	return hr;
}

//=============================================================================
// タイトル画面の終了処理
//=============================================================================
void UninitClear()
{
	// テクスチャ開放

#if MESH_RELEASE_ON
	for (int i = 0; i < MAX_TEX; ++i)
	{
		SAFE_RELEASE(g_pTextureClear[i]);
		if (g_stat)
		{
			g_stat = 0;
		}
	}
#endif
}

//=============================================================================
// タイトル画面の更新処理
//=============================================================================
void UpdateClear()
{
	
	if (g_AStart==true)
	{
		--g_nCountAppear;
		if (g_nCountAppear <= 0)
		{
			g_nCountDisp = (g_nCountDisp + 1) % 80;
			g_bDispStart = (g_nCountDisp <= INTERVAL_DISP);
		}

		if (g_bStart)
		{
			g_bDispStart = g_nCountAppear & 20;
		}


		g_bStart = true;
	}
		
	
}

//=============================================================================
// タイトル画面の描画処理
//=============================================================================
void DrawClear()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	for (int i = 0; i < MAX_TEX; ++i)
	{
		if (!g_stat)
		{
			continue;
		}

		// 背景描画
		SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
		SetPolygonSize(TITLE_SIZE_X, TITLE_SIZE_Y);
		SetPolygonTexture(g_pTextureClear[TEX_TITLE]);
		SetPolygonAlpha(g_Alpha);
		DrawPolygon(pDeviceContext);

	}
	if (g_bDispStart&&g_AStart == true)
	{
		// 開始ボタン描画
		if (g_stat)
		{


			SetPolygonTexture(g_pTextureClear[TEX_START]);
			SetPolygonSize(START_WIDTH, START_HEIGHT);
			SetPolygonPos(START_POS_X, START_POS_Y);
			DrawPolygon(pDeviceContext);
		}
	}
	
	

	SetZWrite(true);
	SetBlendState(BS_NONE);			// アルファブレンド無効

}
//=============================================================================
// コントローラーオブジェクト取得
//=============================================================================
void SetClearControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}

void GetKey(void)
{
	for (int i = 0; i < MAX_TEX; ++i)
	{
		if (g_stat)
		{
			continue;
		}

		
		g_stat = 1;
		
	}
	g_Alpha += 0.004f;
	g_AStart = true;


}
