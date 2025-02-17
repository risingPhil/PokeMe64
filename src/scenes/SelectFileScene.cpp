#include "scenes/SelectFileScene.h"
#include "scenes/SceneManager.h"
#include "scenes/DataCopyScene.h"

static const Rectangle titleBounds = {20, 10, 280, 16};
static const Rectangle fileBrowserBounds = {20, 30, 280, 180};

static void fileConfirmedCallback(void* context, const char* path)
{
    auto scene = (SelectFileScene*)context;
    scene->onFileConfirmed(path);
}

static void dialogFinishedCallback(void* context)
{
    auto scene = (SelectFileScene*)context;
    scene->onDialogDone();
}


SelectFileScene::SelectFileScene(SceneDependencies& deps, void* context)
    : SceneWithDialogWidget(deps)
    , fileBrowser_(deps.animationManager)
    , fileBrowserFocusSegment_{
        .current = &fileBrowser_
    }
    , diag_({0})
    , context_((SelectFileSceneContext*)context)
    , dialogWidgetBackgroundSprite_(nullptr)
    , uiArrowUpSprite_(nullptr)
    , uiArrowDownSprite_(nullptr)
    , uiIconFile_(nullptr)
    , uiIconDirectory_(nullptr)
    , bButtonPressed_(false)
{
}

SelectFileScene::~SelectFileScene()
{
}

void SelectFileScene::init()
{
    if(context_->goBackToPreviousSceneInstead)
    {
        // the context indicates that we should go back to the previous scene instead.
        // this is likely because we ended up back here because DataCopyScene is done
        // and we want to go back to the backup menu.
        deps_.sceneManager.goBackToPreviousScene();
        return;
    }

    dialogWidgetBackgroundSprite_ = sprite_load("rom://menu-bg-9slice.sprite");
    uiArrowUpSprite_ = sprite_load("rom://ui-arrow-up.sprite");
    uiArrowDownSprite_ = sprite_load("rom://ui-arrow-down.sprite");
    uiIconFile_ = sprite_load("rom://ui-icon-file.sprite");
    uiIconDirectory_ = sprite_load("rom://ui-icon-folder.sprite");

    SceneWithDialogWidget::init();

    const FileBrowserWidgetStyle browserStyle = {
        .listStyle = {
            .background = {
                .sprite = dialogWidgetBackgroundSprite_,
                .spriteSettings = {
                    .renderMode = SpriteRenderMode::NINESLICE,
                    .srcRect = {6, 6, 6, 6}
                }
            },
            .margin = {
                .top = 5,
                .bottom = 5
            }
        },
        .itemStyle = {
            .size = {280, 20},
            .icon = {
                .spriteBounds = { 5, 2, 16, 16}
            },
            .titleNotFocused = {
                .fontId = mainFontId_,
                .fontStyleId = fontStyleWhiteId_
            },
            .titleFocused = {
                .fontId = mainFontId_,
                .fontStyleId = fontStyleYellowId_
            },
            .leftMargin = 24,
            .topMargin = 4
        },
        .scrollArrowUpStyle = {
            .image = {
                .sprite = uiArrowUpSprite_,
                .spriteBounds = Rectangle{0, 0, uiArrowUpSprite_->width, uiArrowUpSprite_->height}
            }
        },
        .scrollArrowDownStyle = {
            .image = {
                .sprite = uiArrowDownSprite_,
                .spriteBounds = Rectangle{0, 0, uiArrowDownSprite_->width, uiArrowDownSprite_->height}
            }
        },
        .fileIconSprite = uiIconFile_,
        .directoryIconSprite = uiIconDirectory_
    };

    setFocusChain(&fileBrowserFocusSegment_);
    fileBrowser_.setBounds(fileBrowserBounds);
    fileBrowser_.setStyle(browserStyle);
    fileBrowser_.setItemConfirmedCallback(fileConfirmedCallback, this);
    fileBrowser_.setFileExtensionToFilter(context_->fileExtensionFilter);
    fileBrowser_.setPath((context_->initialPath) ? context_->initialPath : "sd:/");
}

