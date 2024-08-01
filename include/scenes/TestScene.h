#ifndef _TESTSCENE_H
#define _TESTSCENE_H

#include "scenes/AbstractUIScene.h"
#include "core/Sprite.h"
#include "widget/ScrollWidget.h"

#include <vector>

class ImageWidget;
class TextWidget;

typedef std::vector<IWidget*> WidgetList;


class TestScene : public AbstractUIScene
{
public:
    TestScene(SceneDependencies& deps, void* sceneContext);
    virtual ~TestScene();

    void init() override;
    void destroy() override;

    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;
protected:
private:
    ScrollWidget scrollWidget_;
    WidgetFocusChainSegment scrollWidgetFocusSegment_;
    WidgetList widgets_;
    sprite_t* pokeballSprite_;
    sprite_t* oakSprite_;
};

#endif