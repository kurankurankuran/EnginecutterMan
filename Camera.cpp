//=============================================================================
//
// カメラ クラス [Camera.cpp]
// Author :KAZUKI KURA
//
//=============================================================================
#include "Camera.h"
#include "input.h"
#include "debugproc.h"
#include "Sound.h"
#include "CLEAR.h"
#include "fade.h"
#include "scene.h"

//#include "model.h"
//#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MOVE_SPEED (3.0f) // カメラ移動速度
#define CAMERA_ROTATE_SPEED (3.0f) // カメラ回転速度
#define CAMERA_POSX (MODEL_POSX)// カメラ初期位置
#define CAMERA_POSY (MODEL_POSY + 320.0f)// カメラ初期位置
#define CAMERA_POSZ (MODEL_POSZ - 450.0f)// カメラ初期位置
#define RUSH_CAMERA_POSX (RUSHPLAYER_POSX)// カメラ初期位置
#define RUSH_CAMERA_POSY (RUSHPLAYER_POSY + 320.0f +320.0f)// カメラ初期位置
#define RUSH_CAMERA_POSZ (RUSHPLAYER_POSZ - 450.0f-200.0f)// カメラ初期位置

//*****************************************************************************
// グローバル変数定義
//*****************************************************************************
static XMFLOAT3 g_ModelOldPos;
static XMFLOAT3 g_vModelMove;
//XMFLOAT3 g_vCameraUp;

static XMVECTOR g_vCameraX;
static XMVECTOR g_vCameraY;
static XMVECTOR g_vCameraZ;


static CPlayer* g_pPlayer;
static CRushPlayer* g_pRushPlayer;
static CEnemyVacuum*  g_pEnemy;
CEnemyRushVacuum*  g_pRushEnemy;

static bool         g_bWorldMove;
static int          g_cnt;

static XINPUT_STATE* g_pXController;


static bool         g_Time2;
static bool         g_bOpnieng;

// グローバル変数
namespace {
	CCamera g_camera;
};
// コンストラクタ
CCamera::CCamera()
{
	Init();
}

// 初期化
void CCamera::Init()
{
	m_nScene = GetScene();

	switch (m_nScene)
	{
	case SCENE_GAME:
		g_ModelOldPos.x = MODEL_POSX;
		g_ModelOldPos.y = MODEL_POSY;
		g_ModelOldPos.z = MODEL_POSZ;

		m_vPos = XMFLOAT3(-2000.0f, -2000.0f, 2800.0f);
		m_vTarget = XMFLOAT3(0.0f, 0.0f, 400.0f);
		m_vDefvec = XMFLOAT3(CAMERA_POSX, CAMERA_POSY, CAMERA_POSZ/*MODEL_POSX, MODEL_POSY + 280.0f, MODEL_POSZ - 380.0f*/);

		m_vUp = XMFLOAT3(0.0f, 0.0f, 1.0f);

		m_fAspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;//縦横比
		m_fFovY = 45.0f;//視野角

		m_fNearZ = 100.0f;//前方クリップ距離  //これ以上手前に来たら消える
		m_fFarZ = 6000.0f;//後方クリップ距離 //これ以上奥にいったら消える
		g_bWorldMove = false;
		g_Time2 = false;

		m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_cnt = 0;

		g_bOpnieng = true;
		break;

	case SCENE_RUSH:
		g_ModelOldPos.x = RUSHPLAYER_POSX;
		g_ModelOldPos.y = RUSHPLAYER_POSY;
		g_ModelOldPos.z = RUSHPLAYER_POSZ;

		m_vPos = XMFLOAT3(RUSH_CAMERA_POSX, RUSH_CAMERA_POSY, RUSH_CAMERA_POSZ);
		m_vTarget = XMFLOAT3(RUSHPLAYER_POSX, RUSHPLAYER_POSY, RUSHPLAYER_POSZ);
		m_vDefvec = XMFLOAT3(RUSH_CAMERA_POSX, RUSH_CAMERA_POSY, RUSH_CAMERA_POSZ/*MODEL_POSX, MODEL_POSY + 280.0f, MODEL_POSZ - 380.0f*/);

		m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);

		m_fAspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;//縦横比
		m_fFovY = 45.0f;//視野角

		m_fNearZ = 100.0f;//前方クリップ距離  //これ以上手前に来たら消える
		m_fFarZ = 10000.0f;//後方クリップ距離 //これ以上奥にいったら消える
		g_bWorldMove = true;
		g_Time2 = false;

		m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_cnt = 0;

		g_bOpnieng = false;
		
		break;
	default:
		break;
	}
}

