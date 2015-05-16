#ifndef BIOME_MANAGER_H
#define BIOME_MANAGER_H

#include "biomes/biome.h"

/**
 * The BiomeManager manages the biomes.
 * It is in charge of assigning the biome for each chunk.
 *
 * Author: Joshua A. Campbell
 *
 *
 */
class BiomeManager
{
    public:
        virtual ~BiomeManager();

        virtual Biome* get_biome(int x, int y, int z) = 0;

};

#endif
