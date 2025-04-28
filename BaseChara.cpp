#include "Include.h"
#include "BaseChara.h"

BaseChara::BaseChara() :
    modelHandle(-1),
    currentAttachIndex(-1),
    motionNum(-1),
    nowFrameNumber(-1),
    keepPlayTime_anim(-1),
    totalTime_anim(-1),
    angle(-1),
    prevAttachIndex(-1),
    animBlendRate(-1),
    prevPlayTime_anim(-1),
    prevTotalTime_anim(-1),
    currentPlayAnimSpeed(-1),
    prevPlayAnimSpeed(-1),
    targetMoveDirection(VGet(0.0f, 0.0f, 0.0f)),
    position(VGet(0.0f, 0.0f, 0.0f))
{

}

BaseChara::~BaseChara()
{}

/// <summary>
/// 初期化
/// </summary>
void BaseChara::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
void BaseChara::Update(){}

/// <summary>
/// 描画
/// </summary>
void BaseChara::Draw()
{}

/// <summary>
/// 移動
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
void BaseChara::Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection)
{

}

/// <summary>
/// 向き
/// </summary>
void BaseChara::UpdateAngle(int& modelHandle)
{
    // プレイヤーの移動方向にモデルの方向を近づける
    float targetAngle;			// 目標角度
    float difference;			// 目標角度と現在の角度との差

    // 目標の方向ベクトルから角度値を算出する
    targetAngle = static_cast<float>(atan2(targetMoveDirection.x, targetMoveDirection.z));

    // 目標の角度と現在の角度との差を割り出す
    // 最初は単純に引き算
    difference = targetAngle - angle;

    // ある方向からある方向の差が１８０度以上になることは無いので
    // 差の値が１８０度以上になっていたら修正する
    if (difference < -DX_PI_F)
    {
        difference += DX_TWO_PI_F;
    }
    else if (difference > DX_PI_F)
    {
        difference -= DX_TWO_PI_F;
    }

    // 角度の差が０に近づける
    if (difference > 0.0f)
    {
        // 差がプラスの場合は引く
        difference -= angleSpeed;
        if (difference < 0.0f)
        {
            difference = 0.0f;
        }
    }
    else
    {
        // 差がマイナスの場合は足す
        difference += angleSpeed;
        if (difference > 0.0f)
        {
            difference = 0.0f;
        }
    }

    // モデルの角度を更新
    angle = targetAngle - difference;
    MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
}

/// <summary>
/// モーション変更
/// </summary>
/// <param name="motionNum"></param>
void BaseChara::ChangeMotion(const int& motionNum, const float playAnimSpeed)
{

    if (prevAttachIndex != -1)
    {
        MV1DetachAnim(modelHandle, prevAttachIndex);
        prevAttachIndex = -1;
    }

    //いままで情報をprevに保存
    prevAttachIndex = currentAttachIndex;
    prevPlayTime_anim = currentPlayTime_anim;
    prevPlayAnimSpeed = currentPlayAnimSpeed;

    //初期化
  //  MV1DetachAnim(modelHandle, currentAttachIndex);

    //// 再生時間をセットする
    //MV1SetAttachAnimTime(modelHandle, currentAttachIndex, playTime_anim);

    this->motionNum = motionNum;

    // ３Ｄモデルの０番目のアニメーションをアタッチする
    currentAttachIndex = MV1AttachAnim(modelHandle, motionNum, -1, FALSE);
   
    // 再生時間の初期化
    currentPlayTime_anim = 0.0f;
    currentPlayAnimSpeed = playAnimSpeed;

    // アタッチしたアニメーションの総再生時間を取得する
    totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, currentAttachIndex);

    // ブレンド率はPrevが有効ではない場合は１．０ｆ( 現在モーションが１００％の状態 )にする
    animBlendRate = prevAttachIndex == -1 ? 1.0f : 0.0f;


}

/// <summary>
/// モーション更新
/// </summary>
void BaseChara::MotionUpdate()
{
    float totalTime_anim;

    // ブレンド率が１以下の場合は１に近づける
    if (animBlendRate < 1.0f)
    {
        animBlendRate += AnimBlendSpeed;
        if (animBlendRate > 1.0f)
        {
            animBlendRate = 1.0f;
        }
    }

    if (currentAttachIndex != -1)
    {
        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, currentAttachIndex);

        //再生時間更新
        currentPlayTime_anim += currentPlayAnimSpeed;
        keepPlayTime_anim = currentPlayTime_anim;

        //総再生時間を超えたらリセット
        if (currentPlayTime_anim >= totalTime_anim)
        {
            currentPlayTime_anim = static_cast<float>(fmod(currentPlayTime_anim, totalTime_anim));
        }
    
        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, currentAttachIndex, currentPlayTime_anim);
        
        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, currentAttachIndex, animBlendRate);
    }


     //再生しているアニメーション２の処理
    if (prevAttachIndex != -1)
    {
        // アニメーションの総時間を取得
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, prevAttachIndex);

        // 再生時間を進める
        prevPlayTime_anim += prevPlayAnimSpeed;

        // 再生時間が総時間に到達していたら再生時間をループさせる
        if (prevPlayTime_anim > totalTime_anim)
        {
            prevPlayTime_anim = static_cast<float>(fmod(prevPlayTime_anim, totalTime_anim));
        }

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, prevAttachIndex, prevPlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, prevAttachIndex, 1.0f - animBlendRate);
    }
}