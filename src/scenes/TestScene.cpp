#include "scenes/TestScene.h"
#include "core/RDPQGraphics.h"
#include "core/FontManager.h"
#include "widget/ImageWidget.h"
#include "widget/TextWidget.h"

#include <n64sys.h>

static const char* tvtypeToString(tv_type_t type)
{
    switch(type)
    {
        case TV_PAL:
            return "PAL";
        case TV_NTSC:
            return "NTSC";
        case TV_MPAL:
            return "MPAL";
        default:
            return "INVALID";
    }
}

TestScene::TestScene(SceneDependencies& deps, void*)
    : AbstractUIScene(deps)
    , scrollWidget_(deps.animationManager)
    , scrollWidgetFocusSegment_({
        .current = &scrollWidget_
    })
    , widgets_()
    , pokeballSprite_(nullptr)
    , oakSprite_(nullptr)
{
}

TestScene::~TestScene()
{
}

void TestScene::init()
{
    uint8_t widgetType;
    const uint8_t fontId = deps_.fontManager.getFont("rom://Arial.font64");
    pokeballSprite_ = sprite_load("rom:/pokeball.sprite");
    oakSprite_ = sprite_load("rom://oak.sprite");

    debugf("Hello Phil! Your tv type is: %s\r\n", tvtypeToString(get_tv_type()));

    const ScrollWidgetStyle scrollStyle = {
        .scrollStep = 15,
        .marginRight = 50,
        .marginBottom = 50
    };

    scrollWidget_.setBounds(Rectangle{0, 0, 320, 240});
    scrollWidget_.setStyle(scrollStyle);
    scrollWidget_.setFocused(true);
    setFocusChain(&scrollWidgetFocusSegment_);

    const Dimensions textDimensions = {.width = 50, .height = 16};
    const Dimensions oakDimensions = {.width = oakSprite_->width, .height = oakSprite_->height };
    const Dimensions pokeballDimensions = {.width = pokeballSprite_->width, .height = pokeballSprite_->height};

    TextWidgetStyle type1Style = {
        .size = {textDimensions.width, textDimensions.height},
        .renderSettingsNotFocused = {
            .fontId = fontId
        }
    };

    ImageWidgetStyle type2Style = {
        .size = oakDimensions,
        .image = {
            .sprite = oakSprite_,
            .spriteBounds = {0, 0, oakDimensions.width, oakDimensions.height}
        }
    };

    ImageWidgetStyle type3Style = {
        .size = pokeballDimensions,
        .image = {
            .sprite = pokeballSprite_,
            .spriteBounds = {0, 0, pokeballDimensions.width, pokeballDimensions.height}
        }
    };

    int curXPos = 0;
    int curYPos = 0;
    int nextYPos = 0;
    for(uint8_t i=0; i < 6; ++i)
    {
        for(uint8_t j=0; j < 6; ++j)
        {
            widgetType = (j % 3);
            
            switch(widgetType)
            {
                case 0:
                {
                    TextWidget* textWidget = new TextWidget();
                    textWidget->setStyle(type1Style);
                    textWidget->setBounds(Rectangle{curXPos, curYPos, textDimensions.width, textDimensions.height});
                    textWidget->setData("Hello!");
                    scrollWidget_.addWidget(textWidget);
                    widgets_.push_back(textWidget);
                    curXPos += textDimensions.width;
                    if(curYPos + textDimensions.height > nextYPos)
                    {
                        nextYPos = curYPos + textDimensions.height;
                    }
                    break;
                }
                case 1:
                {
                    ImageWidget* imageWidget = new ImageWidget();
                    imageWidget->setStyle(type2Style);
                    imageWidget->setBounds(Rectangle{curXPos, curYPos, oakDimensions.width, oakDimensions.height});
                    scrollWidget_.addWidget(imageWidget);
                    widgets_.push_back(imageWidget);
                    curXPos += oakDimensions.width;
                    if(curYPos + oakDimensions.height > nextYPos)
                    {
                        nextYPos = curYPos + oakDimensions.height;
                    }
                    break;
                }
                case 2:
                {
                    ImageWidget* imageWidget = new ImageWidget();
                    imageWidget->setStyle(type3Style);
                    imageWidget->setBounds(Rectangle{curXPos, curYPos, pokeballDimensions.width, pokeballDimensions.height});
                    scrollWidget_.addWidget(imageWidget);
                    widgets_.push_back(imageWidget);
                    curXPos += pokeballDimensions.width;
                    if(curYPos + pokeballDimensions.height > nextYPos)
                    {
                        nextYPos = curYPos + pokeballDimensions.height;
                    }
                    break;
                }
            default:
                break;
            }
        }
        curYPos = nextYPos;
        curXPos = 0;
    }
}

void TestScene::destroy()
{
    scrollWidget_.clearWidgets();
    for(IWidget* widget : widgets_)
    {
        delete widget;
    }
    widgets_.clear();

    sprite_free(oakSprite_);
    oakSprite_ = nullptr;

    sprite_free(pokeballSprite_);
    pokeballSprite_ = nullptr;
}

void TestScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    scrollWidget_.render(gfx, sceneBounds);
}