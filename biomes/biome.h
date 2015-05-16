#ifndef BIOMES_BIOME_H
#define BIOMES_BIOME_H

#include "../blocks/block.h"

class Biome
{
    public:
        virtual ~Biome();

        virtual void set_chunk_location(int xpos, int ypos, int zpos) = 0;
        virtual Block* get_block(int x, int y, int z) = 0;
};


#endif
