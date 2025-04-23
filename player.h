#pragma once

class Player : public BaseChara, public CameraObserver
{
private:
	static constexpr float modelScale = 0.06f;
	static constexpr float MoveSpeed = 0.4f;	    // 移動速度
	static constexpr float runSpeed = 0.4f;
	static constexpr float dashMoveSpeed = 0.8f;
	static constexpr float dashSpeed = 0.7f;
	static constexpr float attackAnimSpeed = 1.0f;	//攻撃速度
	static constexpr float rollAnimSpeed = 1.5f;	//攻撃速度
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.5f;		//ジャンプパワー
	static constexpr float Gravity = 0.1f;

	int animNumber;
	float jumpPower;
	float currentJumpSpeed;			//現在のジャンプスピード
	float frameCount;
	float nowRunAnimSpeed;
	float assaultTime;

	bool isMove;                    //動いたか
	bool isAttack;					//攻撃してるか
	bool isReset;					//リセットするか
	bool isJump;					//ジャンプしたか
	bool isJump_second;				//二段ジャンプしたか
	bool isSlash;
	bool isSlash_Up;
	bool isSlash_Ratate;
	bool isRoll;
	bool isRush;
	bool isPush;					//ボタンを押したか
	bool isPushAttack;
	bool isCameraLockOn;			

	VECTOR meshPosition;
	VECTOR directionToFace;			//向いている方向

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

