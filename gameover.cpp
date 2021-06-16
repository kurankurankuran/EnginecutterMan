//=============================================================================
//
// ゲームオーバー画面処理 [gameover.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "gameover.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "scene.h"
#include "fade.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAMEOVEAR_TEXTURENAME		L"data/texture/failed.png"	// テクスチャファイル名
#define PUSH_BUTTON		L"data/texture/space_game.png"

#define GAMEOVEAR_POS_X		0					// ポリゴンの表示位置
#define GAMEOVEAR_POS_Y		0					// ポリゴンの表示位置
#define GAMEOVEAR_SIZE_X		SCREEN_WIDTH	// ポリゴンのサイズ
#define GAMEOVEAR_SIZE_Y		SCREEN_HEIGHT	// ポリゴンのサイズ
#define GAMEOVEAR_ANIM_SIZE_X		(5.0f)	// ポリゴンのサイズ
#define GAMEOVEAR_ANIM_SIZE_Y		(5.0f)	// ポリゴンのサイズ


#define GEMEOVER_TIMER		(60*5)		// ゲームオーバー画面からの移行時間

#define	START_POS_X			0			// スタートボタンの位置(X座標)
#define	START_POS_Y			-230		// スタートボタンの位置(Y座標)
#define	START_WIDTH			250			// スタートボタンの幅
#define	START_HEIGHT		50			// スタートボタンの高さ

#define	COUNT_APPEAR		(60 * 0.5/* 秒 */)		// スタートボタン出現までの時間
#define	INTERVAL_DISP		(60 * 1	/* 秒 */)		// スタートボタン点滅の時間

#define MAX_GAMEOVER_ANIM	(128)	// ビルボード最大数
//*****************************************************************************
// 定数
//*****************************************************************************
enum ETex
{
	TEX_GAMEOVEAR = 0,
	TEX_START,

	MAX_TEX
};

static LPCWSTR g_pszTexFName[] =
{
	GAMEOVEAR_TEXTURENAME,
	PUSH_BUTTON
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct GAMEOVER {
	XMFLOAT2	pos;		// 位置
	float		fSizeX;		// 幅
	float		fSizeY;		// 高さ
	int			nCounter;	// カウンター
	int			nPattern;	// パターンNo.
	bool		bUse;		// 使用しているかどうか
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
static GAMEOVER			g_GameOveranim[MAX_GAMEOVER_ANIM];

static bool         g_bLoadMesh = false;
//=============================================================================
// ゲームオーバー画面の初期化処理
//=============================================================================
HRESULT InitGameOver()
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

	GAMEOVER* pGameOver = g_GameOveranim;
	for (int i = 0; i < MAX_GAMEOVER_ANIM; ++i, ++pGameOver)
	{
		pGameOver->pos = XMFLOAT2(0.0f, 0.0f);
		pGameOver->fSizeX = GAMEOVEAR_ANIM_SIZE_X;
		pGameOver->fSizeY = GAMEOVEAR_ANIM_SIZE_Y;
		pGameOver->nCounter = 0;
		pGameOver->nPattern = 0;
		pGameOver->bUse = false;
	}
	pGameOver = g_GameOveranim;
	pGameOver->bUse = true;

	// SE再生開始
	CSound::Play(SE_GAMEOVER);
	// BGM再生
	/*CSound::Play(BGM_GAMEOVER);*/
	return hr;
}

//=============================================================================
// ゲームオーバー画面の終了処理
//=============================================================================
void UninitGameOver()
{
	//// BGM終了
	//CSound::Stop(BGM_GAMEOVER);

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
void UpdateGameOver()
{
	// ゲームオーバーで出すアニメーションの処理
	GAMEOVER* pGameOver = g_GameOveranim;

	if (pGameOver->nPattern >= 27)
	{
		pGameOver->nPattern = 27;
}
	else {
		for (int i = 0; i < MAX_GAMEOVER_ANIM; ++i, ++pGameOver)
		{
			if (pGameOver->bUse)
			{
				++pGameOver->nCounter;
				// ゲームオーバーで出すアニメーションの速度設定
				if (pGameOver->nCounter % 5 == 0)
				{
					pGameOver->nPattern++;

				}

				// 頂点座標の設定
				pGameOver->fSizeX += 0.5f;
				pGameOver->fSizeY += 0.5f;
			}
		}
	}
	// ↑まで

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
		CSound::Play(SE_KETTEI);		// SE再生
		SetFadeOut(SCENE_TITLE);		// タイトル画面へ
		g_bStart = true;
	}
}
//=============================================================================
// ゲームオーバー画面の描画処理
//=============================================================================
void DrawGameOver()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);								// Zバッファ更新しない

	// 背景描画
	SetPolygonPos(GAMEOVEAR_POS_X, GAMEOVEAR_POS_Y);
	SetPolygonSize(GAMEOVEAR_SIZE_X, GAMEOVEAR_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_GAMEOVEAR]);
	SetPolygonFrameSize(1.0f / 28, 1.0f / 1);
	int u = g_GameOveranim[0].nPattern % 28;
	int v = g_GameOveranim[0].nPattern / 28;
	SetPolygonUV((float)u / 28, (float)v / 1);

	DrawPolygon(pDeviceContext);

	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(1.0f, 1.0f);

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
void SetGameOverControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}