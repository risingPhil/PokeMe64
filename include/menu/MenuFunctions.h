#ifndef _MENUFUNCTIONS_H
#define _MENUFUNCTIONS_H

#include "Moves.h"

// these are used to pass as a pointer to the gen1PrepareToTeachPikachu
extern const Move MOVE_SURF;
extern const Move MOVE_FLY;

void printMessage(void* context, const void* param);
void activateFrameLog(void* context, const void* param);

void goToTestScene(void* context, const void* param);

void goToGen1DistributionPokemonMenu(void* context, const void* param);
void goToGen2DistributionPokemonMenu(void* context, const void* param);
void goToGen2PCNYDistributionPokemonMenu(void* context, const void* param);

void gen1PrepareToTeachPikachu(void* context, const void* param);
void gen1TeachPikachu(void* context, const void* param);
void gen2ReceiveGSBall(void* context, const void* param);

#endif