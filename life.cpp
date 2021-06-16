//=============================================================================
//
// ライフ表示処理 [Life.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <string.h>
#include "Life.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FRAME_TEXTURENAME	L"data/texture/frame_Life.png" // 読み込むテクスチャファイル名
#define LIFE_TEXTURENAME	L"data/texture/life0001.png" // 読み込むテクスチャファイル名

#define FRAME_SIZE_X		316							// 枠のサイズ
#define FRAME_SIZE_Y		20							// 枠のサイズ
#define FRAME_POS_X			(350.0f)	// 枠の表示位置
#define FRAME_POS_Y			(-300.0f)	// 枠の表示位置



#define LIFE_SIZE_X			10							// 文字のサイズ
#define LIFE_SIZE_Y			10							// 文字のサイズ
//#define LIFE_POS_X			(-SCREEN_WIDTH/2+FRAME_SIZE_X/2)	// 文字の表示位置
//#define LIFE_POS_Y			(SCREEN_HEIGHT/2-FRAME_SIZE_Y/2)				// 文字の表示位置


#define LIFE_POS_X			(200.0f)	// 文字の表示位置
#define LIFE_POS_Y			(-300.0f)				// 文字の表示位置
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static bool         g_bLoadMesh = false;
static ID3D11ShaderResourceView*	g_pTexture[2];	// テクスチャへのポインタ
//=============================================================================
// ライフ表示の初期化処理
//=============================================================================
HRESULT CLife::InitLife(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// テクスチャの読み込み
	LPCWSTR pszFName[] = { FRAME_TEXTURENAME, LIFE_TEXTURENAME };
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {

		for (int i = 0; i < _countof(g_pTexture); ++i)
		{
			hr = CreateTextureFromFile(pDevice, pszFName[i], &g_pTexture[i]);

			if (FAILED(hr))
			{
				MessageBoxW(NULL, pszFName[i], L"テクスチャ読み込みエラー", MB_OK);
				break;
			}

			SaveTex(g_pTexture[i]);
		}

		g_bLoadMesh = true;
	}

	return hr;
}

//=============================================================================
// ライフ表示の終了処理
//=============================================================================
void CLife::UninitLife(void)
{
#if MESH_RELEASE_ON
	// テクスチャ解放
	for (int i = 0; i < _countof(g_pTexture); ++i) {
		SAFE_RELEASE(g_pTexture[i]);
	}
#endif
}

//=============================================================================
// ライフ表示の更新処理
//=============================================================================
void CLife::UpdateLife(void)
{
	
	
}

//=============================================================================
// ライフ表示処理
//=============================================================================
void CLife::DrawLife(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	
	//SetBlendState(BS_ADDITIVE);
	// 半透明処理無効
	SetBlendState(BS_NONE);
	SetPolygonTexture(g_pTexture[1]);

	SetPolygonSize(LIFE_SIZE_X, LIFE_SIZE_Y);




	for (int i = 0; i < m_nLife; ++i) {
	
		
		SetPolygonPos(LIFE_POS_X + i * LIFE_SIZE_X, LIFE_POS_Y);

		
		DrawPolygon(pDeviceContext);

	}

	
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonColor(0.929412f, 0.152941f, 1.0f/*0.564706f*/);
	SetPolygonTexture(g_pTexture[0]);
	SetPolygonSize(FRAME_SIZE_X, FRAME_SIZE_Y);
	SetPolygonPos(FRAME_POS_X, FRAME_POS_Y);
	DrawPolygon(pDeviceContext);
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	
	
}

//=============================================================================
// ライフ減算処理
//=============================================================================
void CLife::DelLife(int nValue)
{
	m_nLife -= nValue;

	if (m_nLife < 0)
		m_nLife = 0;
}

//=============================================================================
// ライフ取得処理
//=============================================================================
int CLife::GetLife(void)
{
	return m_nLife;
}
