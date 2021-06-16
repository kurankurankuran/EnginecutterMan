//=============================================================================
//
// メッシュ地面処理 [meshfield.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include <Xinput.h>
#define MAX_KIRIME   (60)
// クラスの前方宣言
//class CCamera;
//class CLight;
class CKirime {
private:
	ID3D11ShaderResourceView*	 m_pTexture;				// テクスチャ
	
	XMFLOAT3                     m_NorPos;
	XMFLOAT3                     m_KakunoPos;
	//CPlayer                      m_Player;
	int                m_Cnt[MAX_KIRIME];
	XMFLOAT3		m_posModel[MAX_KIRIME];		// 現在の位置
	XMFLOAT3		m_rotModel[MAX_KIRIME];		// 現在の向き
	XMFLOAT3		m_SclModel[MAX_KIRIME];		// 現在の向き
	XMFLOAT4X4		m_mtxWorld[MAX_KIRIME];         // ワールド行列

	int m_Life1;
	int m_Life2;
	int m_Life3;
	int m_Life4;
public:
	//HRESULT InitmeshKirime(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f);
	HRESULT InitmeshKirime(void);
	void UninitmeshKirime(void);
	void UpdatemeshKirime(void);
	void DrawmeshKirime(void);
	int CollisionKirime(DirectX::XMFLOAT3 vPos, XMFLOAT3 vPosPlayer, float Long,
		float Width, float High,
		int nDamage, DirectX::XMFLOAT3* pvHitPos);
	XMFLOAT3 GetVecXYZ();
	XMFLOAT3* GetKakunoPos();

	/*void SetmeshKirimeL(CLight* pLight);
	void SetmeshKirimeC(CCamera* pCamera);*/
	
	/*HRESULT MakeVertexKirime(ID3D11Device* pDevice,
		int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY,
		float fTexSizeX, float fTexSizeY);*/
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool GetDestflg(int);
void SetKirimeControllerr(XINPUT_STATE* pControlller, XINPUT_VIBRATION *pVibration);
//void SetmeshKirimePlayer(CPlayer* pPlayer);