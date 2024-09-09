#include "scenes/TestScene.h"
#include "scenes/SceneManager.h"

static const Rectangle fileBrowserBounds = {20, 20, 280, 200};

static void fileConfirmedCallback(void* context, const char* path)
{
    auto scene = (TestScene*)context;
    scene->onFileConfirmed(path);
}

static void dialogFinishedCallback(void* context)
{
    auto scene = (TestScene*)context;
    scene->onDialogDone();
}


TestScene::TestScene(SceneDependencies& deps, void*)
    : SceneWithDialogWidget(deps)
    , fileBrowser_(deps.animationManager)
    , fileBrowserFocusSegment_{
        .current = &fileBrowser_
    }
    , diag_({0})
    , dialogWidgetBackgroundSprite_(nullptr)
{
}

TestScene::~TestScene()
{
}

void TestScene::init()
{
    dialogWidgetBackgroundSprite_ = sprite_load("rom://menu-bg-9slice.sprite");

    SceneWithDialogWidget::init();

    const FileBrowserWidgetStyle browserStyle = {
        .background = {
            .sprite = dialogWidgetBackgroundSprite_,
            .renderSettings = {
                .renderMode = SpriteRenderMode::NINESLICE,
                .srcRect = {6, 6, 6, 6}
            }
        },
        .margin = {
            .top = 5
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
    fileBrowser_.setFileExtensionToFilter(".bmp");
    fileBrowser_.setPath("sd:/");
}

void TestScene::destroy()
{
    SceneWithDialogWidget::destroy();
    
    sprite_free(dialogWidgetBackgroundSprite_);
    dialogWidgetBackgroundSprite_ = nullptr;
}

void TestScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    fileBrowser_.render(gfx, sceneBounds);
    SceneWithDialogWidget::render(gfx, sceneBounds);
}

void TestScene::onDialogDone()
{
    deps_.sceneManager.goBackToPreviousScene();
}

void TestScene::onFileConfirmed(const char* path)
{
    setDialogDataText(diag_, "File confirmed: %s", path);
    showDialog(&diag_);
}

void TestScene::showDialog(DialogData* diagData)
{
    SceneWithDialogWidget::showDialog(diagData);
    fileBrowser_.setVisible(false);
    setFocusChain(&dialogFocusChainSegment_);
}


void TestScene::setupDialog(DialogWidgetStyle& style)
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