#pragma once

class SceneManager;

class BaseScene
{
public:

    BaseScene(SceneManager& manager);
    ~BaseScene();

    virtual void Initialize();
    virtual void Update();
    virtual void Draw();

protected:
    void ChangeScene(std::string_view);
private:
    SceneManager& manager;
};

