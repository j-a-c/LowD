#ifndef CHUNK_H 
#define CHUNK_H

#include "block.h"
#include "block_type.h"
#include "constants.h"
#include "simplex.h"

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

    // Face merge algorithm.
    // 0 - front, 1 - back, 2 - left, 3 - right, 4 - top, 5 - bottom
    for (auto& face : FACES)
    {
        // Set to true once we have rendered this face.
        bool rendered [CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_LENGTH] = {false};

        for (int x = 0; x < CHUNK_WIDTH; ++x) 
        {
            const int ipart = x * heightLength;
            for (int y = 0; y < CHUNK_HEIGHT; ++y)
            {
                const long ijpart = ipart + y * CHUNK_LENGTH;
                for (int z = 0; z < CHUNK_LENGTH; ++z)
                {
                    int currentIndex = ijpart + z;

                    currentBlockToRender = _blocks[currentIndex];

                    // Don't render inactive blocks (Air)
                    if(currentBlockToRender.getType() == BlockType_Air)
                        continue;

                    // Already rendered this block.
                    if (rendered[currentIndex])
                        continue;

                    glPushMatrix();

                    float translateDistX = x * BLOCK_WIDTH;
                    float translateDistY = y * BLOCK_HEIGHT;
                    float translateDistZ = z * BLOCK_LENGTH;

                    // index = (x * height + y) * depth + z 
                    glTranslatef(translateDistX, translateDistY, translateDistZ);

        glBegin(GL_TRIANGLES);

                    switch(face) 
                    {
                        case FRONT:

                            if (z == CHUNK_LENGTH-1 ? true : _blocks[toBlockIndex(x,y,z+1)].getType() == BlockType_Air)
                            {
                                int nextXSameY = x;
                                // Calculate how much we can merge in the same Y dimension.
                                while (nextXSameY + 1 < CHUNK_LENGTH &&
                                    !rendered[toBlockIndex(nextXSameY+1,y,z)] &&
                                    (z == CHUNK_LENGTH-1 || _blocks[toBlockIndex(nextXSameY+1,y,z+1)].getType() == BlockType_Air) &&
                                    _blocks[toBlockIndex(nextXSameY+1,y,z)].getType() == currentBlockToRender.getType()
                                    )
                                {
                                    nextXSameY++;
                                }

                                int minX = nextXSameY;
                                int finalX = nextXSameY;
                                int finalY = y;

                                int nextY = y;
                                // Greedily search for a better merge.
                                while(nextY + 1 < CHUNK_HEIGHT &&
                                     !rendered[toBlockIndex(x,nextY+1,z)] &&
                                     _blocks[toBlockIndex(x, nextY+1,z+1)].getType() == BlockType_Air &&
                                     _blocks[toBlockIndex(x, nextY+1, z)].getType() == currentBlockToRender.getType() 
                                    )
                                {
                                    nextXSameY = x;
                                    while(nextXSameY + 1 <= minX &&
                                        !rendered[toBlockIndex(nextXSameY+1,nextY+1,z)] &&
                                        (z == CHUNK_LENGTH-1 || _blocks[toBlockIndex(nextXSameY+1,nextY+1,z+1)].getType() == BlockType_Air) &&
                                        _blocks[toBlockIndex(nextXSameY+1,nextY+1,z)].getType() == currentBlockToRender.getType()
                                        )
                                    {
                                        nextXSameY++;
                                    }

                                    // Track the min width
                                    if (nextXSameY < minX)
                                        minX = nextXSameY;

                                    // If we can merge more triangles, update values.
                                    if ( ((finalX - x)*(finalY-y)) < (nextXSameY-x)*(nextY-y) )
                                    {
                                        finalX = nextXSameY;
                                        finalY = nextY;
                                    }

                                    nextY++;
                                }

                                // Mark rendered faces.
                                for (int rx = x; rx <= finalX; rx++)
                                    for (int ry = y; ry <= finalY; ry++)
                                        rendered[toBlockIndex(rx, ry, z)] = true;

                                _numberOfTriangles += 2;
                                currentBlockToRender.createFront(finalY - y + 1, finalX-x+1);
                            }
                            break;
                        case BACK:
                            if (z == 0 ? true : _blocks[toBlockIndex(x,y,z-1)].getType() == BlockType_Air)
                            {
                                int nextXSameY = x;
                                // Calculate how much we can merge in the same Y dimension.
                                while (nextXSameY + 1 < CHUNK_LENGTH &&
                                    !rendered[toBlockIndex(nextXSameY+1,y,z)] &&
                                    (z == 0 || _blocks[toBlockIndex(nextXSameY+1,y,z-1)].getType() == BlockType_Air) &&
                                    _blocks[toBlockIndex(nextXSameY+1,y,z)].getType() == currentBlockToRender.getType()
                                    )
                                {
                                    nextXSameY++;
                                }

                                int minX = nextXSameY;
                                int finalX = nextXSameY;
                                int finalY = y;

                                int nextY = y;
                                // Greedily search for a better merge.
                                while(nextY + 1 < CHUNK_HEIGHT &&
                                     !rendered[toBlockIndex(x,nextY+1,z)] &&
                                     (z == 0 || _blocks[toBlockIndex(x, nextY+1,z-1)].getType() == BlockType_Air) &&
                                     _blocks[toBlockIndex(x, nextY+1, z)].getType() == currentBlockToRender.getType() 
                                    )
                                {
                                    nextXSameY = x;
                                    while(nextXSameY + 1 <= minX &&
                                        !rendered[toBlockIndex(nextXSameY+1,nextY+1,z)] &&
                                        (z == 0 || _blocks[toBlockIndex(nextXSameY+1,nextY+1,z-1)].getType() == BlockType_Air) &&
                                        _blocks[toBlockIndex(nextXSameY+1,nextY+1,z)].getType() == currentBlockToRender.getType()
                                        )
                                    {
                                        nextXSameY++;
                                    }

                                    // Track the min width
                                    if (nextXSameY < minX)
                                        minX = nextXSameY;

                                    // If we can merge more triangles, update values.
                                    if ( ((finalX - x)*(finalY-y)) < (nextXSameY-x)*(nextY-y) )
                                    {
                                        finalX = nextXSameY;
                                        finalY = nextY;
                                    }

                                    nextY++;
                                }

                                // Mark rendered faces.
                                for (int rx = x; rx <= finalX; rx++)
                                    for (int ry = y; ry <= finalY; ry++)
                                        rendered[toBlockIndex(rx, ry, z)] = true;

                                _numberOfTriangles += 2;
                                currentBlockToRender.createBack(finalY - y + 1, finalX-x+1);

                            }
                            break;
                        case LEFT:
                            if (x == 0 ? true : _blocks[toBlockIndex(x-1,y,z)].getType() == BlockType_Air)
                            {
                                int nextZSameY = z;
                                // Calculate how much we can merge in the same X dimension.
                                while (nextZSameY + 1 < CHUNK_LENGTH &&
                                    !rendered[toBlockIndex(x,y,nextZSameY+1)] &&
                                    (x == 0 || _blocks[toBlockIndex(x-1,y,nextZSameY+1)].getType() == BlockType_Air) &&
                                    _blocks[toBlockIndex(x,y,nextZSameY+1)].getType() == currentBlockToRender.getType()
                                    )
                                {
                                    nextZSameY++;
                                }

                                int minZ = nextZSameY;
                                int finalZ = nextZSameY;
                                int finalY = y;

                                int nextY = y;
                                // Greedily search for a better merge.
                                while(nextY + 1 < CHUNK_HEIGHT &&
                                     !rendered[toBlockIndex(x,nextY+1,z)] &&
                                     (x == 0 || _blocks[toBlockIndex(x-1,nextY+1,z)].getType() == BlockType_Air) &&
                                     _blocks[toBlockIndex(x,nextY+1,z)].getType() == currentBlockToRender.getType() 
                                    )
                                {
                                    nextZSameY = z;
                                    while(nextZSameY + 1 <= minZ &&
                                        !rendered[toBlockIndex(x,y,nextZSameY+1)] &&
                                        (x == 0 || _blocks[toBlockIndex(x-1,y,nextZSameY+1)].getType() == BlockType_Air) &&
                                        _blocks[toBlockIndex(x,nextY+1,nextZSameY+1)].getType() == currentBlockToRender.getType()
                                        )
                                    {
                                        nextZSameY++;
                                    }

                                    // Track the min width
                                    if (nextZSameY < minZ)
                                        minZ = nextZSameY;

                                    // If we can merge more triangles, update values.
                                    if ( ((finalY - y)*(finalZ-z)) < (nextZSameY-z)*(nextY-y) )
                                    {
                                        finalZ = nextZSameY;
                                        finalY = nextY;
                                    }

                                    nextY++;
                                }

                                // Mark rendered faces.
                                for (int ry = y; ry <= finalY; ry++)
                                    for (int rz = z; rz <= finalZ; rz++)
                                        rendered[toBlockIndex(x, ry, rz)] = true;

                                _numberOfTriangles += 2;
                                currentBlockToRender.createLeft(finalY-y+1, finalZ-z+1);

                            }
                            break;
                        case RIGHT:
                            if (x == CHUNK_WIDTH-1 ? true : _blocks[toBlockIndex(x+1,y,z)].getType() == BlockType_Air)
                            {
                                int nextZSameY = z;
                                // Calculate how much we can merge in the same X dimension.
                                while (nextZSameY + 1 < CHUNK_LENGTH &&
                                    !rendered[toBlockIndex(x,y,nextZSameY+1)] &&
                                    (x == CHUNK_WIDTH-1 || _blocks[toBlockIndex(x+1,y,nextZSameY+1)].getType() == BlockType_Air) &&
                                    _blocks[toBlockIndex(x,y,nextZSameY+1)].getType() == currentBlockToRender.getType()
                                    )
                                {
                                    nextZSameY++;
                                }

                                int minZ = nextZSameY;
                                int finalZ = nextZSameY;
                                int finalY = y;

                                int nextY = y;
                                // Greedily search for a better merge.
                                while(nextY + 1 < CHUNK_HEIGHT &&
                                     !rendered[toBlockIndex(x,nextY+1,z)] &&
                                     (x == CHUNK_WIDTH-1 || _blocks[toBlockIndex(x+1,nextY+1,z)].getType() == BlockType_Air) &&
                                     _blocks[toBlockIndex(x,nextY+1,z)].getType() == currentBlockToRender.getType() 
                                    )
                                {
                                    nextZSameY = z;
                                    while(nextZSameY + 1 <= minZ &&
                                        !rendered[toBlockIndex(x,y,nextZSameY+1)] &&
                                        (x == CHUNK_WIDTH-1 || _blocks[toBlockIndex(x+1,y,nextZSameY+1)].getType() == BlockType_Air) &&
                                        _blocks[toBlockIndex(x,nextY+1,nextZSameY+1)].getType() == currentBlockToRender.getType()
                                        )
                                    {
                                        nextZSameY++;
                                    }

                                    // Track the min width
                                    if (nextZSameY < minZ)
                                        minZ = nextZSameY;

                                    // If we can merge more triangles, update values.
                                    if ( ((finalY - y)*(finalZ-z)) < (nextZSameY-z)*(nextY-y) )
                                    {
                                        finalZ = nextZSameY;
                                        finalY = nextY;
                                    }

                                    nextY++;
                                }

                                // Mark rendered faces.
                                for (int ry = y; ry <= finalY; ry++)
                                    for (int rz = z; rz <= finalZ; rz++)
                                        rendered[toBlockIndex(x, ry, rz)] = true;

                                _numberOfTriangles += 2;
                                currentBlockToRender.createRight(finalY-y+1, finalZ-z+1);
                            }
                            break;
                        case TOP:
                            if (y == CHUNK_HEIGHT-1 ? true : _blocks[toBlockIndex(x,y+1,z)].getType() == BlockType_Air)
                            {
                                int nextZSameX = z;
                                // Calculate how much we can merge in the same X dimension.
                                while (nextZSameX + 1 < CHUNK_LENGTH &&
                                    !rendered[toBlockIndex(x,y,nextZSameX+1)] &&
                                    (y == CHUNK_HEIGHT-1 || _blocks[toBlockIndex(x,y+1,nextZSameX+1)].getType() == BlockType_Air) &&
                                    _blocks[toBlockIndex(x,y,nextZSameX+1)].getType() == currentBlockToRender.getType()
                                    )
                                {
                                    nextZSameX++;
                                }

                                int minZ = nextZSameX;
                                int finalZ = nextZSameX;
                                int finalX = x;

                                int nextX = x;
                                // Greedily search for a better merge.
                                while(nextX + 1 < CHUNK_WIDTH &&
                                     !rendered[toBlockIndex(nextX+1,y,z)] &&
                                     (y == CHUNK_HEIGHT-1 || _blocks[toBlockIndex(nextX+1,y+1,z)].getType() == BlockType_Air) &&
                                     _blocks[toBlockIndex(nextX+1,y,z)].getType() == currentBlockToRender.getType() 
                                    )
                                {
                                    nextZSameX = z;
                                    while(nextZSameX + 1 <= minZ &&
                                        !rendered[toBlockIndex(nextX+1,y,nextZSameX+1)] &&
                                        (y == CHUNK_HEIGHT-1 || _blocks[toBlockIndex(nextX+1,y+1,nextZSameX+1)].getType() == BlockType_Air) &&
                                        _blocks[toBlockIndex(nextX+1,y,nextZSameX+1)].getType() == currentBlockToRender.getType()
                                        )
                                    {
                                        nextZSameX++;
                                    }

                                    // Track the min width
                                    if (nextZSameX < minZ)
                                        minZ = nextZSameX;

                                    // If we can merge more triangles, update values.
                                    if ( ((finalX - x)*(finalZ-z)) < (nextZSameX-z)*(nextX-x) )
                                    {
                                        finalZ = nextZSameX;
                                        finalX = nextX;
                                    }

                                    nextX++;
                                }

                                // Mark rendered faces.
                                for (int rx = x; rx <= finalX; rx++)
                                    for (int rz = z; rz <= finalZ; rz++)
                                        rendered[toBlockIndex(rx, y, rz)] = true;

                                _numberOfTriangles += 2;
                                currentBlockToRender.createTop(finalX-x+1, finalZ-z+1);
                            }
                            break;
                        case BOTTOM:
                            if (y == 0 ? true : _blocks[toBlockIndex(x,y-1,z)].getType() == BlockType_Air)
                            {
                                int nextZSameX = z;
                                // Calculate how much we can merge in the same X dimension.
                                while (nextZSameX + 1 < CHUNK_LENGTH &&
                                    !rendered[toBlockIndex(x,y,nextZSameX+1)] &&
                                    (y == 0 || _blocks[toBlockIndex(x,y-1,nextZSameX+1)].getType() == BlockType_Air) &&
                                    _blocks[toBlockIndex(x,y,nextZSameX+1)].getType() == currentBlockToRender.getType()
                                    )
                                {
                                    nextZSameX++;
                                }

                                int minZ = nextZSameX;
                                int finalZ = nextZSameX;
                                int finalX = x;

                                int nextX = x;
                                // Greedily search for a better merge.
                                while(nextX + 1 < CHUNK_WIDTH &&
                                     !rendered[toBlockIndex(nextX+1,y,z)] &&
                                     (y == 0 || _blocks[toBlockIndex(nextX-1,y+1,z)].getType() == BlockType_Air) &&
                                     _blocks[toBlockIndex(nextX+1,y,z)].getType() == currentBlockToRender.getType() 
                                    )
                                {
                                    nextZSameX = z;
                                    while(nextZSameX + 1 <= minZ &&
                                        !rendered[toBlockIndex(nextX+1,y,nextZSameX+1)] &&
                                        (y == 0 || _blocks[toBlockIndex(nextX+1,y-1,nextZSameX+1)].getType() == BlockType_Air) &&
                                        _blocks[toBlockIndex(nextX+1,y,nextZSameX+1)].getType() == currentBlockToRender.getType()
                                        )
                                    {
                                        nextZSameX++;
                                    }

                                    // Track the min width
                                    if (nextZSameX < minZ)
                                        minZ = nextZSameX;

                                    // If we can merge more triangles, update values.
                                    if ( ((finalX - x)*(finalZ-z)) < (nextZSameX-z)*(nextX-x) )
                                    {
                                        finalZ = nextZSameX;
                                        finalX = nextX;
                                    }

                                    nextX++;
                                }

                                // Mark rendered faces.
                                for (int rx = x; rx <= finalX; rx++)
                                    for (int rz = z; rz <= finalZ; rz++)
                                        rendered[toBlockIndex(rx, y, rz)] = true;

                                _numberOfTriangles += 2;
                                currentBlockToRender.createBottom(finalX-x+1, finalZ-z+1);
                            }
                            break;
                    }

        glEnd();
                //glTranslatef(-translateDistX, -translateDistY, -translateDistZ);
                glPopMatrix();

                }
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
