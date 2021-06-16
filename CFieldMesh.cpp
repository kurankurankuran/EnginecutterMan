//=============================================================================
//
// 地面、壁処理 [CFieldMesh.cpp]
// Author :	RYUSUKE YAMADA
//
//=============================================================================
#include "CFieldMesh.h"
#include "Shader.h"
#include "Texture.h"
#include "input.h"
#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	TEXTURE_WALL1		L"data/texture/field000.jpg"	// 壁1
//#define	TEXTURE_WALL2		L"data/texture/field000.jpg"	// 壁2
//#define	TEXTURE_WALL3		L"data/texture/field000.jpg"	// 壁3
//#define	TEXTURE_WALL4		L"data/texture/field000.jpg"	// 壁4
//#define	TEXTURE_FLOOR		L"data/texture/field000.jpg"	// 床
//#define	TEXTURE_CEILING	L"data/texture/field000.jpg"	// 天井

#define	TEXTURE_WALL1		L"data/texture/wall2.png"		// 壁1
#define	TEXTURE_WALL2		L"data/texture/wall2.png"		// 壁2
#define	TEXTURE_WALL3		L"data/texture/wall2.png"		// 壁3
#define	TEXTURE_WALL4		L"data/texture/wall2.png"		// 壁4
#define	TEXTURE_FLOOR		L"data/texture/floor.png"		// 床
#define	TEXTURE_CEILING	L"data/texture/tile.png"	// 天井

#define	M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define	M_SPECULAR	XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define	M_AMBIENT	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define	M_EMISSIVE	XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define	VALUE_MOVE		(5.0f)				// 移動量
#define	VALUE_ROTATE	(3.6f)				// 回転量

#define	SIZE_X					(1000.0f)		// 壁サイズ(X方向)
#define	SIZE_Z					(1000.0f)		// 壁サイズ(Z方向)

#define	MAX_WALL			(6)					// 壁の数	

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static HRESULT MakeVertexField(ID3D11Device* pDevice ,MESH* pMesh);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;								// テクスチャへのポインタ
static MESH											g_meshField[MAX_FILENAME];				// 構造体
static MATERIAL									g_material;								// マテリアル
static int													g_nStat[MAX_WALL];	// 使用判別用

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
// 初期化処理
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


			// 位置・回転・法線ベクトルの初期設定
			g_meshField[i].pos = XMFLOAT3(pos.x, pos.y, pos.z);		// 位置
			g_meshField[i].rot = XMFLOAT3(rot.x, rot.y, rot.z);		// 角度
			g_meshField[i].vNor = XMFLOAT3(nor.x, nor.y, nor.z);		// 法線ベクトル
			g_meshField[i].size = XMFLOAT3(size.x, size.y, size.z);		// サイズ		

			// マテリアルの初期設定
			g_material.Diffuse = M_DIFFUSE;
			g_material.Ambient = M_AMBIENT;
			g_material.Specular = M_SPECULAR;
			g_material.Power = 50.0f;
			g_material.Emissive = M_EMISSIVE;
			g_meshField[i].pMaterial = &g_material;

			// テクスチャの読み込み
			hr = CreateTextureFromFile(pDevice,		// デバイスへのポインタ
				g_pFileName[nFileName],						// ファイルの名前
				&g_meshField[i].pTexture);							// 読み込むメモリー
			if (FAILED(hr))
				return hr;
			SaveMesh(&g_meshField[i]);

			XMStoreFloat4x4(&g_meshField[i].mtxTexture, XMMatrixIdentity());

			// 頂点情報の作成
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
// 終了処理
//=============================================================================
void CFieldMesh::UninitField(void)
{
#if MESH_RELEASE_ON
		for (int i = 0; i < MAX_WALL; i++)
		{
			// メッシュ解放
			ReleaseMesh(&g_meshField[i]);
		}
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void CFieldMesh::UpdateField(void)
{
	for (int i = 0; i < MAX_WALL; i++)
	{
		// メッシュ更新
		UpdateMesh(&g_meshField[i]);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFieldMesh::DrawField(void)
{

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	for (int i = 0; i < MAX_WALL; i++)
	{
		// メッシュの描画
		DrawMesh(pDeviceContext, &g_meshField[i]);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(ID3D11Device* pDevice ,MESH* pMesh)
{
	VERTEX_3D		vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	int					indexWk[NUM_VERTEX];	// インデックス格納ワーク

	/*for (int i = 0; i < MAX_WALL; i++)
	{*/
		// 頂点座標の設定
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

		//// 法線ベクトルの設定
		//vertexWk[0].nor = XMFLOAT3(g_meshField[i].vNor.x, g_meshField[i].vNor.y, g_meshField[i].vNor.z);
		//vertexWk[1].nor = XMFLOAT3(g_meshField[i].vNor.x, g_meshField[i].vNor.y, g_meshField[i].vNor.z);
		//vertexWk[2].nor = XMFLOAT3(g_meshField[i].vNor.x, g_meshField[i].vNor.y, g_meshField[i].vNor.z);
		//vertexWk[3].nor = XMFLOAT3(g_meshField[i].vNor.x, g_meshField[i].vNor.y, g_meshField[i].vNor.z);
		// 法線ベクトルの設定
		vertexWk[0].nor = XMFLOAT3(pMesh->vNor.x, pMesh->vNor.y, pMesh->vNor.z);
		vertexWk[1].nor = XMFLOAT3(pMesh->vNor.x, pMesh->vNor.y, pMesh->vNor.z);
		vertexWk[2].nor = XMFLOAT3(pMesh->vNor.x, pMesh->vNor.y, pMesh->vNor.z);
		vertexWk[3].nor = XMFLOAT3(pMesh->vNor.x, pMesh->vNor.y, pMesh->vNor.z);

		// 拡散反射光の設定
		vertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
		vertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
		vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
		vertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

		//// インデックス配列の設定
		//g_meshField[i].nNumIndex = NUM_VERTEX;
		//indexWk[0] = 0;
		//indexWk[1] = 1;
		//indexWk[2] = 2;
		//indexWk[3] = 3;
		// インデックス配列の設定
		pMesh->nNumIndex = NUM_VERTEX;
		indexWk[0] = 0;
		indexWk[1] = 1;
		indexWk[2] = 2;
		indexWk[3] = 3;

		return MakeMeshVertex(pDevice, pMesh, vertexWk, indexWk);
	//}
}
