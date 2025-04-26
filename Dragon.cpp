#include "Include.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="path"></param>
Dragon::Dragon()
{
	modelHandle = MV1LoadModel("material/mv1/dragon/dragon_0410.mv1");
    framePosition = VGet(0.0f, 0.0f, 0.0f);
    isPush = false;
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon::~Dragon()
{

}

/// <summary>
/// 初期化
/// </summary>
void Dragon::Initialize()
{
    isPush = false;
	position = VGet(0, 0, 30.0f);
	MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
	// プレイヤーのモデルの座標を更新する
	MV1SetPosition(modelHandle, position);
	ChangeMotion(0, PlayAnimSpeed);
    framePosition = MV1GetFramePosition(modelHandle, 6);
}

/// <summary>
/// 更新
/// </summary>
void Dragon::Update()
{
	// プレイヤーのモデルの座標を更新する
	MV1SetPosition(modelHandle, position);
	//UpdateAngle(modelHandle);

	//MotionUpdate();

    if (CheckHitKey(KEY_INPUT_3))
    {
        if (!isPush)
        {
            nowFrameNumber++;
            isPush = true;
        }
    }
    else if (CheckHitKey(KEY_INPUT_4))
    {
        if (!isPush)
        {
            isPush = true;
            nowFrameNumber--;
        }
    }
    else
    {
        isPush = false;
    }
    //6
    framePosition = MV1GetFramePosition(modelHandle, 6);
}

/// <summary>
/// 描画
/// </summary>
void Dragon::Draw()
{
    VECTOR framePosition01 = MV1GetFramePosition(modelHandle, nowFrameNumber);
	
    MV1DrawModel(modelHandle);

    DrawSphere3D(framePosition01, 5.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 255, 255), FALSE);

    printfDx("doragon.framePosition (x: %f,y: %f,z: %f)\n",
        framePosition.x, framePosition.y, framePosition.z);


    printfDx("doragon.frame現在数%d\n", nowFrameNumber);
}

void Dragon::Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection)
{

}

void Dragon::MotionUpdate()
{
    float totalTime_anim;

    // アタッチしたアニメーションの総再生時間を取得する
    totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, currentAttachIndex);

    //それぞれの再生スピードで更新
    currentPlayTime_anim += PlayAnimSpeed;

    //総再生時間を超えたら再生時間をリセット
    if (currentPlayTime_anim >= totalTime_anim)
    {
        currentPlayTime_anim = 0.0f;
    }

    // 再生時間をセットする
    MV1SetAttachAnimTime(modelHandle, currentAttachIndex, currentPlayTime_anim);
}