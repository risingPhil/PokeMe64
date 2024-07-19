#ifndef _SCENE_WITH_DIALOG_WIDGET_H
#define _SCENE_WITH_DIALOG_WIDGET_H

#include "scenes/AbstractUIScene.h"
#include "widget/DialogWidget.h"

class SceneWithDialogWidget : public AbstractUIScene
{
public:
    SceneWithDialogWidget(SceneDependencies& deps);
    virtual ~SceneWithDialogWidget();

    void init() override;
    void destroy() override;

    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;
protected:
    virtual void setupFonts();
    virtual void setupDialog(DialogWidgetStyle& style);

    DialogWidget dialogWidget_;
    WidgetFocusChainSegment dialogFocusChainSegment_;
    uint8_t arialId_;
    uint8_t fontStyleWhiteId_;
private:
};

#endif