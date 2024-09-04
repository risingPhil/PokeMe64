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
    },
    {
        .title = "Gen 3 Transfer Info",
        .onConfirmAction = goToPokeTransporterGBRef
    },
    {
        .title = "Backup Save",
        .onConfirmAction = backupCartridgeSRAMToSDCard
    },
    {
        .title = "About",
        .onConfirmAction = goToAboutScene
    },
    {
        .title = "Progress scene",
        .onConfirmAction = goToProgressScene
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
        .onConfirmAction = goToGen2PCNYDistributionPokemonMenu
    },
    {
        .title = "Unlock Decoration",
        .onConfirmAction = goToGen2DecorationMenu
    },
    {
        .title = "Gen 3 Transfer Info",
        .onConfirmAction = goToPokeTransporterGBRef
    },
    {
        .title = "Backup Save",
        .onConfirmAction = backupCartridgeSRAMToSDCard
    },
    {
        .title = "About",
        .onConfirmAction = goToAboutScene
    },
    {
        .title = "Progress scene",
        .onConfirmAction = goToProgressScene
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
        .title = "Unlock Decoration",
        .onConfirmAction = goToGen2DecorationMenu
    },
    {
        .title = "Gen 3 Transfer Info",
        .onConfirmAction = goToPokeTransporterGBRef
    },
    {
        .title = "Backup Save",
        .onConfirmAction = backupCartridgeSRAMToSDCard
    },
    {
        .title = "About",
        .onConfirmAction = goToAboutScene
    },
    {
        .title = "Progress scene",
        .onConfirmAction = goToProgressScene
    }
};

const uint32_t gen2CrystalMenuEntriesSize = sizeof(gen2CrystalMenuEntries);

MenuItemData gen2DecorationMenuEntries[] = {
    {
        .title = "Pikachu Bed",
        .onConfirmAction = gen2SetEventFlag,
        .itemParam = &GEN2_EVENTFLAG_DECORATION_PIKACHU_BED
    },
    {
        .title = "Unown Doll",
        .onConfirmAction = gen2SetEventFlag,
        .itemParam = &GEN2_EVENTFLAG_DECORATION_UNOWN_DOLL
    },
    {
        .title = "Tentacool Doll",
        .onConfirmAction = gen2SetEventFlag,
        .itemParam = &GEN2_EVENTFLAG_DECORATION_TENTACOOL_DOLL
    }
};

const uint32_t gen2DecorationMenuEntriesSize = sizeof(gen2DecorationMenuEntries);
