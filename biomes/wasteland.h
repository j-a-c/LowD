#ifndef BIOMES_WASTELAND_H
#define BIOMES_WASTELAND_H

#include "biome.h"

class Wasteland: public Biome
{
    public:

        void set_chunk_location(int xpos, int ypos, int zpos) override;
        Block* get_block(int x, int y, int z) override;

    private:
        int curr_x, curr_y, curr_z;
};


#endif