// 更新
void CCamera::Update()
{
	if (!g_bWorldMove)
	{
		if (g_bOpnieng) {
			if (g_cnt == 0)
			{
				m_vPos.y -= 8.5f * SinDeg(m_vAngle.z - 90);
				//CSound::Play(BGM_CMBE);	// 効果音再生
				//CSound::SetVolume(BGM_CMBE, 5.0f);

				//if (m_vPos.z <= 1400.0f)
				//{
				//	//CSound::Stop(BGM_CMBE);	// 効果音再生
				//}
				if (m_vPos.y >= 1000.0f)
				{
					g_cnt += 1;
					m_vPos = XMFLOAT3(2000.0f, 2000.0f, 1800.0f);
					m_vTarget = XMFLOAT3(0.0f, 0.0f, 400.0f);


				}
			}
			if (g_cnt == 1)
			{
				m_vPos.y += 8.5f * SinDeg(m_vAngle.z - 90);


				//CSound::Play(BGM_CMBE);	// 効果音再生
				//if (m_vPos.z <= 400.0f)
				//{
				//	//CSound::Stop(BGM_CMBE);	// 効果音再生
				//}

				if (m_vPos.y <= -700.0f)
				{
					g_cnt += 1;

					m_vPos = XMFLOAT3(-919.0f, 449.0f, 100.0f);
					m_vTarget = XMFLOAT3(-599.0f, -0.0f, 100.0f);
					m_vUp = XMFLOAT3(0.0f, 1.0f, -0.0f);

				}
			}
			if (g_cnt == 2)
			{

				m_vPos.y += 4.5f * SinDeg(m_vAngle.y - 90);

				//CSound::Play(BGM_CMBE);	// 効果音再生

				//if (m_vPos.x <= 200.0f)
				//{
				//	//CSound::Stop(BGM_CMBE);	// 効果音再生
				//}
				if (m_vPos.y <= -600.0f)
				{
					g_cnt += 1;
				}
			}
			if (g_cnt == 3 || GetKeyPress(VK_SPACE)
#if SETCONTROLLER
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_B
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y
#endif
				)
			{
				g_bWorldMove = true;
				m_vPos = XMFLOAT3(/*MODEL_POSX, MODEL_POSY + 280.0f, MODEL_POSZ - 380.0f*/CAMERA_POSX, CAMERA_POSY, CAMERA_POSZ);
 				m_vTarget = XMFLOAT3(MODEL_POSX, MODEL_POSY, MODEL_POSZ);
				//CSound::Play(SE_GMST);	// 効果音再生
				//CSound::SetVolume(SE_GMST, 5.0f);

				g_bOpnieng = false;
			}

		}

	}
	else if (g_Time2)
	{
		SetFadeOut(SCENE_RUSH);

		m_vUp = XMFLOAT3(0.0f, 0.0f, 1.0f);

		m_vPos.y -= 2.0f;
		GetKey();
		if (m_vPos.y <= 1500.0f)
		{
			m_vPos = XMFLOAT3(0.0f, 1500.0f, 288.0f);
			m_vTarget = XMFLOAT3(0.0f, 536.0f, 668.0f);
			m_vAngle.x = 0.0f;
			m_vAngle.y = 0.0f;
			m_vAngle.z = 0.0f;


			
			if (GetKeyPress(VK_SPACE)
#if SETCONTROLLER
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_A
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_B
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_X
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_Y
#endif
				)
			{
							// シーン遷移
				m_vPos = XMFLOAT3(-585.0f, 1000.0f, 1900.0f);
				m_vTarget = XMFLOAT3(-6.0f, 530.0f, 200.0f);
				g_Time2 = false;
				g_bWorldMove = false;
				g_bOpnieng = false;
			}

		}

	}
	else if (g_bWorldMove)
	{
		if (GetKeyPress(VK_3))
		{

			m_vPos = XMFLOAT3(0.0f, 2262.0f, 288.0f);
			m_vTarget = XMFLOAT3(0.0f, 536.0f, 668.0f);
			m_vAngle.x = 0.0f;
			m_vAngle.y = 0.0f;
			m_vAngle.z = 0.0f;

			g_Time2 = true;

		}

		float vAngleY = 0.0f;
		float vAngleX = 0.0f;
		float dx = 0.0f, dz = 0.0f, dy = 0.0f; // 移動量

		switch (m_nScene) {

			XMMATRIX mRY;
			XMMATRIX mRX;
			XMVECTOR v;
			XMVECTOR vDefVec;
			XMFLOAT3* pModelPos;
			XMVECTOR vecZ;
			XMFLOAT3 vZ;
			XMVECTOR Vup;
			XMVECTOR vecX;
			XMFLOAT3 vX;

		case SCENE_GAME:
			//CSound::Stop(BGM_GMST);	// 効果音再生
		   // カメラ移動
		   // カメラの座標にモデルの移動ベクトルを加える（カメラ追従）


			//XMFLOAT3 vCameraMove;
			pModelPos = &g_pPlayer->GetPosModel();

			g_vModelMove.x = pModelPos->x - g_ModelOldPos.x;
			g_vModelMove.y = pModelPos->y - g_ModelOldPos.y;
			g_vModelMove.z = pModelPos->z - g_ModelOldPos.z;

			g_ModelOldPos.x = pModelPos->x;
			g_ModelOldPos.y = pModelPos->y;
			g_ModelOldPos.z = pModelPos->z;

			m_vTarget.x += g_vModelMove.x;
			m_vTarget.y += g_vModelMove.y;
			m_vTarget.z += g_vModelMove.z;


			m_vPos.x += g_vModelMove.x;
			m_vPos.y += g_vModelMove.y;
			m_vPos.z += g_vModelMove.z;

			m_vDefvec.x += g_vModelMove.x;
			m_vDefvec.y += g_vModelMove.y;
			m_vDefvec.z += g_vModelMove.z;





			//************************************************************************
			switch (g_pPlayer->GetOnField())
			{
				XMVECTOR vMove;
				XMVECTOR vMoveLen;
				XMVECTOR VpcNow;
				XMVECTOR VpcNext;
				XMFLOAT3 f3V1, f3V2;
				float fMoveLen;
			case CIRCLE_FIELD:
				vMove = XMVectorSet(g_vModelMove.x, g_vModelMove.y, g_vModelMove.z, 1.0f);
				vMoveLen = XMVector3Length(vMove);
				fMoveLen = XMVectorGetX(vMoveLen);

				if (!fMoveLen == 0.0f)
				{
					VpcNow = XMVectorSet(m_vPos.x - g_pPlayer->GetPosModel().x, m_vPos.y - g_pPlayer->GetPosModel().y, m_vPos.z - g_pPlayer->GetPosModel().z, 1.0f); // プレイヤーからカメラへのベクトル

   					VpcNext = XMVector3TransformNormal(VpcNow, XMLoadFloat4x4(&g_pPlayer->GetRotateMatrix()));


					XMStoreFloat3(&f3V1, VpcNow);
					XMStoreFloat3(&f3V2, VpcNext);

					m_vPos.x += f3V2.x - f3V1.x;
					m_vPos.y += f3V2.y - f3V1.y;
					m_vPos.z += f3V2.z - f3V1.z;

					VpcNow = XMVectorSet(m_vDefvec.x - g_pPlayer->GetPosModel().x, m_vDefvec.y - g_pPlayer->GetPosModel().y, m_vDefvec.z - g_pPlayer->GetPosModel().z, 1.0f); // プレイヤーからカメラへのベクトル

					VpcNext = XMVector3TransformNormal(VpcNow, XMLoadFloat4x4(&g_pPlayer->GetRotateMatrix()));

					XMStoreFloat3(&f3V1, VpcNow);
					XMStoreFloat3(&f3V2, VpcNext);

					m_vDefvec.x += f3V2.x - f3V1.x;
					m_vDefvec.y += f3V2.y - f3V1.y;
					m_vDefvec.z += f3V2.z - f3V1.z;

				}
				break;
			case CYLINDER_FIELD:
				vMove = XMVectorSet(g_vModelMove.x, g_vModelMove.y, g_vModelMove.z, 1.0f);
				vMoveLen = XMVector3Length(vMove);
				fMoveLen = XMVectorGetX(vMoveLen);

				if (!fMoveLen == 0.0f)
				{
					VpcNow = XMVectorSet(m_vPos.x - g_pPlayer->GetPosModel().x, m_vPos.y - g_pPlayer->GetPosModel().y, m_vPos.z - g_pPlayer->GetPosModel().z, 1.0f); // プレイヤーからカメラへのベクトル

					VpcNext = XMVector3TransformNormal(VpcNow, XMLoadFloat4x4(&g_pPlayer->GetRotateMatrix()));

					XMStoreFloat3(&f3V1, VpcNow);
					XMStoreFloat3(&f3V2, VpcNext);

					m_vPos.x += f3V2.x - f3V1.x;
					m_vPos.y += f3V2.y - f3V1.y;
					m_vPos.z += f3V2.z - f3V1.z;

					VpcNow = XMVectorSet(m_vDefvec.x - g_pPlayer->GetPosModel().x, m_vDefvec.y - g_pPlayer->GetPosModel().y, m_vDefvec.z - g_pPlayer->GetPosModel().z, 1.0f); // プレイヤーからカメラへのベクトル

					VpcNext = XMVector3TransformNormal(VpcNow, XMLoadFloat4x4(&g_pPlayer->GetRotateMatrix()));

					XMStoreFloat3(&f3V1, VpcNow);
					XMStoreFloat3(&f3V2, VpcNext);

					m_vDefvec.x += f3V2.x - f3V1.x;
					m_vDefvec.y += f3V2.y - f3V1.y;
					m_vDefvec.z += f3V2.z - f3V1.z;

				}
				break;
			}


			//*****************************************************************************************
			vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);
			//XMFLOAT3 vZ;
			XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
			g_vCameraZ = XMVectorSet(vZ.x, vZ.y, vZ.z, 1.0f);

			switch (g_pPlayer->GetOnField()) {
			case RECT_FIELD:

				break;
			case CIRCLE_FIELD:
				switch (g_pPlayer->GetOnFieldName())
				{
				case BODY_NAME:
					Vup = XMVectorSet(
						g_pEnemy->GetCylinderP()->GetPos().x - m_vPos.x,
						g_pEnemy->GetCylinderP()->GetPos().y - m_vPos.y,
						g_pEnemy->GetCylinderP()->GetPos().z - m_vPos.z, 1.0f);//仮のYベクトル
					break;
				case FRONTCUBE_NAME:
					Vup = XMVectorSet(
						g_pEnemy->GetSpherFrontP()->GetPos().x - m_vPos.x,
						g_pEnemy->GetSpherFrontP()->GetPos().y - m_vPos.y,
						g_pEnemy->GetSpherFrontP()->GetPos().z - m_vPos.z, 1.0f);//仮のYベクトル
					break;
				//case BACKCUBE_NAME:
				//	Vup = XMVectorSet(
				//		g_pEnemy->GetSpherBackP()->GetPos().x - m_vPos.x,
				//		g_pEnemy->GetSpherBackP()->GetPos().y - m_vPos.y,
				//		g_pEnemy->GetSpherBackP()->GetPos().z - m_vPos.z, 1.0f);//仮のYベクトル
				//	break;
				}

				vecX = XMVector3Normalize(XMVector3Cross(vecZ, Vup));
				XMStoreFloat3(&vX, vecX);
				g_vCameraX = XMVectorSet(vX.x, vX.y, vX.z, 1.0f);

				Vup = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
				XMStoreFloat3(&m_vUp, Vup);
				g_vCameraY = Vup;
				break;

			case CYLINDER_FIELD:
				Vup = XMVectorSet(
					g_pEnemy->GetCylinderP()->GetPos().x - m_vPos.x,
					g_pEnemy->GetCylinderP()->GetPos().y - m_vPos.y,
					g_pEnemy->GetCylinderP()->GetPos().z - m_vPos.z, 1.0f);//仮のYベクトル
				vecX = XMVector3Normalize(XMVector3Cross(vecZ, Vup));
				XMStoreFloat3(&vX, vecX);
				g_vCameraX = XMVectorSet(vX.x, vX.y, vX.z, 1.0f);

				Vup = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
				XMStoreFloat3(&m_vUp, Vup);
				g_vCameraY = Vup;
				break;
			}



#if SETCONTROLLER
			///////////////////////////////////////////////////////////////////////////////////
					//--------------		Ｒスティック処理		-----------------
					//・Ｌスティック処理と同じ。まとめると長くなるので分割。
			if (g_pXController->Gamepad.sThumbRX	<  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&&
				g_pXController->Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				g_pXController->Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&&
				g_pXController->Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			{//.遊び部分にＲスティックがある場合は無効化する。
				g_pXController->Gamepad.sThumbRX = 0;
				g_pXController->Gamepad.sThumbRY = 0;
			}
			else
			{
#endif
#ifdef _DEBUG


				/*if (GetKeyPress(VK_W)) {
					dx = CAMERA_MOVE_SPEED * CosDeg(-m_vAngle.y + 90);
					dz = CAMERA_MOVE_SPEED * SinDeg(-m_vAngle.y + 90);
				}
				if (GetKeyPress(VK_A)) {
					dx = CAMERA_MOVE_SPEED * CosDeg(-m_vAngle.y - 180);
					dz = CAMERA_MOVE_SPEED * SinDeg(-m_vAngle.y - 180);
				}
				if (GetKeyPress(VK_S)) {
					dx = CAMERA_MOVE_SPEED * CosDeg(-m_vAngle.y - 90);
					dz = CAMERA_MOVE_SPEED * SinDeg(-m_vAngle.y - 90);
				}
				if (GetKeyPress(VK_D)) {
					dx = CAMERA_MOVE_SPEED * CosDeg(-m_vAngle.y);
					dz = CAMERA_MOVE_SPEED * SinDeg(-m_vAngle.y);
				}
				if (GetKeyPress(VK_Y)) {
					dy = CAMERA_MOVE_SPEED;
				}
				if (GetKeyPress(VK_N)) {
					dy = -CAMERA_MOVE_SPEED;
				}
	*/

	//**************************************************
	// 向きが0°の時
				m_vPos.x += dx;
				m_vPos.y += dy;
				m_vPos.z += dz;
				m_vTarget.x += dx;
				m_vTarget.z += dz;

				// 注視点移動
				dy = 0.0f;

				/*if (GetKeyPress(VK_T)) {
					dy += CAMERA_MOVE_SPEED;
				}
				if (GetKeyPress(VK_B)) {
					dy += -CAMERA_MOVE_SPEED;
				}*/

				m_vTarget.y += dy;


#endif
				if (GetKeyPress(VK_Z)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
#endif
					) {
					m_vAngle.y += CAMERA_ROTATE_SPEED;
					vAngleY += CAMERA_ROTATE_SPEED;

					if (m_vAngle.y >= 360.0f) {
						m_vAngle.y -= 360.0f;
					}
				}

				if (GetKeyPress(VK_C)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
#endif
					) {
					m_vAngle.y -= CAMERA_ROTATE_SPEED;
					vAngleY -= CAMERA_ROTATE_SPEED;

					if (m_vAngle.y < 0.0f) {
						m_vAngle.y += 360.0f;
					}
				}

				if (GetKeyPress(VK_R)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
#endif
					) {
					m_vAngle.x += CAMERA_ROTATE_SPEED;
					vAngleX += CAMERA_ROTATE_SPEED;

					if (m_vAngle.x >= 30.0f) {
						m_vAngle.x = 30.0f;
						vAngleX = 0.0f;
					}
				}

				if (GetKeyPress(VK_V)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
#endif
					) {
					m_vAngle.x -= CAMERA_ROTATE_SPEED;
					vAngleX -= CAMERA_ROTATE_SPEED;

					if (m_vAngle.x < -20.0f) {
						m_vAngle.x = -20.0f;
						vAngleX = 0.0f;
					}
				}


#if SETCONTROLLER

			}
#endif


			// 角度に対する回転マトリックスを求める


			switch (g_pPlayer->GetOnField())
			{
			case RECT_FIELD:

				break;
			case CIRCLE_FIELD:
				switch (g_pPlayer->GetOnFieldName())
				{
				case BODY_NAME:
					mRY = XMMatrixRotationAxis(XMVectorSet(
						g_pPlayer->GetPosModel().x - g_pEnemy->GetCylinderP()->GetPos().x,
						g_pPlayer->GetPosModel().y - g_pEnemy->GetCylinderP()->GetPos().y,
						g_pPlayer->GetPosModel().z - g_pEnemy->GetCylinderP()->GetPos().z,
						1.0f), XMConvertToRadians(vAngleY));
					mRX = XMMatrixRotationAxis(vecX, XMConvertToRadians(vAngleX));
					break;
				case FRONTCUBE_NAME:
					/*XMMATRIX */mRY = XMMatrixRotationAxis(XMVectorSet(
						g_pPlayer->GetPosModel().x - g_pEnemy->GetSpherFrontP()->GetPos().x,
						g_pPlayer->GetPosModel().y - g_pEnemy->GetSpherFrontP()->GetPos().y,
						g_pPlayer->GetPosModel().z - g_pEnemy->GetSpherFrontP()->GetPos().z,
						1.0f), XMConvertToRadians(vAngleY));
					mRX = XMMatrixRotationAxis(vecX, XMConvertToRadians(vAngleX));
					break;
				//case BACKCUBE_NAME:
				//	/*XMMATRIX */mRY = XMMatrixRotationAxis(XMVectorSet(
				//		g_pPlayer->GetPosModel().x - g_pEnemy->GetSpherBackP()->GetPos().x,
				//		g_pPlayer->GetPosModel().y - g_pEnemy->GetSpherBackP()->GetPos().y,
				//		g_pPlayer->GetPosModel().z - g_pEnemy->GetSpherBackP()->GetPos().z,
				//		1.0f), XMConvertToRadians(vAngleY));
				//	mRX = XMMatrixRotationAxis(vecX, XMConvertToRadians(vAngleX));
				//	break;
				}


				//座標を回転マトリックスで回転する
				v = XMVectorSet(m_vPos.x - m_vTarget.x, m_vPos.y - m_vTarget.y, m_vPos.z - m_vTarget.z, 1.0f);

				v = XMVector3TransformCoord(v, mRY);
				v = XMVector3TransformCoord(v, XMLoadFloat4x4(&g_pPlayer->GetRotateNorMatrix()));
				v = XMVector3TransformCoord(v, mRX);

				XMStoreFloat3(&m_vPos, v);


				vDefVec = XMVectorSet(m_vDefvec.x - m_vTarget.x, m_vDefvec.y - m_vTarget.y, m_vDefvec.z - m_vTarget.z, 1.0f);
				vDefVec = XMVector3TransformCoord(vDefVec, XMLoadFloat4x4(&g_pPlayer->GetRotateNorMatrix()));
				XMStoreFloat3(&m_vDefvec, vDefVec);

				break;
			case CYLINDER_FIELD:
				mRY = XMMatrixRotationAxis(XMVectorSet(
					g_pPlayer->GetPosModel().x - g_pEnemy->GetCylinderP()->GetPos().x,
					g_pPlayer->GetPosModel().y - g_pEnemy->GetCylinderP()->GetPos().y,
					g_pPlayer->GetPosModel().z - g_pEnemy->GetCylinderP()->GetPos().z,
					1.0f),
					XMConvertToRadians(vAngleY));
				mRX = XMMatrixRotationAxis(vecX, XMConvertToRadians(vAngleX));
				//座標を回転マトリックスで回転する
				v = XMVectorSet(m_vPos.x - m_vTarget.x, m_vPos.y - m_vTarget.y, m_vPos.z - m_vTarget.z, 1.0f);
				v = XMVector3TransformCoord(v, mRY);
				v = XMVector3TransformCoord(v, XMLoadFloat4x4(&g_pPlayer->GetRotateNorMatrix()));
				v = XMVector3TransformCoord(v, mRX);

				vDefVec = XMVectorSet(m_vDefvec.x - m_vTarget.x, m_vDefvec.y - m_vTarget.y, m_vDefvec.z - m_vTarget.z, 1.0f);
				vDefVec = XMVector3TransformCoord(vDefVec, XMLoadFloat4x4(&g_pPlayer->GetRotateNorMatrix()));
				XMStoreFloat3(&m_vDefvec, vDefVec);

				XMStoreFloat3(&m_vPos, v);
				break;
			}

			if (GetKeyPress(VK_RETURN)
#if SETCONTROLLER
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_RIGHT_SHOULDER
#endif
				) {
				m_vPos.x = m_vDefvec.x;
				m_vPos.y = m_vDefvec.y;
				m_vPos.z = m_vDefvec.z;

				m_vAngle.y = 0.0f;
				m_vAngle.x = 0.0f;

			}

			break;
//********************************************************************************************************************************************
//********************************************************************************************************************************************
		case SCENE_RUSH:

			if (!g_pRushEnemy->GetWash()->GetDeathflg()) {
				//CSound::Stop(BGM_GMST);	// 効果音再生
	   // カメラ移動
	   // カメラの座標にモデルの移動ベクトルを加える（カメラ追従）

				//XMFLOAT3 vCameraMove;
				pModelPos = &g_pRushPlayer->GetPosModel();

				g_vModelMove.x = pModelPos->x - g_ModelOldPos.x;
				g_vModelMove.y = pModelPos->y - g_ModelOldPos.y;
				g_vModelMove.z = pModelPos->z - g_ModelOldPos.z;

				g_ModelOldPos.x = pModelPos->x;
				g_ModelOldPos.y = pModelPos->y;
				g_ModelOldPos.z = pModelPos->z;

				m_vTarget.x += g_vModelMove.x;
				m_vTarget.y += g_vModelMove.y;
				m_vTarget.z += g_vModelMove.z;


				m_vPos.x += g_vModelMove.x;
				m_vPos.y += g_vModelMove.y;
				m_vPos.z += g_vModelMove.z;

				m_vDefvec.x += g_vModelMove.x;
				m_vDefvec.y += g_vModelMove.y;
				m_vDefvec.z += g_vModelMove.z;

				//************************************************************************
				switch (g_pRushPlayer->GetOnField())
				{
					XMVECTOR vMove;
					XMVECTOR vMoveLen;
					XMVECTOR VpcNow;
					XMVECTOR VpcNext;
					XMFLOAT3 f3V1, f3V2;
					float fMoveLen;
				case CIRCLE_FIELD:
					vMove = XMVectorSet(g_vModelMove.x, g_vModelMove.y, g_vModelMove.z, 1.0f);
					vMoveLen = XMVector3Length(vMove);
					fMoveLen = XMVectorGetX(vMoveLen);

					if (!fMoveLen == 0.0f)
					{
						VpcNow = XMVectorSet(m_vPos.x - g_pRushPlayer->GetPosModel().x, m_vPos.y - g_pRushPlayer->GetPosModel().y, m_vPos.z - g_pRushPlayer->GetPosModel().z, 1.0f); // プレイヤーからカメラへのベクトル

						VpcNext = XMVector3TransformNormal(VpcNow, XMLoadFloat4x4(&g_pRushPlayer->GetRotateMatrix()));


						XMStoreFloat3(&f3V1, VpcNow);
						XMStoreFloat3(&f3V2, VpcNext);

						m_vPos.x += f3V2.x - f3V1.x;
						m_vPos.y += f3V2.y - f3V1.y;
						m_vPos.z += f3V2.z - f3V1.z;

						VpcNow = XMVectorSet(m_vDefvec.x - g_pRushPlayer->GetPosModel().x, m_vDefvec.y - g_pRushPlayer->GetPosModel().y, m_vDefvec.z - g_pRushPlayer->GetPosModel().z, 1.0f); // プレイヤーからカメラへのベクトル

						VpcNext = XMVector3TransformNormal(VpcNow, XMLoadFloat4x4(&g_pRushPlayer->GetRotateMatrix()));

						XMStoreFloat3(&f3V1, VpcNow);
						XMStoreFloat3(&f3V2, VpcNext);

						m_vDefvec.x += f3V2.x - f3V1.x;
						m_vDefvec.y += f3V2.y - f3V1.y;
						m_vDefvec.z += f3V2.z - f3V1.z;

					}
					break;
				case CYLINDER_FIELD:
					vMove = XMVectorSet(g_vModelMove.x, g_vModelMove.y, g_vModelMove.z, 1.0f);
					vMoveLen = XMVector3Length(vMove);
					fMoveLen = XMVectorGetX(vMoveLen);

					if (!fMoveLen == 0.0f)
					{
						VpcNow = XMVectorSet(m_vPos.x - g_pRushPlayer->GetPosModel().x, m_vPos.y - g_pRushPlayer->GetPosModel().y, m_vPos.z - g_pRushPlayer->GetPosModel().z, 1.0f); // プレイヤーからカメラへのベクトル

						VpcNext = XMVector3TransformNormal(VpcNow, XMLoadFloat4x4(&g_pRushPlayer->GetRotateMatrix()));

						XMStoreFloat3(&f3V1, VpcNow);
						XMStoreFloat3(&f3V2, VpcNext);

						m_vPos.x += f3V2.x - f3V1.x;
						m_vPos.y += f3V2.y - f3V1.y;
						m_vPos.z += f3V2.z - f3V1.z;

						VpcNow = XMVectorSet(m_vDefvec.x - g_pRushPlayer->GetPosModel().x, m_vDefvec.y - g_pRushPlayer->GetPosModel().y, m_vDefvec.z - g_pRushPlayer->GetPosModel().z, 1.0f); // プレイヤーからカメラへのベクトル

						VpcNext = XMVector3TransformNormal(VpcNow, XMLoadFloat4x4(&g_pRushPlayer->GetRotateMatrix()));

						XMStoreFloat3(&f3V1, VpcNow);
						XMStoreFloat3(&f3V2, VpcNext);

						m_vDefvec.x += f3V2.x - f3V1.x;
						m_vDefvec.y += f3V2.y - f3V1.y;
						m_vDefvec.z += f3V2.z - f3V1.z;

					}
					break;
				}


				//*****************************************************************************************
				vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);

				XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
				g_vCameraZ = XMVectorSet(vZ.x, vZ.y, vZ.z, 1.0f);

				/*XMVECTOR Vup;
				XMVECTOR vecX;
				XMFLOAT3 vX;*/
				switch (g_pRushPlayer->GetOnField()) {
				case RECT_FIELD:

					break;
				case CIRCLE_FIELD:
					switch (g_pRushPlayer->GetOnFieldName())
					{
						//case BODY_NAME:
						//	Vup = XMVectorSet(
						//		g_pRushEnemy->GetCylinderP()->GetPos().x - m_vPos.x,
						//		g_pRushEnemy->GetCylinderP()->GetPos().y - m_vPos.y,
						//		g_pRushEnemy->GetCylinderP()->GetPos().z - m_vPos.z, 1.0f);//仮のYベクトル
						//	break;
					case FRONTCUBE_NAME:
						Vup = XMVectorSet(
							g_pRushEnemy->GetWash()->GetSpherFrontP()->GetPos().x - m_vPos.x,
							g_pRushEnemy->GetWash()->GetSpherFrontP()->GetPos().y - m_vPos.y,
							g_pRushEnemy->GetWash()->GetSpherFrontP()->GetPos().z - m_vPos.z, 1.0f);//仮のYベクトル
						break;
						//case BACKCUBE_NAME:
						//	Vup = XMVectorSet(
						//		g_pRushEnemy->GetSpherBackP()->GetPos().x - m_vPos.x,
						//		g_pRushEnemy->GetSpherBackP()->GetPos().y - m_vPos.y,
						//		g_pRushEnemy->GetSpherBackP()->GetPos().z - m_vPos.z, 1.0f);//仮のYベクトル
						//	break;
					}

					vecX = XMVector3Normalize(XMVector3Cross(vecZ, Vup));
					XMStoreFloat3(&vX, vecX);
					g_vCameraX = XMVectorSet(vX.x, vX.y, vX.z, 1.0f);

					Vup = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
					XMStoreFloat3(&m_vUp, Vup);
					g_vCameraY = Vup;
					break;

					//case CYLINDER_FIELD:
					//	Vup = XMVectorSet(
					//		g_pRushEnemy->GetCylinderP()->GetPos().x - m_vPos.x,
					//		g_pRushEnemy->GetCylinderP()->GetPos().y - m_vPos.y,
					//		g_pRushEnemy->GetCylinderP()->GetPos().z - m_vPos.z, 1.0f);//仮のYベクトル
					//	vecX = XMVector3Normalize(XMVector3Cross(vecZ, Vup));
					//	XMStoreFloat3(&vX, vecX);
					//	g_vCameraX = XMVectorSet(vX.x, vX.y, vX.z, 1.0f);

					//	Vup = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
					//	XMStoreFloat3(&m_vUp, Vup);
					//	g_vCameraY = Vup;
					//	break;
				}
			}


#if SETCONTROLLER
			///////////////////////////////////////////////////////////////////////////////////
					//--------------		Ｒスティック処理		-----------------
					//・Ｌスティック処理と同じ。まとめると長くなるので分割。
			if (g_pXController->Gamepad.sThumbRX	<  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&&
				g_pXController->Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				g_pXController->Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&&
				g_pXController->Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			{//.遊び部分にＲスティックがある場合は無効化する。
				g_pXController->Gamepad.sThumbRX = 0;
				g_pXController->Gamepad.sThumbRY = 0;
			}
			else
			{
#endif
#ifdef _DEBUG


				/*if (GetKeyPress(VK_W)) {
					dx = CAMERA_MOVE_SPEED * CosDeg(-m_vAngle.y + 90);
					dz = CAMERA_MOVE_SPEED * SinDeg(-m_vAngle.y + 90);
				}
				if (GetKeyPress(VK_A)) {
					dx = CAMERA_MOVE_SPEED * CosDeg(-m_vAngle.y - 180);
					dz = CAMERA_MOVE_SPEED * SinDeg(-m_vAngle.y - 180);
				}
				if (GetKeyPress(VK_S)) {
					dx = CAMERA_MOVE_SPEED * CosDeg(-m_vAngle.y - 90);
					dz = CAMERA_MOVE_SPEED * SinDeg(-m_vAngle.y - 90);
				}
				if (GetKeyPress(VK_D)) {
					dx = CAMERA_MOVE_SPEED * CosDeg(-m_vAngle.y);
					dz = CAMERA_MOVE_SPEED * SinDeg(-m_vAngle.y);
				}
				if (GetKeyPress(VK_Y)) {
					dy = CAMERA_MOVE_SPEED;
				}
				if (GetKeyPress(VK_N)) {
					dy = -CAMERA_MOVE_SPEED;
				}
	*/

	//**************************************************
	// 向きが0°の時
				m_vPos.x += dx;
				m_vPos.y += dy;
				m_vPos.z += dz;
				m_vTarget.x += dx;
				m_vTarget.z += dz;

				// 注視点移動
				dy = 0.0f;

				/*if (GetKeyPress(VK_T)) {
					dy += CAMERA_MOVE_SPEED;
				}
				if (GetKeyPress(VK_B)) {
					dy += -CAMERA_MOVE_SPEED;
				}*/

				m_vTarget.y += dy;


#endif
				if (GetKeyPress(VK_Z)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
#endif
					) {
					m_vAngle.y += CAMERA_ROTATE_SPEED;
					vAngleY += CAMERA_ROTATE_SPEED;

					if (m_vAngle.y >= 360.0f) {
						m_vAngle.y -= 360.0f;
					}
				}

				if (GetKeyPress(VK_C)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
#endif
					) {
					m_vAngle.y -= CAMERA_ROTATE_SPEED;
					vAngleY -= CAMERA_ROTATE_SPEED;

					if (m_vAngle.y < 0.0f) {
						m_vAngle.y += 360.0f;
					}
				}

				if (GetKeyPress(VK_R)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
#endif
					) {
					m_vAngle.x += CAMERA_ROTATE_SPEED;
					vAngleX += CAMERA_ROTATE_SPEED;

					if (m_vAngle.x >= 30.0f) {
						m_vAngle.x = 30.0f;
						vAngleX = 0.0f;
					}
				}

				if (GetKeyPress(VK_V)
#if SETCONTROLLER
					|| g_pXController->Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
#endif
					) {
					m_vAngle.x -= CAMERA_ROTATE_SPEED;
					vAngleX -= CAMERA_ROTATE_SPEED;

					if (m_vAngle.x < -20.0f) {
						m_vAngle.x = -20.0f;
						vAngleX = 0.0f;
					}
				}


#if SETCONTROLLER

			}
#endif


			// 角度に対する回転マトリックスを求める


			switch (g_pRushPlayer->GetOnField())
			{
			case RECT_FIELD:

				break;
			case CIRCLE_FIELD:
				switch (g_pRushPlayer->GetOnFieldName())
				{
				/*case BODY_NAME:
					mRY = XMMatrixRotationAxis(XMVectorSet(
						g_pRushPlayer->GetPosModel().x - g_pRushEnemy->GetCylinderP()->GetPos().x,
						g_pRushPlayer->GetPosModel().y - g_pRushEnemy->GetCylinderP()->GetPos().y,
						g_pRushPlayer->GetPosModel().z - g_pRushEnemy->GetCylinderP()->GetPos().z,
						1.0f), XMConvertToRadians(vAngleY));
					mRX = XMMatrixRotationAxis(vecX, XMConvertToRadians(vAngleX));
					break;*/
				case FRONTCUBE_NAME:
					/*XMMATRIX */mRY = XMMatrixRotationAxis(XMVectorSet(
						g_pRushPlayer->GetPosModel().x - g_pRushEnemy->GetWash()->GetSpherFrontP()->GetPos().x,
						g_pRushPlayer->GetPosModel().y - g_pRushEnemy->GetWash()->GetSpherFrontP()->GetPos().y,
						g_pRushPlayer->GetPosModel().z - g_pRushEnemy->GetWash()->GetSpherFrontP()->GetPos().z,
						1.0f), XMConvertToRadians(vAngleY));
					mRX = XMMatrixRotationAxis(vecX, XMConvertToRadians(vAngleX));
					break;
				//case BACKCUBE_NAME:
				//	/*XMMATRIX */mRY = XMMatrixRotationAxis(XMVectorSet(
				//		g_pRushPlayer->GetPosModel().x - g_pRushEnemy->GetSpherBackP()->GetPos().x,
				//		g_pRushPlayer->GetPosModel().y - g_pRushEnemy->GetSpherBackP()->GetPos().y,
				//		g_pRushPlayer->GetPosModel().z - g_pRushEnemy->GetSpherBackP()->GetPos().z,
				//		1.0f), XMConvertToRadians(vAngleY));
				//	mRX = XMMatrixRotationAxis(vecX, XMConvertToRadians(vAngleX));
				//	break;
				}


				//座標を回転マトリックスで回転する
				v = XMVectorSet(m_vPos.x - m_vTarget.x, m_vPos.y - m_vTarget.y, m_vPos.z - m_vTarget.z, 1.0f);

				v = XMVector3TransformCoord(v, mRY);
				v = XMVector3TransformCoord(v, XMLoadFloat4x4(&g_pRushPlayer->GetRotateNorMatrix()));
				v = XMVector3TransformCoord(v, mRX);

				XMStoreFloat3(&m_vPos, v);


				vDefVec = XMVectorSet(m_vDefvec.x - m_vTarget.x, m_vDefvec.y - m_vTarget.y, m_vDefvec.z - m_vTarget.z, 1.0f);
				vDefVec = XMVector3TransformCoord(vDefVec, XMLoadFloat4x4(&g_pRushPlayer->GetRotateNorMatrix()));
				XMStoreFloat3(&m_vDefvec, vDefVec);

				break;
			//case CYLINDER_FIELD:
			//	mRY = XMMatrixRotationAxis(XMVectorSet(
			//		g_pRushPlayer->GetPosModel().x - g_pRushEnemy->GetCylinderP()->GetPos().x,
			//		g_pRushPlayer->GetPosModel().y - g_pRushEnemy->GetCylinderP()->GetPos().y,
			//		g_pRushPlayer->GetPosModel().z - g_pRushEnemy->GetCylinderP()->GetPos().z,
			//		1.0f),
			//		XMConvertToRadians(vAngleY));
			//	mRX = XMMatrixRotationAxis(vecX, XMConvertToRadians(vAngleX));
			//	//座標を回転マトリックスで回転する
			//	v = XMVectorSet(m_vPos.x - m_vTarget.x, m_vPos.y - m_vTarget.y, m_vPos.z - m_vTarget.z, 1.0f);
			//	v = XMVector3TransformCoord(v, mRY);
			//	v = XMVector3TransformCoord(v, XMLoadFloat4x4(&g_pRushPlayer->GetRotateNorMatrix()));
			//	v = XMVector3TransformCoord(v, mRX);

			//	vDefVec = XMVectorSet(m_vDefvec.x - m_vTarget.x, m_vDefvec.y - m_vTarget.y, m_vDefvec.z - m_vTarget.z, 1.0f);
			//	vDefVec = XMVector3TransformCoord(vDefVec, XMLoadFloat4x4(&g_pRushPlayer->GetRotateNorMatrix()));
			//	XMStoreFloat3(&m_vDefvec, vDefVec);

			//	XMStoreFloat3(&m_vPos, v);
			//	break;
			}


			///*mRY = XMMatrixRotationAxis(XMVectorSet(
			//	g_pPlayer->GetPosModel().x - g_pEnemy->GetSpherFrontP()->GetPos().x,
			//	g_pPlayer->GetPosModel().y - g_pEnemy->GetSpherFrontP()->GetPos().y,
			//	g_pPlayer->GetPosModel().z - g_pEnemy->GetSpherFrontP()->GetPos().z,
			//	1.0f), XMConvertToRadians(vAngleY));
			//mRX = XMMatrixRotationAxis(vecX, XMConvertToRadians(vAngleX));*/

			////座標を回転マトリックスで回転する
			//v = XMVectorSet(m_vPos.x - m_vTarget.x, m_vPos.y - m_vTarget.y, m_vPos.z - m_vTarget.z, 1.0f);

			///*v = XMVector3TransformCoord(v, mRY);
			//v = XMVector3TransformCoord(v, XMLoadFloat4x4(&g_pPlayer->GetRotateNorMatrix()));
			//v = XMVector3TransformCoord(v, mRX);*/

			//XMStoreFloat3(&m_vPos, v);


			//vDefVec = XMVectorSet(m_vDefvec.x - m_vTarget.x, m_vDefvec.y - m_vTarget.y, m_vDefvec.z - m_vTarget.z, 1.0f);
			//vDefVec = XMVector3TransformCoord(vDefVec, XMLoadFloat4x4(&g_pPlayer->GetRotateNorMatrix()));
			//XMStoreFloat3(&m_vDefvec, vDefVec);

			if (GetKeyPress(VK_RETURN)
#if SETCONTROLLER
				|| g_pXController->Gamepad.wButtons& XINPUT_GAMEPAD_RIGHT_SHOULDER
#endif
				) {
				m_vPos.x = m_vDefvec.x;
				m_vPos.y = m_vDefvec.y;
				m_vPos.z = m_vDefvec.z;

				m_vAngle.y = 0.0f;
				m_vAngle.x = 0.0f;

			}

			break;
		}

		// 注視点を中心に回転(車を中心にするときは車の座標を足す)
		m_vPos.x += m_vTarget.x;
		m_vPos.y += m_vTarget.y;
		m_vPos.z += m_vTarget.z;

		m_vDefvec.x += m_vTarget.x;
		m_vDefvec.y += m_vTarget.y;
		m_vDefvec.z += m_vTarget.z;

	}

	if (g_pPlayer->GetEnzine()->GetRotSpeed() > BREAK_ROT) {
		m_fFovY -= 0.2f;
		if (m_fFovY < 40.0f) {
			m_fFovY = 40.0f;
		}
	}
	else {
		if (m_fFovY < 45.0f) {
			m_fFovY += 0.2f;
		}
	}

	


	//ビューマトリックスプロジェクション更新
	UpdateMatrix();




	PrintDebugProc("[ｶﾒﾗ ｲﾁ : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	PrintDebugProc("[ﾁｭｳｼﾃﾝ : (%f, %f, %f)]\n", m_vTarget.x, m_vTarget.y, m_vTarget.z);

	PrintDebugProc("[ｶﾒﾗ ﾑｷ : (%f)]\n", m_vAngle.y);
	PrintDebugProc("\n");

	PrintDebugProc("*** ｶﾒﾗ ｿｳｻ ***\n");
	PrintDebugProc("ﾏｴ   ｲﾄﾞｳ : W\n");
	PrintDebugProc("ｳｼﾛ  ｲﾄﾞｳ : S\n");
	PrintDebugProc("ﾋﾀﾞﾘ ｲﾄﾞｳ : A\n");
	PrintDebugProc("ﾐｷﾞ  ｲﾄﾞｳ : D\n");
	PrintDebugProc("ｳｴ   ｲﾄﾞｳ : Y\n");
	PrintDebugProc("ｼﾀ   ｲﾄﾞｳ : N\n");

	PrintDebugProc("ﾋﾀﾞﾘ ｾﾝｶｲ : Z\n");
	PrintDebugProc("ﾐｷﾞ  ｾﾝｶｲ : C\n");

	PrintDebugProc("*** ﾁｭｳｼﾃﾝ ｿｳｻ ***\n");
	PrintDebugProc("ｳｴ   ｲﾄﾞｳ : T\n");
	PrintDebugProc("ｼﾀ   ｲﾄﾞｳ : B\n");

	/*PrintDebugProc("ﾋﾀﾞﾘ ｾﾝｶｲ : Q\n");
	PrintDebugProc("ﾐｷﾞ  ｾﾝｶｲ : E\n");*/
	PrintDebugProc("\n");

}

// ビュー/プロジェクション マトリックス更新
void CCamera::UpdateMatrix()
{

	//*****************************************************************************************
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));

	//SetWorldMatrix(CalcWorldMatrix());
}

