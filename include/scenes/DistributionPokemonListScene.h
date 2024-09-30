#ifndef _DISTRIBUTIONPOKEMONLISTSCENE_H
#define _DISTRIBUTIONPOKEMONLISTSCENE_H

#include "scenes/MenuScene.h"
#include "widget/DistributionPokemonMenuItemWidget.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"
#include "gen1/Gen1GameReader.h"
#include "gen2/Gen2GameReader.h"

/**
 * @brief The Distribution Pokémon list we want to show
 */
enum class DistributionPokemonListType
{
    INVALID,
    /**
     * @brief Main Gen 1 Distribution events
     */
    GEN1,
    /**
     * @brief Main Gen 2 Distribution events
     */
    GEN2,
    /**
     * @brief Gen 2 Pokémon Center New York distribution pokémon
     * 
     */
    GEN2_POKEMON_CENTER_NEW_YORK
};

struct DistributionPokemonListSceneContext : public MenuSceneContext
{
    DistributionPokemonListType listType;
};

typedef DistributionPokemonListSceneContext DistributionPokemonListSceneContext;

/**
 * @brief This scene implementation gives you a list of pokémon to choose and allow you to inject them into your cartridge save by selecting one
 * and pressing the A-button
 */
class DistributionPokemonListScene : public MenuScene
{
public:
    DistributionPokemonListScene(SceneDependencies& deps, void* context);
    virtual ~DistributionPokemonListScene();

    void init() override;
    void destroy() override;

    bool handleUserInput(joypad_port_t port, const joypad_inputs_t& inputs) override;

    /**
     * This function will start the pokémon injection. and show a non-skippable "saving" dialog
     * The actual injection will be done on the next handleUserInput() call. 
     * That will ensure that at least 1 render() call has been handled before we start doing the work
     */
    void triggerPokemonInjection(const void* data);

    /**
     * @brief The core functionality of this class: it will inject the selected pokémon into your cartridge save.
     * 
     * @param data a pointer to the Gen1DistributionPokemon or Gen2DistributionPokemon instance you want to inject.
     */
    void injectPokemon(const void* data);

    void onDialogDone() override;
protected:
    void setupMenu() override;
private:
    void loadDistributionPokemonList();

    TransferPakRomReader romReader_;
    TransferPakSaveManager saveManager_;
    Gen1GameReader gen1Reader_;
    Gen2GameReader gen2Reader_;
    PokemonPartyIconFactory iconFactory_;
    ListItemFiller<VerticalList, DistributionPokemonMenuItemData, DistributionPokemonMenuItem, DistributionPokemonMenuItemStyle> customListFiller_;
    DialogData diag_;
    sprite_t* iconBackgroundSprite_;
    const void* pokeToInject_;
};

void deleteDistributionPokemonListSceneContext(void* context);

#endif