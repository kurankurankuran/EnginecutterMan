//=============================================================================
//
// �n�ʏ��� [field.h]
// Author :KAZUKI KURA
//
//=============================================================================
#pragma once

#include "main.h"

//#include "Camera.h"
#include "CFieldMesh.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CField{
private:
	XMFLOAT3        m_Pos[3];
	XMFLOAT3        m_Scl;
	XMFLOAT3        m_Rot;
	XMFLOAT4X4      m_mtxWorld[3];

	CFieldMesh      m_FieldMesh;

public:
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CField();
	~CField();

	//void SetCamera(CCamera*);
	//void SetLight(CLight*);
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
