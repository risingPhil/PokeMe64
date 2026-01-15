#ifndef _POKESHOPSCENE_H
#define _POKESHOPSCENE_H

#include "scenes/MenuScene.h"
#include "transferpak/TransferPakRomReader.h"
#include "transferpak/TransferPakSaveManager.h"
#include "gen1/Gen1GameReader.h"
#include "gen2/Gen2GameReader.h"
#include "widget/PokeShopMenuItemWidget.h"

/**
 * @brief This scene implementation gives you a list of pokémon you
 * can buy from the PokéShop and inject into your cartridge save by selecting one
 */
class PokeShopScene : public MenuScene
{
public:
    PokeShopScene(SceneDependencies& deps, void* context);
    virtual ~PokeShopScene();

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
    void loadShopList();

    TransferPakRomReader romReader_;
    TransferPakSaveManager saveManager_;
    Gen1GameReader gen1Reader_;
    Gen2GameReader gen2Reader_;
    PokemonPartyIconFactory iconFactory_;
    ListItemFiller<VerticalList, PokeShopMenuItemData, PokeShopMenuItem, PokeShopMenuItemStyle> customListFiller_;
    DialogData diag_;
    sprite_t* iconBackgroundSprite_;
    const void* pokeToInject_;
};

#endif