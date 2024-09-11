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

    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;

    void setProgress(double progress);
protected:
    virtual void setupProgressBar(ProgressBarWidgetStyle& style);
private:
    ProgressBarWidget progressWidget_;
};

#endif