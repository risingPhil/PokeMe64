#ifndef _ANIMATIONMANAGER_H
#define _ANIMATIONMANAGER_H

#include <vector>
#include <cstdint>

class IAnimation;
typedef std::vector<IAnimation*> IAnimationList;

/**
 * @brief This class keeps track of animations and is used to apply an animation step on all pending animations
 * on every frame
 */
class AnimationManager
{
public:
    void add(IAnimation* animation);
    void remove(IAnimation* animation);

    void step(uint32_t elapsedTimeInMs);
protected:
private:
    IAnimationList animations_;
};

#endif