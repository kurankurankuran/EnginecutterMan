#pragma once
#include "main.h"

#include "model.h"
#include "rushPlayer.h"
//#include "enemy.h"
#include "enemyVacuum.h"
#include "enemyRushVacuum.h"
#include <Xinput.h>
// クラスの前方宣言
class CPlayer;
class CRushPlayer;
class CEnemy;

namespace {
	enum {
		INS_CENEMY = 0,

		MAX_INS,

	}UpdateInstans;
}
class CCamera
{
private:
	DirectX::XMFLOAT3 m_vPos;		// 視点
	DirectX::XMFLOAT3 m_vTarget;	// 注視点
	DirectX::XMFLOAT3 m_vUp;		// 上方ベクトル
	//DirectX::XMFLOAT3 m_vMove;		// 運動量
	DirectX::XMFLOAT3 m_vDefvec;		// カメラ位置リセット用
	

	float m_fAspectRatio;			// 縦横比
	float m_fFovY;					// 視野角(Degree)
	float m_fNearZ;					// 前方クリップ距離
	float m_fFarZ;					// 後方クリップ距離

	DirectX::XMFLOAT4X4 m_mtxWorld;	// ワールド マトリックス
	DirectX::XMFLOAT4X4 m_mtxView;	// ビュー マトリックス
	DirectX::XMFLOAT4X4 m_mtxProj;	// プロジェクション マトリックス

	DirectX::XMFLOAT3 m_vAngle;		// カメラの角度
	float m_fLengthInterval;		// カメラの視点と注視点の距離

	int m_nScene;
	int m_nUpdateInstans;
public:
	CCamera();

	void Init();
	void Update();

	void SetPos(DirectX::XMFLOAT3& vPos) { m_vPos = vPos; }
	DirectX::XMFLOAT3& GetPos() { return m_vPos; }
	DirectX::XMFLOAT3& GetTarget() { return m_vTarget; }
	void SetPos(float x, float y, float z) { m_vPos.x = x, m_vPos.y = y, m_vPos.z = z; }
	void SetTarget(DirectX::XMFLOAT3& vTarget) { m_vTarget = vTarget; }
	void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
	void SetUpVector(DirectX::XMFLOAT3& vUp) { m_vUp = vUp; }
	void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
	void SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld);
	DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_mtxWorld; }
	DirectX::XMFLOAT4X4& CalcWorldMatrix();
	void SetFovY(float fFovY) { m_fFovY = fFovY; }
	void SetAspectRatio(float fAspect) { m_fAspectRatio = fAspect; }
	void SetRangeZ(float fNearZ, float fFarZ) { m_fNearZ = fNearZ, m_fFarZ = fFarZ; }
	void UpdateMatrix();
	DirectX::XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
	DirectX::XMFLOAT4X4& GetProjMatrix() { return m_mtxProj; }
	DirectX::XMFLOAT3& GetAngle() { return m_vAngle; }
	static CCamera* Get();
	XMVECTOR* GetCameraXYZ(int nXYZ);
	XMFLOAT3* GetCameraUp() { return &m_vUp; }
	
	void SetCameraPlayer(CPlayer* pPlayer);/* { g_pPlayer = pPlayer; }*/
	void SetCameraRushPlayer(CRushPlayer* pPlayer);/* { g_pPlayer = pPlayer; }*/
	void SetCameraEnemy(CEnemyVacuum* pEnemy);/* { g_pEnemy = pEnemy; }*/
	void SetCameraRushEnemy(CEnemyRushVacuum* pEnemy);/* { g_pEnemy = pEnemy; }*/

	void SetUpdateIns(int nIns) { m_nUpdateInstans = nIns; }
	int GetUpdateIns(void) { return m_nUpdateInstans; }
	bool GetWorldMoveflg(void);
};
//void SetCameraPlayer(CPlayer* pPlayer);
//void SetCameraEnemy(CEnemy* pEnemy);

bool GetWorldMoveflg(void);
void SetWorldMoveflg(bool bWorldMove);
void SetCameraControllerr(XINPUT_STATE* pControlller);
void SetTime2(bool bTime2);