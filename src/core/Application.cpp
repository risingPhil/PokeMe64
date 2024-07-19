#include "core/Application.h"
#include "scenes/IScene.h"

Application::Application()
    : graphics_()
    , animationManager_()
    , fontManager_()
    , tpakManager_()
    , sceneManager_(graphics_, animationManager_, fontManager_, tpakManager_)
    , sceneBounds_({0})
{
}

Application::~Application()
{
    graphics_.destroy();

    display_close();
    timer_close();
    joypad_close();
}

void Application::init()
{
    // Based on example code https://github.com/DragonMinded/libdragon/wiki/OpenGL-on-N64
    debug_init_isviewer();
    //console_set_debug(true);

    joypad_init();
    timer_init();
    
    dfs_init(DFS_DEFAULT_LOCATION);

    graphics_.init();
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    sceneBounds_ = {.x = 0, .y = 0, .width = 320, .height = 240 };
    //display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);

    sceneManager_.switchScene(SceneType::INIT_TRANSFERPAK);
}

void Application::run()
{
    while(1)
    {
        graphics_.beginFrame();

        //const uint64_t before = get_ticks();
        animationManager_.step(20);
        joypad_poll();
        sceneManager_.handleUserInput();
        
        sceneManager_.render(sceneBounds_);
        //const uint64_t after = get_ticks();
        //debugf("frame took %lu ms\r\n", static_cast<uint32_t>(TICKS_TO_MS(after - before)));

        graphics_.finishAndShowFrame();
    }
}
