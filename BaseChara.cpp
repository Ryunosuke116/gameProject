#include "Include.h"
#include "BaseChara.h"

BaseChara::BaseChara() :
    modelHandle(-1),
    attachIndex(-1),
    motionNum(-1),
    nowFrameNumber(-1),
    animHandle_now(-1),
    playTime_anim(-1),
    totalTime_anim(-1),
    angle(-1),
    prevAttachIndex(-1),
    animBlendRate(-1),
    prevPlayTime_anim(-1),
    prevTotalTime_anim(-1),
    targetMoveDirection(VGet(0.0f, 0.0f, 0.0f)),
    position(VGet(0.0f, 0.0f, 0.0f))
{

}

BaseChara::~BaseChara()
{}

/// <summary>
/// ������
/// </summary>
void BaseChara::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
void BaseChara::Update(){}

/// <summary>
/// �`��
/// </summary>
void BaseChara::Draw()
{}

/// <summary>
/// �ړ�
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
void BaseChara::Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection)
{

}

/// <summary>
/// ����
/// </summary>
void BaseChara::UpdateAngle(int& modelHandle)
{
    // �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
    float targetAngle;			// �ڕW�p�x
    float difference;			// �ڕW�p�x�ƌ��݂̊p�x�Ƃ̍�

    // �ڕW�̕����x�N�g������p�x�l���Z�o����
    targetAngle = static_cast<float>(atan2(targetMoveDirection.x, targetMoveDirection.z));

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
/// ���[�V�����ύX
/// </summary>
/// <param name="motionNum"></param>
void BaseChara::ChangeMotion(const int& motionNum)
{

    //if (prevAttachIndex != -1)
    //{
    //    MV1DetachAnim(modelHandle, prevAttachIndex);
    //    prevAttachIndex = -1;
    //}

    ////���܂܂ŏ���prev�ɕۑ�
    //prevAttachIndex = attachIndex;
    //prevAnimHandle = animHandle_now;
    //prevPlayTime_anim = playTime_anim;

    //������
    MV1DetachAnim(modelHandle, attachIndex);
    // �Đ����Ԃ̏�����
    playTime_anim = 0;
    // �Đ����Ԃ��Z�b�g����
    MV1SetAttachAnimTime(modelHandle, attachIndex, playTime_anim);

    this->motionNum = motionNum;

    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    attachIndex = MV1AttachAnim(modelHandle, this->motionNum, -1, FALSE);

    // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
    totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);

    // �u�����h����Prev���L���ł͂Ȃ��ꍇ�͂P�D�O��( ���݃��[�V�������P�O�O���̏�� )�ɂ���
    animBlendRate = prevAttachIndex == -1 ? 1.0f : 0.0f;

}

/// <summary>
/// ���[�V�����X�V
/// </summary>
void BaseChara::MotionUpdate()
{
    float totalTime_anim;

    // �u�����h�����P�ȉ��̏ꍇ�͂P�ɋ߂Â���
    if (animBlendRate < 1.0f)
    {
        animBlendRate += AnimBlendSpeed;
        if (animBlendRate > 1.0f)
        {
            animBlendRate = 1.0f;
        }
    }

    // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
    totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);

    //�Đ����ԍX�V
    playTime_anim += PlayAnimSpeed;

    //���Đ����Ԃ𒴂����烊�Z�b�g
    if (playTime_anim >= totalTime_anim)
    {
        playTime_anim = 0.0f;
    }
    
    // �Đ����Ԃ��Z�b�g����
    MV1SetAttachAnimTime(modelHandle, attachIndex, playTime_anim);

    // �A�j���[�V�����̃��f���ɑ΂��锽�f�����Z�b�g
    //MV1SetAttachAnimBlendRate(modelHandle, playTime_anim, animBlendRate);

    // �Đ����Ă���A�j���[�V�����Q�̏���
    //if (prevAttachIndex != -1)
    //{
    //    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    //    prevAttachIndex = MV1AttachAnim(modelHandle, this->motionNum, prevAnimHandle);

    //    // �A�j���[�V�����̑����Ԃ��擾
    //    totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, prevAttachIndex);

    //    // �Đ����Ԃ�i�߂�
    //    prevPlayTime_anim += PlayAnimSpeed;

    //    // �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
    //    if (prevPlayTime_anim > totalTime_anim)
    //    {
    //        prevPlayTime_anim = 0.0f;
    //    }

    //    // �ύX�����Đ����Ԃ����f���ɔ��f������
    //    MV1SetAttachAnimTime(modelHandle, prevAttachIndex, prevPlayTime_anim);

    //    // �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
    //    MV1SetAttachAnimBlendRate(modelHandle, prevPlayTime_anim, 1.0f - animBlendRate);
    //}
}