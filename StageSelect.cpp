//=============================================================================
//
// ステージセレクト処理 [StageSelect.cpp]
// Author : RYUSUKE	YAMADA
//-----------------------------------------------------------------------------
// 選択と決定の音出るようにしておきました。
//=============================================================================

#include "main.h"
#include "Texture.h"
#include "StageSelect.h"
#include "input.h"
#include "fade.h"
#include "scene.h"
#include "polygon.h"
#include "debugproc.h"
#include "Sound.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	SSELECT_POS_X						(0)			// ポリゴンの表示位置
#define	SSELECT_POS_Y						(0)			// ポリゴンの表示位置
#define	SSELECT_SIZE_X					(300)			// ポリゴンのサイズ
#define	SSELECT_SIZE_Y					(300)			// ポリゴンのサイズ

#define	NUM_SSELECT						(3)				// ステージセレクト画面での選択数
#define	SSELECT_MENU_INTERVAL	(400.0f)				// ステージセレクト間隔
#define	SSELECT_MENU_WIDTH		(180.0f)				// ステージセレクト幅
#define	SSELECT_MENU_HEIGHT		(300.0f)				// ステージセレクト高さ
#define	SSELECT_MENU_POS_X		(-SSELECT_MENU_INTERVAL)// ステージセレクト位置(X座標)
#define	SSELECT_MENU_POS_Y		(0.0f)					// ステージセレクト位置(Y座標)

#define	SCALE_SPEED		(1)			// 選択している奴の拡大縮小スピード
#define	COLOR_SPEED		(0.01f)			// 選択している奴の色変化スピード

#define	PICK_UP			L"data/texture/white.png"			// 選択した後ろ
#define	YAZIRUSHI		L"data/texture/yazirusi.png"		// 矢印

#define	HAIKEI_TEXTURE	L"data/texture/BossCheck.png"	// 背景画像
#define MAX_HAIKAEI_ANIM (128)
#define HAIKAEI_ANIM_SIZE_X		(5.0f)	// ポリゴンのサイズ
#define HAIKAEI_ANIM_SIZE_Y		(5.0f)	// ポリゴンのサイズ
#define HAIKEI_POS_X 0
#define HAIKEI_POS_Y 0
#define HAIKEI_SIZE_X		SCREEN_WIDTH	// ポリゴンのサイズ
#define HAIKEI_SIZE_Y		SCREEN_HEIGHT	// ポリゴンのサイズ


