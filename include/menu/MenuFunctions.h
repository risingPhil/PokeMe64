#ifndef _MENUFUNCTIONS_H
#define _MENUFUNCTIONS_H

#include "Moves.h"

#include <cstdint>

// these are used to pass as a pointer to the gen1PrepareToTeachPikachu
extern const Move MOVE_SURF;
extern const Move MOVE_FLY;

extern const uint16_t GEN2_EVENTFLAG_DECORATION_PIKACHU_BED;
extern const uint16_t GEN2_EVENTFLAG_DECORATION_UNOWN_DOLL;
extern const uint16_t GEN2_EVENTFLAG_DECORATION_TENTACOOL_DOLL;

void activateFrameLog(void* context, const void* param);
void advanceDialog(void* context, const void* param);

void goToTestScene(void* context, const void* param);
void goToPokeTransporterGBRef(void* context, const void* param);
void goToAboutScene(void* context, const void* param);

void goToGen1DistributionPokemonMenu(void* context, const void* param);
void goToGen2DistributionPokemonMenu(void* context, const void* param);
void goToGen2PCNYDistributionPokemonMenu(void* context, const void* param);
void goToGen2DecorationMenu(void* context, const void* param);

void gen1PrepareToTeachPikachu(void* context, const void* param);
void gen1TeachPikachu(void* context, const void* param);
void gen2ReceiveGSBall(void* context, const void* param);
void gen2SetEventFlag(void* context, const void* param);


#endif