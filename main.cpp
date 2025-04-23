#include "Include.h"

//基本設定
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{

    // 画面モードのセット
    ChangeWindowMode(TRUE);
    SetGraphMode(1600, 900, 16);

    // DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
    // Effekseerを使用するには必ず設定する。
    SetUseDirect3DVersion(DX_DIRECT3D_11);

    // ＤＸライブラリ初期化処理
    if (DxLib_Init() == -1)
    {
        return -1;	// エラーが起きたら直ちに終了
    }

    // Effekseerを初期化する。
    // 引数には画面に表示する最大パーティクル数を設定する。
    if (Effekseer_Init(8000) == -1)
    {
        DxLib_End();
        return -1;
    }

    // フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
    // Effekseerを使用する場合は必ず設定する。
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

    // DXライブラリのデバイスロストした時のコールバックを設定する。
    // ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
    // ただし、DirectX11を使用する場合は実行する必要はない。
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();

    // Ｚバッファを有効にする
    SetUseZBuffer3D(TRUE);

    // Ｚバッファへの書き込みを有効にする
    SetWriteZBuffer3D(TRUE);



    // エスケープキーが押されるかウインドウが閉じられるまでループ
    LONGLONG frameTime = 0;

    SceneManager* sceneManager = new SceneManager();

    sceneManager->Add<Game>("Game");

    while (!ProcessMessage() && !ClearDrawScreen() && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        frameTime = GetNowHiPerformanceCount();

        //エフェクシアを3Dに同期
        Effekseer_Sync3DSetting();

        //更新
        sceneManager->Update();

        //画面の初期化
        ClearDrawScreen();

        //描画
        sceneManager->Draw();

        // 裏画面の内容を表画面に反映させる
        ScreenFlip();

        // 雑に60fps固定
        while ((GetNowHiPerformanceCount() - frameTime) < 16667) {}
    }

    // Effekseerを終了する。
    Effkseer_End();

    // DXライブラリを終了する。
    DxLib_End();

    return 0;
}