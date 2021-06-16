//=============================================================================
//
// 地面、壁処理 [CFieldMesh.h]
// Author :	RYUSUKE YAMADA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// 列挙体宣言
//*****************************************************************************
typedef enum
{
	WALL1 = 0,		// 壁1
	WALL2,			// 壁2
	WALL3,			// 壁3
	WALL4,			// 壁4
	FLOOR,			// 床
	CEILING,			// 天井

	MAX_FILENAME
}FILENAME;

enum EAlphaMode
{
	ALPHAMODE_NONE = 0,				// 区別なく表示
	ALPHAMODE_OPACITY,				// 不透明のみ描画
	ALPHAMODE_TRANSPARENCY,	// 半透明のみ描画
	ALPHAMODE_MAX
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFieldMesh {
private:


public:
	HRESULT InitField(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 nor, XMFLOAT3 size, int nFileName);
	void UninitField(void);
	void UpdateField(void);
	void DrawField(void);
};

