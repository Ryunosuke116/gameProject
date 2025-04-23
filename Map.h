#pragma once
class Map
{
private:
	int backGroundHandle;
    int fieldHandle;

	VECTOR position;

public:
    Map();
    ~Map();

    void Initialize();
    void Update();
    void Draw();
};

