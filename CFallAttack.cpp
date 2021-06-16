//=============================================================================
//
// ���f������ [CFallAttack.cpp]
// Author : RYUSUKE YAMADA
//
//=============================================================================
//#include "main.h"
#include "AssimpModel.h"
#include "CFallAttack.h"
#include "Collision.h"
#include "Texture.h"
#include "mesh.h"
#include "model.h"
#include "hiteffect.h"
#include "explosion.h"
#include "CHitStop.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define ENEMYATTACK_MODEL			"data/model/car001.fbx"
#define MODEL_ATKSET			   "data/model/Enemy/sphere.fbx"
#define MODEL_ATKBULLET			  "data/model/EnemyCore/LeftHand_Attack1.fbx"
#define TEXTURE_HITEFFECT L"data/texture/HitEffect.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define ENEMYATTACK_MOVE		(2)			// �G�������x
#define ENEMYATTACK_DAMAGE      (10)        // �_���[�W
#define ENEMYATTACK_FALLTIME    (5*60)      // ��������
#define ENEMYATTACK_SETY        (200.0f)    // �U�������ݒu���鍂�� 
#define ENEMYATTACK_TIME        (3*60)      // �U�����~���Ă��鎞�Ԃ̊Ԋu 

#define MATERIAL_DIFFUSE	XMFLOAT4(1,1,1,1)
#define MATERIAL_AMBIENT	XMFLOAT4(1,1,1,1)
#define MATERIAL_SPECULAR	XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE	XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER		(0.0f)
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAssimpModel	g_FallAtk;		// ���f��
CAssimpModel	g_lenge;	// ���f��
//static MESH			g_meshHitEffect;			// ���b�V�����
static MATERIAL		g_material;					// �}�e���A��
static CFallAttack*  g_CFA;

static bool g_bModelLoad = false;

CFallAttack::CFallAttack()
{

}

