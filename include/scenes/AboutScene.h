#ifndef _ABOUTSCENE_H
#define _ABOUTSCENE_H

#include "scenes/AbstractUIScene.h"
#include "widget/ScrollWidget.h"
#include "widget/ImageWidget.h"
#include "widget/TextWidget.h"

/**
 * @brief This scene shows the version number and credits of the PokeMe64 project.
 */
class AboutScene : public AbstractUIScene
{
public:
    AboutScene(SceneDependencies& deps, void* context);
    virtual ~AboutScene();

    void init() override;
    void destroy() override;

    bool handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs) override;
    void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) override;
protected:
private:
    uint8_t fontIdArial_;
    uint8_t fontArialStyleWhiteId_;
    sprite_t* logoLibDragon_;
    sprite_t* logoBulbagarden_;
    sprite_t* logoRetroGameMechanicsExplained_;
    sprite_t* logoDataCrystal_;
    sprite_t* logoPKHEX_;
    sprite_t* logoGBDevIO_;
    sprite_t* logoNESDevWiki_;
    sprite_t* logoPokeTransporterGB_;
    ScrollWidget scrollWidget_;
    TextWidget headerText_;
    ImageWidget imgDragonWidget_;
    ImageWidget imgBulbagardenWidget_;
    ImageWidget imgRetroGameMechanicsExplained_;
    ImageWidget imgDataCrystal_;
    TextWidget dataCrystalText_;
    ImageWidget imgPKHEX_;
    TextWidget PKHEXText_;
    TextWidget pretPokeCrystalText_;
    ImageWidget imgGBDevIO_;
    TextWidget gbDevIOText_;
    ImageWidget imgNESDevWiki_;
    ImageWidget imgPokeTransporterGB_;
    TextWidget otherCreditsText_;
    WidgetFocusChainSegment scrollFocusSegment_;
    bool bButtonPressed_;
};

#endif