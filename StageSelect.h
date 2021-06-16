//=============================================================================
//
// ステージセレクト処理 [StageSelect.h]
// Author : RYUSUKE	YAMADA
//
//=============================================================================

#pragma once
#include <Xinput.h>
// ポーズメニュー
typedef enum
{
	/* ステージ追加の場合下に追記 */
	STAGE_1,	
	STAGE_2,
	STAGE_3,

	STAGE_SELECT_MAX
} STAGE_SELECT_MENU;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStageSelect();
void UninitStageSelect();
void UpdateStageSelect();
void DrawStageSelect();

void SetStageSelect(void);
STAGE_SELECT_MENU GetStageSelect(void);
void SetStageSelectControllerr(XINPUT_STATE* pControlller);