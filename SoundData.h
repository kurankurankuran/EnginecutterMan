// BGM/SE定義 (SoundData.h)

enum eBGM {
	BGM_GAME1 = 0,	 // GAME1 BGM
	BGM_TITLE,		 // TITLE BGM
	BGM_GAMECLEAR,	 // GAMECLEAR BGM
	BGM_PRAAY,       // 暴走
	BGM_TOKI,
	BGM_CMBE,        // カメラの移動音
	


	MAX_BGM
};

enum eSE {
	SE_PATK = 0,	// PlayerAttack音
	SE_EATK,        // EnemyAttack音
	SE_PMBE,        // プレイヤー移動音
	SE_BUTTON,		// ボタン選択
	SE_KETTEI,		// 決定音
	SE_HIT,			// ヒット音
	SE_DEATH,		// 死亡音
	SE_GAMEOVER,	// ゲームオーバー音
	//SE_GMST,
	

	MAX_SE
};