void SelectFileScene::destroy()
{
    SceneWithDialogWidget::destroy();

    if(uiIconDirectory_)
    {
        sprite_free(uiIconDirectory_);
        uiIconDirectory_ = nullptr;
    }

    if(uiIconFile_)
    {
        sprite_free(uiIconFile_);
        uiIconFile_ = nullptr;
    }

    if(dialogWidgetBackgroundSprite_)
    {
        sprite_free(dialogWidgetBackgroundSprite_);
        dialogWidgetBackgroundSprite_ = nullptr;
    }

    if(uiArrowUpSprite_)
    {
        sprite_free(uiArrowUpSprite_);
        uiArrowUpSprite_ = nullptr;
    }

    if(uiArrowDownSprite_)
    {
        sprite_free(uiArrowDownSprite_);
        uiArrowDownSprite_ = nullptr;
    }
}

bool SelectFileScene::handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs)
{
    bool goBackToPreviousSceneOnUnhandledBPress = false;
    // keep track of the b button
    // if the FileBrowserWidget doesn't handle the release
    // then we need to go back to the previous scene
    if(!bButtonPressed_ && inputs.btn.b)
    {
        bButtonPressed_ = true;
    }
    else if(bButtonPressed_ && !inputs.btn.b)
    {
        bButtonPressed_ = false;
        goBackToPreviousSceneOnUnhandledBPress = true;
    }

    bool ret = SceneWithDialogWidget::handleUserInput(port, inputs);
    if(ret)
    {
        return ret;
    }

    if(goBackToPreviousSceneOnUnhandledBPress)
    {
        deps_.sceneManager.goBackToPreviousScene();
        return true;
    }

    return false;
    // not handled by normal means
}

void SelectFileScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    fileBrowser_.render(gfx, sceneBounds);
    SceneWithDialogWidget::render(gfx, sceneBounds);

    if(context_->titleText && !isZeroSizeRectangle(titleBounds))
    {
        const TextRenderSettings renderSettings = {
            .fontId = mainFontId_,
            .fontStyleId = fontStyleWhiteId_,
            .halign = ALIGN_CENTER
        };
        const Rectangle absoluteTextBounds = addOffset(titleBounds, sceneBounds);
        gfx.drawText(absoluteTextBounds, context_->titleText, renderSettings);
    }
}

void SelectFileScene::onDialogDone()
{
    deps_.sceneManager.goBackToPreviousScene();
}

void SelectFileScene::onFileConfirmed(const char* path)
{
    if(context_->nextScene.type == SceneType::COPY_DATA)
    {
        auto nextSceneContext = (DataCopySceneContext*)context_->nextScene.context;
        nextSceneContext->saveToRestorePath = strdup(path);
    }
    context_->goBackToPreviousSceneInstead = true;
    deps_.sceneManager.switchScene(context_->nextScene.type, context_->nextScene.deleteContextFunc, context_->nextScene.context);
}

void SelectFileScene::showDialog(DialogData* diagData)
{
    SceneWithDialogWidget::showDialog(diagData);
    fileBrowser_.setVisible(false);
    setFocusChain(&dialogFocusChainSegment_);
}


void SelectFileScene::setupDialog(DialogWidgetStyle& style)
{
    style.background.sprite = dialogWidgetBackgroundSprite_;
    style.background.spriteSettings = {
        .renderMode = SpriteRenderMode::NINESLICE,
        .srcRect = { 6, 6, 6, 6 }
    };

    SceneWithDialogWidget::setupDialog(style);

    dialogWidget_.setOnDialogFinishedCallback(dialogFinishedCallback, this);
    dialogWidget_.setVisible(false);
}

void deleteSelectFileSceneContext(void* context)
{
    auto sceneContext = (SelectFileSceneContext*)context;
    delete sceneContext;
}