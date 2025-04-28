#pragma once

class CameraObserver;

class Camera
{
private:
	VECTOR AimPosition;
	VECTOR aimPosition_Lockon;
	VECTOR LookPosition;
	VECTOR keepPlayerPosition;
	VECTOR keepEnemyPosition;
	VECTOR cameraDirection;
	VECTOR leftPosition;
	VECTOR rightPosition;
	float a;
	float angle;
	float cross_right;
	float cross_left;
	float pravCross_right;
	float pravCross_left;
	bool isLockOn;		//���b�N�I�����邩
	bool isPush;		//�����Ă��邩
	bool isReflected;	//�f���Ă��邩

	static constexpr float cameraSpeed = 0.02f;
	static constexpr float cameraSpeed_ = 0.01f;

	CameraObserver* observer;

public:
	Camera();
	~Camera();

	void Initialize();
	void Update(const VECTOR& playerPosition, const VECTOR& dragonPosition);
	void Draw();
	void Leap(VECTOR& changePosition, const VECTOR& playerPosition,const float& speed);
	void LockOnCamera(const VECTOR& playerPosition, const VECTOR& dragonPosition);
	bool isObjectVisible(const VECTOR& cameraDirection, const VECTOR& objectPosition, const VECTOR& playerPosition);

	void SetObserver(CameraObserver* obs);

	///////////////////////////////////////////////////////
	// �Q�b�^�[
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
};

