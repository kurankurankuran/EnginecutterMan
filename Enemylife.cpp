//=============================================================================
//
// ライフ表示処理 [EnemyLife.cpp]
// Author : KAZUKI KURA
//
//=============================================================================
#include <string.h>
#include "EnemyLife.h"
#include "polygon.h"
#include "Texture.h"
#include "mesh.h"
#include "scene.h"
#include "Camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE         (11)
//#define FRAME_TEXTURENAME	L"data/texture/frame_life1.png" // 読み込むテクスチャファイル名
//#define LIFE_TEXTURENAME	L"data/texture/life0001.png" // 読み込むテクスチャファイル名
#define LIFE_100_TEXTURENAME L"data/texture/ENEMYLIFE_MAX.png" // 読み込むテクスチャファイル名
#define LIFE_90_TEXTURENAME	L"data/texture/ENEMYLIFE_90.png" // 読み込むテクスチャファイル名
#define LIFE_80_TEXTURENAME	L"data/texture/ENEMYLIFE_80.png" // 読み込むテクスチャファイル名
#define LIFE_70_TEXTURENAME	L"data/texture/ENEMYLIFE_70.png" // 読み込むテクスチャファイル名
#define LIFE_60_TEXTURENAME	L"data/texture/ENEMYLIFE_60.png" // 読み込むテクスチャファイル名
#define LIFE_50_TEXTURENAME	L"data/texture/ENEMYLIFE_50.png" // 読み込むテクスチャファイル名
#define LIFE_40_TEXTURENAME	L"data/texture/ENEMYLIFE_40.png" // 読み込むテクスチャファイル名
#define LIFE_30_TEXTURENAME	L"data/texture/ENEMYLIFE_30.png" // 読み込むテクスチャファイル名
#define LIFE_20_TEXTURENAME	L"data/texture/ENEMYLIFE_20.png" // 読み込むテクスチャファイル名
#define LIFE_10_TEXTURENAME	L"data/texture/ENEMYLIFE_10.png" // 読み込むテクスチャファイル名
#define LIFE_00_TEXTURENAME	L"data/texture/ENEMYLIFE_00.png" // 読み込むテクスチャファイル名

//#define FRAME_TEXTURENAME2	L"data/texture/frame_life2.png" // 読み込むテクスチャファイル名
//#define LIFE_TEXTURENAME2	L"data/texture/life0002.png" // 読み込むテクスチャファイル名
//#define FRAME_TEXTURENAME3	L"data/texture/frame_life3.png" // 読み込むテクスチャファイル名
//#define LIFE_TEXTURENAME3	L"data/texture/life0003.png" // 読み込むテクスチャファイル名

//#define FRAME_SIZE_X		400							// 枠のサイズ
//#define FRAME_SIZE_Y		15							// 枠のサイズ
//#define FRAME_POS_X			(0.0f)	// 枠の表示位置
//#define FRAME_POS_Y			(300.0f)	// 枠の表示位置


//#define MAX_HP			10							// 表示桁数

#define LIFE_SIZE_X			300.0f							// 文字のサイズ
#define LIFE_SIZE_Y			100.0f							// 文字のサイズ
//#define LIFE_POS_X			(-SCREEN_WIDTH/2+FRAME_SIZE_X/2)	// 文字の表示位置
//#define LIFE_POS_Y			(SCREEN_HEIGHT/2-FRAME_SIZE_Y/2)				// 文字の表示位置

//
//#define LIFE_POS_X			(-243.0f)	// 文字の表示位置
//#define LIFE_POS_Y			(299.0f)	// 文字の表示位置


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexLife(ID3D11Device* pDevice);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
//static ID3D11ShaderResourceView*	g_pTexture[2];	// テクスチャへのポインタ

static MESH				g_meshEnemyLife[MAX_TEXTURE];
//static CCamera*         g_pCamera;
static bool             g_bLoadMesh = false;
//
//static EnemyHP						g_HP[1];	// ビルボード弾

//=============================================================================
//  コンストラクタ
//=============================================================================
CEnemyLife::CEnemyLife()
{
	/*Init(XMFLOAT3(0.0f, 0.0f, 0.0f), 0, true);*/
}

CEnemyLife::CEnemyLife(XMFLOAT3 pos, int nLife,bool bIraseflg)
{
	Init(pos, nLife, bIraseflg);
}

//=============================================================================
//  デストラクタ
//=============================================================================
CEnemyLife::~CEnemyLife()
{
	//Uninit();
}


