#ifndef _CURSORWIDGET_H
#define _CURSORWIDGET_H

#include "widget/IWidget.h"
#include "animations/MoveAnimation.h"
#include "core/Sprite.h"

class AnimationManager;

typedef struct CursorStyle
{
    sprite_t* sprite;
    SpriteRenderSettings spriteSettings;
    Rectangle idleMoveDiff;
    uint16_t idleAnimationDurationInMs;
    uint16_t moveAnimationDurationInMs;
} CursorStyle;

/**
 * @brief This widget represents the cursor in a list.
 * It can be used to point to the selected item in a list.
 * It's up to the "user"/dev to implement a way (hint: IFocusListener) to move
 * this cursor when the focus changes. The reason for this is that the Cursor may
 * need to be drawn at a scene-specific offset or any other scene-specific shenanigans.
 * By not handling the focus change behaviour inside this widget, we're not restricting the "users"
 * of this widget
 */
class CursorWidget : public IWidget
{
public:
    CursorWidget(AnimationManager& animManager);
    virtual ~CursorWidget();

    bool isFocused() const override;
    void setFocused(bool isFocused) override;

    bool isVisible() const override;
    void setVisible(bool visible) override;

    Rectangle getBounds() const override;
    void setBounds(const Rectangle& bounds) override;

    /**
     * @brief This function animates a move to the specified bounds
     */
    void moveToBounds(const Rectangle& targetBounds);

    Dimensions getSize() const override;
    
    bool handleUserInput(const joypad_inputs_t& userInput) override;
    void render(RDPQGraphics& gfx, const Rectangle& parentBounds) override;

    void setStyle(const CursorStyle& style);

    /**
     * @brief This function is called when our moveAnimation has finished
     */
    void onMoveAnimationFinished();
protected:
private:
    MoveAnimation idleAnimation_;
    MoveAnimation moveAnimation_;
    AnimationManager& animManager_;
    CursorStyle style_;
    Rectangle bounds_;
    bool visible_;
};

#endif