////=============================================================================
////
//// モデル処理 [modelRect.cpp]
//// Author : KAZUKI KURA
////
////=============================================================================
//#include "enemyRect.h"
//#include "main.h"
//#include "input.h"
//#include "AssimpModel.h"
//#include "debugproc.h"
//#include "Collision.h"
//#include "model.h"
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define MODEL_RECT		"data/model/car002.fbx"
//
//
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//static CCamera*			g_pCamera;
//static CLight*			g_pLight;
//CAssimpModel		g_enemy;
//
////=============================================================================
//// 初期化処理
////=============================================================================
//HRESULT CEnemyRect::InitEnemy(void)
//{
//	HRESULT hr = S_OK;
//	ID3D11Device* pDevice = GetDevice();
//	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
//
//	SetPos(XMFLOAT3 (10.0f, 10.0f, 10.0f));
//	SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));
//	m_f3EnemyScl = XMFLOAT3(200.0f, 100.0f, 50.0f);
//
//	SetXYZBox(XMFLOAT3(200.0f, 200.0f, 130.0f), XMFLOAT3(50.0f, 50.0f, 50.0f), X_VECY);
//
//	// モデルデータの読み込み
//	if (!g_enemy.Load(pDevice, pDeviceContext, MODEL_RECT)) {
//		hr = E_FAIL;
//		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitEnemy", MB_OK);
//		return hr;
//	}
//}
//
////=============================================================================
//// 終了処理
////=============================================================================
//void CEnemyRect::UninitEnemy(void)
//{
//	// モデルの解放
//	g_enemy.Release();
//}
//
////=============================================================================
//// 更新処理
////=============================================================================
//void CEnemyRect::UpdateEnemy(void)
//{
//	// カメラの向き取得
//	XMFLOAT3 rotCamera = g_pCamera->GetAngle();
//
//	//==============================================================================================
//	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;
//
//	// ワールドマトリックスの初期化
//	mtxWorld = XMMatrixIdentity();
//
//	mtxScl = XMMatrixScaling(GetScl().x, GetScl().y, GetScl().z);
//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);
//
//	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(GetRot().x),
//		XMConvertToRadians(GetRot().y), XMConvertToRadians(GetRot().z));
//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);
//
//	mtxTranslate = XMMatrixTranslation(GetPos().x, GetPos().y, GetPos().z);
//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);
//
//	// ワールドマトリックス設定
//	XMStoreFloat4x4(GetmtxWorld(), mtxWorld);
//
//	// カメラ、光源設定
//
//	g_enemy.SetCamera(g_pCamera);
//	g_enemy.SetLight(g_pLight);
//}
//
////=============================================================================
//// 描画処理
////=============================================================================
//void CEnemyRect::DrawEnemy(void)
//{
//	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
//
//	// 不透明部分を描画
//	g_enemy.Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eOpacityOnly);
//
//
//	// 半透明部分を描画
//	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
//	SetZWrite(false);				// Zバッファ更新しない
//
//	g_enemy.Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eTransparentOnly);
//
//	SetZWrite(true);				// Zバッファ更新する
//	SetBlendState(BS_NONE);			// アルファブレンド無効
//}
//
////=============================================================================
//// カメラの設定
////=============================================================================
//void CEnemyRect::SetEnemyCamera(CCamera* pCamera)
//{
//	g_pCamera = pCamera;
//}
//
////=============================================================================
//// ライトの設定
////=============================================================================
//void CEnemyRect::SetEnemyLight(CLight* pLight)
//{
//	g_pLight = pLight;
//}
//
////=============================================================================
//// 敵の座標ポインタを取得
////=============================================================================
//XMFLOAT3* CEnemyRect::GetPosEnemy(void)
//{
//	return &GetPos();
//}
//
//
//void CEnemyRect::SetXYZBox(XMFLOAT3 f3Pos, XMFLOAT3 f3Scl, int nXYZBox)
//{
//	int nXYZNum = NONE_CHANGE;
//	for (int i = 0; i < MAX_XYZBOX; ++i) {
//		if (m_XYZBox[i].GetUseflg())
//			continue;
//		
//		m_XYZBox[i].SetXYZBox(f3Pos, f3Scl, nXYZBox);
//		m_XYZBox[i].SetUseflg(true);
//
//		break;
//	}
//
//}
//
//
//int CEnemyRect::GetVecY(XMFLOAT3 f3Pos)
//{
//	int nVecYNum = NONE_VECY;
//	for (int i = 0; i < MAX_XYZBOX; ++i) {
//		if (!m_XYZBox[i].GetUseflg())
//			continue;
//		/*if (m_changeSpace[i].GetChangeType() == nNowOnFiled)
//			continue;*/
//		if (!CollisionRectToPoint(m_XYZBox[i].GetPos(), m_XYZBox[i].GetScl(), f3Pos))
//			continue;
//
//		nVecYNum = m_XYZBox[i].GetXYZBoxType();
//		break;
//	}
//
//	return nVecYNum;
//}
