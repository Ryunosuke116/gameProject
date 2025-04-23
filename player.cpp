#include "Include.h"
#include "Math.h"
#include "player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
{
    position = VGet(0.0f, 0.0f, 0.0f);
    framePosition = VGet(0.0f, 0.0f, 0.0f);
    targetMoveDirection = VGet(0.0f, 0.0f, 0.0f);
    directionToFace = VGet(0.0f, 0.0f, 0.0f);
    nowFrameNumber = -1;
    angle = -1;
    nowRunAnimSpeed = -1;
    currentJumpSpeed = 0.0f;
    frameCount = 0.0f;
    assaultTime = 0.0f;
    isJump = false;
    isJump_second = false;
    isMove = false;
    isAttack = false;
    isReset = false;
    isSlash = false;
    isSlash_Up = false;
    isSlash_Ratate = false;
    isPush = false;
    isPushAttack = false;
    isRush = false;
    modelHandle = MV1LoadModel("material/mv1/Sword_Man/SwordMan_0419.mv1");
    MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
    //MV1SetScale(modelHandle, VGet(0.005f, 0.005f, 0.005f));
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{
    position = VGet(0.0f, 0.0f, 0.0f);
    framePosition = VGet(0.0f, 0.0f, 0.0f);
    directionToFace = VGet(0.0f, 0.0f, 0.0f);
    targetMoveDirection = VGet(0.0f, 0.0f, 0.0f);
    MV1SetRotationXYZ(modelHandle, VGet(0, 0, 0));
    ChangeMotion(idle);
    nowFrameNumber = 50;
    angle = 0;
    playTime_anim = 0;
    prevAttachIndex = -1;
    prevPlayTime_anim = 0;
    prevTotalTime_anim = 0;
    currentJumpSpeed = 0.0f;
    frameCount = 0.0f;
    assaultTime = 0.0f;
    nowRunAnimSpeed = runSpeed;
    isJump = false;
    isJump_second = false;
    isMove = false;
    isAttack = false;
    isReset = false;
    isSlash = false;
    isSlash_Up = false;
    isSlash_Ratate = false;
    isRoll = false;
    isPush = false;
    isPushAttack = false;
    isRush = false;
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(const Input& input,const VECTOR& cameraDirection,const VECTOR& DragonPosition)
{
    VECTOR moveVec = VGet(0, 0, 0);

    //なにかしらのアクションをしていない場合可能
    if (!isAttack && !isRoll)
    {
        Move(input, moveVec,cameraDirection);
        Jump(input);
        //ロールアクション
        if (CheckHitKey(KEY_INPUT_Q) && !isJump)
        {
            ChangeMotion(roll);
            isRoll = true;
            if (VSize(moveVec) != 0)
            {
                directionToFace = VNorm(moveVec);
            }
        }
    }

    //向きを確定する
   //ロックオンしていたら敵の方向を向く
    if (isCameraLockOn && !isRoll && !isAttack)
    {
        directionToFace = VNorm(VSub(DragonPosition, position));

        if (isJump && VSize(moveVec) != 0)
        {
            directionToFace = VNorm(moveVec);
        }
    }
    else if(isMove && !isRoll && !isAttack)
    {
        if (VSize(moveVec) != 0)
        {
            directionToFace = VNorm(moveVec);
        }
    }

    //動いていたら進むスピード計算
    if (isMove && !isAttack && !isRoll)
    {
        //ロックオンしていなければ通常run
        if (animNumber != run && !isCameraLockOn && !isJump)
        {
            ChangeMotion(run);
        }

        targetMoveDirection = VNorm(moveVec);

        //進むスピードを乗算
        moveVec = VScale(targetMoveDirection, MoveSpeed);
    }
    //何もしていない場合は立ちモーション
    else if(!isAttack && !isRoll && !isJump)
    {
        if (animNumber != idle)
        {
            ChangeMotion(idle);
        }
    }

    RushProcess(cameraDirection);

    UpdateAngle(directionToFace);

    Attack(input);

    MotionUpdate();

    StateCheck();

    position = VAdd(position, moveVec);

    if (playTime_anim > 5.0f && animNumber == jump)
    {
        position.y += currentJumpSpeed;
    }
    else if (animNumber == run_Jump)
    {
        position.y += currentJumpSpeed;
    }

    if (position.y < 0.0f)
    {
        position.y = 0.0f;
        isJump = false;
        isJump_second = false;
    }

    JumpCalclation();

    // プレイヤーのモデルの座標を更新する
    MV1SetPosition(modelHandle, position);

    if (CheckHitKey(KEY_INPUT_1))
    {
        nowFrameNumber++;
    }
    else if (CheckHitKey(KEY_INPUT_2))
    {
        nowFrameNumber--;
    }
    //70
    framePosition = MV1GetFramePosition(modelHandle, 1);
    
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
    MV1DrawModel(modelHandle);
    printfDx("framePosition (x: %f,y: %f,z: %f)\n", 
        position.x, position.y, position.z);

    printfDx("frame最大数%d\n", MV1GetFrameNum(modelHandle));
    printfDx("frame現在数%d\n", nowFrameNumber);
    printfDx("mesh数%d\n", MV1GetMeshNum(modelHandle));

    DrawSphere3D(framePosition, 1.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 0),FALSE);

    printfDx("playTime%f\n", playTime_anim);

    // １番のメッシュの表示状態を「非表示」にする
    //MV1SetMeshVisible(modelHandle, 3, FALSE);
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
void Player::Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection)
{

    isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //正規化
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //上入力されたとき
    if (padInput.isUp(input))
    {
        if (isCameraLockOn && animNumber != run && 
            !isJump)
        {
            ChangeMotion(run);
        }
        moveVec = VAdd(moveVec, upMove);
        isMove = true;
    }

    //下入力されたとき
    if (padInput.isDown(input))
    {
        if (isCameraLockOn && animNumber != run_Back &&
            !isJump)
        {
            ChangeMotion(run_Back);
        }
        moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
        isMove = true;
    }

    //左入力されたとき
    if (padInput.isLeft(input))
    {
        if (isCameraLockOn && animNumber != run_Left &&
            !isJump)
        {
            ChangeMotion(run_Left);
        }
        moveVec = VAdd(moveVec, rightMove);
        isMove = true;
    }

    //右入力されたとき
    if (padInput.isRight(input))
    {
        if (isCameraLockOn && animNumber != run_Right &&
            !isJump)
        {
            ChangeMotion(run_Right);
        }
        moveVec = VAdd(moveVec, VScale(rightMove, -1.0f));
        isMove = true;
    }

    DashUpdate();

}

/// <summary>
/// プレイヤーの回転制御
/// </summary>
void Player::UpdateAngle(const VECTOR direction)
{
    // プレイヤーの移動方向にモデルの方向を近づける
    float targetAngle;			// 目標角度
    float difference;			// 目標角度と現在の角度との差

    // 目標の方向ベクトルから角度値を算出する
    targetAngle = static_cast<float>(atan2(direction.x, direction.z));

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
/// 攻撃
/// </summary>
void Player::Attack(const Input& input)
{
    if (padInput.isUp(input) && CheckHitKey(KEY_INPUT_F) && 
        isCameraLockOn)
    {
        if (animNumber != attack_Assault && !isAttack)
        {
            ChangeMotion(attack_Assault);
            isAttack = true;
        }
    }
    //ボタンを押したら攻撃
    else if (CheckHitKey(KEY_INPUT_F))
    {
        if (!isPushAttack)
        {
            if (!isSlash && animNumber != attack &&
                !isSlash_Up && !isSlash_Ratate)
            {
                ChangeMotion(attack_02);
                isAttack = true;
                isSlash = true;
            }
            else if (isSlash && !isAttack)
            {
                ChangeMotion(attack_Up);
                isAttack = true;
                isSlash_Up = true;
                isSlash = false;
                frameCount = 0.0f;
            }
            else if (isSlash_Up && !isAttack)
            {
                ChangeMotion(attack_Rotate);
                isAttack = true;
                isSlash_Ratate = true;
                isSlash_Up = false;
                frameCount = 0.0f;
            }
            isPushAttack = true;
        }
    }
    else
    {
        isPushAttack = false;
    }
}

/// <summary>
/// 突進処理
/// </summary>
void Player::RushProcess(const VECTOR& cameraDirection)
{

    if (isRush)
    {
        //正規化
        VECTOR moveVec = VNorm(cameraDirection);
        
        //進むスピードを乗算
        moveVec = VScale(moveVec, 0.9f);
        
        //加算
        position = VAdd(position, moveVec);
        assaultTime++;
    }

    if (playTime_anim == 19.0f && !isRush && isAttack && 
        animNumber == attack_Assault)
    {
        isRush = true;
    }

    if (assaultTime > 18.0f && isRush)
    {
        isRush = false;
        assaultTime = 0.0f;
    }
 
}

/// <summary>
/// ジャンプ
/// </summary>
void Player::Jump(const Input& input)
{
    if (CheckHitKey(KEY_INPUT_SPACE) || input.GetNowFrameNewInput() & PAD_INPUT_A)
    {
        //一回目のジャンプ
        if (!isJump)
        {
            isPush = true;
            currentJumpSpeed = addJumpPower;
            isJump = true;
            if (isMove)
            {
                ChangeMotion(run_Jump);
            }
            else
            {
                ChangeMotion(jump);
            }
        }
        //二段ジャンプ
        if (isJump && !isPush && !isJump_second)
        {
            isPush = true;
            isJump_second = true;
            currentJumpSpeed = addJumpPower;
            if (isMove)
            {
                ChangeMotion(run_Jump);
            }
            else
            {
                ChangeMotion(jump);
            }
        }
    }
    else
    {
        isPush = false;
    }
}

/// <summary>
/// ジャンプ処理
/// </summary>
void Player::JumpCalclation()
{
    if (isJump && playTime_anim > 5.0f && animNumber == jump)
    {
        currentJumpSpeed += -Gravity;
    }
    else if (isJump && animNumber == run_Jump)
    {
        currentJumpSpeed += -Gravity;
    }
}

/// <summary>
/// ステータスチェック
/// </summary>
void Player::StateCheck()
{
    //斬攻撃したらカウント開始
    if ((isSlash && !isAttack) || (isSlash_Up && !isAttack))
    {
        frameCount++;
    }

    if (isAttack)
    {
        if (isReset)
        {
            isAttack = false;
            //if(isSlash_Up)
            //{
            //    isSlash_Up = false;
            //}
            if (isSlash_Ratate)
            {
                isSlash_Ratate = false;
                position = framePosition;
                position.y = 0.0f;
            }
        }
    }

    if (isSlash_Up && frameCount >= 20.0f)
    {
        isSlash_Up = false;
        frameCount = 0.0f;
    }
    else if (isSlash && frameCount >= 20.0f)
    {
        isSlash = false;
        frameCount = 0.0f;
    }
}

/// <summary>
/// モーション変更
/// </summary>
/// <param name="motionNum"></param>
void Player::ChangeMotion(const int& motionNum)
{
    BaseChara::ChangeMotion(motionNum);
    animNumber = motionNum;

}

/// <summary>
/// モーション更新
/// </summary>
void Player::MotionUpdate()
{
    float totalTime_anim;

    // アタッチしたアニメーションの総再生時間を取得する
    totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);

    //それぞれの再生スピードで更新
    if (animNumber == roll)
    {
        playTime_anim += rollAnimSpeed;
    }
    //攻撃モーション時は攻撃スピードで更新
    else if(animNumber == attack || animNumber == attack_Up ||
        animNumber == attack_Rotate || animNumber == attack_02 || 
        animNumber == attack_Assault)
    {
        if (!isRush)
        {
            playTime_anim += attackAnimSpeed;
        }
    }
    else if (animNumber == run)
    {
        playTime_anim += nowRunAnimSpeed;
    }
    else
    {
        playTime_anim += PlayAnimSpeed;
        nowRunAnimSpeed = runSpeed;
    }

    //総再生時間を超えたら再生時間をリセット
    if (playTime_anim >= totalTime_anim)
    {
        //回転斬りの場合、立ち状態に戻す
        if (animNumber == attack_Rotate)
        {
            ChangeMotion(idle);
        }
        //ロールの場合
        if (isRoll)
        {
            isRoll = false;
            position = framePosition;
            position.y = 0.0f;
        }
        playTime_anim = 0;
        isReset = true;
    }
    else
    {
        isReset = false;
    }

    // 再生時間をセットする
    MV1SetAttachAnimTime(modelHandle, attachIndex, playTime_anim);
}


void Player::DashUpdate()
{
    if (CheckHitKey(KEY_INPUT_L))
    {
        nowRunAnimSpeed += 0.02f;
    }
    else
    {
        nowRunAnimSpeed -= 0.02f;
    }

    //一定値を超えたらそこで止める
    if (nowRunAnimSpeed > dashMoveSpeed)
    {
        nowRunAnimSpeed = dashMoveSpeed;
    }
    if (nowRunAnimSpeed < runSpeed)
    {
        nowRunAnimSpeed = runSpeed;
    }
}

/// <summary>
/// 上入力
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isUp(const Input& input)
{
    if (input.GetNowFrameInput() & PAD_INPUT_UP ||
        CheckHitKey(KEY_INPUT_UP))
    {
        return true;
    }
    return false;
}

/// <summary>
/// 下入力
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isDown(const Input& input)
{
    if (input.GetNowFrameInput() & PAD_INPUT_DOWN ||
        CheckHitKey(KEY_INPUT_DOWN))
    {
        return true;
    }
    return false;
}

/// <summary>
/// 右入力
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isRight(const Input& input)
{
    if (input.GetNowFrameInput() & PAD_INPUT_RIGHT ||
        CheckHitKey(KEY_INPUT_RIGHT))
    {
        return true;
    }
    return false;
}

/// <summary>
/// 左入力
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isLeft(const Input& input)
{
    if (input.GetNowFrameInput() & PAD_INPUT_LEFT ||
        CheckHitKey(KEY_INPUT_LEFT))
    {
        return true;
    }
    return false;
}


