#ifndef _MENUFUNCTIONS_H
#define _MENUFUNCTIONS_H

void printMessage(void* context, const void* param);
void activateFrameLog(void* context, const void* param);

void goToTestScene(void* context, const void* param);

void goToGen1DistributionPokemonMenu(void* context, const void* param);
void goToGen2DistributionPokemonMenu(void* context, const void* param);
void goToGen2PCNYDistributionPokemonMenu(void* context, const void* param);

void gen2ReceiveGSBall(void* context, const void* param);

#endif