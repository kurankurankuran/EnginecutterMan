//=============================================================================
//
// ���f������ [model.h]
// Author :KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"

#include "enemy.h"
#include "life.h"
#include "engine.h"
//#include "enemyVacuum.h"
#include <Xinput.h>
//#include "AssimpModel.h"
//*****************************************************************************
// �}�N���錾
//*****************************************************************************
#define MODEL_POSX (0.0f)
#define MODEL_POSY (600.0f)
#define MODEL_POSZ (100.0f)
#define BREAK_ROT           (270)         // �P�ʎ��ԉ�]���̌��E
//*****************************************************************************
// �񋓑�
//*****************************************************************************
namespace {
	enum {
		NONE_FIELD = 0,
		RECT_FIELD,
		CIRCLE_FIELD,
		CYLINDER_FIELD,

		MAX_FIELDTYPE

	}FIELDTYPE;

	enum {
		NONE_NAME = 0,
		BODY_NAME,
		FRONTCUBE_NAME,
		BACKCUBE_NAME,

		MAX_FIELDNAME

	}FIELDNAME;
}

// �N���X�̑O���錾
//class CCamera;
//class Enemy;
class CEnemyVacuum;
class CAssimpModel;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer
{
private:
	//int             m_nLife;            // ���C�t

	XMFLOAT4X4		m_mtxWorld;         // ���[���h�s��

	XMFLOAT3		m_posModel;         // ���W�p
	XMFLOAT3		m_posCut;         // �n���W�p
	XMFLOAT3        m_vecCut;         // �n�x�N�g��

	XMFLOAT3		m_sclModel;         // �傫�������p

	XMFLOAT3		m_moveModel;	    // �ړ���

	XMFLOAT3        m_f3modelY;         // �v���C���[�̏�����x�N�g��
	
 	XMFLOAT3        m_vecRotDest;                    // �ړI�����x�N�g��
	XMFLOAT3        m_vecNowRot;
	
	
	XMFLOAT4X4      m_VRotToNextMat4x4;    // �����t���[�����̉^���ʕ��v���C���[���W����]������s��
	XMFLOAT4X4      m_RotMat4x4;           // ��ɋ��̂̒��S�����ɂ��邽�߂̉�]�s��
	XMFLOAT4X4      m_RotationY4x4;                         // �v���C���[�̏�x�N�g������]�s��
	XMFLOAT4X4      m_RotationNor4x4;                       // �v���C���[�̌�����ς����]�s��


	int             m_nOnFieldType;                    // ����Ă���n�`�̌`��
	int             m_nOnFieldName;                    // ����Ă���n�`�̖���

	bool            m_bBreak;            // �\���t���O

	CLife           m_Life;
	int             m_nInvTime;         // ���G����
	bool            m_bInvflg;          // ���G�t���O
	CEnzine         m_Enzine;

	float           m_fRazianDest;
	float           m_fRazian; 
	bool            m_bJumpflg;
	//XMFLOAT3        m_f3JumpMoveDest;
	XMFLOAT3        m_f3JumpMoveUp;
	XMFLOAT3        m_f3JumpMoveDown;
	bool            m_bMove;

	int             m_nModelType;
protected:

public:
	
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CPlayer() {}
	CPlayer(int nLife) :m_Life(nLife)
	{
		Init();
	}
	~CPlayer() 
	{
		Uninit();
	}


	XMFLOAT3 GetModelY(void) { return m_f3modelY; }
	XMFLOAT3 GetPosModel(void) { return m_posModel; }
	XMFLOAT3 GetPosCut(void) { return m_posCut; }
	XMFLOAT4X4/*XMMATRIX */GetRotateMatrix(void){ return m_VRotToNextMat4x4; }
	XMFLOAT4X4/*XMMATRIX */ GetTotateWorldMatrix(void){ return m_RotMat4x4; }
	int GetOnField(void) { return m_nOnFieldType; }
	int GetOnFieldName(void) { return m_nOnFieldName; }
	bool GetBreakflg(void) { return m_bBreak; }
	XMFLOAT3 GetMove(void) { return m_moveModel; }
	XMFLOAT4X4/*XMMATRIX */ GetRotateNorMatrix(void){ return m_RotationNor4x4; }
	CEnzine* GetEnzine(void) { return &m_Enzine; }
	CLife* GetLife(void) { return &m_Life; }
	bool GetInvflg(void) { return m_bInvflg; }
	bool GetMoveflg(void) { return m_bMove; }
	//void SetPlayerCamera(CCamera*);
	//void SetPlayerLight(CLight*);
	void SetPlayerEnemy(CEnemyVacuum* pEnemy);
	void SetOnFieldType(int nType) { m_nOnFieldType = nType; }
	void SetOnFieldName(int nName) { m_nOnFieldName = nName; }
	void SetInvflg(bool bInv) { m_bInvflg = bInv; }
	void SetMoveflg(bool bMove) { m_bMove = bMove; }

	
	//void DamageLife(int nDamage) { m_nLife -= nDamage; }
};
CAssimpModel* GetPlayerModel(void);

void SetControllerr(XINPUT_STATE* pControlller);
XMVECTOR  GetModelNor(void);