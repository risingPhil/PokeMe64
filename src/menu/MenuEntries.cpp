#include "menu/MenuEntries.h"
#include "menu/MenuFunctions.h"

MenuItemData gen1MenuEntries[] = {
    {
        .title = "Event Pokémon",
        .onConfirmAction = goToGen1DistributionPokemonMenu
    },
    {
        .title = "Teach Pikachu Surf",
        .onConfirmAction = gen1PrepareToTeachPikachu,
        .itemParam = &MOVE_SURF
    },
    {
        .title = "Teach Pikachu Fly",
        .onConfirmAction = gen1PrepareToTeachPikachu,
        .itemParam = &MOVE_FLY
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
    },
    {
        .title = "Unlock Pikachu Bed",
        .onConfirmAction = gen2SetEventFlag,
        .itemParam = &GEN2_EVENTFLAG_DECORATION_PIKACHU_BED
    },
    {
        .title = "Unlock Tentacool Doll",
        .onConfirmAction = gen2SetEventFlag,
        .itemParam = &GEN2_EVENTFLAG_DECORATION_TENTACOOL_DOLL
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
        .title = "Unlock GS Ball",
        .onConfirmAction = gen2ReceiveGSBall
    },
    {
        .title = "Unlock Pikachu Bed",
        .onConfirmAction = gen2SetEventFlag,
        .itemParam = &GEN2_EVENTFLAG_DECORATION_PIKACHU_BED
    },
    {
        .title = "Unlock Tentacool Doll",
        .onConfirmAction = gen2SetEventFlag,
        .itemParam = &GEN2_EVENTFLAG_DECORATION_TENTACOOL_DOLL
    }
};

const uint32_t gen2CrystalMenuEntriesSize = sizeof(gen2CrystalMenuEntries);