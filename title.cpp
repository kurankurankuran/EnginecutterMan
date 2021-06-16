//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "scene.h"
#include "fade.h"
#include "AssimpModel.h"
#include "model.h"
#include "mesh.h"
#include "Sound.h"
#include <stdlib.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TITLE_TEXTURENAME		L"data/texture/タイトル1.png"	// テクスチャファイル名
#define	START_TEXTURENAME		L"data/texture/space_game.png"	// テクスチャファイル名
#define	LINE_TEXTURENAME		L"data/texture/title3.png"		// テクスチャファイル名(背景)
//#define TITLE_LOGO				L"data/texture/titlelogo.png"	// テクスチャファイル名(黒タイトルロゴ)
#define TITLE_WHITELOGO			L"data/texture/titlelogowhite.png"	// テクスチャファイル名(白タイトルロゴ)
#define TITLE_EFFECT			L"data/texture/titleeffect3.png"

#define	TITLE_POS_X		0							// ポリゴンの表示位置
#define	TITLE_POS_Y		0							// ポリゴンの表示位置
#define	TITLE_LOGO_POS_X		25					// タイトルロゴの表示位置
#define	TITLE_LOGO_POS_Y		70					// タイトルロゴの表示位置
#define	TITLE_SIZE_X		SCREEN_WIDTH			// ポリゴンのサイズ
#define	TITLE_SIZE_Y		SCREEN_HEIGHT			// ポリゴンのサイズ
#define	TITLE_LOGO_SIZE_X	900						// タイトルロゴのサイズ
#define	TITLE_LOGO_SIZE_Y	200						// タイトルロゴのサイズ
#define TITLE_ANIM_SIZEX	(5.0f)
#define TITLE_ANIM_SIZEY	(5.0f)

#define	START_POS_X			0						// スタートボタンの位置(X座標)
#define	START_POS_Y			-320					// スタートボタンの位置(Y座標)
#define	START_WIDTH			250						// スタートボタンの幅
#define	START_HEIGHT		50						// スタートボタンの高さ

#define	COUNT_APPEAR		(60 * 0.5f /* 秒 */)		// スタートボタン出現までの時間
#define	INTERVAL_DISP		(60 * 1		/* 秒 */)	// スタートボタン点滅の時間

#define	NUM_TITLE_MENU				(3)				// タイトル画面での選択数
#define	TITLE_MENU_WIDTH			(180.0f)		// タイトルメニュー幅
#define	TITLE_MENU_HEIGHT			(60.0f)			// タイトルメニュー高さ
#define	TITLE_MENU_POS_X			(0.0f)			// タイトルメニュー位置(X座標)
#define	TITLE_MENU_POS_Y			(-100.0f)		// タイトルメニュー位置(Y座標)
#define	TITLE_MENU_INTERVAL	(75.0f)			// タイトルメニュー間隔

#define MAX_TITLE_ANIM	(128)	// ビルボード最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TITLE {
	//XMFLOAT4X4	mtxWorld;	// ワールドマトリックス
	XMFLOAT2	pos;		// 位置
	//XMFLOAT4	col;		// 色
	float		fSizeX;		// 幅
	float		fSizeY;		// 高さ
	int			nCounter;	// カウンター
	int			nPattern;	// パターンNo.
	//float		fAlpha;		// アルファ値
	bool		bUse;		// 使用しているかどうか
};

//*****************************************************************************
// 定数
//*****************************************************************************
enum ETex
{
	TEX_TITLE = 0,
	TEX_START,
	TEX_ANIMTITLE,
	TEX_LOGO,
	TEX_TITLEEFFECT,


	MAX_TEX
};

static LPCWSTR g_pszTexFName[] =
{
	TITLE_TEXTURENAME,
	START_TEXTURENAME,
	LINE_TEXTURENAME,
	//TITLE_LOGO,
	TITLE_WHITELOGO,
	TITLE_EFFECT
};

static LPCWSTR c_aFileNamePauseMenu[NUM_TITLE_MENU] =
{
	L"data/texture/title_game_start.png",		// ゲームスタート
	L"data/texture/title_tutorial.png",			// チュートリアル
	L"data/texture/quit_game.png",				// ゲーム終了
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture[MAX_TEX];	// テクスチャ
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };	// テクスチャへのポインタ

static TITLE_MENU g_nSelectMenu = GAME_START;		// 初期選択してるの

static MESH				g_mesh;
static MATERIAL			g_material;
static TITLE			g_titleanim[MAX_TITLE_ANIM];	// 爆発ワーク
static TITLE			g_titleeffect[MAX_TITLE_ANIM];

static int		g_nCountAppear = 0;
static int			g_nCountDisp = 0;
static bool			g_bDispStart = false;
static bool			g_bStart = false;			// スタートボタン押下

//CAssimpModel		g_modelTitle;			// モデル
static XINPUT_STATE*	g_pXController;

static float g_fCurve = 1.0f;
static bool         g_bInputUpNow;
static bool         g_bInputUpOld;
static bool         g_bInputDownNow;
static bool         g_bInputDownOld;

