//=============================================================================
//
// 一時停止処理 [pause.cpp]
// Author : RYUSUKE	YAMADA
//
//=============================================================================
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_TUTORIAL L"data/texture/tutorial.png"


#define	NUM_PAUSE_MENU			(3)						// ポーズメニュー数
#define	PAUSE_MENU_WIDTH		(180.0f)				// ポーズメニュー幅
#define	PAUSE_MENU_HEIGHT		(60.0f)				// ポーズメニュー高さ
#define	PAUSE_MENU_POS_X			(0.0f)					// ポーズメニュー位置(X座標)
#define	PAUSE_MENU_POS_Y			(100.0f)				// ポーズメニュー位置(Y座標)
#define	PAUSE_MENU_INTERVAL	(100.0f)				// ポーズメニュー間隔
#define	PLATE_WIDTH					(360.0f)				// プレートの幅(横)
#define	PLATE_HEIGHT					(430.0f)				// プレートの幅(縦)
#define	PLATE_POS_X						(0.0f)					// プレートの位置(X座標)
#define	PLATE_POS_Y						(0.0f)					// プレートの位置(Y座標)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// (なし)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = {nullptr};	// テクスチャへのポインタ
static ID3D11ShaderResourceView*	g_pTextureTutorial[1];	// テクスチャへのポインタ

static PAUSE_MENU g_nSelectMenu = PAUSE_MENU_CONTINUE;	// 初期選択してるの
static float g_fCurve	= 1.0f;
static float g_fCol		= 0.0f;

static bool         g_bInputUpNow;
static bool         g_bInputUpOld;
static bool         g_bInputDownNow;
static bool         g_bInputDownOld;

static XINPUT_STATE* g_pXController;

static LPCWSTR c_aFileNamePauseMenu[NUM_PAUSE_MENU] =
{
	L"data/texture/tuzukerupause.png",	// 続きから
	L"data/texture/startpause.png",			// 最初から
	L"data/texture/titlepause.png",			// タイトルへ
};

static bool         g_bLoadMesh = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	g_bInputUpNow = false;
	g_bInputUpOld = false;
	g_bInputDownNow = false;
	g_bInputDownOld = false;

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++)
		{
			// テクスチャの読み込み
			hr = CreateTextureFromFile(pDevice,					// デバイスへのポインタ
				c_aFileNamePauseMenu[nCntPauseMenu],		// ファイルの名前
				&g_pTextures[nCntPauseMenu]);					// 読み込むメモリー

			// テクスチャ―の保存
			SaveTex(g_pTextures[nCntPauseMenu]);


		}
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_TUTORIAL,		// ファイルの名前
			&g_pTextureTutorial[0]);					// 読み込むメモリー
			// テクスチャ―の保存
		SaveTex(g_pTextureTutorial[0]);

		g_bLoadMesh = true;
	}
	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	g_fCurve = 0.0f;

	// 効果音初期化
	/*g_pSE_Select = CreateSound(SE_SELECT_PATH);
*/
	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
#if MESH_RELEASE_ON
	// テクスチャの開放
	SAFE_RELEASE(g_pTextureTutorial[0]);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++) 
	{
		
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{

	g_bInputUpOld = g_bInputUpNow;
	g_bInputDownOld = g_bInputDownNow;

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


	if (GetKeyTrigger(VK_W) || GetKeyTrigger(VK_UP)
		|| g_bInputUpNow && !g_bInputUpOld)
	{
		CSound::Play(SE_BUTTON);
		g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	} 
	else if (GetKeyTrigger(VK_S) || GetKeyTrigger(VK_DOWN)
		|| g_bInputDownNow && !g_bInputDownOld
		)
	{
		CSound::Play(SE_BUTTON);
		g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
		SetPauseMenu();
	}

	g_fCurve += XM_PI * 0.01f;
	if (g_fCurve > XM_PI) 
	{
		g_fCurve -= XM_2PI;
	}

	// 反射光の設定
	//g_fCol = cosf(g_fCurve) * 0.2f + 0.5f;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);								// Zバッファ更新しない

	SetPolygonTexture(nullptr);
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X - 300.0f, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(1.0f, 0.2f, 0.2f/* g_fCol, g_fCol, g_fCol */);
	SetPolygonAlpha(0.4f);
	DrawPolygon(pDeviceContext);

	
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X - 300.0f, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
	SetPolygonTexture(g_pTextureTutorial[0]);
	DrawPolygon(pDeviceContext);


	//**************************************************************************



	SetPolygonTexture(nullptr);
	SetPolygonSize(PLATE_WIDTH, PLATE_HEIGHT);
	SetPolygonPos(PLATE_POS_X+300, PLATE_POS_Y);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(1.0f, 0.2f, 0.2f/* g_fCol, g_fCol, g_fCol */);
	SetPolygonAlpha(0.4f);
	DrawPolygon(pDeviceContext);



	SetPolygonSize(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++)
	{
		SetPolygonPos(PAUSE_MENU_POS_X+300, PAUSE_MENU_POS_Y - nCntPauseMenu * PAUSE_MENU_INTERVAL);
		if (nCntPauseMenu == g_nSelectMenu)
		{
			// 選択されている
			SetPolygonColor(1.0f, 1.0f, 1.0f);
		}
		else
		{
			// 選択されていない
			SetPolygonColor(0.8f, 0.8f, 0.8f);
		}
		SetPolygonAlpha(1.0f);

		// テクスチャの設定
		SetPolygonTexture(g_pTextures[nCntPauseMenu]);

		// ポリゴンの描画
		DrawPolygon(pDeviceContext);
	}
	SetPolygonColor(0.0f, 0.0f, 0.0f);

	SetZWrite(true);							// Zバッファ更新する
	SetBlendState(BS_NONE);		// アルファブレンド無効
}


//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetPauseMenu(void)
{
	g_fCurve = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
PAUSE_MENU GetPauseMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetPauseMenu(void)
{
	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	CSound::Play(SE_BUTTON);
	SetPauseMenu();
}

//=============================================================================
// コントローラーオブジェクト取得
//=============================================================================
void SetPauseControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}