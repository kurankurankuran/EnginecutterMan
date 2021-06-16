//=============================================================================
//
// ゲームクリア画面処理 [gameclear.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "gameclear.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "scene.h"
#include "fade.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define GAMECLEAR_TEXTURENAME		L"data/texture/gameclear000.png"	// テクスチャファイル名
#define GAMECLEAR_TEXTURENAME		L"data/texture/gameclear.jpg"	// テクスチャファイル名
#define PUSH_BUTTON		L"data/texture/space_game.png"

#define GAMECLEAR_POS_X			0								// ポリゴンの表示位置
#define GAMECLEAR_POS_Y			0								// ポリゴンの表示位置
#define GAMECLEAR_SIZE_X		SCREEN_WIDTH		// ポリゴンのサイズ
#define GAMECLEAR_SIZE_Y		SCREEN_HEIGHT	// ポリゴンのサイズ

#define GEMECLEAR_TIMER		(60*5)						// ゲームオーバー画面からの移行時間

#define	START_POS_X			0				// スタートボタンの位置(X座標)
#define	START_POS_Y			-230		// スタートボタンの位置(Y座標)
#define	START_WIDTH		250			// スタートボタンの幅
#define	START_HEIGHT		50			// スタートボタンの高さ

#define	COUNT_APPEAR		(60 * 0.5/* 秒 */)		// スタートボタン出現までの時間
#define	INTERVAL_DISP		(60 * 1		/* 秒 */)			// スタートボタン点滅の時間

//*****************************************************************************
// 定数
//*****************************************************************************
enum ETex
{
	TEX_GAMECLEAR = 0,
	TEX_START,

	MAX_TEX
};

static LPCWSTR g_pszTexFName[] =
{
	GAMECLEAR_TEXTURENAME,
	PUSH_BUTTON
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[MAX_TEX];	// テクスチャ
static int			g_nCountAppear = 0;		//
static int			g_nCountDisp = 0;			//
static bool		g_bDispStart = false;		//
static bool		g_bStart = false;			// スタートボタン押下
static XINPUT_STATE*	g_pXController;


static bool         g_bLoadMesh = false;
//=============================================================================
// ゲームオーバー画面の初期化処理
//=============================================================================
HRESULT InitGameClear()
{
	HRESULT hr = S_OK;
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		// テクスチャ読み込み
		for (int i = 0; i < MAX_TEX; i++)
		{
			hr = CreateTextureFromFile(GetDevice(), g_pszTexFName[i], &g_pTexture[i]);
			if (FAILED(hr))
			{
				MessageBoxW(GetMainWnd(), g_pszTexFName[i],
					L"テクスチャ読み込みエラー", MB_ICONSTOP | MB_OK);
				return hr;
			}

			SaveTex(g_pTexture[i]);
		}
		g_bLoadMesh = true;
	}
	// 変数初期化
	g_nCountAppear = COUNT_APPEAR;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_bStart = false;

	// BGM再生
	CSound::Play(BGM_GAMECLEAR);

	return hr;
}

//=============================================================================
// ゲームオーバー画面の終了処理
//=============================================================================
void UninitGameClear()
{
	// BGM終了
	CSound::Stop(BGM_GAMECLEAR);

#if MESH_RELEASE_ON
	// テクスチャ開放
	for (int i = 0; i < MAX_TEX; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
#endif
}

//=============================================================================
// ゲームオーバー画面の更新処理
//=============================================================================
void UpdateGameClear()
{
	--g_nCountAppear;
	if (g_nCountAppear <= 0)
	{
		g_nCountDisp = (g_nCountDisp + 1) % 80;
		g_bDispStart = (g_nCountDisp <= INTERVAL_DISP);
	}

	if (g_bStart)
	{
		g_bDispStart = g_nCountAppear & 2;
	}

	// [Enter]またはスペースキーを押したらチュートリアルへ
	if (!g_bStart && (/*GetKeyRelease(VK_RETURN) || */GetKeyRelease(VK_SPACE))
#if SETCONTROLLER
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_B
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y
#endif
		)
	{
		//CSound::Play(SE_KETTEI);
		SetFadeOut(SCENE_TITLE);		// タイトル画面へ
		g_bStart = true;
	}
}

//=============================================================================
// ゲームオーバー画面の描画処理
//=============================================================================
void DrawGameClear()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);

	// 背景描画
	SetPolygonPos(GAMECLEAR_POS_X, GAMECLEAR_POS_Y);
	SetPolygonSize(GAMECLEAR_SIZE_X, GAMECLEAR_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_GAMECLEAR]);
	DrawPolygon(pDeviceContext);

	// 開始ボタン描画
	if (g_bDispStart)
	{
		SetPolygonTexture(g_pTexture[TEX_START]);
		SetPolygonSize(START_WIDTH, START_HEIGHT);
		SetPolygonPos(START_POS_X, START_POS_Y);
		DrawPolygon(pDeviceContext);
	}

	SetZWrite(true);							// Zバッファ更新する
	SetBlendState(BS_NONE);		// アルファブレンド無効
}

//=============================================================================
// コントローラーオブジェクト取得
//=============================================================================
void SetGameClearControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}