static bool			  g_bBGM;

static bool         g_bLoadMesh = false;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetVertexTitleAnim(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorTitleAnim(int nIdxHitEffect, XMFLOAT4 col);

//=============================================================================
// タイトル画面の初期化処理
//=============================================================================
HRESULT InitTitle()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	g_bInputUpNow = false;
	g_bInputUpOld = false;
	g_bInputDownNow = false;
	g_bInputDownOld = false;
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
	
		// タイトル選択のテクスチャ読み込み
		for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_TITLE_MENU; nCntPauseMenu++)
		{

			hr = CreateTextureFromFile(pDevice,				// デバイスへのポインタ
				c_aFileNamePauseMenu[nCntPauseMenu],		// ファイルの名前
				&g_pTextures[nCntPauseMenu]);				// 読み込むメモリー

			SaveTex(g_pTextures[nCntPauseMenu]);
		}
		g_bLoadMesh = true;
	}

	// 変数初期化
	g_nCountAppear = COUNT_APPEAR;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_bStart = false;

	g_fCurve = 0.0f;

	TITLE* pTA = g_titleanim;
	for (int i = 0; i < MAX_TITLE_ANIM; ++i, ++pTA)
	{
		//pTA->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pTA->pos = XMFLOAT2(0.0f, 0.0f);
		//pTA->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pTA->fSizeX = TITLE_ANIM_SIZEX;
		pTA->fSizeY = TITLE_ANIM_SIZEY;
		pTA->nCounter = 0;
		pTA->nPattern = 0;
		//pTA->fAlpha = 1.0f;
		pTA->bUse = false;
	}
	pTA = g_titleanim;
	pTA->bUse = true;

	TITLE* pTE = g_titleeffect;
	for (int i = 0; i < MAX_TITLE_ANIM; ++i, ++pTE)
	{
		pTE->pos = XMFLOAT2(0.0f, 0.0f);
		pTE->fSizeX = TITLE_ANIM_SIZEX;
		pTE->fSizeY = TITLE_ANIM_SIZEY;
		pTE->nCounter = 0;
		pTE->nPattern = 0;
		pTE->bUse = false;
	}
	pTE = g_titleeffect;
	pTE->bUse = true;
	// BGM再生開始
	CSound::Play(BGM_TITLE);
	g_bBGM = true;

	return hr;
}

//=============================================================================
// タイトル画面の終了処理
//=============================================================================
void UninitTitle()
{
#if MESH_RELEASE_ON
		// テクスチャ開放
		for (int i = 0; i < MAX_TEX; ++i)
		{
			SAFE_RELEASE(g_pTexture[i]);
		}

		// テクスチャの開放
		for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_TITLE_MENU; nCntPauseMenu++)
		{
			SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
		}
#endif

	// BGM停止
	if (g_bBGM) {
		CSound::Stop(BGM_TITLE);
		g_bBGM = false;
	}
}

//=============================================================================
// タイトル画面の更新処理
//=============================================================================
void UpdateTitle()
{
	// タイトルで出すアニメーションの処理
	TITLE* pTA = g_titleanim;
	for (int i = 0; i < MAX_TITLE_ANIM; ++i, ++pTA)
	{
		if (pTA->bUse)
		{
			++pTA->nCounter;
			// タイトルで出すアニメーションの速度設定
			if (pTA->nCounter % 3 == 0)
			{
				pTA->nPattern++;
			}

			// 頂点座標の設定
			pTA->fSizeX += 0.5f;
			pTA->fSizeY += 0.5f;

			
		}
	}
	// ↑まで
	// タイトルエフェクトアニメーション処理
	TITLE* pTE = g_titleeffect;
	for (int i = 0; i < MAX_TITLE_ANIM; ++i, ++pTE)
	{
		if (pTE->bUse)
		{
			++pTE->nCounter;
			// タイトルエフェクトの速度設定
			if (pTE->nCounter % 3 == 0)
			{
				pTE->nPattern++;
			}

			// 頂点座標の設定
			pTE->fSizeX += 0.5f;
			pTE->fSizeY += 0.5f;
		}
	}
	// ↑まで
	g_bInputUpOld = g_bInputUpNow;
	g_bInputDownOld = g_bInputDownNow;

	--g_nCountAppear;
	if (g_nCountAppear <= 0)
	{
		g_nCountDisp = (g_nCountDisp + 1) % 80;
		g_bDispStart = (g_nCountDisp <= INTERVAL_DISP);
	}

	if (g_bStart == true)
	{
		g_bDispStart = g_nCountAppear & 2;
	}

#if SETCONTROLLER
	if (g_pXController->Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_UP
		) {
		g_bInputUpNow = true;
	}
	else
	{
		g_bInputUpNow = false;
	}

	if (g_pXController->Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_DOWN
		) {
		g_bInputDownNow = true;
	}
	else
	{
		g_bInputDownNow = false;
	}
#endif




	// 選択カーソル合わせる
	// カーソルを上に移動
	if (GetKeyTrigger(VK_W) || GetKeyTrigger(VK_UP)
		|| g_bInputUpNow && !g_bInputUpOld
		)
	{
		CSound::Play(SE_BUTTON); // SE再生開始
		g_nSelectMenu = (TITLE_MENU)((g_nSelectMenu + NUM_TITLE_MENU - 1) % NUM_TITLE_MENU);
		SetTitleMenu();
	}
	// カーソルを下移動
	else if (GetKeyTrigger(VK_S) || GetKeyTrigger(VK_DOWN)
		|| g_bInputDownNow && !g_bInputDownOld
		)
	{
		CSound::Play(SE_BUTTON); // SE再生開始
		g_nSelectMenu = (TITLE_MENU)((g_nSelectMenu + 1) % NUM_TITLE_MENU);
		SetTitleMenu();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI)
	{
		g_fCurve -= XM_2PI;
	}

	// 選択したシーンに移動
	if (GetFade() == FADE_NONE)
	{
		// スペースキー又はEnterで決定
		if (GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE)
#if SETCONTROLLER
			|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
			|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_B
			|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
			|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y
#endif
			)
		{
			switch (GetTitleMenu())
			{
				/***** シーン選択 *****/
				// ゲームスタート
			case GAME_START:
				SetFadeOut(SCENE_STAGE_SELECT);
				CSound::Play(SE_KETTEI); // SE再生開始
				break;
				// チュートリアル
			case TUTORIAL_START:
				SetFadeOut(SCENE_TITLE);
				break;
				// ゲーム終了
			case GAME_END:
				exit(1);
				break;
			}
			g_bStart = true;		// 点滅
		}
	}
}

