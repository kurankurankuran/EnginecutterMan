//=============================================================================
//
// スモーク処理 [smoke.h]
// Author : 釜本　麟太郎
//
//=============================================================================
#pragma once

#include "main.h"
//#include "model.h"
//#include "camera.h"

class CCamera;
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_SMOKE		(100)							// エフェクト最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	XMFLOAT3 pos;			// 位置
	XMFLOAT3 rot;			// 回転
	XMFLOAT3 move;			// 移動量
	XMFLOAT4 col;			// 色
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nIdxShadow;			// 影ID
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか
} SMOKE;



//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSmoke {
private:


	SMOKE			m_aSmoke[MAX_SMOKE];			// パーティクルワーク
	XMFLOAT3		m_posBase;						// ビルボード発生位置

	float           m_lenX;                         // ビルボード発生範囲X
	float           m_lenY;                         // ビルボード発生範囲Y
	float			m_fWidthBase = 1.0f;			// 基準の幅
	float			m_fHeightBase = 1.0f;			// 基準の高さ
	int	     		m_nTimer;						// スモーク発生させる時間
	bool            m_bGenerate;
	bool			m_bPause = false;				// ポーズON/OFF
	CCamera*		m_pCamera;
	//static EnemyParts*      m_pEnemyParts;
	
public:
	HRESULT InitSmoke(void);
	void UninitSmoke(void);
	void UpdateSmoke(void);
	void DrawSmoke(void);

	int SetSmoke(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);

	/*void SetCamera(CCamera* pCamera);*/

	void SetCallSmoke(XMFLOAT3 pos, float lenX, float lenY);

	void SetVertexSmoke(int nIdxSmoke, float fSizeX, float fSizeY);
	void SetColorSmoke(int nIdxSmoke, XMFLOAT4 col);
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//int SpawnSmoke(XMFLOAT3 pos, XMFLOAT3 dir);

//void SetSmokeEnemyParts(EnemyParts* pEnemyParts);
