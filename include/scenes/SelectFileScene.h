#ifndef _SELECTFILESCENE_H
#define _SELECTFILESCENE_H

#include "scenes/SceneWithDialogWidget.h"
#include "widget/FileBrowserWidget.h"
#include "widget/ImageWidget.h"

typedef struct SelectFileSceneContext
{
    const char* titleText;
    struct {
        SceneType type;
        void* context;
        void (*deleteContextFunc)(void*);
    } nextScene;
    // initial path. If left NULL, it will default to sd:/
    const char* initialPath;
    // file extension filter. if set, only files with the specified extension will be shown
    const char* fileExtensionFilter;
    // HACK: indicates that -instead of showing this scene- we want to navigate back to the previous scene instead
    // this is useful for influencing back behaviour after the DataCopyScene is done.
    // we want to end up back in the backup menu, so we need to go back twice in the scene history.
    // This was the easiest option, because the main menu is game specific (and therefore much more difficult to go to directly)
    // and so is the backup menu.
    bool goBackToPreviousSceneInstead;
} SelectFileSceneContext;

class SelectFileScene : public SceneWithDialogWidget
{
public:
    SelectFileScene(SceneDependencies& deps, void* sceneContext);
    virtual ~SelectFileScene();

    void init() override;
    void destroy() override;

    bool handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs) override;
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
    SelectFileSceneContext* context_;
    sprite_t* dialogWidgetBackgroundSprite_;
    sprite_t* uiArrowUpSprite_;
    sprite_t* uiArrowDownSprite_;
    sprite_t* uiIconFile_;
    sprite_t* uiIconDirectory_;
    bool bButtonPressed_;
};

void deleteSelectFileSceneContext(void* context);

#endif