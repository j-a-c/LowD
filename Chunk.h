#ifndef CHUNK_H 
#define CHUNK_H

#include "Block.h"
#include "Constants.h"
#include "Simplex.h"
#include "math/Vector3D.h"

// TODO delete after debug
#include <iostream>

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
        void setActive(int x, int y, int z);

    private:
        // Display list for this chunk
        GLuint _theChunk;
        // Data for blocks in this chuck
        Block*** _blocks;

};

Chunk::Chunk()
{
    // Create the blocks
    _blocks = new Block**[CHUNK_SIZE];
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        _blocks[x] = new Block*[CHUNK_SIZE];

        for (int y = 0; y < CHUNK_SIZE; y++)
            _blocks[x][y] = new Block[CHUNK_SIZE];
    }
}

Chunk::~Chunk()
{
    // Delete the blocks
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_SIZE; ++y)
        {
            delete [] _blocks[x][y];
        }
        delete [] _blocks[x];
    }
    delete [] _blocks;
}


void Chunk::reset()
{
    for (int x = 0; x < CHUNK_SIZE; x++)
        for (int y = 0; y < CHUNK_SIZE; y++)
            for (int z = 0; z < CHUNK_SIZE; z++)
                _blocks[x][y][z].setActive(false);
}


/**
 * Creates a display list for the chunk.
 */
void Chunk::createMesh()
{
    _theChunk = glGenLists(1);
    glNewList(_theChunk, GL_COMPILE);

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                // Don't render inactive blocks
                if(_blocks[x][y][z].isActive() == false)
                    continue;

                glPushMatrix();
                glTranslatef(x*BLOCK_WIDTH, y*BLOCK_HEIGHT, z*BLOCK_LENGTH);
                _blocks[x][y][z].createBlock();
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
    /*
       for (int x = 0; x < CHUNK_SIZE; x++)
       for (int y = 0; y < CHUNK_SIZE; y++)
       for (int z = 0; z < CHUNK_SIZE; z++) 
       {
       if ( simplex_noise(1, xpos+x, ypos+y, zpos+z) > 1.1 )
       if ( (ypos == 0) && (y == 0 || 
       (xpos == 1 && zpos == -1 && y == 2) || 
       (z== 2 && x==1 && y == 3 )) ) 

       _blocks[x][y][z].setActive(true);
       else
       _blocks[x][y][z].setActive(false);
       }
       */

    // First pass terrain

    // Seed and max height
    int seed = 100;
    int maxHeight = 10;

    // Base height at player start position 
    int offset = noise(0,0,seed)*maxHeight;

    float n = noise(xpos, zpos, seed);       
    int heightHere = maxHeight * n - offset;

    int heightY = 0;

    while (heightHere > CHUNK_SIZE)
    {
    heightHere -= CHUNK_SIZE;
    heightY++;
    }

    while (heightHere < 0)
    {
    heightHere += CHUNK_SIZE;
    heightY--;
    }

    if (heightY == ypos)
    for (int x = 0; x < CHUNK_SIZE; x++)
    //for(int y = 0; y < heightHere; y++)
    for (int z = 0; z < CHUNK_SIZE; z++)
    _blocks[x][heightHere][z].setActive(true);



    /*
    float intensity = 5;
    float width = 100;

    for (int x = 0; x < CHUNK_SIZE; x++)
        for(int y = 0; y < CHUNK_SIZE; y++)
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                float x1 = CHUNK_SIZE*xpos + x;
                float y1 = CHUNK_SIZE*ypos + y;
                float z1 = CHUNK_SIZE*zpos + z;

                float xd = x1 - width*0.25;
                float yd = y1 - width*0.20;
                float zd = z1 - width*0.25;
                
                if(yd > 0) 
                    yd *= yd*0.05;
                float xz = noise(x1, z1);
                float distance = (xd*xd+yd*yd*intensity+zd*zd)*0.0004;
                float density = noise(x1/intensity, y1/intensity, z1/intensity) - distance;
                if(density > -0.75){
                    _blocks[x][y][z].setActive(true);
                }
                if(density > -0.85){
                    _blocks[x][y][z].setActive(true);
                }
                if(density > -1.0){
                    y1 > intensity+xz*4 ? _blocks[x][y][z].setActive(false) : _blocks[x][y][z].setActive(true);
                }
            }
        */
}


/**
 * Render the display list representing the chunk.
 */
void Chunk::render()
{
    glCallList(_theChunk);
}



void Chunk::setActive(int x, int y, int z)
{
    _blocks[x][y][z].setActive(true);
}


bool Chunk::isActive(int x, int y, int z)
{
    std::cout << "\t\tEntering Chunk.isActive()" << std::endl;

    bool ret = _blocks[x][y][z].isActive();

    std::cout << "\t\tExiting Chunk.isActive()" << std::endl;

    return ret; 
}

#endif
