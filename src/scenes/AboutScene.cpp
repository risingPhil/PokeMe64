#include "scenes/AboutScene.h"
#include "core/FontManager.h"
#include "scenes/SceneManager.h"

static const ScrollWidgetStyle scrollWidgetStyle = {
    .scrollStep = 15,
    .marginRight = 0,
    .marginBottom = 0
};

static const Rectangle scrollWidgetBounds = {0, 0, 320, 240};
static const Rectangle headerTextBounds = {10, 10, 300, 70};
static const Rectangle imgDragonBounds = {5, 80, 133, 78};
static const Rectangle imgBulbagardenBounds = {163, 121, 134, 17};
static const Rectangle imgRetroGameMechanicsExplainedBounds = {5, 173, 133, 39};
static const Rectangle imgDataCrystalBounds = {153, 163, 60, 60};
static const Rectangle dataCrystalTextBounds = {215, 185, 100, 16};
static const Rectangle imgPKHEXBounds = {25, 233, 38, 37};
static const Rectangle PKHEXTextBounds = {68, 248, 50, 16};
static const Rectangle pretPokeCrystalTextBounds = {153, 248, 175, 16};
static const Rectangle imgGBDevIoBounds = {28, 280, 34, 55};
static const Rectangle gbDevIOTextBounds = {68, 300, 75, 16};
static const Rectangle imgNESDevWikiBounds = {163, 280, 96, 60};
static const Rectangle otherCreditsTextBounds = {5, 350, 310, 80};

static const char* otherCreditsString = R"delim(github.com/magical/pokemon-sprites-rby
glitchcity.wiki
github.com/seanmorris/pokemon-parser
github.com/LinusU/pokemon-sprite-compression
github.com/xvillaneau/poke-sprite-python
)delim";

static const char* headerTextString = R"delim(PokeMe64 Version 0.1
by risingPhil

SPECIAL THANKS TO:
(For docs, tools and/or inspiration)
)delim";

AboutScene::AboutScene(SceneDependencies& deps, void*)
    : AbstractUIScene(deps)
    , fontIdArial_(1)
    , fontArialStyleWhiteId_(0)
    , logoLibDragon_(nullptr)
    , logoBulbagarden_(nullptr)
    , logoRetroGameMechanicsExplained_(nullptr)
    , logoDataCrystal_(nullptr)
    , logoPKHEX_(nullptr)
    , logoGBDevIO_(nullptr)
    , logoNESDevWiki_(nullptr)
    , scrollWidget_(deps.animationManager)
    , headerText_()
    , imgDragonWidget_()
    , imgBulbagardenWidget_()
    , imgRetroGameMechanicsExplained_()
    , imgDataCrystal_()
    , dataCrystalText_()
    , imgPKHEX_()
    , PKHEXText_()
    , pretPokeCrystalText_()
    , imgGBDevIO_()
    , gbDevIOText_()
    , imgNESDevWiki_()
    , otherCreditsText_()
    , scrollFocusSegment_(WidgetFocusChainSegment{
        .current = &scrollWidget_
    })
    , bButtonPressed_(false)
{
    fontIdArial_ = deps.fontManager.getFont("rom://Arial.font64");

    const rdpq_fontstyle_t arialWhite = {
        .color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF),
        .outline_color = RGBA32(0, 0, 0, 0xFF)
    };

    deps.fontManager.registerFontStyle(fontIdArial_, fontArialStyleWhiteId_, arialWhite);
}

AboutScene::~AboutScene()
{
}