static LPCWSTR c_aFileNamePauseMenu[NUM_SSELECT] =
{
	L"data/texture/Vacuum.jpg",		// ステージ１
	L"data/texture/Vacuum.jpg",		// ステージ２
	L"data/texture/Vacuum.jpg",		// ステージ３
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct SELECT {
	XMFLOAT2	pos;		// 位置
	float		fSizeX;		// 幅
	float		fSizeY;		// 高さ
	int			nCounter;	// カウンター
	int			nPattern;	// パターンNo.
	bool		bUse;		// 使用しているかどうか
};

//*****************************************************************************
// 定数
//*****************************************************************************
enum ETex
{
	TEX_SELECT = 0,
	TEX_START,

	MAX_TEX
};
static LPCWSTR g_pszTexFName[] =
{
	HAIKEI_TEXTURE
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[3] = { nullptr };			// テクスチャへのポインタ
static ID3D11ShaderResourceView*	g_pTexture;			// テクスチャ
static ID3D11ShaderResourceView*	g_pBgTexture[MAX_TEX];			// テクスチャ
static ID3D11ShaderResourceView*	g_yazirushi;			// テクスチャ
static ID3D11ShaderResourceView*	g_haikei;			// テクスチャ


static STAGE_SELECT_MENU g_nSelectMenu = STAGE_2;			// 選択してるの

static float	g_fCurve = 1.0f;

static int		timer = 0;
static bool	plus	= true;
static bool	minus = false;

static int		select_timer = 0;
static float	rgb_r	= 0.0f;
static float	rgb_g	= 0.0f;
static float	rgb_b	= 1.0f;

static bool	select_stage = true;
static bool	Enter_Use = false;
static int		Enter_time;

static XINPUT_STATE*	g_pXController;
static bool         g_bInputLeftNow;
static bool         g_bInputLeftOld;
static bool         g_bInputRightNow;
static bool         g_bInputRightOld;

static SELECT g_SelectAnim[MAX_HAIKAEI_ANIM];


static bool         g_bLoadMesh = false;

//=============================================================================
// ステージセレクトの初期化処理
//=============================================================================
HRESULT InitStageSelect()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {
		// テクスチャ読み込み
		CreateTextureFromFile(GetDevice(), PICK_UP, &g_pTexture);
		if (FAILED(hr))
		{
			return hr;
		}
		SaveTex(g_pTexture);

		// テクスチャ読み込み
		CreateTextureFromFile(GetDevice(), YAZIRUSHI, &g_yazirushi);
		if (FAILED(hr))
		{
			return hr;
		}
		SaveTex(g_yazirushi);

		// テクスチャ読み込み
		/*for (int i = 0; i < MAX_TEX; i++)
		{*/
		hr = CreateTextureFromFile(GetDevice(), g_pszTexFName[0], &g_pBgTexture[0]);
		if (FAILED(hr))
		{
			MessageBoxW(GetMainWnd(), g_pszTexFName[0],
				L"テクスチャ読み込みエラー", MB_ICONSTOP | MB_OK);
			return hr;
		}
		//}
		SaveTex(g_pBgTexture[0]);

	
		// タイトル選択のテクスチャ読み込み
		for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_SSELECT; nCntPauseMenu++)
		{
			hr = CreateTextureFromFile(pDevice,					// デバイスへのポインタ
				c_aFileNamePauseMenu[nCntPauseMenu],		// ファイルの名前
				&g_pTextures[nCntPauseMenu]);					// 読み込むメモリー

			SaveTex(g_pTextures[nCntPauseMenu]);
		}
		g_bLoadMesh = true;
	}
	g_fCurve = 0.0f;

	timer = 0;
	plus = true;
	minus = false;

	select_timer = 0;	

	rgb_r = 0.0f;
	rgb_g = 0.0f;
	rgb_b = 1.0f;
	
	Enter_Use = false;
	select_stage = true;

	g_bInputLeftNow = false;
	g_bInputLeftOld = false;
	g_bInputRightNow = false;
	g_bInputRightOld = false;

	// 背景アニメーション初期化
	SELECT* pSelect = g_SelectAnim;
	for (int i = 0; i < MAX_HAIKAEI_ANIM; ++i, ++pSelect)
	{
		pSelect->pos = XMFLOAT2(0.0f, 0.0f);
		pSelect->fSizeX = HAIKAEI_ANIM_SIZE_X;
		pSelect->fSizeY = HAIKAEI_ANIM_SIZE_Y;
		pSelect->nCounter = 0;
		pSelect->nPattern = 0;
		pSelect->bUse = false;
	}
	pSelect = g_SelectAnim;
	pSelect->bUse = true;

	return hr;
}


//=============================================================================
// ステージセレクトの終了処理
//=============================================================================
void UninitStageSelect()
{

#if  MESH_RELEASE_ON
	// テクスチャの開放
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_SSELECT; nCntPauseMenu++)
	{
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
	// テクスチャ開放
	for (int i = 0; i < MAX_TEX; i++)
	{
		SAFE_RELEASE(g_pBgTexture[i]);
	}
	SAFE_RELEASE(g_yazirushi);
	SAFE_RELEASE(g_haikei);
	SAFE_RELEASE(g_pTexture);
#endif
}


