#ifndef _POKESHOPENTRIES_H
#define _POKESHOPENTRIES_H

#include "gen1/Gen1DistributionPokemon.h"
#include "gen2/Gen2DistributionPokemon.h"

typedef struct Gen1PokeShopEntry
{
    const Gen1DistributionPokemon* pokemon;
    uint32_t price;
} Gen1PokeShopEntry;

typedef struct Gen2PokeShopEntry
{
    const Gen2DistributionPokemon* pokemon;
    uint32_t price;
} Gen2PokeShopEntry;

/**
 * @brief This function returns the menu entries for the PokéShop scene for a specific gen 1 game variant
 */
void getGen1PokeShopEntries(uint8_t gameVariant, uint8_t localization, const Gen1PokeShopEntry *&entries, uint32_t &outNumEntries);

/**
 * @brief This function returns the menu entries for the PokéShop scene for a specific gen 2 game variant
 */
void getGen2PokeShopEntries(uint8_t gameVariant, uint8_t localization, const Gen2PokeShopEntry *&entries, uint32_t &outNumEntries);

#endif