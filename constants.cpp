#include "constants.h"

int toChunkIndex(int x, int y, int z)
{
    // index = (x * height + y) * depth + z 
    return (x * RENDER_SIZE + y) * RENDER_SIZE + z; 
}

int toBlockIndex(int x, int y, int z)
{
    // index = (x * height + y) * depth + z 
    return (x * CHUNK_HEIGHT + y) * CHUNK_LENGTH + z; 
}
