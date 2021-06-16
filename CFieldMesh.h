//=============================================================================
//
// �n�ʁA�Ǐ��� [CFieldMesh.h]
// Author :	RYUSUKE YAMADA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �񋓑̐錾
//*****************************************************************************
typedef enum
{
	WALL1 = 0,		// ��1
	WALL2,			// ��2
	WALL3,			// ��3
	WALL4,			// ��4
	FLOOR,			// ��
	CEILING,			// �V��

	MAX_FILENAME
}FILENAME;

enum EAlphaMode
{
	ALPHAMODE_NONE = 0,				// ��ʂȂ��\��
	ALPHAMODE_OPACITY,				// �s�����̂ݕ`��
	ALPHAMODE_TRANSPARENCY,	// �������̂ݕ`��
	ALPHAMODE_MAX
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFieldMesh {
private:


public:
	HRESULT InitField(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 nor, XMFLOAT3 size, int nFileName);
	void UninitField(void);
	void UpdateField(void);
	void DrawField(void);
};

