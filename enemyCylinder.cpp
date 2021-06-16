//=============================================================================
//
// モデル処理 [enemyCylinder.cpp]
// Author :KAZUKI KURA
//
//=============================================================================
#include "enemyCylinder.h"
//#include "main.h"
//#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "Collision.h"
#include "Camera.h"
#include "Light.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_CYLINDER		"data/model/Enemy/sphere.fbx"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//static CCamera*			g_pCamera;
//static CLight*			g_pLight;
CAssimpModel		g_enemyCylinder;
//CPlayer*         g_pPlayer;
static bool g_bModelLoad = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemyCylinder::InitEnemy(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	
	//// 位置・回転・スケールの初期設定(敵)
	//SetPos(XMFLOAT3(200.0f, MODEL_POSY/2, 100.0f));
	SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetScl(XMFLOAT3(ENEMY_RAD, ENEMY_RAD, ENEMY_RAD));
	

	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	mtxScl = XMMatrixScaling(GetScl().x, GetScl().y, GetScl().z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(GetRot().x),
		XMConvertToRadians(GetRot().y), XMConvertToRadians(GetRot().z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	mtxTranslate = XMMatrixTranslation(GetPos().x, GetPos().y, GetPos().z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(GetmtxWorld(), mtxWorld);
#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif
	if (!g_bModelLoad) {
		// モデルデータの読み込み
		if (!g_enemyCylinder/*GetModel()*/.Load(pDevice, pDeviceContext, MODEL_CYLINDER)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_enemyCylinder);
		g_bModelLoad = true;
	}

	hr = InitmeshKirime(2, 2, 30.0f, 30.0f, 1.0f, 1.0f);
	if (FAILED(hr))
		return hr;

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemyCylinder::UninitEnemy(void)
{
	UninitmeshKirime();

	// モデルの解放
#if MODEL_RELEASE_ON
		g_enemyCylinder[0].Release();
#endif

}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemyCylinder::UpdateEnemy(void)
{
	
	//XMFLOAT3 rotCamera = GetCamera()->GetAngle();

	SetPos(XMFLOAT3(GetPos().x, GetPos().y, GetPlayer()->GetPosModel().z/*GetPos().z*/));
	//==============================================================================================
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	mtxScl = XMMatrixScaling(GetScl().x, GetScl().y, GetScl().z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(GetRot().x),
		XMConvertToRadians(GetRot().y), XMConvertToRadians(GetRot().z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	mtxTranslate = XMMatrixTranslation(GetPos().x, GetPos().y, GetPos().z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(GetmtxWorld(), mtxWorld);



	UpdatemeshKirime();

}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemyCylinder::DrawEnemy(void)
{
	// カメラ、光源設定
	g_enemyCylinder.SetCamera(CCamera::Get());
	g_enemyCylinder.SetLight(CLight::Get());

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 不透明部分を描画
	//g_enemyCylinder[0].Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eOpacityOnly);


	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない

	//g_enemyCylinder[0].Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eTransparentOnly);

	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効

	
	DrawmeshKirime();
}

