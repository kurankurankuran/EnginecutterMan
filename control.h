#pragma once
#include "main.h"
//-------	XInput.h セット	--------//
//#include <Windows.h>				//.本当は<Windows.h>も必要だけど"main.h"にあるので省略
#include <Xinput.h>					//.<XInput.h>は動かす部分にのみIncludeで十分
#pragma comment(lib, "xinput.lib")	//
//----------------------------------//
#include "model.h"
#include "engine.h"
#include "title.h"
#include "StageSelect.h"
#include "gameclear.h"
#include "gameover.h"
#include "rushPlayer.h"
#include "enemyParts.h"
#include "pause.h"
#include "game.h"
//.			構造体			.//

//.XINPUT_STATE のインスタンスを用意
XINPUT_STATE			Xstate;
//---	XINPUT_STATE 内訳	---//
//	dwPacketNumber	->	コントローラーの識別番号（１P,２Pなど）、最大４つ（０～３）まで接続可能
//	Gamepad			->	コントローラーのボタンの情報（どこのボタンが押されているかなど）


//.XINPUT_VIBRATION のインスタンスを用意
XINPUT_VIBRATION		Vibration;	//.バイブレーション機能を使用しない場合は不要
//---	XINPUT_VIBRATION 内訳	---//
//	wLeftMotorSpeed		->	左のバイブレーション（モーター）の強さ
//	wRightMotorSpeed	->	右のバイブレーション（モーター）の強さ

//.			定数定義		.//

#define MAX_THUMB		32700
#define MAX_PLAYER		(4)

//.		グローバル部		.//
using namespace			std;		//.iostream用
bool					Continue;	//.main関数ループ用

// 		エントリポイント	 //
//.コントローラーの初期化
void InitController()
{
	//.コントローラーのメモリ初期化＆確保
	ZeroMemory(&Xstate, sizeof(XINPUT_STATE));
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	DWORD dwResult = XInputGetState(0, &Xstate);		//.１Ｐ（０番目）のハンドル獲得
	DWORD dwResultV = XInputSetState(0, &Vibration);

	if (dwResult == ERROR_SUCCESS)					//.コントローラーが接続されているかどうか
	{//.コントローラーが接続されている場合
		// Controller is connected
		SetControllerr(&Xstate);
		SetEnzineControllerr(&Xstate, &Vibration);
		SetTitleControllerr(&Xstate);
		SetCameraControllerr(&Xstate);
		SetKirimeControllerr(&Xstate, &Vibration);
		SetStageSelectControllerr(&Xstate);
		SetGameClearControllerr(&Xstate);
		SetGameOverControllerr(&Xstate);
		SetRushPlayerController(&Xstate);
		SetControllerrEnemyParts(&Xstate);
		SetPauseControllerr(&Xstate);
		SetGameControllerr(&Xstate);

	}
	else
	{//.コントローラーが接続されていない場合
		MessageBox(NULL, _T("コントローラーが接続されていません"), _T("WARNING"), MB_OK);
	}


}

//.コントローラーの終了
void UnInitController()
{//.コントローラーの終了処理。
	XInputEnable(false);	//バイブレーションを使用する場合は必須。
}

void UpdateController()
{
	DWORD dwResult = XInputGetState(0, &Xstate);
	DWORD dwResultV = XInputSetState(0, &Vibration);
}

XINPUT_STATE* GetXinputState(void)
{
	return &Xstate;
}

XINPUT_VIBRATION* GetXinputVibration(void)
{
	return  &Vibration; 
}

