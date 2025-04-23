#pragma once
class SceneManager
{
public:

    SceneManager();
    ~SceneManager();

    template <class SceneType>
    void Add(const std::string_view name)
    {
        scenes[name] = new SceneType{ *this };

        if (nowScene == nullptr)
        {
            nowScene = scenes[name];
            nowScene->Initialize();
        }
    }

    void Initialize();
    void Update();
    void Draw();

    void ChangeScene(std::string_view name);
private:

    std::unordered_map<std::string_view, BaseScene*>scenes;

    BaseScene* nowScene;
};

