#ifndef _MOVEANIMATION_H
#define _MOVEANIMATION_H

#include "animations/IAnimation.h"
#include "core/common.h"

class IWidget;

/**
 * This animation manipulates the bounds of a Widget over time
 */
class MoveAnimation : public AbstractAnimation
{
public:
    MoveAnimation(IWidget* target);
    virtual ~MoveAnimation();

    AnimationDistanceTimeFunctionType getDistanceTimeFunctionType() const override;
    uint32_t getDurationInMs() const override;

    void start(const Rectangle& startBounds, const Rectangle& moveVectorStartEnd, uint32_t durationInMs);
    
    /**     * 
     * reset the animation back to its start position
     */
    void reset();
protected:
private:
    void apply(float pos) override;

    IWidget* target_;
    Rectangle startBounds_;
    Rectangle diffStartEnd_;
    uint32_t durationInMs_;
};

#endif