CFallAttack::~CFallAttack()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFallAttack::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	//// �e�N�X�`���̓ǂݍ���


	for (int i = 0; i < MAX_ATKLENGENUM; ++i) {
		m_bLenge[i] = false;
		m_nAtkSetTime[i] = 0;
		m_bUseLenge[i] = false;
		m_posLenge[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);   // �z�u�ʒu�i�����͈͂̓���j
		m_radLenge[i] = 0.0f;   // �_���[�W���̔��a
		m_sclLenge[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);   // �����͈͂̑傫���i����X�A�c��Y�A���s��Z�j
		m_rotLenge[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);   // ��]�p�x (�΂߂ɂ���Ƃ�����)
		

		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(m_sclLenge[i].x, m_sclLenge[i].y, m_sclLenge[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotLenge[i].x),
			XMConvertToRadians(m_rotLenge[i].y), XMConvertToRadians(m_rotLenge[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_posLenge[i].x, m_posLenge[i].y, m_posLenge[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorldLenge[i], mtxWorld);

		
	}

	

	m_bUseLenge[0] = true;
	m_posLenge[0] = XMFLOAT3(530.0f, 80.0f, 710.0f);
	m_radLenge[0] = 40.0f * 2;
	m_sclLenge[0] = XMFLOAT3(m_radLenge[0] / 2, m_radLenge[0] / 2, m_radLenge[0] / 2);   // �����͈͂̑傫���i����X�A�c��Y�A���s��Z�j

	m_bUseLenge[1] = true;
	m_posLenge[1] = XMFLOAT3(-540.0f, 0.0f, 600.0f);
	m_radLenge[1] = 40.0f * 2;
	m_sclLenge[1] = XMFLOAT3(m_radLenge[1] / 2, m_radLenge[1] / 2, m_radLenge[1] / 2);   // �����͈͂̑傫���i����X�A�c��Y�A���s��Z�j

	//m_pLife[0] = new CEnemyLife(m_posLenge[0], 100/*m_nLifeSet[0]*/, true);
	//m_pLife[1] = new CEnemyLife(m_posLenge[1], 100/*m_nLifeSet[1]*/,true);
	m_Life[0].Init(m_posLenge[0], 100/*m_nLifeSet[0]*/, true);
	m_Life[1].Init(m_posLenge[1], 100/*m_nLifeSet[0]*/, true);
	//************************************************************************************************:

	for (int i = 0; i < MAX_FALLATK; ++i) {
		m_bUseAtk[i] = false;
		m_fall[i] = false;
		m_pos[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rad[i] = 20.0f;
		m_scl[i] = XMFLOAT3(20.0f, 20.0f, 20.0f);
		m_rot[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_move[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_nDamage[i] = ENEMYATTACK_DAMAGE;
		m_nFallTime[i] = 20 * 60;
		
		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(m_scl[i].x, m_scl[i].y, m_scl[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rot[i].x),
			XMConvertToRadians(m_rot[i].y), XMConvertToRadians(m_rot[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_pos[i].x, m_pos[i].y, m_pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);

		
	}
#if MODEL_RELEASE_ON
	g_bModelLoad = false;
#endif
	if (!g_bModelLoad) {
		// ���f���f�[�^�̓ǂݍ���
		if (!g_lenge.Load(pDevice, pDeviceContext, MODEL_ATKSET)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_lenge);

		// ���f���f�[�^�̓ǂݍ���
		if (!g_FallAtk.Load(pDevice, pDeviceContext, MODEL_ATKBULLET)) {
			hr = E_FAIL;
			MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitEnemy", MB_OK);
			return hr;
		}
		SaveModel(&g_FallAtk);

		g_bModelLoad = true;
	}
	//// �ۉe�̐���
	//for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	//{
	//	g_nShadow[i] = CreateShadow(m_posEnemy[i], 20.0f);
	//}

	return hr;
}


//=============================================================================
// �I������
//=============================================================================

void CFallAttack::Uninit()
{
	//for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	//{
	//	// �ۉe�̉��
	//	ReleaseShadow(g_nShadow[i]);
	//}
	/*delete m_pLife[1];
	delete m_pLife[0];*/
	m_Life[1].Uninit();
	m_Life[0].Uninit();

	
	// ���f���̉��
#if MODEL_RELEASE_ON
		for (int i = 0; i < MAX_ATKLENGENUM; ++i) {
			g_lenge.Release();
		}
		for (int i = 0; i < MAX_FALLATK; ++i) {
			g_FallAtk.Release();
		}
#endif
	//// ���b�V���̊J��
	//ReleaseMesh(&g_meshHitEffect);
}

//=============================================================================
// �X�V����
//=============================================================================
void CFallAttack::Update()
{

	// �Z�b�g�����U���͈͓��Ƀv���C���[�����邩�ǂ����̔���
	for (int i = 0; i < MAX_ATKLENGENUM; ++i) {
		if (!m_bUseLenge[i])continue;

		if (!m_bLenge[i]) {
			if (CollisionCircleToCircle(m_posLenge[i], m_pPlayer->GetPosModel(), m_radLenge[i], 50.0f)) {
				SetAtkFall();
				m_bLenge[i] = true;
				m_nAtkSetTime[i] = ENEMYATTACK_TIME;
				
			}
		}
		else
		{
			--m_nAtkSetTime[i];
			if (m_nAtkSetTime[i] < 0) {
				m_nAtkSetTime[i] = 0;
				m_bLenge[i] = false;
			}
		}

		if (CollisionCircleToCircle(m_posLenge[i], m_pPlayer->GetPosCut()/*m_pPlayer->GetPosModel()*/, m_radLenge[i], 50.0f)) {
			// �q�b�g�G�t�F�N�g����
			if (m_pPlayer->GetBreakflg()) {
				/*--m_nLife[i];*/
				//m_pLife[i]->DelEnemyLife(10);
				m_Life[i].DelEnemyLife(10);
				SetExplosion(m_posLenge[i], 700.0f, 700.0f);

				CSound::Play(SE_PATK);	// ���ʉ��Đ�
				CSound::SetVolume(SE_PATK, 3.0f);

				CHitStop::Get()->HitStopOn(2);
			}
			if (/*m_pLife[i]->GetLife() <= 0*/m_Life[i].GetLife() <= 0) {
				m_bUseLenge[i] = false;
				SetExplosion(XMFLOAT3(m_posLenge[i].x + 50.0f, m_posLenge[i].y + 50.0f, m_posLenge[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_posLenge[i].x + (-50.0f), m_posLenge[i].y + 50.0f, m_posLenge[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_posLenge[i].x + (-50.0f), m_posLenge[i].y + (-50.0f), m_posLenge[i].z + 50.0f),
					700.0f, 700.0f);
				SetExplosion(XMFLOAT3(m_posLenge[i].x + (-50.0f), m_posLenge[i].y + (-50.0f), m_posLenge[i].z + (-50.0f)),
					700.0f, 700.0f);

			}
		}

		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(m_sclLenge[i].x, m_sclLenge[i].y, m_sclLenge[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotLenge[i].x),
			XMConvertToRadians(m_rotLenge[i].y), XMConvertToRadians(m_rotLenge[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_posLenge[i].x, m_posLenge[i].y, m_posLenge[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorldLenge[i], mtxWorld);

		
	}


	// �_���[�W�̂���͈͂��~���Ă����鏈��(���������������)�Ƃ��ꂪ�v���C���[�Ɠ�����Α̗͂����炷����
	for (int i = 0; i < MAX_FALLATK; ++i) {
		if (!m_bUseAtk[i]) continue;

		if (!m_fall[i]) {
			m_pos[i].x = m_pPlayer->GetPosModel().x + m_pPlayer->GetModelY().x*ENEMYATTACK_SETY;
			m_pos[i].y = m_pPlayer->GetPosModel().y + m_pPlayer->GetModelY().y*ENEMYATTACK_SETY;
			m_pos[i].z = m_pPlayer->GetPosModel().z + m_pPlayer->GetModelY().z*ENEMYATTACK_SETY;
		}
		else {
			--m_nFallTime[i];
			m_pos[i].x -= m_move[i].x*ENEMYATTACK_MOVE;
			m_pos[i].y -= m_move[i].y*ENEMYATTACK_MOVE;
			m_pos[i].z -= m_move[i].z*ENEMYATTACK_MOVE;

			if (CollisionCircleToCircle(m_pos[i], m_pPlayer->GetPosModel(), m_rad[i], 50.0f)) {

				if (!m_pPlayer->GetInvflg())
				{
					m_pPlayer->GetLife()->DelLife(m_nDamage[i]);
					SetHitEffect(m_pPlayer->GetPosModel(), 400.0f, 400.0f);
				}

				m_pos[i].x = m_pPlayer->GetPosModel().x + m_pPlayer->GetModelY().x*ENEMYATTACK_SETY;
				m_pos[i].y = m_pPlayer->GetPosModel().y + m_pPlayer->GetModelY().y*ENEMYATTACK_SETY;
				m_pos[i].z = m_pPlayer->GetPosModel().z + m_pPlayer->GetModelY().z*ENEMYATTACK_SETY;

				m_move[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);

				m_nFallTime[i] = ENEMYATTACK_FALLTIME;
				m_bUseAtk[i] = false;
				m_fall[i] = false;
				m_pPlayer->SetInvflg(true);
			}

			if (m_nFallTime[i] < 0) {
				m_nFallTime[i] = ENEMYATTACK_FALLTIME;
				m_fall[i] = false;
				m_bUseAtk[i] = false;
			}
		}


		XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(m_scl[i].x, m_scl[i].y, m_scl[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rot[i].x),
			XMConvertToRadians(m_rot[i].y), XMConvertToRadians(m_rot[i].z));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_pos[i].x, m_pos[i].y, m_pos[i].z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		XMStoreFloat4x4(&m_mtxWorld[i], mtxWorld);

		//// �ۉe�̈ړ�
		//MoveShadow(g_nShadow[i], m_posEnemy[i]);


	

	}

	/*m_pLife[0]->Update();
	m_pLife[1]->Update();
*/
	m_Life[0].Update();
	m_Life[1].Update();

	
}



//=============================================================================
// �`�揈��
//=============================================================================
void CFallAttack::Draw()
{
	/*m_pLife[0]->SetCamera(m_pCamera);
	m_pLife[1]->SetCamera(m_pCamera);*/
	//m_Life[0].SetCamera(m_pCamera);
	//m_Life[1].SetCamera(m_pCamera);

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �J�����A�����ݒ�
	g_FallAtk.SetCamera(CCamera::Get());
	g_FallAtk.SetLight(CLight::Get());

	for (int i = 0; i < MAX_FALLATK; ++i) {
		if (!m_bUseAtk[i])continue;


		// �s����������`��
		g_FallAtk.Draw(pDeviceContext, m_mtxWorld[i], eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_FallAtk.Draw(pDeviceContext, m_mtxWorld[i], eTransparentOnly);

		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}

	for (int i = 0; i < MAX_ATKLENGENUM; ++i) {
		if (!m_bUseLenge[i])continue;

		// �J�����A�����ݒ�
		g_lenge.SetCamera(CCamera::Get());
		g_lenge.SetLight(CLight::Get());

		// �s����������`��
		g_lenge.Draw(pDeviceContext, m_mtxWorldLenge[i], eOpacityOnly);

		// ������������`��
		SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
		SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
		g_lenge.Draw(pDeviceContext, m_mtxWorldLenge[i], eTransparentOnly);

		SetZWrite(true);				// Z�o�b�t�@�X�V����
		SetBlendState(BS_NONE);			// �A���t�@�u�����h����
	}
	/*m_pLife[0]->Draw();
	m_pLife[1]->Draw();*/
	m_Life[0].Draw();
	m_Life[1].Draw();
}

void CFallAttack::SetAtkFall(void)
{
	for (int i = 0; i < MAX_FALLATK; ++i) {
		if (m_bUseAtk[i])continue;
		m_bUseAtk[i] = true;
		m_fall[i] = true;
		m_pos[i].x = m_pPlayer->GetPosModel().x + m_pPlayer->GetModelY().x*ENEMYATTACK_SETY;
		m_pos[i].y = m_pPlayer->GetPosModel().y + m_pPlayer->GetModelY().y*ENEMYATTACK_SETY;
		m_pos[i].z = m_pPlayer->GetPosModel().z + m_pPlayer->GetModelY().z*ENEMYATTACK_SETY;
		m_move[i] = XMFLOAT3(m_pPlayer->GetModelY().x, m_pPlayer->GetModelY().y, m_pPlayer->GetModelY().z);

		break;
	}
}

//int CFallAttack::SetCFallAttack(XMFLOAT3 pos, float fSizeX, float fSizeY)
//{
//	int nIdxFallAttack = -1;
//
//	CFallAttack* pCFA = g_CFA;
//}