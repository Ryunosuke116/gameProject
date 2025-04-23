#include "Include.h"

/// <summary>
/// インスタンス化
/// </summary>
Map::Map()
{
	backGroundHandle = MV1LoadModel("material/skyDome/sunSet.mv1");
	fieldHandle = MV1LoadModel("material/mv1/uploads_files_2988017_round+table.mv1");
	position = VGet(0, 0, 0);
}

/// <summary>
/// デストラクタ
/// </summary>
Map::~Map()
{

}

/// <summary>
/// 初期化
/// </summary>
void Map::Initialize()
{
	//モデルの大きさ調整
	MV1SetScale(fieldHandle, VGet(1.0f, 0.17f, 1.0f));

	MV1SetPosition(fieldHandle, position);
}

/// <summary>
/// 更新
/// </summary>
void Map::Update()
{

}

/// <summary>
/// 描画
/// </summary>
void Map::Draw()
{
	//3d描画
	MV1DrawModel(backGroundHandle);
	MV1DrawModel(fieldHandle);

}