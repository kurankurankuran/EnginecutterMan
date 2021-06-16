//=============================================================================
//
// �X���[�N���� [smoke.h]
// Author : ���{�@�ّ��Y
//
//=============================================================================
#pragma once

#include "main.h"
//#include "model.h"
//#include "camera.h"

class CCamera;
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_SMOKE		(100)							// �G�t�F�N�g�ő吔
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	XMFLOAT3 pos;			// �ʒu
	XMFLOAT3 rot;			// ��]
	XMFLOAT3 move;			// �ړ���
	XMFLOAT4 col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nIdxShadow;			// �eID
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
} SMOKE;



//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSmoke {
private:


	SMOKE			m_aSmoke[MAX_SMOKE];			// �p�[�e�B�N�����[�N
	XMFLOAT3		m_posBase;						// �r���{�[�h�����ʒu

	float           m_lenX;                         // �r���{�[�h�����͈�X
	float           m_lenY;                         // �r���{�[�h�����͈�Y
	float			m_fWidthBase = 1.0f;			// ��̕�
	float			m_fHeightBase = 1.0f;			// ��̍���
	int	     		m_nTimer;						// �X���[�N���������鎞��
	bool            m_bGenerate;
	bool			m_bPause = false;				// �|�[�YON/OFF
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
// �v���g�^�C�v�錾
//*****************************************************************************


//int SpawnSmoke(XMFLOAT3 pos, XMFLOAT3 dir);

//void SetSmokeEnemyParts(EnemyParts* pEnemyParts);
