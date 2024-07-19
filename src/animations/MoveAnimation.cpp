#include "animations/MoveAnimation.h"
#include "widget/IWidget.h"

#include <cmath>

MoveAnimation::MoveAnimation(IWidget* target)
    : AbstractAnimation(1.f)
    , target_(target)
    , startBounds_({0})
    , diffStartEnd_({0})
    , durationInMs_(0)
{
}

MoveAnimation::~MoveAnimation()
{
}

AnimationDistanceTimeFunctionType MoveAnimation::getDistanceTimeFunctionType() const
{
    return AnimationDistanceTimeFunctionType::EASE_IN_EASE_OUT;
}

uint32_t MoveAnimation::getDurationInMs() const
{
    return durationInMs_;
}

void MoveAnimation::start(const Rectangle& startBounds, const Rectangle& moveVectorStartEnd, uint32_t durationInMs)
{
    currentTimePos_ = 0.f;
    startBounds_ = startBounds;
    diffStartEnd_ = moveVectorStartEnd;
    durationInMs_ = durationInMs;
}

void MoveAnimation::reset()
{
    currentTimePos_ = 0.f;
}

void MoveAnimation::apply(float pos)
{
    const Rectangle newBounds = {
        .x = startBounds_.x + static_cast<int>(ceilf(pos * diffStartEnd_.x)),
        .y = startBounds_.y + static_cast<int>(ceilf(pos * diffStartEnd_.y)),
        .width = startBounds_.width + static_cast<int>(ceilf(pos * diffStartEnd_.width)),
        .height = startBounds_.height + static_cast<int>(ceilf(pos * diffStartEnd_.height))
    };

    target_->setBounds(newBounds);
}