//=============================================================================
// ライフ表示の初期化処理
//=============================================================================
HRESULT CEnemyLife::Init(XMFLOAT3 pos,int nLife,bool bIraseflg)
{
	ID3D11Device* pDevice = GetDevice();
	
	m_nLife = 0;

	//// 頂点情報の作成
	//MakeVertexLife(pDevice);

	HRESULT hr = S_OK;

	// テクスチャの読み込み
	LPCWSTR pszFName[11] = { /*FRAME_TEXTURENAME, */
		LIFE_00_TEXTURENAME,
		LIFE_10_TEXTURENAME,
		LIFE_20_TEXTURENAME,
		LIFE_30_TEXTURENAME,
		LIFE_40_TEXTURENAME,
		LIFE_50_TEXTURENAME,
		LIFE_60_TEXTURENAME,
		LIFE_70_TEXTURENAME,
		LIFE_80_TEXTURENAME,
		LIFE_90_TEXTURENAME,
		LIFE_100_TEXTURENAME,
	};

#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {


	
		for (int i = 0; i < MAX_TEXTURE; ++i)
		{
			ZeroMemory(&g_meshEnemyLife[i], sizeof(MESH));
			hr = CreateTextureFromFile(pDevice, pszFName[i], &(g_meshEnemyLife[i].pTexture)/*&g_pTexture[i]*/);

			if (FAILED(hr))
			{
				MessageBoxW(NULL, pszFName[i], L"テクスチャ読み込みエラー", MB_OK);
				break;
			}
			SaveMesh(&g_meshEnemyLife[i]);

			XMStoreFloat4x4(&g_meshEnemyLife[i].mtxTexture, XMMatrixIdentity());

			// 頂点情報の作成
			MakeVertexLife(pDevice);

		}
		g_bLoadMesh = true;
	}
 	m_bIraseflg = bIraseflg;

	// ライフ初期化
	m_nLife = nLife;
	
	m_nStat = 1;
	XMVECTOR vRocalY = XMVector3Normalize(XMVectorSet(pos.x - 0.0f, pos.y - 0.0f, 0.0f, 1.0f));
	m_nNum = (MAX_ENEMYLIFE / 10);
	if (GetScene() == SCENE_GAME) {
		m_Pos = XMFLOAT3(pos.x + XMVectorGetX(vRocalY)*200.0f, pos.y + XMVectorGetY(vRocalY)*200.0f, pos.z + XMVectorGetZ(vRocalY)*200.0f);
	}
	else if (GetScene() == SCENE_RUSH) {
		m_Pos = XMFLOAT3(pos.x, pos.y, pos.z);
	}
	m_Scl = XMFLOAT2(/*LIFE_SIZE_X*/nLife * 3, LIFE_SIZE_Y);

	return hr;
}

//=============================================================================
// ライフ表示の終了処理
//=============================================================================
void CEnemyLife::Uninit(void)
{
	 //テクスチャ解放
#if MESH_RELEASE_ON
	for (int i = 0; i < MAX_TEXTURE; ++i) {
		SAFE_RELEASE(g_meshEnemyLife[i].pTexture/*g_pTexture[i]*/);
}
#endif
	
}

//=============================================================================
// ライフ表示の更新処理
//=============================================================================
void CEnemyLife::Update(void)
{

	//ID3D11Device* pDevice = GetDevice();

	if (m_nLife <= 0) {
		m_nLife = 0;
		m_nStat = 0;
		m_nNum = 0;
	}
	if (m_nLife > 0) {
		m_nNum = m_nLife / 10;
	}
	/*if (GetScene() == SCENE_GAME) {*/
		/*if (m_nNum <= 0) {
			m_nStat = 0;
		}*/
	//}

}

