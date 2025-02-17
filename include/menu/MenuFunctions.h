#ifndef _MENUFUNCTIONS_H
#define _MENUFUNCTIONS_H

#include "scenes/DataCopyScene.h"
#include "Moves.h"

#include <cstdint>

// these are used to pass as a pointer to the gen1PrepareToTeachPikachu function
extern const Move MOVE_SURF;
extern const Move MOVE_FLY;

// these are used to pass as a pointer to the goToDataCopyScene function
extern const DataCopyOperation DATACOPY_BACKUP_SAVE;
extern const DataCopyOperation DATACOPY_BACKUP_ROM;
extern const DataCopyOperation DATACOPY_RESTORE_SAVE;
extern const DataCopyOperation DATACOPY_WIPE_SAVE;

extern const uint16_t GEN2_EVENTFLAG_DECORATION_PIKACHU_BED;
extern const uint16_t GEN2_EVENTFLAG_DECORATION_UNOWN_DOLL;
extern const uint16_t GEN2_EVENTFLAG_DECORATION_TENTACOOL_DOLL;

void activateFrameLog(void* context, const void* param);
void advanceDialog(void* context, const void* param);

// UI navigation functions
void goToTestScene(void* context, const void* param);
void goToPokeTransporterGBRef(void* context, const void* param);
void goToAboutScene(void* context, const void* param);
void goToDataCopyScene(void* context, const void* param);
void goToGen1MovesMenu(void* context, const void* param);
void goToGen1DistributionPokemonMenu(void* context, const void* param);
void goToGen2DistributionPokemonMenu(void* context, const void* param);
void goToGen2PCNYDistributionPokemonMenu(void* context, const void* param);
void goToGen2DecorationMenu(void* context, const void* param);
void goToBackupRestoreMenu(void* context, const void* param);

/**
 * The first step to teach pikachu something. In this step we find a pikachu in the Trainer's
 * party and check if there's currently room to add the new move.
 *
 * If not, we will prompt the user to select a move to replace.
 *
 * When the selection of the move index has been made, we'll call gen1TeachPikachu()
 *
 * @param context a MenuScene*
 * @param param a const Move* indicating which move to teach
 */
void gen1PrepareToTeachPikachu(void* context, const void* param);

/**
 * This function actually sets the given Move at the specified index in the party pokémon's data
 * and saves the game.
 *
 * @param context a MenuScene*
 * @param param a Gen1TeachPikachuParams struct pointer
 */
void gen1TeachPikachu(void* context, const void* param);

/**
 * This function is the first stage of the move deleter functionality.
 * It gives the user a prompt to select the pokémon in the trainer's party
 * from which to delete a move.
 *
 * @param context a MenuScene*
 * @param param a nullpointer (dummy param)
 */
void gen1MoveDeleterSelectPokemon(void* context, const void* param);

/**
 * This function is the second stage of the move deleter functionality.
 * It gives the user a prompt to select the move of the selected pokémon to delete
 *
 * @param context a MenuScene*
 * @param param a uint8_t value stored as the const void* value
 */
void gen1MoveDeleterSelectMove(void* context, const void* param);

/**
 * This function is the third stage of the move deleter functionality.
 * It applies the move deletion to the selected pokémon.
 *
 * @param context a MenuScene*
 * @param param a MoveDeleterParams struct pointer
 */
void gen1MoveDeleterApply(void* context, const void* param);

/**
 * This function will unlock the GS Ball event in a Pokémon Crystal save
 *
 * @param context a MenuScene*
 * @param param a nullpointer (dummy param)
 */
void gen2ReceiveGSBall(void* context, const void* param);
/**
 * This function will set a gen II event flag
 *
 * @param context a MenuScene*
 * @param param a uint16_t* which indicates the event flag index
 */
void gen2SetEventFlag(void* context, const void* param);

/**
 * This function will ask for confirmation to wipe a save from the cartridge.
 *
 * If confirmed by the user, the DataCopyScene will be navigated to with the DATACOPY_WIPE_SAVE command
 */
void askConfirmationWipeSave(void* context, const void* param);

/**
 * This function will change an SRAM field to let gen 2 games prompt you
 * to reconfigure the game clock
 *
 * @param context a MenuScene* context
 * @param param a nullpointer (dummy param)
 */
void resetRTC(void* context, const void* param);

#endif