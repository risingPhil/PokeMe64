#include "scenes/AbstractUIScene.h"
#include "core/DragonUtils.h"
#include "widget/IWidget.h"

const uint16_t AbstractUIScene::MINIMUM_TIME_BETWEEN_INPUT_EVENTS = 150;

AbstractUIScene::AbstractUIScene(SceneDependencies& deps)
    : deps_(deps)
    , focusChain_()
    , lastInputHandleTime_(0)
{
}

AbstractUIScene::~AbstractUIScene()
{
}

void AbstractUIScene::processUserInput()
{
    if(!focusChain_)
    {
        return;
    }

    const uint64_t now = get_ticks();
    if(TICKS_TO_MS(now - lastInputHandleTime_) < MINIMUM_TIME_BETWEEN_INPUT_EVENTS)
    {
        // not enough time has passed since last handled input event. Ignore
        return;
    }

    const joypad_inputs_t inputs = joypad_get_inputs(JOYPAD_PORT_1);
    if(handleUserInput(JOYPAD_PORT_1, inputs))
    {
        lastInputHandleTime_ = now;
    }
}

bool AbstractUIScene::handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs)
{
    bool ret = focusChain_->current->handleUserInput(inputs);

    if(!ret)
    {
        // the widget did not handle the userInput. If we're dealing with a navigation key, we may want to switch focus
        WidgetFocusChainSegment* nextChainEntry;
        const UINavigationDirection navDirection = determineUINavigationDirection(inputs, NavigationInputSourceType::BOTH);

        switch(navDirection)
        {
            case UINavigationDirection::UP:
                nextChainEntry = focusChain_->onUp;
                break;
            case UINavigationDirection::DOWN:
                nextChainEntry = focusChain_->onDown;
                break;
            case UINavigationDirection::LEFT:
                nextChainEntry = focusChain_->onLeft;
                break;
            case UINavigationDirection::RIGHT:
                nextChainEntry = focusChain_->onRight;
                break;
            case UINavigationDirection::MAX:
            default:
                nextChainEntry = nullptr;
                break;
        }

        if(nextChainEntry)
        {
            focusChain_->current->setFocused(false);
            focusChain_ = nextChainEntry;
            focusChain_->current->setFocused(true);
            ret = true;
        }
    }
    return ret;
}

void AbstractUIScene::destroy()
{
    setFocusChain(nullptr);
}

void AbstractUIScene::setFocusChain(WidgetFocusChainSegment* focusChain)
{
    if(focusChain_)
    {
        focusChain_->current->setFocused(false);
    }
    focusChain_ = focusChain;

    if(focusChain_)
    {
        focusChain_->current->setFocused(true);
    }
}

SceneDependencies& AbstractUIScene::getDependencies()
{
    return deps_;
}