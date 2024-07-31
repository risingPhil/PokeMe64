#include "scenes/MenuScene.h"
#include "core/FontManager.h"
#include "scenes/SceneManager.h"
#include "transferpak/TransferPakManager.h"
#include "menu/MenuFunctions.h"

#include <cstdio>

static void dialogFinishedCallback(void* context)
{
    MenuScene* scene = (MenuScene*)context;
    scene->onDialogDone();
}

MenuScene::MenuScene(SceneDependencies& deps, void* context)
    : SceneWithDialogWidget(deps)
    , context_(static_cast<MenuSceneContext*>(context))
    , menu9SliceSprite_(nullptr)
    , cursorSprite_(nullptr)
    , menuList_(deps.animationManager)
    , cursorWidget_(deps.animationManager)
    , menuListFiller_(menuList_)
    , listFocusChainSegment_(WidgetFocusChainSegment{
        .current = &menuList_
    })
    , bButtonPressed_(false)
{
}

MenuScene::~MenuScene()
{
    // we registered ourselves as context before. (setupMenu)
    // this instance is about to become delete'd
    // we need to reset every context referring to this instance to prevent
    // crashes the next time we load the same menuentries
    for(unsigned i=0; i < context_->numMenuEntries; ++i)
    {
        if(context_->menuEntries[i].context == this)
        {
            context_->menuEntries[i].context = nullptr;
        }
    }
}

void MenuScene::init()
{
    // load these sprites before the parent init because setupDialog(style) will need them
    menu9SliceSprite_ = sprite_load("rom://menu-bg-9slice.sprite");
    cursorSprite_ = sprite_load("rom://hand-cursor.sprite");

    SceneWithDialogWidget::init();

    setupMenu();
    
    setFocusChain(&listFocusChainSegment_);
}

void MenuScene::destroy()
{
    menuList_.unregisterFocusListener(this);
    menuList_.clearWidgets();
    menuList_.setStyle({0});
    cursorWidget_.setStyle({0});

    // destroy the parent before releasing the sprites because the dialog widget
    // may still have a reference to them
    SceneWithDialogWidget::destroy();

    sprite_free(cursorSprite_);
    sprite_free(menu9SliceSprite_);
}

void MenuScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    menuList_.render(gfx, sceneBounds);
    cursorWidget_.render(gfx, sceneBounds);
    SceneWithDialogWidget::render(gfx, sceneBounds);

    TextRenderSettings renderSettings = {
        .fontId = arialId_,
        .fontStyleId = fontStyleWhiteId_
    };
    gfx.drawText(Rectangle{40, 10, 280, 16}, "PokeMe64 by risingPhil. Version 0.1", renderSettings);
}

bool MenuScene::handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs)
{
    if(SceneWithDialogWidget::handleUserInput(port, inputs))
    {
        return true;
    }

    if(inputs.btn.b)
    {
        // we will only handle b button release.
        bButtonPressed_ = true;
        return true;
    }
    else if(bButtonPressed_)
    {
        // b button release occurred. Switch back to previous scene.
        bButtonPressed_ = false;

        if(context_->bButtonMeansUserWantsToSwitchCartridge)
        {
            DialogData* diag = new DialogData{
                .shouldDeleteWhenDone = true
            };

            setDialogDataText(*diag, "Please turn the console off to switch gameboy cartridges!");

            showDialog(diag);
        }
        else
        {
            // now do the actual switch back to the previous scene
            deps_.sceneManager.goBackToPreviousScene();
        }
        return true;
    }
    return false;
}

void MenuScene::onDialogDone()
{
    dialogWidget_.setVisible(false);
    menuList_.setVisible(true);
    cursorWidget_.setVisible(true);
    setFocusChain(&listFocusChainSegment_);
}

void MenuScene::focusChanged(const FocusChangeStatus& status)
{
    const Rectangle newCursorBounds = {
        .x = status.focusBounds.x + 2,
        .y = status.focusBounds.y,
        .width = cursorSprite_->width,
        .height = cursorSprite_->height
    };

    cursorWidget_.moveToBounds(newCursorBounds);
}

SceneDependencies& MenuScene::getDependencies()
{
    return deps_;
}

void MenuScene::setupMenu()
{
    const VerticalListStyle listStyle = {
        .background = {
            .sprite = menu9SliceSprite_,
            .spriteSettings = {
                .renderMode = SpriteRenderMode::NINESLICE,
                .srcRect = { 6, 6, 6, 6 }
            }
        },
        .margin = {
            .top = 5
        }
    };

    const CursorStyle cursorStyle = {
        .sprite  = cursorSprite_,
        .idleMoveDiff = { 5, 0, 0, 0 },
        .idleAnimationDurationInMs = 500,
        .moveAnimationDurationInMs = 250
    };

    menuList_.setStyle(listStyle);
    menuList_.setBounds(Rectangle{100, 30, 150, 150});
    menuList_.setVisible(true);
    cursorWidget_.setStyle(cursorStyle);
    cursorWidget_.setVisible(true);
    menuList_.registerFocusListener(this);

    const MenuItemStyle itemStyle = {
        .size = {150, 16},
        .titleNotFocused = {
            .fontId = arialId_,
            .fontStyleId = fontStyleWhiteId_
        },
        .titleFocused = {
            .fontId = arialId_,
            .fontStyleId = fontStyleYellowId_
        },
        .leftMargin = 35,
        .topMargin = 1
    };

    for(unsigned i=0; i < context_->numMenuEntries; ++i)
    {
        if(!context_->menuEntries[i].context)
        {
            context_->menuEntries[i].context = this;
        }
    }

    menuListFiller_.addItems(context_->menuEntries, context_->numMenuEntries, itemStyle);
}

void MenuScene::setupDialog(DialogWidgetStyle& style)
{
    style.background.sprite = menu9SliceSprite_;
    style.background.spriteSettings = {
        .renderMode = SpriteRenderMode::NINESLICE,
        .srcRect = { 6, 6, 6, 6 }
    };

    SceneWithDialogWidget::setupDialog(style);

    dialogWidget_.setOnDialogFinishedCallback(dialogFinishedCallback, this);
    dialogWidget_.setVisible(false);
}

void MenuScene::showDialog(DialogData* diagData)
{
    SceneWithDialogWidget::showDialog(diagData);
    menuList_.setVisible(false);
    cursorWidget_.setVisible(false);
    setFocusChain(&dialogFocusChainSegment_);
}

void deleteMenuSceneContext(void* context)
{
    MenuSceneContext* menuContext = static_cast<MenuSceneContext*>(context);
    delete menuContext;
}