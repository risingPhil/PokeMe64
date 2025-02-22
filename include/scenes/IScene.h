#ifndef _ISCENE_H
#define _ISCENE_H

#include <libdragon.h>

class RDPQGraphics;
class SceneManager;
class AnimationManager;
class FontManager;
class TransferPakManager;

typedef struct Rectangle Rectangle;

enum class SceneType
{
    NONE,
    INIT_TRANSFERPAK,
    MENU,
    DISTRIBUTION_POKEMON_LIST,
    STATS,
    TEST,
    POKETRANSPORTER_GB_REF,
    SELECT_FILE,
    COPY_DATA,
    ABOUT
};

typedef struct SceneDependencies
{
    RDPQGraphics& gfx;
    AnimationManager& animationManager;
    FontManager& fontManager;
    TransferPakManager& tpakManager;
    SceneManager& sceneManager;
    char playerName[16];
    uint8_t generation;
    uint8_t specificGenVersion;
    uint8_t localization;
} SceneDependencies;

class IScene
{
public:
    virtual ~IScene();

    virtual void init() = 0;
    virtual void destroy() = 0;

    /**
     * @brief This function should implement the procedure of obtaining the relevant user input
     * and directing it as necessary, but it should not implement how to handle it. For the latter you should implement handleUserInput instead.
     */
    virtual void processUserInput() = 0;

    virtual bool handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs) = 0;

    virtual void render(RDPQGraphics& gfx, const Rectangle& sceneBounds) = 0;
protected:
private:
};

#endif