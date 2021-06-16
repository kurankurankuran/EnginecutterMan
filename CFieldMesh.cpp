//=============================================================================
//
// �n�ʁA�Ǐ��� [CFieldMesh.cpp]
// Author :	RYUSUKE YAMADA
//
//=============================================================================
#include "CFieldMesh.h"
#include "Shader.h"
#include "Texture.h"
#include "input.h"
#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	TEXTURE_WALL1		L"data/texture/field000.jpg"	// ��1
//#define	TEXTURE_WALL2		L"data/texture/field000.jpg"	// ��2
//#define	TEXTURE_WALL3		L"data/texture/field000.jpg"	// ��3
//#define	TEXTURE_WALL4		L"data/texture/field000.jpg"	// ��4
//#define	TEXTURE_FLOOR		L"data/texture/field000.jpg"	// ��
//#define	TEXTURE_CEILING	L"data/texture/field000.jpg"	// �V��

#define	TEXTURE_WALL1		L"data/texture/wall2.png"		// ��1
#define	TEXTURE_WALL2		L"data/texture/wall2.png"		// ��2
#define	TEXTURE_WALL3		L"data/texture/wall2.png"		// ��3
#define	TEXTURE_WALL4		L"data/texture/wall2.png"		// ��4
#define	TEXTURE_FLOOR		L"data/texture/floor.png"		// ��
#define	TEXTURE_CEILING	L"data/texture/tile.png"	// �V��

#define	M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define	M_SPECULAR	XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define	M_AMBIENT	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define	M_EMISSIVE	XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define	VALUE_MOVE		(5.0f)				// �ړ���
#define	VALUE_ROTATE	(3.6f)				// ��]��

#define	SIZE_X					(1000.0f)		// �ǃT�C�Y(X����)
#define	SIZE_Z					(1000.0f)		// �ǃT�C�Y(Z����)

#define	MAX_WALL			(6)					// �ǂ̐�	

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static HRESULT MakeVertexField(ID3D11Device* pDevice ,MESH* pMesh);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;								// �e�N�X�`���ւ̃|�C���^
static MESH											g_meshField[MAX_FILENAME];				// �\����
static MATERIAL									g_material;								// �}�e���A��
static int													g_nStat[MAX_WALL];	// �g�p���ʗp

static bool g_bLoadMesh = false;

const wchar_t* g_pFileName[] =
{
	TEXTURE_WALL1,
	TEXTURE_WALL2,
	TEXTURE_WALL3,
	TEXTURE_WALL4,
	TEXTURE_FLOOR,
	TEXTURE_CEILING
};



//=============================================================================
// ����������
//=============================================================================
HRESULT CFieldMesh::InitField(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 nor, XMFLOAT3 size, int nFileName)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr=S_OK;
#if MESH_RELEASE_ON
	g_bLoadMesh = false;
