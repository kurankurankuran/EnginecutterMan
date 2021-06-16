//=============================================================================
//
// 弾処理 [engine.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "engine.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENGINE_TEXTURENAME	L"data/texture/Engine.png"	// テクスチャファイル名
//#define SE_ENGINE1_PATH      "data/sound/se_maoudamashii_retro19.mp3"

//#define SE_ENGINE2_PATH      "data/sound/se_maoudamashii_element_thunder05.mp3"


#define ENGINE_SIZE_X		174								// ポリゴンのサイズ
#define ENGINE_SIZE_Y		174								// ポリゴンのサイズ
#define ENGINE_FRAME_X		4								// 横フレーム数
#define ENGINE_FRAME_Y		4								// 縦フレーム数




#define ANIM_DEATHSHOW_FRAME      (BREAK_ROT/15)   //アニメーションの１コマのフレーム数
#define ANIM_DEATHSHOW_SPLIT_X    (4)   //画像の分割数（横）
#define ANIM_DEATHSHOW_SPLIT_Y    (4)   //画像の分割数（縦）




//*****************************************************************************
// グローバル変数
//*****************************************************************************
static XINPUT_STATE* g_pXController;
XINPUT_VIBRATION* g_pVibration;

static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ

static bool         g_bLoadMesh = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnzine::Initengine()
{
	HRESULT hr = S_OK;
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {	
		// テクスチャ読み込み
	    hr = CreateTextureFromFile(GetDevice(),
			ENGINE_TEXTURENAME, &g_pTexture);
		SaveTex(g_pTexture);

		g_bLoadMesh = true;
	}
	m_RotSpeed = 0;

	// 情報初期化
	for (int i = 0; i < MAX_ENGINE; ++i)
	{
		
		g_engine[i].pos = XMFLOAT2(500.0f, -250.0f);

		g_engine[i].posTexCoord = XMFLOAT2(0.0f, 0.0f);

		//初期は一番画像の一番左上のコマ
		g_engine[i].sizeTexCoord = XMFLOAT2(1.0f / (float)ENGINE_FRAME_X,
			1.0f / (float)ENGINE_FRAME_Y);

		//テクスチャサイズは後で分割数の定数を変えるだけでいいようにしておく
		g_engine[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//エフェクトの色合いを変えだければここでｒｇｂの編集を
		g_engine[i].frame = 0;          //このカウンターを更新時に増やしていく
		g_engine[i].currentAnimNo = 0;  //画像のコマ割りした何コマ目か「０スタート」

	}
	
	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnzine::Uninitengine()
{
	CSound::Stop(BGM_PRAAY);

#if SETCONTROLLER
	g_pVibration->wLeftMotorSpeed = 0;
	g_pVibration->wRightMotorSpeed = 0;
#endif

#if  MESH_RELEASE_ON
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnzine::Updateengine()
{
	if (GetKeyPress(VK_SPACE)
#if SETCONTROLLER
		|| g_pXController->Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD
#endif
		)
	{
		++m_RotSpeed;
		if (m_RotSpeed > VAL_MAXROT) {
			m_RotSpeed = VAL_MAXROT;
		}
	}
	else {
		--m_RotSpeed;
		if (m_RotSpeed < 0) {
			m_RotSpeed = 0;
		}
	}

	// ブレーキ
	if (GetKeyPress(VK_B)
#if SETCONTROLLER
		|| g_pXController->Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD
#endif
		) {
		m_RotSpeed -= VAL_MAXROT / (2 * 60);
		if (m_RotSpeed < 0) {
			m_RotSpeed = 0;
		}
	}


	// 更新
	ST_ENGINE_PARAM* pengine = g_engine;

	for (int i = 0; i < MAX_ENGINE; ++i, ++pengine)
	{

		g_engine[i].frame = m_RotSpeed/*g_pPlayer->GetRotSpeed()*/;
		/*if (g_engine[i].frame > BREAK_ROT)
			g_engine[i].frame = BREAK_ROT;
		else if (g_engine[i].frame < 0)
			g_engine[i].frame = 0;*/

		if (GetKeyPress(VK_SPACE) 
#if SETCONTROLLER
			|| g_pXController->Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD
#endif
			)
		{
#if SETCONTROLLER
			//バイブレーション回転数増加
			if (g_pVibration->wLeftMotorSpeed < MAX_VIBRATION)g_pVibration->wLeftMotorSpeed += ADD_VIBRATION;
			if (g_pVibration->wRightMotorSpeed < MAX_VIBRATION)g_pVibration->wRightMotorSpeed += ADD_VIBRATION;
#endif

			if (!(g_engine[i].frame > BREAK_ROT))
			{
				//アニメーション（フレーム）更新
				if (0 == g_engine[i].frame%ANIM_DEATHSHOW_FRAME)
				{
					g_engine[i].currentAnimNo++;
				}
				//アニメーション番号更新（次のアニメーションへ変更をかける)
				if (16 == g_engine[i].currentAnimNo)
				{
					g_engine[i].currentAnimNo = 15;  //現在のコマ番号を更新する

				}
			}
		}
		else
		{
#if SETCONTROLLER
			//バイブレーション回転数減少
			if (g_pVibration->wLeftMotorSpeed > 0)g_pVibration->wLeftMotorSpeed -= ADD_VIBRATION * 2;
			if (g_pVibration->wRightMotorSpeed > 0)g_pVibration->wRightMotorSpeed -= ADD_VIBRATION * 2;
#endif

			if (!(g_engine[i].frame >= BREAK_ROT))
			{
				if (0 == g_engine[i].frame%ANIM_DEATHSHOW_FRAME)
				{
					g_engine[i].currentAnimNo--;
				}
				//アニメーション番号更新（次のアニメーションへ変更をかける)
				if (1 >= g_engine[i].currentAnimNo)
				{
					g_engine[i].currentAnimNo = 0;

				}
			}

			CSound::Stop(BGM_PRAAY);
			


		}

		if (15 == g_engine[i].currentAnimNo)
		{
			
			CSound::Play(BGM_PRAAY);	// 効果音再生
			CSound::SetVolume(BGM_PRAAY, 1.0f);

		}

		
		

	}

	
}

//=============================================================================
//描画処理
//=============================================================================
void CEnzine::Drawengine()
{
	// 描画
	// 半透明処理有効

	//SetPolygonColor(1.0f, 1.0f, 1.0f);
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	SetBlendState(BS_ALPHABLEND);
	SetPolygonSize(ENGINE_SIZE_X, ENGINE_SIZE_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonFrameSize(1.0f / ENGINE_FRAME_X, 1.0f / ENGINE_FRAME_Y);
	
	

	ST_ENGINE_PARAM* pengine = g_engine;
	for (int i = 0; i < MAX_ENGINE; ++i, ++pengine) {
		
		SetPolygonPos(pengine->pos.x, pengine->pos.y);
		int u =g_engine->currentAnimNo % ENGINE_FRAME_X;
		int v = g_engine->currentAnimNo / ENGINE_FRAME_X;
		SetPolygonUV(u / (float)ENGINE_FRAME_X, v / (float)ENGINE_FRAME_Y);

		ST_ENGINE_PARAM* pBullet = g_engine;
	
	}
	DrawPolygon(pDeviceContext);
	// フレームを戻しておく
	//
	//SetPolygonColor(0.0f, 0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);

	// 半透明処理無効
	SetBlendState(BS_NONE);
}

//=============================================================================
// コントローラーオブジェクト取得
//=============================================================================
void SetEnzineControllerr(XINPUT_STATE* pControlller, XINPUT_VIBRATION *pVibration)
{
	g_pXController = pControlller;
	g_pVibration = pVibration;
}

