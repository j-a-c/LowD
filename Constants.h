#ifndef CONSTANTS_H
#define CONSTANTS_H

// Enable debug messages.
const bool DEBUG  = true;

// Any number
// Length, Width, Height of a Block
#define BLOCK_HEIGHT 1.0
#define BLOCK_LENGTH 1.0
#define BLOCK_WIDTH 1.0

// Even square
// Length, Width, Height of a Chunk (in Blocks)
#define CHUNK_HEIGHT 16 
#define CHUNK_LENGTH 16 
#define CHUNK_WIDTH 16 

// Odd number
// Length, Width, Height of Render window (in Chunks)
#define RENDER_SIZE 3

// Player attributes
#define PLAYER_TOP_DIST 0.25
#define PLAYER_BOT_DIST 0.75
#define PLAYER_HALF_WIDTH = 0.5
#define PLAYER_HALF_LENGTH = 0.5


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

#endif