//=============================================================================
// ライフ表示処理
//=============================================================================
void CEnemyLife::Draw(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	
	// ビューマトリックスを取得
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	SetBlendState(BS_ADDITIVE);

	// ライティングを無効に
	CLight::Get()->SetDisable();


	//for(int i=0;i<m_nNum;++i){	
	//	if (*(m_pLifeTipStat + i) <= 0)continue;

	//	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	//	//ワールドマトリックスの初期化
	//	mtxWorld = XMMatrixIdentity();
	//	XMStoreFloat4x4(&g_meshEnemyLife[i].mtxWorld, mtxWorld);

	//	g_meshEnemyLife[i].mtxWorld._11 = mtxView._11;
	//	g_meshEnemyLife[i].mtxWorld._12 = mtxView._21;
	//	g_meshEnemyLife[i].mtxWorld._13 = mtxView._31;
	//	g_meshEnemyLife[i].mtxWorld._21 = mtxView._12;
	//	g_meshEnemyLife[i].mtxWorld._22 = mtxView._22;
	//	g_meshEnemyLife[i].mtxWorld._23 = mtxView._32;
	//	g_meshEnemyLife[i].mtxWorld._31 = mtxView._13;
	//	g_meshEnemyLife[i].mtxWorld._32 = mtxView._23;
	//	g_meshEnemyLife[i].mtxWorld._33 = mtxView._33;

	//	mtxWorld = XMLoadFloat4x4(&g_meshEnemyLife[i].mtxWorld);

	//	// スケールを反映
	//	mtxScale = XMMatrixScaling(m_Scl.x,
	//		m_Scl.y, 1.0f);
	//	mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

	//	// 移動を反映
	//	mtxTranslate = XMMatrixTranslation(/*m_Pos.x*/m_fLifeTipPosX[i],
	//		m_Pos.y,
	//		m_Pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ワールドマトリックスの設定
	//	XMStoreFloat4x4(&g_meshEnemyLife[i].mtxWorld, mtxWorld);

	//	// メッシュの描画
	//	DrawMesh(pDeviceContext, &g_meshEnemyLife[i]);


	//}


	//// ライティングを有効に
	//CLight::Get()->SetEnable();

	//// 半透明処理無効
	//SetBlendState(BS_NONE);
	//******************************************************************************
	//ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;
	//// ビューマトリックスを取得
	//XMFLOAT4X4& mtxView = g_pCamera->GetViewMatrix();

	//SetBlendState(BS_ADDITIVE);

	// ライティングを無効に
	CLight::Get()->SetDisable();
	if (m_bIraseflg) {
		if (m_nStat > 0) {
			//ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld, mtxWorld);

			g_meshEnemyLife[m_nNum].mtxWorld._11 = mtxView._11;
			g_meshEnemyLife[m_nNum].mtxWorld._12 = mtxView._21;
			g_meshEnemyLife[m_nNum].mtxWorld._13 = mtxView._31;
			g_meshEnemyLife[m_nNum].mtxWorld._21 = mtxView._12;
			g_meshEnemyLife[m_nNum].mtxWorld._22 = mtxView._22;
			g_meshEnemyLife[m_nNum].mtxWorld._23 = mtxView._32;
			g_meshEnemyLife[m_nNum].mtxWorld._31 = mtxView._13;
			g_meshEnemyLife[m_nNum].mtxWorld._32 = mtxView._23;
			g_meshEnemyLife[m_nNum].mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld);

			// スケールを反映
			mtxScale = XMMatrixScaling(m_Scl.x,
				m_Scl.y, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(m_Pos.x,
				m_Pos.y,
				m_Pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			XMStoreFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld, mtxWorld);

			// メッシュの描画
			DrawMesh(pDeviceContext, &g_meshEnemyLife[m_nNum]);
		}
		//****************************************************************************
	}
	else {
		//ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();
		XMStoreFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld, mtxWorld);

		g_meshEnemyLife[m_nNum].mtxWorld._11 = mtxView._11;
		g_meshEnemyLife[m_nNum].mtxWorld._12 = mtxView._21;
		g_meshEnemyLife[m_nNum].mtxWorld._13 = mtxView._31;
		g_meshEnemyLife[m_nNum].mtxWorld._21 = mtxView._12;
		g_meshEnemyLife[m_nNum].mtxWorld._22 = mtxView._22;
		g_meshEnemyLife[m_nNum].mtxWorld._23 = mtxView._32;
		g_meshEnemyLife[m_nNum].mtxWorld._31 = mtxView._13;
		g_meshEnemyLife[m_nNum].mtxWorld._32 = mtxView._23;
		g_meshEnemyLife[m_nNum].mtxWorld._33 = mtxView._33;

		mtxWorld = XMLoadFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld);

		// スケールを反映
		mtxScale = XMMatrixScaling(m_Scl.x,
			m_Scl.y, 1.0f);
		mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_Pos.x,
			m_Pos.y,
			m_Pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		XMStoreFloat4x4(&g_meshEnemyLife[m_nNum].mtxWorld, mtxWorld);

		// メッシュの描画
		DrawMesh(pDeviceContext, &g_meshEnemyLife[m_nNum]);
	}

	// ライティングを有効に
	CLight::Get()->SetEnable();

	// 半透明処理無効
	SetBlendState(BS_NONE);
}

//=============================================================================
// ライフ減算処理
//=============================================================================
void CEnemyLife::DelEnemyLife(int nValue)
{
	m_nLife -= nValue;

	
}

//=============================================================================
// ライフ取得処理
//=============================================================================
int CEnemyLife::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexLife(ID3D11Device* pDevice)
{
	HRESULT hr;
	for (int i = 0; i < MAX_TEXTURE; ++i) {
		// オブジェクトの頂点配列を生成
		g_meshEnemyLife[i].nNumVertex = 4;
		VERTEX_3D* pVertexWk = new VERTEX_3D[g_meshEnemyLife[i].nNumVertex];

		// 頂点配列の中身を埋める
		VERTEX_3D* pVtx = pVertexWk;

		// 頂点座標の設定
		pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[1].vtx = XMFLOAT3(-1.0f / 2, 1.0f / 2, 0.0f);
		pVtx[2].vtx = XMFLOAT3(1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[3].vtx = XMFLOAT3(1.0f / 2, 1.0f / 2, 0.0f);

		// 法線の設定
		pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
		pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
		pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
		pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

		// インデックス配列を生成
		g_meshEnemyLife[i].nNumIndex = 4;
		int* pIndexWk = new int[g_meshEnemyLife[i].nNumIndex];

		// インデックス配列の中身を埋める
		pIndexWk[0] = 0;
		pIndexWk[1] = 1;
		pIndexWk[2] = 2;
		pIndexWk[3] = 3;

		// 頂点バッファ生成
		hr = MakeMeshVertex(pDevice, &g_meshEnemyLife[i], pVertexWk, pIndexWk);

		// 一時配列の解放
		delete[] pIndexWk;
		delete[] pVertexWk;
	}
	return hr;
}


//void CEnemyLife::SetCamera(CCamera* pCamera)
//{
//	g_pCamera = pCamera;
//}
