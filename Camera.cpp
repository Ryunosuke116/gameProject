#include"Include.h"
#include "Camera.h"

Camera::Camera()
{
	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(3.5f, 5000.0f);

	AimPosition = VGet(0.0f, 0.0f, 0.0f);
	LookPosition = VGet(0.0f, 0.0f, 0.0f);
	keepPlayerPosition = VGet(0.0f, 0.0f, 0.0f);
	cameraDirection = VGet(0.0f, 0.0f, 0.0f);
	a = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{

}

/// <summary>
/// 初期化
/// </summary>
void Camera::Initialize()
{
	AimPosition = VGet(30.0f, 15, -10);
	LookPosition = VGet(0.0f, 0, 20.0f);
	leftPosition= VGet(0.0f, 0, 10.0f);
	rightPosition= VGet(0.0f, 0, -10.0f);

	angle = 0.0f;
	pravCross_right = 0;
	pravCross_left = 0;
	a = -177.55f;
	isLockOn = false;
	SetCameraPositionAndTarget_UpVecY(AimPosition, LookPosition);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update(const VECTOR& playerPosition, const VECTOR& dragonPosition)
{
	keepEnemyPosition = dragonPosition;

	//カメラ移動処理
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

		float angle = a * DX_PI_F / 360.0f;
		this->angle = angle;

		//AimPosition = VAdd(AimPosition, addAimPosition);
		AimPosition.x = LookPosition.x + 30 * cos(angle);
		AimPosition.z = LookPosition.z + 30 * sin(angle);

		float maxRange = 5.0f;
		float maxRange_ = 10.0f;

		//中心からの距離を測る
		float r = VSize(VSub(playerPosition, LookPosition));


		//一定の距離に達したらそれ以上いけないようにする
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

		//isReflected = CheckCameraViewClip(dragonPosition);
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
			////最も近い接点
			//VECTOR nearPoint_left = VAdd(dragonPosition, VScale(leftPosition, leftScalar));
			//VECTOR nearPoint_right = VAdd(dragonPosition, VScale(rightPosition, rightScalar));
		
			////最も近い接点からplayerへのベクトル
			//VECTOR diff_left = VSub(playerPosition, nearPoint_left);
			//VECTOR diff_right = VSub(playerPosition, nearPoint_right);

			////点と直線の距離
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

/// <summary>
/// 描画
/// </summary>
void Camera::Draw()
{
	printfDx("AimPosition.x %f\AimPosition.z %f\n", AimPosition.x, AimPosition.z);
	printfDx("LookPosition.x %f\LookPosition.z %f\n", LookPosition.x, LookPosition.z);
	printfDx("angle %f\n",angle);
	printfDx("right.x %f\nright.y %f\nright.z %f\n", rightPosition.x, rightPosition.y, rightPosition.z);
	printfDx("left.x %f\nleft.y %f\nleft.z %f", leftPosition.x, leftPosition.y, leftPosition.z);
	
	if (!isReflected)
	{
		printfDx("映っている");
	}
	
	DrawLine3D(keepEnemyPosition, leftPosition, GetColor(255, 255, 255));
	DrawLine3D(keepEnemyPosition, rightPosition, GetColor(255, 255, 255));

}

/// <summary>
/// ラープ
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
/// ロックオン時のカメラの挙動
/// </summary>
/// <param name="playerPosition"></param>
/// <param name="dragonPosition"></param>
void Camera::LockOnCamera(const VECTOR& playerPosition,const VECTOR& dragonPosition)
{
	//ドラゴン→playerへの方向ベクトル
	VECTOR direction = VSub(playerPosition, dragonPosition);
	direction = VNorm(direction);
	direction = VScale(direction, 15.0f);

	//VECTOR cameraDirection = VSub(AimPosition, dragonPosition);
	//cameraDirection = VNorm(cameraDirection);
	
	//AimPosition = VAdd(playerPosition, direction);
	
	//TODO::ロックオン時のカメラの制御を円周移動にする
	//ラジアンを求める

	//カメラから外れた時
	if (isReflected)
	{
		//すべてenemyの座標を中心点として考える
		//カメラの角度
		float aimPos_angle = atan2(AimPosition.z, AimPosition.x);
		//プレイヤーの角度
		float angle = atan2(direction.z, direction.x);

		//左右の範囲の角度を求める
		float angle_left = atan2(leftPosition.z, leftPosition.x);
		float angle_right = atan2(rightPosition.z, rightPosition.x);

		//プレイヤーと左右の範囲の差
		float diff_left = angle_left - angle;
		float diff_right = angle_right - angle;

		////差が小さい方にする
		//float diff = (diff_left < diff_right) ? diff_left : diff_right;


		////差が180°超えていたら180°引いて小さくする
		//while (diff > DX_PI) diff -= DX_PI;
		//while (diff < -DX_PI) diff += DX_PI;

		////カメラの角度に差を引く
		//aimPos_angle += diff;


		//if その目標値から一定の左右値を超えた時、超えた分補正する
		float subAngle = angle - aimPos_angle;
		float scaleAngle = 0.0f;

		if (subAngle > DX_PI)
		{
			subAngle = DX_PI * 2 - subAngle;
		}

		/*scaleAngle = subAngle * cameraSpeed;
		aimPos_angle = aimPos_angle + scaleAngle;*/

		aimPos_angle = subAngle;

		//円周移動
		AimPosition.x = LookPosition.x + 60 * cos(aimPos_angle);
		AimPosition.z = LookPosition.z + 60 * sin(aimPos_angle);


		//float left_len2 = VDot(leftPosition, leftPosition);
		//float right_len2 = VDot(rightPosition, rightPosition);
		//float leftScalar = VDot(direction, leftPosition) / left_len2;
		//float rightScalar = VDot(direction, rightPosition) / right_len2;

		////最も近い接点
		//VECTOR nearPoint_left = VAdd(dragonPosition, VScale(leftPosition, leftScalar));
		//VECTOR nearPoint_right = VAdd(dragonPosition, VScale(rightPosition, rightScalar));

		////最も近い接点からplayerへのベクトル
		//VECTOR diff_left = VSub(playerPosition, nearPoint_left);
		//VECTOR diff_right = VSub(playerPosition, nearPoint_right);

		////点と直線の距離
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

	SetCameraPositionAndTarget_UpVecY(AimPosition, LookPosition);
}



/// <summary>
/// 対象物がカメラに写っているか
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fov"></param>
/// <param name="objectPosition"></param>
/// <param name="objectWidth"></param>
/// <param name="objectHeight"></param>
/// <returns></returns>
bool Camera::isObjectVisible(const VECTOR& cameraDirection,const VECTOR& objectPosition, const VECTOR& playerPosition)
{
	
	//ターゲット→カメラの方向
	VECTOR toObject = VSub(AimPosition, objectPosition);
	toObject = VNorm(toObject);

	///Y軸回転行列をMatrixに代入
	MATRIX matrix_right = MGetRotY(DX_PI_F / 12.0f);
	MATRIX matrix_left = MGetRotY(DX_PI_F / -12.0f);

	//
	rightPosition = VTransform(toObject, matrix_right);
	leftPosition = VTransform(toObject, matrix_left);

	rightPosition = VScale(rightPosition, 1000.0f);
	leftPosition = VScale(leftPosition, 1000.0f);

	rightPosition.y = objectPosition.y;
	leftPosition.y = objectPosition.y;

	//オブジェクト→プレイヤーの方向ベクトル
	VECTOR toPlayer = VSub(playerPosition, objectPosition);

	////プレイヤーの角度
	//float player_angle = atan2(toPlayer.z, toPlayer.x);

	////右の角度
	//float rightPos_angle = atan2(rightPosition.z, rightPosition.x);

	////左の角度
	//float leftPos_angle = atan2(leftPosition.z, leftPosition.x);

	//float sumRight_angle = player_angle + rightPos_angle;
	//float sumleft_angle = player_angle + leftPos_angle;

	//クロス積(xz平面)
	cross_right = rightPosition.x * toPlayer.z - rightPosition.z * toPlayer.x;
	cross_left = leftPosition.x * toPlayer.z - leftPosition.z * toPlayer.x;

	//線を越えたか
	if (pravCross_left * cross_left < 0 || pravCross_right * cross_right < 0)
	{
		return true;
	}

	//次回のために記録
	pravCross_left = cross_left;
	pravCross_right = cross_right;

	return false;
}


void Camera::SetObserver(CameraObserver* obs)
{
	observer = obs;
}