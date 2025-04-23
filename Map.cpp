#include "Include.h"

/// <summary>
/// �C���X�^���X��
/// </summary>
Map::Map()
{
	backGroundHandle = MV1LoadModel("material/skyDome/sunSet.mv1");
	fieldHandle = MV1LoadModel("material/mv1/uploads_files_2988017_round+table.mv1");
	position = VGet(0, 0, 0);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Map::~Map()
{

}

/// <summary>
/// ������
/// </summary>
void Map::Initialize()
{
	//���f���̑傫������
	MV1SetScale(fieldHandle, VGet(1.0f, 0.17f, 1.0f));

	MV1SetPosition(fieldHandle, position);
}

/// <summary>
/// �X�V
/// </summary>
void Map::Update()
{

}

/// <summary>
/// �`��
/// </summary>
void Map::Draw()
{
	//3d�`��
	MV1DrawModel(backGroundHandle);
	MV1DrawModel(fieldHandle);

}