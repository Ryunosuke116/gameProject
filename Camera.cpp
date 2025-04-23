#include"Include.h"
#include "Camera.h"

Camera::Camera()
{
	// �O���t�B�b�N�̕`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(3.5f, 5000.0f);

	AimPosition = VGet(0.0f, 0.0f, 0.0f);
	LookPosition = VGet(0.0f, 0.0f, 0.0f);
	keepPlayerPosition = VGet(0.0f, 0.0f, 0.0f);
	cameraDirection = VGet(0.0f, 0.0f, 0.0f);
	a = 0;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{

}

/// <summary>
/// ������
/// </summary>
void Camera::Initialize()
{
	AimPosition = VGet(30.0f, 15, -10);
	LookPosition = VGet(0.0f, 0, 20.0f);
	leftPosition= VGet(0.0f, 0, 10.0f);
	rightPosition= VGet(0.0f, 0, -10.0f);

	radian = 0.0f;
	pravCross_right = 0;
	pravCross_left = 0;
	a = -177.55f;
	isLockOn = false;
	SetCameraPositionAndTarget_UpVecY(AimPosition, LookPosition);

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();
}

/// <summary>
/// �X�V
/// </summary>
void Camera::Update(const VECTOR& playerPosition, const VECTOR& dragonPosition)
{
	keepEnemyPosition = dragonPosition;

	//�J�����ړ�����
	if (CheckHitKey(KEY_INPUT_A))
	{
		a += 2.0f;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		a -= 2.0f;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		AimPosition.y += 0.5f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		AimPosition.y -= 0.5f;
	}

	if (CheckHitKey(KEY_INPUT_T))
	{
		a = -177.55f;
	}

	if (CheckHitKey(KEY_INPUT_R))
	{
		if (!isPush)
		{
			isPush = true;
			if (!isLockOn)
			{
				VECTOR direction = VSub(playerPosition, dragonPosition);
				direction = VNorm(direction);
				direction = VScale(direction, 20.0f);
				AimPosition = VAdd(playerPosition, direction);
				AimPosition.y = 2.0f;
				aimPosition_Lockon = AimPosition;

				keepPlayerPosition = playerPosition;
				isLockOn = true;
			}
			else
			{
				isLockOn = false;
			}
		}
	}
	else
	{
		isPush = false;
	}	

	if (!isLockOn)
	{

		float radian = a * DX_PI_F / 360.0f;
		this->radian = radian;

		//AimPosition = VAdd(AimPosition, addAimPosition);
		AimPosition.x = LookPosition.x + 30 * cos(radian);
		AimPosition.z = LookPosition.z + 30 * sin(radian);

		float maxRange = 5.0f;
		float maxRange_ = 10.0f;

		//���S����̋����𑪂�
		float r = VSize(VSub(playerPosition, LookPosition));


		//���̋����ɒB�����炻��ȏア���Ȃ��悤�ɂ���
		if (r >= maxRange_ || r <= -maxRange_)
		{
			Leap(LookPosition, playerPosition, cameraSpeed);
		}
		else if (r >= maxRange || r <= -maxRange)
		{
			Leap(LookPosition, playerPosition, cameraSpeed_);
		}

		SetCameraPositionAndTarget_UpVecY(AimPosition, LookPosition);

		cameraDirection = VSub(LookPosition, AimPosition);
		cameraDirection = VNorm(cameraDirection);

	}
	else if (isLockOn)
	{
		LookPosition = dragonPosition;
	
		LockOnCamera(playerPosition,dragonPosition);
		isReflected = isObjectVisible(cameraDirection, dragonPosition, playerPosition);

		if (isReflected)
		{
			//VECTOR direction = VSub(playerPosition, dragonPosition);
			//float left_len2 = VDot(leftPosition, leftPosition);
			//float right_len2 = VDot(rightPosition, rightPosition);
			//float leftScalar = VDot(direction, leftPosition) / left_len2;
			//float rightScalar = VDot(direction, rightPosition) / right_len2;
			//
			////�ł��߂��ړ_
			//VECTOR nearPoint_left = VAdd(dragonPosition, VScale(leftPosition, leftScalar));
			//VECTOR nearPoint_right = VAdd(dragonPosition, VScale(rightPosition, rightScalar));
		
			////�ł��߂��ړ_����player�ւ̃x�N�g��
			//VECTOR diff_left = VSub(playerPosition, nearPoint_left);
			//VECTOR diff_right = VSub(playerPosition, nearPoint_right);

			////�_�ƒ����̋���
			//float distance_left = VSize(diff_left);
			//float distance_right = VSize(diff_right);

			//if (distance_left > distance_right)
			//{
			//	AimPosition = VAdd(AimPosition, nearPoint_right);
			//}
			//else
			//{
			//	AimPosition = VAdd(AimPosition, nearPoint_left);
			//}
		}
	}


	observer->OnLockOnChanged(isLockOn);

}

void Camera::Draw()
{
	printfDx("AimPosition.x %f\AimPosition.z %f\n", AimPosition.x, AimPosition.z);
	printfDx("LookPosition.x %f\LookPosition.z %f\n", LookPosition.x, LookPosition.z);
	printfDx("radian %f\n",radian);
	printfDx("right.x %f\nright.y %f\nright.z %f\n", rightPosition.x, rightPosition.y, rightPosition.z);
	printfDx("left.x %f\nleft.y %f\nleft.z %f", leftPosition.x, leftPosition.y, leftPosition.z);
	
	if (!isReflected)
	{
		printfDx("�f���Ă���");
	}
	
	DrawLine3D(keepEnemyPosition, leftPosition, GetColor(255, 255, 255));
	DrawLine3D(keepEnemyPosition, rightPosition, GetColor(255, 255, 255));

}

/// <summary>
/// ���[�v
/// </summary>
/// <param name="AimPosition"></param>
void Camera::Leap(VECTOR& changePosition, const VECTOR& playerPosition,const float& speed)
{
	VECTOR SubPosition = VSub(playerPosition, changePosition);
	VECTOR scalePosition = VGet(0, 0, 0);

	scalePosition = VScale(SubPosition, speed);
	changePosition = VAdd(changePosition, scalePosition);
}

/// <summary>
/// ���b�N�I�����̃J�����̋���
/// </summary>
/// <param name="playerPosition"></param>
/// <param name="dragonPosition"></param>
void Camera::LockOnCamera(const VECTOR& playerPosition,const VECTOR& dragonPosition)
{
	//�h���S����player�ւ̕����x�N�g��
	VECTOR direction = VSub(playerPosition, dragonPosition);
	direction = VNorm(direction);
	direction = VScale(direction, 15.0f);
	//AimPosition.z = playerPosition.z + direction.z;
	AimPosition = VAdd(playerPosition, direction);

	//�J��������O�ꂽ��
	if (isReflected)
	{
		//float left_len2 = VDot(leftPosition, leftPosition);
		//float right_len2 = VDot(rightPosition, rightPosition);
		//float leftScalar = VDot(direction, leftPosition) / left_len2;
		//float rightScalar = VDot(direction, rightPosition) / right_len2;

		////�ł��߂��ړ_
		//VECTOR nearPoint_left = VAdd(dragonPosition, VScale(leftPosition, leftScalar));
		//VECTOR nearPoint_right = VAdd(dragonPosition, VScale(rightPosition, rightScalar));

		////�ł��߂��ړ_����player�ւ̃x�N�g��
		//VECTOR diff_left = VSub(playerPosition, nearPoint_left);
		//VECTOR diff_right = VSub(playerPosition, nearPoint_right);

		////�_�ƒ����̋���
		//float distance_left = VSize(diff_left);
		//float distance_right = VSize(diff_right);

		//if (distance_left > distance_right)
		//{
		//	AimPosition = VAdd(AimPosition, diff_right);
		//}
		//else
		//{
		//	AimPosition = VAdd(AimPosition, diff_left);
		//}
	}

	AimPosition.y = 4.0f;
	cameraDirection = VSub(LookPosition, AimPosition);
	cameraDirection.y = 0.0f;
	cameraDirection = VNorm(cameraDirection);

	//float objectWidth = 2.0f;
	//float objectHeight = 6.0f;
	//float fov = CalculateFOV(36.0f, 50.0f);

	//float visible = CalculateVisibleWidth(VSize(VSub(playerPosition, aimPosition_Lockon)), fov);

	//isReflected = isObjectVisible(cameraDirection, fov, playerPosition, objectWidth, objectHeight);

	SetCameraPositionAndTarget_UpVecY(AimPosition, LookPosition);
}



/// <summary>
/// �Ώە����J�����Ɏʂ��Ă��邩
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fov"></param>
/// <param name="objectPosition"></param>
/// <param name="objectWidth"></param>
/// <param name="objectHeight"></param>
/// <returns></returns>
bool Camera::isObjectVisible(const VECTOR& cameraDirection,const VECTOR& objectPosition, const VECTOR& playerPosition)
{
	
	//�^�[�Q�b�g���J�����̕���
	VECTOR toObject = VSub(AimPosition, objectPosition);
	toObject = VNorm(toObject);

	///Y����]�s���Matrix�ɑ��
	MATRIX matrix_right = MGetRotY(DX_PI_F / 12.0f);
	MATRIX matrix_left = MGetRotY(DX_PI_F / -12.0f);

	//
	rightPosition = VTransform(toObject, matrix_right);
	leftPosition = VTransform(toObject, matrix_left);

	rightPosition = VScale(rightPosition, 1000.0f);
	leftPosition = VScale(leftPosition, 1000.0f);

	rightPosition.y = objectPosition.y;
	leftPosition.y = objectPosition.y;

	//�I�u�W�F�N�g���v���C���[�̕����x�N�g��
	VECTOR toPlayer = VSub(playerPosition, objectPosition);

	//�N���X��(xz����)
	cross_right = rightPosition.x * toPlayer.z - rightPosition.z * toPlayer.x;
	cross_left = leftPosition.x * toPlayer.z - leftPosition.z * toPlayer.x;

	//�����z������
	if (pravCross_left * cross_left < 0 || pravCross_right * cross_right < 0)
	{
		return true;
	}

	//����̂��߂ɋL�^
	pravCross_left = cross_left;
	pravCross_right = cross_right;

	return false;
}


void Camera::SetObserver(CameraObserver* obs)
{
	observer = obs;
}