#ifndef _TESTSCENE_H
#define _TESTSCENE_H

#include "scenes/SceneWithDialogWidget.h"
#include "widget/FileBrowserWidget.h"


class TestScene : public SceneWithDialogWidget
{
public:
    TestScene(SceneDependencies& deps, void* sceneContext);
    virtual ~TestScene();

    void init() override;
    void destroy() override;

    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;

    void onDialogDone();

    void onFileConfirmed(const char* path);

    void showDialog(DialogData* diagData) override;
protected:
    void setupDialog(DialogWidgetStyle& style) override;
private:
    FileBrowserWidget fileBrowser_;
    WidgetFocusChainSegment fileBrowserFocusSegment_;
    DialogData diag_;
    sprite_t* dialogWidgetBackgroundSprite_;
};

#endif