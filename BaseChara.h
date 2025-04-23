#pragma once
class BaseChara
{
protected:

	int modelHandle;
	int attachIndex;
	int motionNum;
	float animBlendRate;		
	int animHandle_now;				//���݂̃A�j���[�V����
	int prevAnimHandle;
	int prevAttachIndex;			//�O�̃A�j���[�V�������
	float prevPlayTime_anim;		//�O��̍Đ�����
	float prevTotalTime_anim;		//�O��̑��Đ�����
	
	int nowFrameNumber;

	float playTime_anim;
	float totalTime_anim;
	float angle;

	VECTOR targetMoveDirection;
	VECTOR framePosition;
	VECTOR position;

	static constexpr float	MoveSpeed = 0.4f;	    // �ړ����x
	static constexpr float angleSpeed = 0.2f;
	static constexpr float	AnimBlendSpeed = 0.1f;		// �A�j���[�V�����̃u�����h���ω����x
	static constexpr float PlayAnimSpeed = 0.5f;	//�A�j���Đ����x
public:

	BaseChara();
	~BaseChara();

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	virtual void ChangeMotion(const int& motionNum);
	virtual void MotionUpdate();
	virtual void Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection);
	void UpdateAngle(int& modelHandle);

	/////////////////////////////////////////////
	// �Q�b�^�[
	/////////////////////////////////////////////
	const VECTOR GetPosition() { return position; }
	const VECTOR GetFramePosition()const { return framePosition; }
};

