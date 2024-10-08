#ifndef _MAINMENUSCENE_H
#define _MAINMENUSCENE_H

#include "scenes/SceneWithDialogWidget.h"
#include "widget/VerticalList.h"
#include "widget/ImageWidget.h"
#include "widget/DialogWidget.h"
#include "widget/CursorWidget.h"
#include "widget/MenuItemWidget.h"
#include "widget/ListItemFiller.h"
#include "widget/IFocusListener.h"
#include "widget/IScrollWindowListener.h"

typedef struct MenuSceneContext
{
    MenuItemData* menuEntries;
    uint32_t numMenuEntries;
    bool bButtonMeansUserWantsToSwitchCartridge;
} MenuSceneContext;

/**
 * @brief A scene showing a menu
 * 
 */
class MenuScene : public SceneWithDialogWidget, public IFocusListener, public IScrollWindowListener
{
public:
    MenuScene(SceneDependencies& deps, void* context);
    virtual ~MenuScene();

    void init() override;
    void destroy() override;

    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;

    bool handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs) override;

    virtual void onDialogDone();

    void focusChanged(const FocusChangeStatus& status) override;
    void onScrollWindowChanged(const ScrollWindowUpdate& update) override;

    void showDialog(DialogData* diagData) override;
protected:
    virtual void setupMenu();
    void setupDialog(DialogWidgetStyle& style) override;

    MenuSceneContext* context_;
    sprite_t* menu9SliceSprite_;
    sprite_t* cursorSprite_;
    sprite_t* uiArrowUpSprite_;
    sprite_t* uiArrowDownSprite_;
    VerticalList menuList_;
    ImageWidget scrollArrowUp_;
    ImageWidget scrollArrowDown_;
    CursorWidget cursorWidget_;
    ListItemFiller<VerticalList, MenuItemData, MenuItemWidget, MenuItemStyle>  menuListFiller_;
    WidgetFocusChainSegment listFocusChainSegment_;
    bool bButtonPressed_;
private:
};

void deleteMenuSceneContext(void* context);

#endif