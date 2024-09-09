#include "scenes/SelectFileScene.h"
#include "scenes/SceneManager.h"
#include "scenes/DataCopyScene.h"

static const Rectangle fileBrowserBounds = {20, 20, 280, 200};

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
                .top = 5
            }
        },
        .itemStyle = {
            .size = {280, 16},
            .titleNotFocused = {
                .fontId = arialId_,
                .fontStyleId = fontStyleWhiteId_
            },
            .titleFocused = {
                .fontId = arialId_,
                .fontStyleId = fontStyleYellowId_
            },
            .leftMargin = 10,
            .topMargin = 1
        }
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

    if(dialogWidgetBackgroundSprite_)
    {
        sprite_free(dialogWidgetBackgroundSprite_);
        dialogWidgetBackgroundSprite_ = nullptr;
    }
}

void SelectFileScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    fileBrowser_.render(gfx, sceneBounds);
    SceneWithDialogWidget::render(gfx, sceneBounds);

    if(context_->title.text && !isZeroSizeRectangle(context_->title.bounds))
    {
        const Rectangle absoluteTextBounds = addOffset(context_->title.bounds, sceneBounds);
        gfx.drawText(absoluteTextBounds, context_->title.text, context_->title.renderSettings);
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