//=============================================================================
// ステージセレクトの更新処理
//=============================================================================
void UpdateStageSelect()
{
	// ステージ選択で出すアニメーションの処理
	SELECT* pSelect = g_SelectAnim;
	for (int i = 0; i < MAX_HAIKAEI_ANIM; ++i, ++pSelect)
	{
		if (pSelect->bUse)
		{
			++pSelect->nCounter;
			// ステージ選択で出すアニメーションの速度設定
			if (pSelect->nCounter % 5 == 0)
			{
				pSelect->nPattern++;

			}

			// 頂点座標の設定
			pSelect->fSizeX += 0.5f;
			pSelect->fSizeY += 0.5f;
		}
	}
	// ↑まで

	g_bInputLeftOld = g_bInputLeftNow;
	g_bInputRightOld = g_bInputRightNow;

#if SETCONTROLLER
	if (g_pXController->Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_LEFT
		) {
		g_bInputLeftNow = true;
	}
	else
	{
		g_bInputLeftNow = false;
	}

	if (g_pXController->Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_DPAD_RIGHT
		) {
		g_bInputRightNow = true;
	}
	else
	{
		g_bInputRightNow = false;
	}
#endif


	// 選択カーソル合わせる
	// カーソルを左移動
	//if (GetKeyTrigger(VK_A) || GetKeyTrigger(VK_LEFT)
	//	|| g_bInputLeftNow && !g_bInputLeftOld
	//	)
	//{
	//	CSound::Play(SE_BUTTON); // SE再生開始
	//	g_nSelectMenu = (STAGE_SELECT_MENU)((g_nSelectMenu + NUM_SSELECT - 1) % NUM_SSELECT);
	//	SetStageSelect();
	//}
	//// カーソルを右移動
	//if (GetKeyTrigger(VK_D) || GetKeyTrigger(VK_RIGHT)
	//	|| g_bInputRightNow && !g_bInputRightOld
	//	)
	//{
	//	CSound::Play(SE_BUTTON); // SE再生開始
	//	g_nSelectMenu = (STAGE_SELECT_MENU)((g_nSelectMenu + 1) % NUM_SSELECT);
	//	SetStageSelect();
	//}


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
			switch (GetStageSelect())
			{
				/***** ステージ選択 *****/
				// ステージ１
			case STAGE_1:
				SetFadeOut(SCENE_GAME);
				//SetFadeOut(SCENE_STAGE1);
				CSound::Play(SE_KETTEI); // SE再生開始
				break;
				// ステージ２
			case STAGE_2:
				SetFadeOut(SCENE_GAME);
				//SetFadeOut(SCENE_STAGE2);
				CSound::Play(SE_KETTEI); // SE再生開始
				break;
				// ステージ３
			case STAGE_3:
				SetFadeOut(SCENE_GAME);
				//SetFadeOut(SCENE_STAGE3);
				CSound::Play(SE_KETTEI); // SE再生開始
				break;
			}		
			Enter_Use = true;
		}
	}
	if (Enter_Use == true || Enter_time == 0)
	{
		Enter_time++;
		select_stage = true;
	}

	if (Enter_time >= 5)
	{
		select_stage = false;
		Enter_time = 0;
	}


	/***** 選択されたステージ画像の拡大縮小 *****/
	if (plus == true)
	{
		minus = false;
		timer += SCALE_SPEED;
		if (timer >= 50)
		{
			minus = true;
		}
	}
	if (minus == true)
	{
		plus = false;
		timer -= SCALE_SPEED;
		if (timer <= -10)
		{
			plus = true;
		}
	}

	/***** 選択された枠(背景画像)の表示*****/
	if (rgb_g <= 0.0f && rgb_b <= 1.0f)
	{
		rgb_b -= COLOR_SPEED;
		rgb_r += COLOR_SPEED;
		if (rgb_b <= 0.0f)
		{
			rgb_b = 0.0f;
		}
		if (rgb_r >= 1.0f)
		{
			rgb_r = 1.0f;
		}
	}
	if (rgb_r <= 1.0f && rgb_b <= 0.0f)
	{
		rgb_r -= COLOR_SPEED;
		rgb_g += COLOR_SPEED;
		if (rgb_r <= 0.0f)
		{
			rgb_r = 0.0f;
		}
		if (rgb_g >= 1.0f)
		{
			rgb_g = 1.0f;
		}
	}
	if (rgb_r <= 0.0f && rgb_g <= 1.0f)
	{
		rgb_g -= COLOR_SPEED;
		rgb_b += COLOR_SPEED;
		if (rgb_g <= 0.0f)
		{
			rgb_g = 0.0f;
		}
		if (rgb_b >= 1.0f)
		{
			rgb_b = 1.0f;
		}
	}
	/*StartDebugProc();
	PrintDebugProc("R(%f)\n", rgb_r);
	PrintDebugProc("G(%f)\n", rgb_g);
	PrintDebugProc("B(%f)\n", rgb_b);
	PrintDebugProc("ﾀｲﾏｰ:%d", Enter_time);*/
}


