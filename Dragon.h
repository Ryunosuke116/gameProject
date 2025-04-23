#pragma once
class Dragon : public BaseChara
{
private:
	static constexpr float modelScale = 0.01f;
public:

	Dragon();
	~Dragon();

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection)override;
	void MotionUpdate() override;
};

