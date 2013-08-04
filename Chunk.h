#ifndef CHUNK_H 
#define CHUNK_H

#include "Block.h"
#include "BlockType.h"
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

    private:
        // Display list for this chunk
        GLuint _theChunk;
        // Data for blocks in this chuck
        Block*** _blocks;

};

Chunk::Chunk()
{
    // Create the blocks
    _blocks = new Block**[CHUNK_WIDTH];
    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        _blocks[x] = new Block*[CHUNK_HEIGHT];

        for (int y = 0; y < CHUNK_HEIGHT; y++)
            _blocks[x][y] = new Block[CHUNK_LENGTH];
    }
}

Chunk::~Chunk()
{
    // Delete the blocks
    for (int x = 0; x < CHUNK_WIDTH; ++x)
    {
        for (int y = 0; y < CHUNK_HEIGHT; ++y)
        {
            delete [] _blocks[x][y];
        }
        delete [] _blocks[x];
    }
    delete [] _blocks;
}


void Chunk::reset()
{
    for (int x = 0; x < CHUNK_WIDTH; x++)
        for (int y = 0; y < CHUNK_HEIGHT; y++)
            for (int z = 0; z < CHUNK_LENGTH; z++)
                _blocks[x][y][z].setType(BlockType_Air);
}


/**
 * Creates a display list for the chunk.
 */
void Chunk::createMesh()
{

    // TODO greedy mesh

    _theChunk = glGenLists(1);
    glNewList(_theChunk, GL_COMPILE);


    Block currentBlockToRender;
    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_LENGTH; z++)
            {
                currentBlockToRender = _blocks[x][y][z];

                // Don't render inactive blocks (Air)
                if(currentBlockToRender.getType() == BlockType_Air)
                    continue;

                // TODO skip hidden blocks

                //glPushMatrix();
                float translateDistX = x * BLOCK_WIDTH;
                float translateDistY = y * BLOCK_HEIGHT;
                float translateDistZ = z * BLOCK_LENGTH;

                glTranslatef(translateDistX, translateDistY, translateDistZ);

                // Don't render blocks not touching air (e.g. not visable)
                if (x == 0 ? true : _blocks[x-1][y][z].getType() == BlockType_Air)
                    currentBlockToRender.createLeft();

                if (x == CHUNK_WIDTH-1 ? true : _blocks[x+1][y][z].getType() == BlockType_Air)
                    currentBlockToRender.createRight();

                if (y == 0 ? true : _blocks[x][y-1][z].getType() == BlockType_Air)
                    currentBlockToRender.createBottom();
                
                if (y == CHUNK_HEIGHT-1 ? true : _blocks[x][y+1][z].getType() == BlockType_Air)
                    currentBlockToRender.createTop();
                
                if (z == 0 ? true : _blocks[x][y][z-1].getType() == BlockType_Air)
                    currentBlockToRender.createBack();

                if (z == CHUNK_WIDTH-1 ? true : _blocks[x][y][z+1].getType() == BlockType_Air)
                    currentBlockToRender.createFront();

                //_blocks[x][y][z].createBlock();
                
                glTranslatef(-translateDistX, -translateDistY, -translateDistZ);
                //glPopMatrix();
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
        for (int z = 0; z < CHUNK_LENGTH; z++)
        {
            // Height at this spot
            int blockx = xpos*CHUNK_WIDTH+ x;
            int blockz = zpos*CHUNK_LENGTH+ z;
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
                    _blocks[x][y][z].setType(BlockType_Grass);

            // Height is above chunk
            else if (heightY > ypos)
                for (int y = 0; y < CHUNK_HEIGHT; y++)
                    _blocks[x][y][z].setType(BlockType_Grass);
        }



    /*
    // seed and max height
    int seed = 100;
    int maxheight = 10;
    
    // how spread apart values are
    float scale = 1.0f / 4.0f;
    // how jagged terrain is
    int smoothness = 2;

    // base height at player start position 
    int offset = noise((0/smoothness)*scale, (0/smoothness)*scale,seed)*maxheight;

    // height at this spot
    float n = noise((xpos/smoothness)*scale, (zpos/smoothness)*scale, seed);       
    std::cout << "\t\tnoise: " << n << std::endl;
    int heighthere = maxheight * n - offset;

    int heighty = 0;

    // is height located in this chunk?
    while (heighthere > chunk_size)
    {
        heighthere -= chunk_size;
        heighty++;
    }

    while (heighthere < 0)
    {
        heighthere += chunk_size;
        heighty--;
    }

    if (heighty == ypos)
        for (int x = 0; x < chunk_size; x++)
        //for(int y = 0; y < heighthere; y++)
            for (int z = 0; z < chunk_size; z++)
                _blocks[x][heighthere][z].setactive(true);
    */


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


bool Chunk::isActive(int x, int y, int z)
{
    std::cout << "\t\tEntering Chunk.isActive()" << std::endl;

    bool ret = _blocks[x][y][z].getType() != BlockType_Air;

    std::cout << "\t\tExiting Chunk.isActive()" << std::endl;

    return ret; 
}

#endif
