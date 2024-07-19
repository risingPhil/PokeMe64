#ifndef _IANIMATION_H
#define _IANIMATION_H

#include <cstdint>

/**
 * A Distance time function indicates at a certain normalized time point in the [0.0, 1.0] range in the animation,
 * how much of the total movement the animation should have done (also normalized in the [0.0, 1.0])
 * 
 * This gives you a way to control the acceleration/deceleration of the animation.
 */
enum class AnimationDistanceTimeFunctionType
{
    /**
     * No distance time function means no animation -> The animation will immediately be move to the end position (1.0f)
     */
    NONE,

    /**
     * Linear distance time function means that the animation always moves at the same speed
     */
    LINEAR,

    /**
     * The Ease-in Ease-out distance time function slowly accelerates the animation at the start and slowly decelerates the animation at the end
     */
    EASE_IN_EASE_OUT
};

/**
 * @brief This enum defines what loop type the animation has (if any)
 */
enum class AnimationLoopType
{
    /**
     * No loop
     */
    NONE,

    /**
     * After the end of the animation is reached, it restarts from the beginning
     */
    NORMAL_LOOP,

    /**
     * After the end of the animation is reached, it will start going backwards until the beginning is reached again. And then it will go forward again.
     * This cycle repeats until someone/something stops the animation.
     */
    BACK_AND_FORTH
};

class IAnimation
{
public:
    virtual ~IAnimation();

    virtual AnimationDistanceTimeFunctionType getDistanceTimeFunctionType() const = 0;

    /**
     * Applies the specified step. This should be a stepSize in the [0.f - 1.f] range
     */
    virtual void step(float stepSize, bool suppressFinishedCallback = false) = 0;

    /**
     * @brief returns the duration of the animation in milliseconds
     */
    virtual uint32_t getDurationInMs() const = 0;

    /**
     * Skips to the end of the animation
     */
    virtual void skipToEnd() = 0;

    /** Indicates whether this animation is finished and should therefore be removed from AnimationManager */
    virtual bool isFinished() const = 0;

    /**
     * Returns the current loop type (if any)
     */
    virtual AnimationLoopType getLoopType() const = 0;

    /**
     * Sets the loop type (if any)
     */
    virtual void setLoopType(AnimationLoopType loopType) = 0;

    /**
     * @brief Sets a callback that should be called when the animation has finished.
     * Note: this only applies to scenarios where AnimationLoopType == NONE
     */
    virtual void setAnimationFinishedCallback(void* context, void (*animationFinishedCb)(void*)) = 0;
protected:
    /**
     * @brief This function applies the relative pos [0.f-1.f] to the 
     * specific animation implementation.
     * 
     * This pos indicates the point in the animation between the start and the end, not in function of time
     * but in function of distance (the actual point that needs to be applied)
     */
    virtual void apply(float pos) = 0;
private:
};

/**
 * @brief Abstract implementation of common functionality defined in IAnimation
 * 
 */
class AbstractAnimation : public IAnimation
{
public:
    AbstractAnimation(float initialTimePos = 0);
    virtual ~AbstractAnimation();

    void step(float stepSize, bool suppressFinishedCallback = false) override;

    void skipToEnd() override;
    bool isFinished() const override;

    /**
     * Returns the current loop type (if any)
     */
    AnimationLoopType getLoopType() const override;

    /**
     * Sets the loop type (if any)
     */
    void setLoopType(AnimationLoopType loopType) override;

    /**
     * @brief Sets a callback that should be called when the animation has finished.
     * Note: this only applies to scenarios where AnimationLoopType == NONE
     */
    void setAnimationFinishedCallback(void* context, void (*animationFinishedCb)(void*));

protected:
    float currentTimePos_;
private:
    void (*animationFinishedCb_)(void*);
    void *animationFinishedCallbackContext_;
    AnimationLoopType loopType_;
    bool isStepIncreasing_;
};

#endif