// ワールド マトリックス設定
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{

	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

// 視点/注視点/上方ベクトルからワールド マトリックス算出
XMFLOAT4X4& CCamera::CalcWorldMatrix()
{
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
	XMVECTOR vecY = XMLoadFloat3(&m_vUp);


	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);

	m_mtxWorld._11 = vX.x;
	m_mtxWorld._12 = vX.y;
	m_mtxWorld._13 = vX.z;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = vY.x;
	m_mtxWorld._22 = vY.y;
	m_mtxWorld._23 = vY.z;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._31 = vZ.x;
	m_mtxWorld._32 = vZ.y;
	m_mtxWorld._33 = vZ.z;
	m_mtxWorld._34 = 0.0f;
	m_mtxWorld._41 = m_vPos.x;
	m_mtxWorld._42 = m_vPos.y;
	m_mtxWorld._43 = m_vPos.z;
	m_mtxWorld._44 = 1.0f;

	return m_mtxWorld;
}



XMVECTOR* CCamera::GetCameraXYZ(int nXYZ)
{
	switch (nXYZ)
	{
	case 0:
		return &g_vCameraX;
	case 1:
		return &g_vCameraY;
	case 2:
		return &g_vCameraZ;
	default:
		break;
	}
	return 0;
}

//=============================================================================
// プレイヤーオブジェクト取得
//=============================================================================
void CCamera::SetCameraPlayer(CPlayer* pPlayer)
{
	g_pPlayer = pPlayer;
}

