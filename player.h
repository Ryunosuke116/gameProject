#pragma once

class Player : public BaseChara, public CameraObserver
{
private:
	static constexpr float modelScale = 0.06f;
	static constexpr float MoveSpeed = 0.4f;	    // �ړ����x
	static constexpr float runSpeed = 0.4f;
	static constexpr float dashMoveSpeed = 0.8f;
	static constexpr float dashSpeed = 0.7f;
	static constexpr float attackAnimSpeed = 1.0f;	//�U�����x
	static constexpr float rollAnimSpeed = 1.5f;	//�U�����x
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.5f;		//�W�����v�p���[
	static constexpr float Gravity = 0.1f;

	int animNumber;
	float jumpPower;
	float currentJumpSpeed;			//���݂̃W�����v�X�s�[�h
	float frameCount;
	float nowRunAnimSpeed;
	float assaultTime;

	bool isMove;                    //��������
	bool isAttack;					//�U�����Ă邩
	bool isReset;					//���Z�b�g���邩
	bool isJump;					//�W�����v������
	bool isJump_second;				//��i�W�����v������
	bool isSlash;
	bool isSlash_Up;
	bool isSlash_Ratate;
	bool isRoll;
	bool isRush;
	bool isPush;					//�{�^������������
	bool isPushAttack;
	bool isCameraLockOn;			

	VECTOR meshPosition;
	VECTOR directionToFace;			//�����Ă������

	enum animationNumber
	{
		attack,
		attack_Down,
		attack_Rotate,
		attack_Up,
		attack_02,
		block,
		block_Idle,
		idle,
		roll,
		run,
		run_Back,
		run_Left,
		run_Right,
		attack_Assault,
		jump,
		run_Jump
	};

public:


	Player();
	~Player();

	void Initialize();
	void Update(const Input& input, const VECTOR& cameraDirection, const VECTOR& DragonPosition);
	void Draw();

	void Attack(const Input& input);
	void Jump(const Input& input);
	void JumpCalclation();
	void StateCheck();
	void Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection) override;
	void UpdateAngle(const VECTOR direction);
	void ChangeMotion(const int& motionNum)override;
	void MotionUpdate() override;
	void OnLockOnChanged(bool flag)override { isCameraLockOn = flag; }
	void DashUpdate();
	void RushProcess(const VECTOR& cameraDirection);
	
	struct PadInput
	{
		bool isUp(const Input& input);
		bool isDown(const Input& input);
		bool isRight(const Input& input);
		bool isLeft(const Input& input);
	};

	PadInput padInput;

	//////////////////////////////////
	// getter
	/////////////////////////////////

};

