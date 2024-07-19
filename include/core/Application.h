#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "animations/AnimationManager.h"
#include "scenes/SceneManager.h"
#include "core/RDPQGraphics.h"
#include "core/FontManager.h"
#include "transferpak/TransferPakManager.h"

class Application
{
public:
    Application();
    ~Application();
    
    void init();

    void run();
protected:
private:
    RDPQGraphics graphics_;
    AnimationManager animationManager_;
    FontManager fontManager_;
    TransferPakManager tpakManager_;
    SceneManager sceneManager_;
    Rectangle sceneBounds_;
};

#endif