//=============================================================================
// プレイヤーオブジェクト取得
//=============================================================================
void CCamera::SetCameraRushPlayer(CRushPlayer* pPlayer)
{
	g_pRushPlayer = pPlayer;
}

//=============================================================================
// エネミーオブジェクト取得
//=============================================================================
void CCamera::SetCameraEnemy(CEnemyVacuum* pEnemy)
{
	g_pEnemy = pEnemy;
}

void CCamera::SetCameraRushEnemy(CEnemyRushVacuum* pEnemy)
{
	g_pRushEnemy = pEnemy;
}

//=============================================================================
// コントローラーオブジェクト取得
//=============================================================================
void SetCameraControllerr(XINPUT_STATE* pControlller)
{
	g_pXController = pControlller;
}
//=============================================================================
// 世界が動いているフラグ取得
//=============================================================================
bool CCamera::GetWorldMoveflg(void)
{
	return g_bWorldMove;
}

// カメラインスタンス取得
CCamera* CCamera::Get()
{
	return &g_camera;
}
bool GetWorldMoveflg(void)
{
	return g_bWorldMove;

}
void SetWorldMoveflg(bool bWorldMove)
{
	g_bWorldMove = bWorldMove;

}

void SetTime2(bool bTime2) {
	g_Time2 = bTime2;
}

