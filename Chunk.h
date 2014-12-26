#ifndef CHUNK_H 
#define CHUNK_H

#include "Block.h"
#include "BlockType.h"
#include "Constants.h"
#include "Simplex.h"

#include <vector>

/**
 * A chunk holds data for a bunch of blocks.
 *
 * Author: Joshua A. Campbell
 *
 *
 */
class Chunk
{
    public:
        Chunk();
        ~Chunk();

        void reset();
        void generate(int x, int y, int z);
        void createMesh();
        void render();

        bool isActive(int x, int y, int z);
        
        int getNumberOfTriangles();

    private:
        // Display list for this chunk
        GLuint _theChunk;
        // Data for blocks in this chuck
        std::vector<Block> _blocks;
        // The number of renderable triangles in this chunk.
        int _numberOfTriangles = 0;

};

Chunk::Chunk()
{
    // Create the blocks
    for(int i = 0; i < CHUNK_WIDTH*CHUNK_HEIGHT*CHUNK_LENGTH; i++)
        _blocks.push_back(Block());
}

Chunk::~Chunk()
{

}


void Chunk::reset()
{
    for (auto &block: _blocks) 
        block.setType(BlockType_Air);
}


/**
 * Creates a display list for the chunk.
 */
void Chunk::createMesh()
{

    // TODO greedy strip mesh


    _theChunk = glGenLists(1);
    glNewList(_theChunk, GL_COMPILE);

    Block currentBlockToRender;

    _numberOfTriangles = 0;
    
    const int heightLength = CHUNK_HEIGHT * CHUNK_LENGTH;
    for (int x = 0; x < CHUNK_WIDTH; ++x)
    {
        const int ipart = x * heightLength;
        for (int y = 0; y < CHUNK_HEIGHT; ++y)
        {
            const long ijpart = ipart + y * CHUNK_LENGTH;
            for (int z = 0; z < CHUNK_LENGTH; ++z)
            {
                currentBlockToRender = _blocks[ijpart+z];
                //currentBlockToRender = _blocks[x][y][z];

                // Don't render inactive blocks (Air)
                if(currentBlockToRender.getType() == BlockType_Air)
                    continue;

                // TODO skip hidden blocks

                glPushMatrix();

                float translateDistX = x * BLOCK_WIDTH;
                float translateDistY = y * BLOCK_HEIGHT;
                float translateDistZ = z * BLOCK_LENGTH;

                glTranslatef(translateDistX, translateDistY, translateDistZ);

 // index = (x * height + y) * depth + z 

    glBegin(GL_TRIANGLES);
                // Don't render blocks not touching air (e.g. not visable)
                if (x == 0 ? true : _blocks[toBlockIndex(x-1,y,z)].getType() == BlockType_Air)
                {
                    _numberOfTriangles += 2;
                    currentBlockToRender.createLeft();
                }

                if (x == CHUNK_WIDTH-1 ? true : _blocks[toBlockIndex(x+1,y,z)].getType() == BlockType_Air)
                {
                    _numberOfTriangles += 2;
                    currentBlockToRender.createRight();
                }

                if (y == 0 ? true : _blocks[toBlockIndex(x,y-1,z)].getType() == BlockType_Air)
                {
                    _numberOfTriangles += 2;
                    currentBlockToRender.createBottom();
                }
                
                if (y == CHUNK_HEIGHT-1 ? true : _blocks[toBlockIndex(x,y+1,z)].getType() == BlockType_Air)
                {
                    _numberOfTriangles += 2;
                    currentBlockToRender.createTop();
                }
                
                if (z == 0 ? true : _blocks[toBlockIndex(x,y,z-1)].getType() == BlockType_Air)
                {
                    _numberOfTriangles += 2;
                    currentBlockToRender.createBack();
                }

                if (z == CHUNK_WIDTH-1 ? true : _blocks[toBlockIndex(x,y,z+1)].getType() == BlockType_Air)
                {
                    _numberOfTriangles += 2;
                    currentBlockToRender.createFront();
                }
                
    glEnd();
                //glTranslatef(-translateDistX, -translateDistY, -translateDistZ);
                glPopMatrix();

            }
        }
    }

    glEndList();
}

/**
 * Generate chunks. xpos,ypos,zpos is chunk's position.
 */
void Chunk::generate(int xpos, int ypos, int zpos)
{

    // First pass terrain
    
    // Seed and max height
    int seed = 100;
    int maxHeight = 10;
    
    // How spread apart values are
    float scale = 1.0f / 4.0f;
    // How jagged terrain is
    int smoothness = 8;

    // Base height at player start position 
    int offset = noise((0/smoothness)*scale, (0/smoothness)*scale,seed)*maxHeight;

    // Set height
    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_LENGTH; z++)
        {
            // Height at this spot
            int blockx = xpos*CHUNK_WIDTH + x;
            int blockz = zpos*CHUNK_LENGTH + z;
            float n = noise((blockx/smoothness)*scale, (blockz/smoothness)*scale, seed);       
            int heightHere = maxHeight * n - offset;

            int heightY = 0;

            // Is height located in this chunk?
            while (heightHere > CHUNK_HEIGHT)
            {
                heightHere -= CHUNK_HEIGHT;
                heightY++;
            }
            while (heightHere < 0)
            {
                heightHere += CHUNK_HEIGHT;
                heightY--;
            }

            // Height is located in chunk
            if (heightY == ypos)
                for (int y = 0; y <= heightHere; y++)
                    _blocks[toBlockIndex(x,y,z)].setType(BlockType_Grass);

            // Height is above chunk
            else if (heightY > ypos)
                for (int y = 0; y < CHUNK_HEIGHT; y++)
                    _blocks[toBlockIndex(x,y,z)].setType(BlockType_Grass);
        }
    }
}


/**
 * Render the display list representing the chunk.
 */
void Chunk::render()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(_theChunk);
}


bool Chunk::isActive(int x, int y, int z)
{
    bool ret = _blocks[toBlockIndex(x,y,z)].getType() != BlockType_Air;
    return ret; 
}

int Chunk::getNumberOfTriangles() 
{
    return _numberOfTriangles; 
}

#endif
