#include "wasteland.h"

#include "../blocks/grass_block.h"

void Wasteland::set_chunk_location(int xpos, int ypos, int zpos)
{
    curr_x = xpos;
    curr_y = ypos;
    curr_z = zpos;
}

Block* Wasteland::get_block(int x, int y, int z)
{
    if (curr_y == 0 and y == 0)
    {
        return  GrassBlock::getBlock();
    }
}