//=============================================================================
// タイトル画面の描画処理
//=============================================================================
void DrawTitle()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);								// Zバッファ更新しない

	// 背景描画
	SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
	SetPolygonSize(TITLE_SIZE_X, TITLE_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_ANIMTITLE]);
	SetPolygonFrameSize(1.0f / 6, 1.0f / 1);
	int u = g_titleanim[0].nPattern % 6;
	int v = g_titleanim[0].nPattern / 6;
	SetPolygonUV((float)u/6, (float)v/1);
	
	DrawPolygon(pDeviceContext);

	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(1.0f, 1.0f);


	// 背景エフェクト描画
	SetPolygonPos(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y);
	SetPolygonSize(TITLE_SIZE_X, TITLE_SIZE_Y / 1.5f);
	SetPolygonTexture(g_pTexture[TEX_TITLEEFFECT]);
	SetPolygonFrameSize(1.0f / 5, 1.0f / 3);
	int x = g_titleeffect[0].nPattern % 15;
	int y = g_titleeffect[0].nPattern / 15;
	SetPolygonUV((float)x / 5, (float)y / 3);

	DrawPolygon(pDeviceContext);

	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(1.0f, 1.0f);

	// タイトルロゴ描画
	SetPolygonPos(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y);
	SetPolygonSize(TITLE_LOGO_SIZE_X, TITLE_LOGO_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_LOGO]);
	DrawPolygon(pDeviceContext);

	// 開始ボタン描画
	if (g_bDispStart == true)
	{
		SetPolygonTexture(g_pTexture[TEX_START]);
		SetPolygonSize(START_WIDTH, START_HEIGHT);
		SetPolygonPos(START_POS_X, START_POS_Y);
		DrawPolygon(pDeviceContext);
	}

	// 選択画面描画
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_TITLE_MENU; nCntPauseMenu++)
	{
		SetPolygonPos(TITLE_MENU_POS_X, TITLE_MENU_POS_Y - nCntPauseMenu * TITLE_MENU_INTERVAL);
		if (nCntPauseMenu == g_nSelectMenu)
		{
			SetPolygonSize(TITLE_MENU_WIDTH + 20, TITLE_MENU_HEIGHT + 20);
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SetPolygonSize(TITLE_MENU_WIDTH, TITLE_MENU_HEIGHT);
			SetPolygonColor(0.5f, 0.5f, 0.5f);
		}
		SetPolygonAlpha(1.0f);

		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntPauseMenu]);

		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}

	SetZWrite(true);							// Zバッファ更新する
	SetBlendState(BS_NONE);		// アルファブレンド無効
}

//=============================================================================
// コントローラーオブジェクト取得
//=============================================================================
void SetTitleControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}

//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetTitleMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
TITLE_MENU GetTitleMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexTitleAnim(int nIdxHitEffect, float fSizeX, float fSizeY)
{
	if (nIdxHitEffect >= 0 && nIdxHitEffect < MAX_TITLE_ANIM) {
		TITLE* pExp = &g_titleanim[nIdxHitEffect];
		pExp->fSizeX = fSizeX;
		pExp->fSizeY = fSizeY;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorTitleAnim(int nIdxHitEffect, XMFLOAT4 col)
{
	if (nIdxHitEffect >= 0 && nIdxHitEffect < MAX_TITLE_ANIM) {
		//g_titleanim[nIdxHitEffect].col = col;
	}
}