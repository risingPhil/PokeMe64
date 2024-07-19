#include "widget/CursorWidget.h"
#include "animations/AnimationManager.h"
#include "core/RDPQGraphics.h"

static void moveAnimationFinishedCallback(void* context)
{
    CursorWidget *thiz = (CursorWidget*)context;
    thiz->onMoveAnimationFinished();
}

CursorWidget::CursorWidget(AnimationManager& animationManager)
    : idleAnimation_(this)
    , moveAnimation_(this)
    , animManager_(animationManager)
    , style_({0})
    , bounds_({0})
    , visible_(true)
{
    idleAnimation_.setLoopType(AnimationLoopType::BACK_AND_FORTH);
    moveAnimation_.setAnimationFinishedCallback(this, moveAnimationFinishedCallback);

    // idleAnimation is active first
    animManager_.add(&idleAnimation_);
}

CursorWidget::~CursorWidget()
{
    animManager_.remove(&moveAnimation_);
    animManager_.remove(&idleAnimation_);
}

bool CursorWidget::isFocused() const
{
    //irrelevant
    return true;
}

void CursorWidget::setFocused(bool)
{
    //irrelevant
}

bool CursorWidget::isVisible() const
{
    return visible_;
}

void CursorWidget::setVisible(bool visible)
{
    visible_ = visible;
}

Rectangle CursorWidget::getBounds() const
{
    return bounds_;
}

void CursorWidget::setBounds(const Rectangle& bounds)
{
    bounds_ = bounds;
}

void CursorWidget::moveToBounds(const Rectangle& targetBounds)
{
    animManager_.remove(&idleAnimation_);
    
    if(moveAnimation_.isFinished())
    {
        // moveAnimation is in the finished state.
        // therefore it wasn't part of AnimationManager yet
        animManager_.add(&moveAnimation_);
    }
    else
    {
        // previous move animation hasn't finished.
        // this means it's also already registered on AnimationManager
        // just skip it to its end position before starting a new animation
        moveAnimation_.skipToEnd();
    }

    const Rectangle diffVector = {
        .x = targetBounds.x - bounds_.x,
        .y = targetBounds.y - bounds_.y,
        .width = targetBounds.width - bounds_.width,
        .height = targetBounds.height - bounds_.height
    };

    moveAnimation_.start(bounds_, diffVector, style_.moveAnimationDurationInMs);
}

Dimensions CursorWidget::getSize() const
{
    return Dimensions{
        .width = bounds_.width,
        .height = bounds_.height
    };
}

bool CursorWidget::handleUserInput(const joypad_inputs_t&)
{
    // irrelevant
    return false;
}

void CursorWidget::render(RDPQGraphics& gfx, const Rectangle& parentBounds)
{
    if(!visible_ || !style_.sprite)
    {
        return;
    }
    const Rectangle myBounds = addOffset(bounds_, parentBounds);

    gfx.drawSprite(myBounds, style_.sprite, style_.spriteSettings);
}

void CursorWidget::setStyle(const CursorStyle& style)
{
    style_ = style;
}

void CursorWidget::onMoveAnimationFinished()
{
    // when the move animation is done, remove it from AnimationManager
    // and replace it with the idleAnimation
    animManager_.remove(&moveAnimation_);
    animManager_.add(&idleAnimation_);

    // reset idle animation starting from the new bounds_
    idleAnimation_.start(bounds_, style_.idleMoveDiff, style_.idleAnimationDurationInMs);
}
