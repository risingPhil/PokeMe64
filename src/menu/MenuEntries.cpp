#include "menu/MenuEntries.h"
#include "menu/MenuFunctions.h"

MenuItemData gen1MenuEntries[] = {
    {
        .title = "Backup/Restore",
        .onConfirmAction = goToBackupRestoreMenu
    },
    {
        .title = "Event Pokémon",
        .onConfirmAction = goToGen1DistributionPokemonMenu
    },
    {
        .title = "Pokémon Moves",
        .onConfirmAction = goToGen1MovesMenu
    },
    {
        .title = "Gen 3 Transfer Info",
        .onConfirmAction = goToPokeTransporterGBRef
    },
    {
        .title = "About",
        .onConfirmAction = goToAboutScene
    }
};

const uint32_t gen1MenuEntriesSize = sizeof(gen1MenuEntries);

MenuItemData gen1MovesMenuEntries[] = {
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
        .title = "Delete Move",
        .onConfirmAction = gen1MoveDeleterSelectPokemon
    }
};

const uint32_t gen1MovesMenuEntriesSize = sizeof(gen1MovesMenuEntries);

MenuItemData gen2MenuEntries[] = {
    {
        .title = "Backup/Restore",
        .onConfirmAction = goToBackupRestoreMenu
    },
    {
        .title = "Event Pokémon",
        .onConfirmAction = goToGen2DistributionPokemonMenu
    },
    {
        .title = "PCNY Pokémon",
        .onConfirmAction = goToGen2PCNYDistributionPokemonMenu
    },
    {
        .title = "Mystery Gift",
        .onConfirmAction = goToGen2MysteryGiftMenu
    },
    {
        .title = "Gen 3 Transfer Info",
        .onConfirmAction = goToPokeTransporterGBRef
    },
    {
        .title = "Reset Game Clock",
        .onConfirmAction = resetRTC
    },
#ifdef PRINT_CLOCK_FUNCTION_ENABLED
    {
        .title = "Print Game Clock",
        .onConfirmAction = printClock
    },
#endif
    {
        .title = "About",
        .onConfirmAction = goToAboutScene
    }
};

const uint32_t gen2MenuEntriesSize = sizeof(gen2MenuEntries);

MenuItemData gen2CrystalMenuEntries[] = {
    {
        .title = "Backup/Restore",
        .onConfirmAction = goToBackupRestoreMenu
    },
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
        .title = "Mystery Gift",
        .onConfirmAction = goToGen2MysteryGiftMenu
    },
    {
        .title = "Gen 3 Transfer Info",
        .onConfirmAction = goToPokeTransporterGBRef
    },
    {
        .title = "Reset Game Clock",
        .onConfirmAction = resetRTC
    },
#ifdef PRINT_CLOCK_FUNCTION_ENABLED
    {
        .title = "Print Game Clock",
        .onConfirmAction = printClock
    },
#endif
    {
        .title = "About",
        .onConfirmAction = goToAboutScene
    }
};

const uint32_t gen2CrystalMenuEntriesSize = sizeof(gen2CrystalMenuEntries);

MenuItemData gen2MysteryGiftMenuEntries[] = {
    {
        .title = "Get Mystery Gift",
        .onConfirmAction = gen2ReceiveMysteryGift
    },
    {
        .title = "Get Pikachu Bed",
        .onConfirmAction = gen2SetEventFlag,
        .itemParam = &GEN2_EVENTFLAG_DECORATION_PIKACHU_BED
    },
    {
        .title = "Get Unown Doll",
        .onConfirmAction = gen2SetEventFlag,
        .itemParam = &GEN2_EVENTFLAG_DECORATION_UNOWN_DOLL
    },
    {
        .title = "Get Tentacool Doll",
        .onConfirmAction = gen2SetEventFlag,
        .itemParam = &GEN2_EVENTFLAG_DECORATION_TENTACOOL_DOLL
    }
};

const uint32_t gen2MysteryGiftMenuEntriesSize = sizeof(gen2MysteryGiftMenuEntries);

MenuItemData backupRestoreMenuEntries[] = {
    {
        .title = "Backup Save",
        .onConfirmAction = goToDataCopyScene,
        .itemParam = &DATACOPY_BACKUP_SAVE
    },
    {
        .title = "Backup ROM",
        .onConfirmAction = goToDataCopyScene,
        .itemParam = &DATACOPY_BACKUP_ROM
    },
    {
        .title = "Restore Save",
        .onConfirmAction = goToDataCopyScene,
        .itemParam = &DATACOPY_RESTORE_SAVE
    },
    {
        .title = "Wipe Save",
        .onConfirmAction = askConfirmationWipeSave
    }
};

const uint32_t backupRestoreMenuEntriesSize = sizeof(backupRestoreMenuEntries);