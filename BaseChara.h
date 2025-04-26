#pragma once
class BaseChara
{
protected:

	int modelHandle;
	int motionNum;
	int HP;
	int nowFrameNumber;

	int prevAttachIndex;			//前のアニメーション情報
	float prevPlayTime_anim;		//前回の再生時間
	float prevTotalTime_anim;		//前回の総再生時間
	float prevPlayAnimSpeed;
	
	int currentAttachIndex;
	float currentPlayTime_anim;			//現在の再生時間
	float currentPlayAnimSpeed;
	float animBlendRate;		
	float keepPlayTime_anim;

	float totalTime_anim;
	float angle;

	VECTOR targetMoveDirection;
	VECTOR framePosition;
	VECTOR position;

	static constexpr float	MoveSpeed = 0.4f;	    // 移動速度
	static constexpr float angleSpeed = 0.2f;
	static constexpr float	AnimBlendSpeed = 0.1f;		// アニメーションのブレンド率変化速度
	static constexpr float PlayAnimSpeed = 0.5f;	//アニメ再生速度
public:

	BaseChara();
	~BaseChara();

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	virtual void ChangeMotion(const int& motionNum, const float playAnimSpeed);
	virtual void MotionUpdate();
	virtual void Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection);
	void UpdateAngle(int& modelHandle);

	/////////////////////////////////////////////
	// ゲッター
	/////////////////////////////////////////////
	const VECTOR GetPosition() { return position; }
	const VECTOR GetFramePosition()const { return framePosition; }
};

