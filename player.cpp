#include "Include.h"
#include "Math.h"
#include "player.h"

/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	
}

/// <summary>
/// ������
/// </summary>
void Player::Initialize()
{
    position = VGet(0.0f, 0.0f, 0.0f);
    framePosition = VGet(0.0f, 0.0f, 0.0f);
    directionToFace = VGet(0.0f, 0.0f, 0.0f);
    targetMoveDirection = VGet(0.0f, 0.0f, 0.0f);
    MV1SetRotationXYZ(modelHandle, VGet(0, 0, 0));
    ChangeMotion(idle, PlayAnimSpeed);
    nowFrameNumber = 50;
    angle = 0;
    currentPlayTime_anim = 0;
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
/// �X�V
/// </summary>
void Player::Update(const Input& input,const VECTOR& cameraDirection,const VECTOR& DragonPosition)
{
    VECTOR moveVec = VGet(0, 0, 0);

    //�Ȃɂ�����̃A�N�V���������Ă��Ȃ��ꍇ�\
    if (!isAttack && !isRoll)
    {
        Move(input, moveVec,cameraDirection);
        Jump(input);
        //���[���A�N�V����
        if (CheckHitKey(KEY_INPUT_Q) && !isJump)
        {
            ChangeMotion(roll, rollAnimSpeed);
            isRoll = true;
            if (VSize(moveVec) != 0)
            {
                directionToFace = VNorm(moveVec);
            }
        }
    }

    //�������m�肷��
   //���b�N�I�����Ă�����G�̕���������
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

    //�����Ă�����i�ރX�s�[�h�v�Z
    if (isMove && !isAttack && !isRoll)
    {
        //���b�N�I�����Ă��Ȃ���Βʏ�run
        if (animNumber != run && !isCameraLockOn && !isJump)
        {
            ChangeMotion(run, nowRunAnimSpeed);
        }

        targetMoveDirection = VNorm(moveVec);

        //�i�ރX�s�[�h����Z
        moveVec = VScale(targetMoveDirection, MoveSpeed);
    }
    //�������Ă��Ȃ��ꍇ�͗������[�V����
    else if(!isAttack && !isRoll && !isJump)
    {
        if (animNumber != idle)
        {
            ChangeMotion(idle, PlayAnimSpeed);
        }
    }

    RushProcess(cameraDirection);

    UpdateAngle(directionToFace);

    Attack(input);

    MotionUpdate();

    StateCheck();

    position = VAdd(position, moveVec);

    if (currentPlayTime_anim > 5.0f && animNumber == jump)
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

    // �v���C���[�̃��f���̍��W���X�V����
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
/// �`��
/// </summary>
void Player::Draw()
{
    MV1DrawModel(modelHandle);
    printfDx("framePosition (x: %f,y: %f,z: %f)\n", 
        position.x, position.y, position.z);

    printfDx("frame�ő吔%d\n", MV1GetFrameNum(modelHandle));
    printfDx("frame���ݐ�%d\n", nowFrameNumber);
    printfDx("mesh��%d\n", MV1GetMeshNum(modelHandle));

    DrawSphere3D(framePosition, 1.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 0),FALSE);

    printfDx("playTime%f\n", currentPlayTime_anim);
    printfDx("animBlendRate %f\n", animBlendRate);

    // �P�Ԃ̃��b�V���̕\����Ԃ��u��\���v�ɂ���
    //MV1SetMeshVisible(modelHandle, 3, FALSE);
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
void Player::Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection)
{

    isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //���K��
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //����͂��ꂽ�Ƃ�
    if (padInput.isUp(input))
    {
        if (isCameraLockOn && animNumber != run && 
            !isJump)
        {
            ChangeMotion(run,nowRunAnimSpeed);
        }
        moveVec = VAdd(moveVec, upMove);
        isMove = true;
    }

    //�����͂��ꂽ�Ƃ�
    if (padInput.isDown(input))
    {
        if (isCameraLockOn && animNumber != run_Back &&
            !isJump)
        {
            ChangeMotion(run_Back, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
        isMove = true;
    }

    //�����͂��ꂽ�Ƃ�
    if (padInput.isLeft(input))
    {
        if (isCameraLockOn && animNumber != run_Left &&
            !isJump)
        {
            ChangeMotion(run_Left, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, rightMove);
        isMove = true;
    }

    //�E���͂��ꂽ�Ƃ�
    if (padInput.isRight(input))
    {
        if (isCameraLockOn && animNumber != run_Right &&
            !isJump)
        {
            ChangeMotion(run_Right, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, VScale(rightMove, -1.0f));
        isMove = true;
    }

    DashUpdate();

}

/// <summary>
/// �v���C���[�̉�]����
/// </summary>
void Player::UpdateAngle(const VECTOR direction)
{
    // �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
    float targetAngle;			// �ڕW�p�x
    float difference;			// �ڕW�p�x�ƌ��݂̊p�x�Ƃ̍�

    // �ڕW�̕����x�N�g������p�x�l���Z�o����
    targetAngle = static_cast<float>(atan2(direction.x, direction.z));

    // �ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�������o��
    // �ŏ��͒P���Ɉ����Z
    difference = targetAngle - angle;

    // ����������炠������̍����P�W�O�x�ȏ�ɂȂ邱�Ƃ͖����̂�
    // ���̒l���P�W�O�x�ȏ�ɂȂ��Ă�����C������
    if (difference < -DX_PI_F)
    {
        difference += DX_TWO_PI_F;
    }
    else if (difference > DX_PI_F)
    {
        difference -= DX_TWO_PI_F;
    }

    // �p�x�̍����O�ɋ߂Â���
    if (difference > 0.0f)
    {
        // �����v���X�̏ꍇ�͈���
        difference -= angleSpeed;
        if (difference < 0.0f)
        {
            difference = 0.0f;
        }
    }
    else
    {
        // �����}�C�i�X�̏ꍇ�͑���
        difference += angleSpeed;
        if (difference > 0.0f)
        {
            difference = 0.0f;
        }
    }

    // ���f���̊p�x���X�V
    angle = targetAngle - difference;
    MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
}

/// <summary>
/// �U��
/// </summary>
void Player::Attack(const Input& input)
{
    if (padInput.isUp(input) && CheckHitKey(KEY_INPUT_F) && 
        isCameraLockOn && !isJump)
    {
        if (animNumber != attack_Assault && !isAttack)
        {
            ChangeMotion(attack_Assault, attackAnimSpeed);
            isAttack = true;
        }
    }
    //�{�^������������U��
    else if (CheckHitKey(KEY_INPUT_F) && !isJump)
    {
        if (!isPushAttack)
        {
            if (!isSlash && animNumber != attack &&
                !isSlash_Up && !isSlash_Ratate)
            {
                ChangeMotion(attack_02, attackAnimSpeed);
                isAttack = true;
                isSlash = true;
            }
            else if (isSlash && !isAttack)
            {
                ChangeMotion(attack_Up, attackAnimSpeed);
                isAttack = true;
                isSlash_Up = true;
                isSlash = false;
                frameCount = 0.0f;
            }
            else if (isSlash_Up && !isAttack)
            {
                ChangeMotion(attack_Rotate, attackAnimSpeed);
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
/// �ːi����
/// </summary>
void Player::RushProcess(const VECTOR& cameraDirection)
{

    if (isRush)
    {
        //���K��
        VECTOR moveVec = VNorm(cameraDirection);
        
        //�i�ރX�s�[�h����Z
        moveVec = VScale(moveVec, 0.9f);
        
        //���Z
        position = VAdd(position, moveVec);
        assaultTime++;
    }

    if (currentPlayTime_anim == 19.0f && !isRush && isAttack && 
        animNumber == attack_Assault)
    {
        isRush = true;
    }

    //�J�E���g��18f�𒴂�����I��
    if (assaultTime > 25.0f && isRush)
    {
        isRush = false;
        assaultTime = 0.0f;
    }
 
}

/// <summary>
/// �W�����v
/// </summary>
void Player::Jump(const Input& input)
{
    if (CheckHitKey(KEY_INPUT_SPACE) || input.GetNowFrameNewInput() & PAD_INPUT_A)
    {
        //���ڂ̃W�����v
        if (!isJump)
        {
            isPush = true;
            currentJumpSpeed = addJumpPower;
            isJump = true;
            if (isMove)
            {
                ChangeMotion(run_Jump, PlayAnimSpeed);
            }
            else
            {
                ChangeMotion(jump, PlayAnimSpeed);
            }
        }
        //��i�W�����v
        if (isJump && !isPush && !isJump_second)
        {
            isPush = true;
            isJump_second = true;
            currentJumpSpeed = addJumpPower;
            if (isMove)
            {
                ChangeMotion(run_Jump, PlayAnimSpeed);
            }
            else
            {
                ChangeMotion(jump, PlayAnimSpeed);
            }
        }
    }
    else
    {
        isPush = false;
    }
}

/// <summary>
/// �W�����v����
/// </summary>
void Player::JumpCalclation()
{
    if (isJump && currentPlayTime_anim > 5.0f && animNumber == jump)
    {
        currentJumpSpeed += -Gravity;
    }
    else if (isJump && animNumber == run_Jump)
    {
        currentJumpSpeed += -Gravity;
    }
}

/// <summary>
/// �X�e�[�^�X�`�F�b�N
/// </summary>
void Player::StateCheck()
{
    //�a�U��������J�E���g�J�n
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
/// ���[�V�����ύX
/// </summary>
/// <param name="motionNum"></param>
void Player::ChangeMotion(const int& motionNum, const float playAnimSpeed)
{
    BaseChara::ChangeMotion(motionNum, playAnimSpeed);
    animNumber = motionNum;
}

/// <summary>
/// ���[�V�����X�V
/// </summary>
void Player::MotionUpdate()
{
    float totalTime_anim;

    //if (isReset)
    //{
    //    animBlendRate = 1.0f;
    //}

    // �u�����h�����P�ȉ��̏ꍇ�͂P�ɋ߂Â���
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
        // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, currentAttachIndex);

        //�Đ����ԍX�V
     /*   currentPlayTime_anim += currentPlayAnimSpeed;
        keepPlayTime_anim = currentPlayTime_anim;*/

        //���ꂼ��̍Đ��X�s�[�h�ōX�V
        if (animNumber == roll)
        {
            currentPlayTime_anim += rollAnimSpeed;
        }
        //�U�����[�V�������͍U���X�s�[�h�ōX�V
        else if (animNumber == attack || animNumber == attack_Up ||
            animNumber == attack_Rotate || animNumber == attack_02 ||
            animNumber == attack_Assault)
        {
            if (!isRush)
            {
                currentPlayTime_anim += attackAnimSpeed;
            }
        }
        else if (animNumber == run)
        {
            currentPlayTime_anim += nowRunAnimSpeed;
        }
        else
        {
            currentPlayTime_anim += PlayAnimSpeed;
            nowRunAnimSpeed = runSpeed;
        }

        //���Đ����Ԃ𒴂�����Đ����Ԃ����Z�b�g
        if (currentPlayTime_anim >= totalTime_anim)
        {
            //���[���̏ꍇ
            if (isRoll)
            {
                isRoll = false;
                position = framePosition;
                position.y = 0.0f;
            }
            keepPlayTime_anim = 0;
            isReset = true;
        }
        else
        {
            isReset = false;
        }


        //���Đ����Ԃ𒴂����烊�Z�b�g
        if (currentPlayTime_anim >= totalTime_anim)
        {
            currentPlayTime_anim = static_cast<float>(fmod(currentPlayTime_anim, totalTime_anim));
        }

        // �Đ����Ԃ��Z�b�g����
        MV1SetAttachAnimTime(modelHandle, currentAttachIndex, currentPlayTime_anim);

        //�A�j���[�V�����̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, currentAttachIndex, animBlendRate);
    }

    //�Đ����Ă���A�j���[�V�����Q�̏���
    if (prevAttachIndex != -1)
    {
        // �A�j���[�V�����̑����Ԃ��擾
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, prevAttachIndex);

        // �Đ����Ԃ�i�߂�
        prevPlayTime_anim += prevPlayAnimSpeed;

        // �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
        if (prevPlayTime_anim > totalTime_anim)
        {
            prevPlayTime_anim = static_cast<float>(fmod(prevPlayTime_anim, totalTime_anim));
        }

        // �ύX�����Đ����Ԃ����f���ɔ��f������
        MV1SetAttachAnimTime(modelHandle, prevAttachIndex, prevPlayTime_anim);

        // �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, prevAttachIndex, 1.0f - animBlendRate);
    }
}

/// <summary>
/// �_�b�V���X�V
/// </summary>
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

    //���l�𒴂����炻���Ŏ~�߂�
    if (nowRunAnimSpeed > Max_DashMoveSpeed)
    {
        nowRunAnimSpeed = Max_DashMoveSpeed;
    }
    if (nowRunAnimSpeed < runSpeed)
    {
        nowRunAnimSpeed = runSpeed;
    }
}

/// <summary>
/// �����
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
/// ������
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
/// �E����
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
/// ������
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

void Player::FlagProcess()
{

}