//=============================================================================
// ステージセレクトの描画処理
//=============================================================================
void DrawStageSelect()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);

	int nCntPauseMenu;

	/*SetPolygonSize(SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3);
	SetPolygonColor(0.0f, 0.0f, 0.0f);
	SetPolygonTexture(g_haikei);
	DrawPolygon(pDeviceContext);*/
	// 背景描画
	SetPolygonPos(HAIKEI_POS_X, HAIKEI_POS_Y);
	SetPolygonSize(HAIKEI_SIZE_X, HAIKEI_SIZE_Y);
	SetPolygonTexture(g_pBgTexture[TEX_SELECT]);
	SetPolygonFrameSize(1.0f / 6, 1.0f / 1);
	int u = g_SelectAnim[0].nPattern % 6;
	int v = g_SelectAnim[0].nPattern / 6;
	SetPolygonUV((float)u / 6, (float)v / 1);

	DrawPolygon(pDeviceContext);

	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(1.0f, 1.0f);

	// 選択画面描画
	for (nCntPauseMenu = 1; nCntPauseMenu < 2/*NUM_SSELECT*/; nCntPauseMenu++)
	{
		SetPolygonPos(SSELECT_MENU_POS_X + nCntPauseMenu * SSELECT_MENU_INTERVAL, SSELECT_MENU_POS_Y);
		if (nCntPauseMenu == g_nSelectMenu)
		{
			// 選択してる
			if (select_stage == true)
			{
				// 枠(背景画像)表示
				SetPolygonSize(SSELECT_MENU_WIDTH + timer + 20, SSELECT_MENU_HEIGHT + timer + 20);
				SetPolygonColor(rgb_r, rgb_g, rgb_b);
				SetPolygonAlpha(1.0f);
				SetPolygonTexture(g_pTexture);
				DrawPolygon(pDeviceContext);

				SetPolygonSize(SSELECT_MENU_WIDTH + timer, SSELECT_MENU_HEIGHT + timer);
				SetPolygonColor(1.0f, 1.0f, 1.0f);
				SetPolygonAlpha(1.0f);
				SetPolygonTexture(g_pTextures[nCntPauseMenu]);
				DrawPolygon(pDeviceContext);
			}
		}
		else
		{
			// 選択してない
			SetPolygonSize(SSELECT_MENU_WIDTH, SSELECT_MENU_HEIGHT);
			SetPolygonColor(0.5f, 0.5f, 0.5f);
			SetPolygonAlpha(1.0f);
			SetPolygonTexture(g_pTextures[nCntPauseMenu]);
			DrawPolygon(pDeviceContext);
		}
	}

	

	SetZWrite(true);							// Zバッファ更新する
	SetBlendState(BS_NONE);		// アルファブレンド無効
}


//=============================================================================
// ステージセレクトの設定
//=============================================================================
void SetStageSelect(void)
{
	g_fCurve = 0.0f;
}


//=============================================================================
// ステージセレクトの取得
//=============================================================================
STAGE_SELECT_MENU GetStageSelect(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// コントローラーオブジェクト取得
//=============================================================================
void SetStageSelectControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}