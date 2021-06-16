////=============================================================================
////
//// ���f������ [modelRect.cpp]
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
//// �}�N����`
////*****************************************************************************
//#define MODEL_RECT		"data/model/car002.fbx"
//
//
//
////*****************************************************************************
//// �O���[�o���ϐ�
////*****************************************************************************
//static CCamera*			g_pCamera;
//static CLight*			g_pLight;
//CAssimpModel		g_enemy;
//
////=============================================================================
//// ����������
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
//	// ���f���f�[�^�̓ǂݍ���
//	if (!g_enemy.Load(pDevice, pDeviceContext, MODEL_RECT)) {
//		hr = E_FAIL;
//		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
//		return hr;
//	}
//}
//
////=============================================================================
//// �I������
////=============================================================================
//void CEnemyRect::UninitEnemy(void)
//{
//	// ���f���̉��
//	g_enemy.Release();
//}
//
////=============================================================================
//// �X�V����
////=============================================================================
//void CEnemyRect::UpdateEnemy(void)
//{
//	// �J�����̌����擾
//	XMFLOAT3 rotCamera = g_pCamera->GetAngle();
//
//	//==============================================================================================
//	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;
//
//	// ���[���h�}�g���b�N�X�̏�����
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
//	// ���[���h�}�g���b�N�X�ݒ�
//	XMStoreFloat4x4(GetmtxWorld(), mtxWorld);
//
//	// �J�����A�����ݒ�
//
//	g_enemy.SetCamera(g_pCamera);
//	g_enemy.SetLight(g_pLight);
//}
//
////=============================================================================
//// �`�揈��
////=============================================================================
//void CEnemyRect::DrawEnemy(void)
//{
//	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
//
//	// �s����������`��
//	g_enemy.Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eOpacityOnly);
//
//
//	// ������������`��
//	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
//	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
//
//	g_enemy.Draw(pDeviceContext/*Enemy*/, *GetmtxWorld(), eTransparentOnly);
//
//	SetZWrite(true);				// Z�o�b�t�@�X�V����
//	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
//}
//
////=============================================================================
//// �J�����̐ݒ�
////=============================================================================
//void CEnemyRect::SetEnemyCamera(CCamera* pCamera)
//{
//	g_pCamera = pCamera;
//}
//
////=============================================================================
//// ���C�g�̐ݒ�
////=============================================================================
//void CEnemyRect::SetEnemyLight(CLight* pLight)
//{
//	g_pLight = pLight;
//}
//
////=============================================================================
//// �G�̍��W�|�C���^���擾
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
