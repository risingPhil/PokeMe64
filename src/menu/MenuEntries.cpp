#include "menu/MenuEntries.h"
#include "menu/MenuFunctions.h"

MenuItemData gen1MenuEntries[] = {
    {
        .title = "Event Pokémon",
        .onConfirmAction = goToGen1DistributionPokemonMenu
    }
};

const uint32_t gen1MenuEntriesSize = sizeof(gen1MenuEntries);

MenuItemData gen2MenuEntries[] = {
    {
        .title = "Event Pokémon",
        .onConfirmAction = goToGen2DistributionPokemonMenu
    },
    {
        .title = "PCNY Pokémon",
        .onConfirmAction = goToGen2PCNYDistributionPokemonMenu,
    }
};

const uint32_t gen2MenuEntriesSize = sizeof(gen2MenuEntries);

MenuItemData gen2CrystalMenuEntries[] = {
    {
        .title = "Event Pokémon",
        .onConfirmAction = goToGen2DistributionPokemonMenu
    },
    {
        .title = "PCNY Pokémon",
        .onConfirmAction = goToGen2PCNYDistributionPokemonMenu,
    },
    {
        .title = "Receive GS Ball",
        .onConfirmAction = gen2ReceiveGSBall
    }
};

const uint32_t gen2CrystalMenuEntriesSize = sizeof(gen2CrystalMenuEntries);