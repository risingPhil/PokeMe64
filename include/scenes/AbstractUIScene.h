#ifndef _ABSTRACTUISCENE_H
#define _ABSTRACTUISCENE_H

#include "scenes/IScene.h"
#include <cstdint>

class IWidget;

/**
 * The WidgetFocusChain is an important event handling mechanism in AbstractUIScene based scenes.
 * 
 * If the current widget does not handle a navigation key input, it will determine to which widget the focus should be shifted if any
 * 
 * It's so integral into the AbstractUIScene based scenes, that there's no input handling without setting one. So don't forget to set one!
 */
typedef struct WidgetFocusChainSegment
{
    /**
     * @brief widget that is supposed to be focused when this segment is active
     */
    IWidget* current;
    /**
     * @brief WidgetFocusChainSegment that is supposed to become active when the user navigates to the left with analog stick or dpad
     */
    WidgetFocusChainSegment* onLeft;
    /**
     * @brief WidgetFocusChainSegment that is supposed to become active when the user navigates to the right with analog stick or dpad
     */
    WidgetFocusChainSegment* onRight;
    /**
     * @brief WidgetFocusChainSegment that is supposed to become active when the user navigates upwards with analog stick or dpad
     */
    WidgetFocusChainSegment* onUp;

    /**
     * @brief WidgetFocusChainSegment that is supposed to become active when the user navigates downwards with analog stick or dpad
     */
    WidgetFocusChainSegment* onDown;
} WidgetFocusChainSegment;

/**
 * @brief This abstract IScene implementation implements some common functionality for UI scenes.
 * 
 * The common functionality here is:
 * - user input handling and forwarding.
 * - focus handling -> if the focused widget hasn't handled a directional input on analog stick/dpad, AbstractUIScene will check the focusChain if 
 *  the focus needs to be switched to a different widget.
 */
class AbstractUIScene : public IScene
{
public:
    static const uint16_t MINIMUM_TIME_BETWEEN_INPUT_EVENTS;

    AbstractUIScene(SceneDependencies& deps);
    virtual ~AbstractUIScene();

    /**
     * @brief This function reads the user input and forwards it to the focused widget (as determined by the focusChain)
     * if the focused widget doesn't handle any pressed navigational keys, AbstractUIScene will check the current WidgetFocusChainSegment
     * to see if a different widget can be selected for the given direction.
     */
    void processUserInput() override;
    bool handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs) override;

    void destroy() override;

    /**
     * This function sets a focus chain onto the scene.
     * 
     * The focus chain is an important part of the event handling chain in the AbstractUIScene based screens
     * 
     * The initial focus is set by this and events are basically being directed according to the focusChain.
     * Without a focusChain, there's no event handling. At least for AbstractUIScene based Scenes
     * 
     * So don't forget to set one!
     */
    void setFocusChain(WidgetFocusChainSegment* focusChain);
protected:
    SceneDependencies& deps_;
private:
    WidgetFocusChainSegment* focusChain_;
    uint64_t lastInputHandleTime_;
};

#endif