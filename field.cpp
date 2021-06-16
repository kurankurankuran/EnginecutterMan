//=============================================================================
//
// 地面処理 [field.cpp]
// Author :KAZUKI KURA
//
//=============================================================================
#include "field.h"
#include "AssimpModel.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"
#include "Camera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_CHAIR		 "data/model/chair.fbx"
#define MODEL_TELEVISION "data/model/television.fbx"
#define MODEL_TABLE		 "data/model/table.fbx"

#define	TEXTURE_FILENAME	L"data/texture/field002.jpg"	// テクスチャファイル名

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define EN					(3.14159265359f)	// 円周率
#define ROOM_SIZE	(1000.0f)					// 部屋大きさ


//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAssimpModel		g_field[3+10];
//static CCamera*			g_pCamera;
//static CLight*			g_pLight;

//static MESH							g_mesh;					// 構造体
static MATERIAL						g_material;				// マテリアル

static bool g_bModelLoad = false;
CField::CField()
{
	Init();
}

CField::~CField()
{
	Uninit();
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CField::Init(void)
{
	// 位置・回転・スケールの初期設定(フィールド)
	m_Pos[0] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Pos[1] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Pos[2] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rot = XMFLOAT3(90.0f, 0.0f, 0.0f);
	m_Scl = XMFLOAT3(50.0f, 50.0f, 50.0f);

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif

	if (!g_bModelLoad) {
		// モデルデータの読み込み
		if (!g_field[0].Load(pDevice, pDeviceContext, MODEL_CHAIR)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitField", MB_OK);
			return hr;
		}
		SaveModel(&g_field[0]);

		if (!g_field[1].Load(pDevice, pDeviceContext, MODEL_TELEVISION)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitField", MB_OK);
			return hr;
		}
		SaveModel(&g_field[1]);

		if (!g_field[2].Load(pDevice, pDeviceContext, MODEL_TABLE)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitField", MB_OK);
			return hr;
		}
		SaveModel(&g_field[2]);

		g_bModelLoad = true;
	}

	// マテリアルの初期設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 50.0f;
	g_material.Emissive = M_EMISSIVE;
	
	//// フィールド初期化((座標), (角度), (法線ベクトル), (表示させるの))
 // 	m_FieldMesh.InitField(XMFLOAT3(0.0f, -ROOM_SIZE / 1.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), FLOOR);			// 床
	//m_FieldMesh.InitField(XMFLOAT3(0.0f, ROOM_SIZE *  1.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, EN), XMFLOAT3(0.0f, -1.0f, 0.0f), CEILING);	// 天井
	//m_FieldMesh.InitField(XMFLOAT3(ROOM_SIZE, 0.0f, 0.0f), XMFLOAT3(-EN / 2, EN / 2, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), WALL1);			// 右
	//m_FieldMesh.InitField(XMFLOAT3(-ROOM_SIZE, 0.0f, 0.0f), XMFLOAT3(EN / 2, EN / 2, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), WALL2);				// 左
	//m_FieldMesh.InitField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(EN / 2, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), WALL3);								// 手前
	//m_FieldMesh.InitField(XMFLOAT3(0.0f, 0.0f, ROOM_SIZE), XMFLOAT3(-EN / 2, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), WALL4);			// 奥

	// ************* フィールド初期化 (座標), (角度), (法線ベクトル), (大きさ), (表示させる壁) *************
	// ----------------------------------------- 床　 ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(EN / 2, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), FLOOR);
	// ----------------------------------------- 天井 ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(0.0f, 0.0f, 4000.0f/*ROOM_SIZE * 4*/), XMFLOAT3(-EN / 2, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), CEILING);
	// ----------------------------------------- 奥　 ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(0.0f, ROOM_SIZE * 2, ROOM_SIZE * 2), XMFLOAT3(EN, EN / 2, 0.0f),
		XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), WALL1);
	// ----------------------------------------- 手前 ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(0.0f, -ROOM_SIZE * 2, ROOM_SIZE * 2), XMFLOAT3(0.0f, EN / 2, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), WALL2);
	// ----------------------------------------- 右　 ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(ROOM_SIZE * 2, 0.0f, ROOM_SIZE * 2), XMFLOAT3(-EN / 2, EN / 2, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), WALL3);
	// ----------------------------------------- 左　 ----------------------------------------
	m_FieldMesh.InitField(XMFLOAT3(-ROOM_SIZE * 2, 0.0f, ROOM_SIZE * 2), XMFLOAT3(EN / 2, EN / 2, 0.0f),
		XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(2000.0f, 0.0f, 2000.0f), WALL4);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void CField::Uninit(void)
{
	//// メッシュ解放
	//ReleaseMesh(&g_mesh);
	m_FieldMesh.UninitField();

	// モデルの解放
#if MODEL_RELEASE_ON
		for (int i = 0; i < 3; ++i) {
			g_field[i].Release();
		}
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void CField::Update(void)
{
	// カメラの向き取得
	//XMFLOAT3 rotCamera = g_pCamera->GetAngle();

	//==============================================================================================
	for (int i = 0; i < 3; ++i) {
		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		mtxScl = XMMatrixScaling(m_Scl.x, m_Scl.y, m_Scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rot.x),
			XMConvertToRadians(m_Rot.y), XMConvertToRadians(m_Rot.z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		mtxTranslate = XMMatrixTranslation(m_Pos[i].x, m_Pos[i].y, m_Pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックス設定
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);

		
	}

	//// メッシュ更新
	//UpdateMesh(&g_mesh);
	m_FieldMesh.UpdateField();

}

//=============================================================================
// 描画処理
//=============================================================================
void CField::Draw(void)
{
	
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	
	for (int i = 0; i < 3; ++i) {
		// カメラ、光源設定
		g_field[i].SetCamera(CCamera::Get());
		g_field[i].SetLight(CLight::Get()/*g_pLight*/);

		// 不透明部分を描画
		g_field[i].Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);


		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない

		g_field[i].Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);

		SetZWrite(true);				// Zバッファ更新する
		SetBlendState(BS_NONE);			// アルファブレンド無効
	}

	
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//// メッシュの描画
	//DrawMesh(pDC, &g_mesh);
	m_FieldMesh.DrawField();
}

////=============================================================================
//// カメラの設定
////=============================================================================
//void CField::SetCamera(CCamera* Camera)
//{
//	g_pCamera = Camera;
//}

////=============================================================================
//// ライトの設定
////=============================================================================
//void CField::SetLight(CLight* Light)
//{
//	g_pLight = Light;
//}