#endif
	if (!g_bLoadMesh) {

		for (int i = 0; i < MAX_WALL; i++)
		{
			if (!(g_nStat[i] <= 0))
			{
				continue;
			}


			// �ʒu�E��]�E�@���x�N�g���̏����ݒ�
			g_meshField[i].pos = XMFLOAT3(pos.x, pos.y, pos.z);		// �ʒu
			g_meshField[i].rot = XMFLOAT3(rot.x, rot.y, rot.z);		// �p�x
			g_meshField[i].vNor = XMFLOAT3(nor.x, nor.y, nor.z);		// �@���x�N�g��
			g_meshField[i].size = XMFLOAT3(size.x, size.y, size.z);		// �T�C�Y		

			// �}�e���A���̏����ݒ�
			g_material.Diffuse = M_DIFFUSE;
			g_material.Ambient = M_AMBIENT;
			g_material.Specular = M_SPECULAR;
			g_material.Power = 50.0f;
			g_material.Emissive = M_EMISSIVE;
			g_meshField[i].pMaterial = &g_material;

			// �e�N�X�`���̓ǂݍ���
			hr = CreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
				g_pFileName[nFileName],						// �t�@�C���̖��O
				&g_meshField[i].pTexture);							// �ǂݍ��ރ������[
			if (FAILED(hr))
				return hr;
			SaveMesh(&g_meshField[i]);

			XMStoreFloat4x4(&g_meshField[i].mtxTexture, XMMatrixIdentity());

			// ���_���̍쐬
			hr = MakeVertexField(pDevice, &g_meshField[i]);

			g_nStat[i] = 1;

			if (i == (MAX_WALL-1)) {
				g_bLoadMesh = true;
			}

			break;
		}
		
	}

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void CFieldMesh::UninitField(void)
{
#if MESH_RELEASE_ON
		for (int i = 0; i < MAX_WALL; i++)
		{
			// ���b�V�����
			ReleaseMesh(&g_meshField[i]);
		}
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void CFieldMesh::UpdateField(void)
{
	for (int i = 0; i < MAX_WALL; i++)
	{
		// ���b�V���X�V
		UpdateMesh(&g_meshField[i]);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFieldMesh::DrawField(void)
{

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_WALL; i++)
	{
		// ���b�V���̕`��
		DrawMesh(pDeviceContext, &g_meshField[i]);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(ID3D11Device* pDevice ,MESH* pMesh)
{
	VERTEX_3D		vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	int					indexWk[NUM_VERTEX];	// �C���f�b�N�X�i�[���[�N

	/*for (int i = 0; i < MAX_WALL; i++)
	{*/
		// ���_���W�̐ݒ�
		pMesh->nNumVertex = NUM_VERTEX;
		//g_meshField[i].nNumVertex = NUM_VERTEX;
		vertexWk[0].vtx = XMFLOAT3(-pMesh->size.x, pMesh->size.y, pMesh->size.z);
		vertexWk[1].vtx = XMFLOAT3(pMesh->size.x, pMesh->size.y, pMesh->size.z);
		vertexWk[2].vtx = XMFLOAT3(-pMesh->size.x, pMesh->size.y, -pMesh->size.z);
		vertexWk[3].vtx = XMFLOAT3(pMesh->size.x, pMesh->size.y, -pMesh->size.z);
		/*vertexWk[0].vtx = XMFLOAT3(-SIZE_X, 0.0f, SIZE_Z);
		vertexWk[1].vtx = XMFLOAT3(SIZE_X, 0.0f, SIZE_Z);
		vertexWk[2].vtx = XMFLOAT3(-SIZE_X, 0.0f, -SIZE_Z);
		vertexWk[3].vtx = XMFLOAT3(SIZE_X, 0.0f, -SIZE_Z);*/

		//// �@���x�N�g���̐ݒ�
		//vertexWk[0].nor = XMFLOAT3(g_meshField[i].vNor.x, g_meshField[i].vNor.y, g_meshField[i].vNor.z);
		//vertexWk[1].nor = XMFLOAT3(g_meshField[i].vNor.x, g_meshField[i].vNor.y, g_meshField[i].vNor.z);
		//vertexWk[2].nor = XMFLOAT3(g_meshField[i].vNor.x, g_meshField[i].vNor.y, g_meshField[i].vNor.z);
		//vertexWk[3].nor = XMFLOAT3(g_meshField[i].vNor.x, g_meshField[i].vNor.y, g_meshField[i].vNor.z);
		// �@���x�N�g���̐ݒ�
		vertexWk[0].nor = XMFLOAT3(pMesh->vNor.x, pMesh->vNor.y, pMesh->vNor.z);
		vertexWk[1].nor = XMFLOAT3(pMesh->vNor.x, pMesh->vNor.y, pMesh->vNor.z);
		vertexWk[2].nor = XMFLOAT3(pMesh->vNor.x, pMesh->vNor.y, pMesh->vNor.z);
		vertexWk[3].nor = XMFLOAT3(pMesh->vNor.x, pMesh->vNor.y, pMesh->vNor.z);

		// �g�U���ˌ��̐ݒ�
		vertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
		vertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
		vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
		vertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

		//// �C���f�b�N�X�z��̐ݒ�
		//g_meshField[i].nNumIndex = NUM_VERTEX;
		//indexWk[0] = 0;
		//indexWk[1] = 1;
		//indexWk[2] = 2;
		//indexWk[3] = 3;
		// �C���f�b�N�X�z��̐ݒ�
		pMesh->nNumIndex = NUM_VERTEX;
		indexWk[0] = 0;
		indexWk[1] = 1;
		indexWk[2] = 2;
		indexWk[3] = 3;

		return MakeMeshVertex(pDevice, pMesh, vertexWk, indexWk);
	//}
}
