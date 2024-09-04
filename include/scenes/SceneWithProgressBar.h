#ifndef _SCENEWITHPROGRESSBAR_H
#define _SCENEWITHPROGRESSBAR_H

#include "scenes/SceneWithDialogWidget.h"
#include "widget/ProgressBarWidget.h"

/**
 * @brief This scene implementation adds a ProgressBarWidget on top of the 
 * SceneWithDialogWidget
 */
class SceneWithProgressBar : public SceneWithDialogWidget
{
public:
    SceneWithProgressBar(SceneDependencies& deps);
    virtual ~SceneWithProgressBar();

    void init() override;
    void destroy() override;

    bool handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs) override;
    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;

    void onDialogDone();
protected:
    void setupFonts() override;
    void setupDialog(DialogWidgetStyle& style) override;
private:
    ProgressBarWidget progressWidget_;
    sprite_t* dialogWidgetSprite_;
    sprite_t* progressBackgroundSprite_;
    uint8_t progress_;
    uint8_t arialSmallId_;
};

#endif