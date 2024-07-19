#include "animations/AnimationManager.h"
#include "animations/IAnimation.h"

#include <algorithm>

static float normalizeTimeStep(IAnimation* anim, uint32_t elapsedTimeInMs)
{
    return static_cast<float>(elapsedTimeInMs) / anim->getDurationInMs();
}

void AnimationManager::add(IAnimation* animation)
{
    animations_.push_back(animation);
}

void AnimationManager::remove(IAnimation* animation)
{
    auto it = std::find(animations_.begin(), animations_.end(), animation);
    if(it != animations_.end())
    {
        animations_.erase(it);
    }
}

void AnimationManager::step(uint32_t elapsedTimeInMs)
{
    float animationStep;

    for(IAnimationList::iterator it = animations_.begin(); it != animations_.end(); ++it)
    {
        if(!(*it)->isFinished() && (*it)->getDurationInMs() > 0)
        {
            animationStep = normalizeTimeStep((*it), elapsedTimeInMs);
            (*it)->step(animationStep);
        }
    }
}