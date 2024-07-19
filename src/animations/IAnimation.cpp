#include "animations/IAnimation.h"

static float calculateAnimationPos(AnimationDistanceTimeFunctionType distanceTimeType, float timePos)
{
    float result;

    switch(distanceTimeType)
    {
        case AnimationDistanceTimeFunctionType::NONE:
            return 1.f;
        case AnimationDistanceTimeFunctionType::LINEAR:
            result = timePos;
            break;
        case AnimationDistanceTimeFunctionType::EASE_IN_EASE_OUT:
            // using the bezier solution here: https://stackoverflow.com/questions/13462001/ease-in-and-ease-out-animation-formula
            result = timePos * timePos * (3.0f - 2.0f * timePos);
            break;
        default:
            result = 0.f;
            break;
    }

    return result;
}

IAnimation::~IAnimation()
{
}

AbstractAnimation::AbstractAnimation(float initialTimePos)
    : currentTimePos_(initialTimePos)
    , animationFinishedCb_(nullptr)
    , animationFinishedCallbackContext_(nullptr)
    , loopType_(AnimationLoopType::NONE)
    , isStepIncreasing_(true)
{
}

AbstractAnimation::~AbstractAnimation()
{
}

void AbstractAnimation::step(float stepSize, bool suppressFinishedCallback)
{
    if(isStepIncreasing_)
    {
        currentTimePos_ += stepSize;
        if(currentTimePos_ >= 1.f)
        {
            switch(loopType_)
            {
                case AnimationLoopType::NONE:
                    currentTimePos_ = 1.f;
                    break;
                case AnimationLoopType::NORMAL_LOOP:
                    currentTimePos_ = 0.f;
                    break;
                case AnimationLoopType::BACK_AND_FORTH:
                    currentTimePos_ = 1.f;
                    isStepIncreasing_ = false;
                    break;
            }
        }
    }
    else
    {
        currentTimePos_ -= stepSize;
        if(currentTimePos_ <= 0.f)
        {
            // reached the beginning of the animation, so start moving in the forward direction again.
            currentTimePos_ = 0.f;
            // We can only be in the !isStepIncreasing_ state if AnimationLoopType == BACK_AND_FORTH
            isStepIncreasing_ = true;
        }
    }

    const float animPos = calculateAnimationPos(getDistanceTimeFunctionType(), currentTimePos_);
    apply(animPos);

    // call the animationFinished callback if set and applicable
    if(animationFinishedCb_ && !suppressFinishedCallback && isFinished())
    {
        animationFinishedCb_(animationFinishedCallbackContext_);
    }
}

void AbstractAnimation::skipToEnd()
{
    if(isFinished())
    {
        return;
    }
    // the step function will autocorrect to 1.f total
    // when we call the skipToEnd() function, we don't want to fire the animation finished callback
    step(1.f, true);
}

bool AbstractAnimation::isFinished() const
{
    return (loopType_ == AnimationLoopType::NONE) && (currentTimePos_ >= 1.f);
}

AnimationLoopType AbstractAnimation::getLoopType() const
{
    return loopType_;
}

void AbstractAnimation::setLoopType(AnimationLoopType loopType)
{
    loopType_ = loopType;
}

void AbstractAnimation::setAnimationFinishedCallback(void* context, void (*animationFinishedCb)(void*))
{
    animationFinishedCb_ = animationFinishedCb;
    animationFinishedCallbackContext_ = context;
}