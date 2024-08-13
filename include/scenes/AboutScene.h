#ifndef _ABOUTSCENE_H
#define _ABOUTSCENE_H

#include "scenes/AbstractUIScene.h"
#include "widget/ScrollWidget.h"

class AboutScene : public AbstractUIScene
{
public:
    AboutScene(SceneDependencies& deps, void* context);
    virtual ~AboutScene();

    void init() override;
    void destroy() override;

    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;
protected:
private:
    ScrollWidget scrollWidget_;
};

#endif