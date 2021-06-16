//=============================================================================
//
// モデル処理 [model.cpp]
// Author :KAZUKI KURA
//
//=============================================================================
#include "enemySpher.h"
#include "main.h"
#include "input.h"
#include "AssimpModel.h"
#include "debugproc.h"
#include "Collision.h"
#include "model.h"
#include "scene.h"
#include "Light.h"
#include "Camera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_SPHERE		"data/model/Enemy/sphere.fbx"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//static CCamera*			g_pCamera;
//static CLight*			g_pLight;
CAssimpModel		g_enemySphere;
static int              g_nScene;

static bool g_bModelLoad = false;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemySpher::InitEnemy(void)
{
	SetAlphaType(eNoAffect);
	g_nScene = GetScene();

	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	
	//// 位置・回転・スケールの初期設定(敵)
	//SetPos(XMFLOAT3(200.0f,220.0f, 75.0f));
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
		if (!g_enemySphere.Load(pDevice, pDeviceContext, MODEL_SPHERE)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
			
			
			return hr;
		}

		SaveModel(&g_enemySphere);

		g_bModelLoad = true;
	}
	/*if (!GetModel().Load(pDevice, pDeviceContext, MODEL_SPHERE)) {
		hr = E_FAIL;
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
		return hr;
	}*/
	if (!(g_nScene == SCENE_RUSH)) {
		hr = InitmeshKirime(10, 10, 10.0f, 10.0f, 1.0f, 1.0f);
		if (FAILED(hr))
			return hr;
	}

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemySpher::UninitEnemy(void)
{
	if (!(g_nScene == SCENE_RUSH)) {
		UninitmeshKirime();
	}
	
	// モデルの解放
#if MODEL_RELEASE_ON
		g_enemySphere.Release();
#endif
	
	//GetModel().Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemySpher::UpdateEnemy(void)
{
	
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



	if (!(g_nScene == SCENE_RUSH)) {
		UpdatemeshKirime();
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemySpher::DrawEnemy(void)
{
	// カメラ、光源設定

	g_enemySphere.SetCamera(CCamera::Get());
	g_enemySphere.SetLight(CLight::Get());

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	if (GetAlphaType() == Clear) {
		// 不透明部分を描画
		g_enemySphere.Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eOpacityOnly);

		// 半透明部分を描画
		SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
		SetZWrite(false);				// Zバッファ更新しない

		g_enemySphere.Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eTransparentOnly);

		g_enemySphere.Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), Clear);
	}
	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効
	
	if (!(g_nScene == SCENE_RUSH)) {
		DrawmeshKirime();
	}
}