void AboutScene::init()
{
    logoLibDragon_ = sprite_load("rom://logo-libdragon.sprite");
    logoBulbagarden_ = sprite_load("rom://logo-bulbagarden.sprite");
    logoRetroGameMechanicsExplained_ = sprite_load("rom://logo-retrogamemechanicsexplained.sprite");
    logoDataCrystal_ = sprite_load("rom://logo-datacrystal.sprite");
    logoPKHEX_ = sprite_load("rom://logo-pkhex.sprite");
    logoGBDevIO_ = sprite_load("rom://logo-gbdevio.sprite");
    logoNESDevWiki_ = sprite_load("rom://logo-nesdevwiki.sprite");
    scrollWidget_.setBounds(scrollWidgetBounds);
    scrollWidget_.setStyle(scrollWidgetStyle);

    const TextWidgetStyle headerTextStyle = {
        .renderSettingsNotFocused = {
            .fontId = fontIdArial_,
            .fontStyleId = fontArialStyleWhiteId_,
            .halign = ALIGN_CENTER
        }
 //       ,.backgroundColor_ = 0xF801
    };

    const TextWidgetStyle commonLeftAlignedTextStyle = {
        .renderSettingsNotFocused = {
            .fontId = fontIdArial_,
            .fontStyleId = fontArialStyleWhiteId_,
            .halign = ALIGN_LEFT
        }
    };
    const TextWidgetStyle commonCenterAlignedTextStyle = {
        .renderSettingsNotFocused = {
            .fontId = fontIdArial_,
            .fontStyleId = fontArialStyleWhiteId_,
            .halign = ALIGN_CENTER
        }
    };

    headerText_.setBounds(headerTextBounds);
    headerText_.setStyle(headerTextStyle);
    headerText_.setData(headerTextString);
    scrollWidget_.addWidget(&headerText_);

    const ImageWidgetStyle imgDragonStyle = {
        .image = {
            .sprite = logoLibDragon_,
            .spriteBounds = Rectangle{0, 0, imgDragonBounds.width, imgDragonBounds.height}
        }
    };

    imgDragonWidget_.setBounds(imgDragonBounds);
    imgDragonWidget_.setStyle(imgDragonStyle);
    scrollWidget_.addWidget(&imgDragonWidget_);

    const ImageWidgetStyle imgBulbaStyle = {
        .image = {
            .sprite = logoBulbagarden_,
            .spriteBounds = Rectangle{0, 0, imgBulbagardenBounds.width, imgBulbagardenBounds.height}
        }
    };

    imgBulbagardenWidget_.setBounds(imgBulbagardenBounds);
    imgBulbagardenWidget_.setStyle(imgBulbaStyle);
    scrollWidget_.addWidget(&imgBulbagardenWidget_);


    const ImageWidgetStyle imgRetroGameMechanicsExplainedStyle = {
        .image = {
            .sprite = logoRetroGameMechanicsExplained_,
            .spriteBounds = Rectangle{0, 0, imgRetroGameMechanicsExplainedBounds.width, imgRetroGameMechanicsExplainedBounds.height}
        }
    };

    imgRetroGameMechanicsExplained_.setBounds(imgRetroGameMechanicsExplainedBounds);
    imgRetroGameMechanicsExplained_.setStyle(imgRetroGameMechanicsExplainedStyle);
    scrollWidget_.addWidget(&imgRetroGameMechanicsExplained_);

    const ImageWidgetStyle imgDataCrystalStyle = {
        .image = {
            .sprite = logoDataCrystal_,
            .spriteBounds = Rectangle{0, 0, imgDataCrystalBounds.width, imgDataCrystalBounds.height}
        }
    };

    imgDataCrystal_.setBounds(imgDataCrystalBounds);
    imgDataCrystal_.setStyle(imgDataCrystalStyle);
    scrollWidget_.addWidget(&imgDataCrystal_);

    dataCrystalText_.setBounds(dataCrystalTextBounds);
    dataCrystalText_.setStyle(commonLeftAlignedTextStyle);
    dataCrystalText_.setData("Datacrystal");
    scrollWidget_.addWidget(&dataCrystalText_);

    const ImageWidgetStyle imgPKHEXStyle = {
        .image = {
            .sprite = logoPKHEX_,
            .spriteBounds = Rectangle{0, 0, imgPKHEXBounds.width, imgPKHEXBounds.height}
        }
    };

    imgPKHEX_.setBounds(imgPKHEXBounds);
    imgPKHEX_.setStyle(imgPKHEXStyle);
    scrollWidget_.addWidget(&imgPKHEX_);

    PKHEXText_.setBounds(PKHEXTextBounds);
    PKHEXText_.setStyle(commonLeftAlignedTextStyle);
    PKHEXText_.setData("PKHeX");
    scrollWidget_.addWidget(&PKHEXText_);

    pretPokeCrystalText_.setBounds(pretPokeCrystalTextBounds);
    pretPokeCrystalText_.setStyle(commonLeftAlignedTextStyle);
    pretPokeCrystalText_.setData("github.com/pret/pokecrystal");
    scrollWidget_.addWidget(&pretPokeCrystalText_);

    const ImageWidgetStyle imgGBDevIOStyle = {
        .image = {
            .sprite = logoGBDevIO_,
            .spriteBounds = Rectangle{0, 0, imgGBDevIoBounds.width, imgGBDevIoBounds.height}
        }
    };

    imgGBDevIO_.setBounds(imgGBDevIoBounds);
    imgGBDevIO_.setStyle(imgGBDevIOStyle);
    scrollWidget_.addWidget(&imgGBDevIO_);

    gbDevIOText_.setBounds(gbDevIOTextBounds);
    gbDevIOText_.setStyle(commonLeftAlignedTextStyle);
    gbDevIOText_.setData("gbdev.io");
    scrollWidget_.addWidget(&gbDevIOText_);

    const ImageWidgetStyle imgNESDevWikiStyle = {
        .image = {
            .sprite = logoNESDevWiki_,
            .spriteBounds = Rectangle{0, 0, imgNESDevWikiBounds.width, imgNESDevWikiBounds.height}
        }
    };

    imgNESDevWiki_.setBounds(imgNESDevWikiBounds);
    imgNESDevWiki_.setStyle(imgNESDevWikiStyle);
    scrollWidget_.addWidget(&imgNESDevWiki_);

    otherCreditsText_.setBounds(otherCreditsTextBounds);
    otherCreditsText_.setStyle(commonCenterAlignedTextStyle);
    otherCreditsText_.setData(otherCreditsString);
    scrollWidget_.addWidget(&otherCreditsText_);

    setFocusChain(&scrollFocusSegment_);
}

void AboutScene::destroy()
{
    sprite_free(logoLibDragon_);
    logoLibDragon_ = nullptr;
    sprite_free(logoBulbagarden_);
    logoBulbagarden_ = nullptr;
    sprite_free(logoRetroGameMechanicsExplained_);
    logoRetroGameMechanicsExplained_ = nullptr;
    sprite_free(logoDataCrystal_);
    logoDataCrystal_ = nullptr;
    sprite_free(logoPKHEX_);
    logoPKHEX_ = nullptr;
    sprite_free(logoGBDevIO_);
    logoGBDevIO_ = nullptr;
    sprite_free(logoNESDevWiki_);
    logoNESDevWiki_ = nullptr;
}

bool AboutScene::handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs)
{
    if(AbstractUIScene::handleUserInput(port, inputs))
    {
        return true;
    }

    if(inputs.btn.b && !bButtonPressed_)
    {
        bButtonPressed_ = true;
        return true;
    }
    else if(!inputs.btn.b && bButtonPressed_)
    {
        deps_.sceneManager.goBackToPreviousScene();
        return true;
    }
    return false;
}

void AboutScene::render(RDPQGraphics& gfx, const Rectangle& sceneBounds)
{
    scrollWidget_.render(gfx, sceneBounds);
}