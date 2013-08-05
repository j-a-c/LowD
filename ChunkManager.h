#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "Chunk.h"
#include "Constants.h"
#include "datastructures/Vector3D.h"

#include <iostream>
#include <math.h>


/**
 * The ChunkManager manages the chunks in the environment.
 *
 * Author: Joshua A. Campbell
 *
 *
 */
class ChunkManager
{
    public:
        ChunkManager();
        ~ChunkManager();

        void initializeWorld();

        void render();

        Vector3D update(Vector3D position);

        bool isActive(Vector3D position);

    private:
        //Chunks
        Chunk**** _chunks;

        // Current chunk
        // Player starts off in chunk 0, 0, 0 - before being moved due to
        // terrain
        int _x;
        int _y;
        int _z;

        // Offset bc the player is assumed to be in the center of render cube
        int _offset;

        // Render offset bc player is in center of render cube
        int _renderOffsetX;
        int _renderOffsetY;
        int _renderOffsetZ;

        // Height, length, width in units
        int _height;
        int _length;
        int _width; 

};

ChunkManager::ChunkManager()
{
    _x = 0;
    _y = 0;
    _z = 0;

    _offset = RENDER_SIZE / 2;

    _renderOffsetX = int(RENDER_SIZE / 2) * CHUNK_WIDTH * BLOCK_HEIGHT;
    _renderOffsetY = int(RENDER_SIZE / 2) * CHUNK_HEIGHT * BLOCK_LENGTH;
    _renderOffsetZ = int(RENDER_SIZE / 2) * CHUNK_LENGTH * BLOCK_WIDTH;

    _height = CHUNK_HEIGHT* BLOCK_HEIGHT;
    _length = CHUNK_LENGTH * BLOCK_LENGTH;
    _width = CHUNK_WIDTH * BLOCK_WIDTH;

    std::cout << "\tEntering ChunkManager()" << std::endl;

    // Initialize the chunks
    _chunks = new Chunk***[RENDER_SIZE];
    for (int x = 0; x < RENDER_SIZE; x++)
    {
        _chunks[x] = new Chunk**[RENDER_SIZE];

        for (int y = 0; y < RENDER_SIZE; y++)
        {
            _chunks[x][y] = new Chunk*[RENDER_SIZE];
            for (int z = 0; z < RENDER_SIZE; z++)
                _chunks[x][y][z] = new Chunk();
        }

    }

    std::cout << "\tExiting ChunkManager()" << std::endl;

}

ChunkManager::~ChunkManager()
{
    // Delete the chunks
    for (int x = 0; x < RENDER_SIZE; x++)
    {
        for (int y = 0; y < RENDER_SIZE; y++)
        {
            for (int z = 0; z < RENDER_SIZE; z++)
                delete _chunks[x][y][z];
            delete [] _chunks[x][y];
        }
        delete [] _chunks[x];
    }
    delete [] _chunks;
}


// TODO
void ChunkManager::initializeWorld()
{
    std::cout << "\tEnter ChunkManager.initializeWorld()" << std::endl;


    for (int x = 0; x < RENDER_SIZE; x++)
    {
        for (int y = 0; y < RENDER_SIZE; y++)
        {
            for (int z = 0; z < RENDER_SIZE; z++)
            {
                std::cout << "Initing: " << x+_x-_offset << "," << y+_y-_offset << "," << z+_z-_offset << std::endl;

                _chunks[x][y][z]->generate(x+_x-_offset,y+_y-_offset,z+_z-_offset);
                _chunks[x][y][z]->createMesh();
            }
        }
    }

    std::cout << "\tExiting ChunkManager.initializeWorld()" << std::endl;
}

/**
 * Renders all Chunks in the ChunkManager.
 */
void ChunkManager::render()
{
    std::cout << "\tEntering ChunkManager.render()" << std::endl;

    // TODO render spherically from center outwards
    // render visible
    for (int x = 0; x < RENDER_SIZE; x++)
        for (int y = 0; y < RENDER_SIZE; y++)
            for (int z = 0; z < RENDER_SIZE; z++)
            {
                glPushMatrix();
                // float translateX = x*CHUNK_WIDTH*BLOCK_WIDTH - _renderOffsetX;
                // float translateY = y*CHUNK_HEIGHT*BLOCK_HEIGHT - _renderOffsetY;
                // float translateZ = z*CHUNK_LENGTH*BLOCK_LENGTH - _renderOffsetZ;
                // glTranslatef(translateX, translateY, translateZ);

                glTranslatef(
                        x*CHUNK_WIDTH*BLOCK_WIDTH - _renderOffsetX,
                        y*CHUNK_HEIGHT*BLOCK_HEIGHT - _renderOffsetY,
                        z*CHUNK_LENGTH*BLOCK_LENGTH - _renderOffsetZ);
                _chunks[x][y][z]->render();
                glPopMatrix();
            }

    std::cout << "\tExiting ChunkManager.render()" << std::endl;
}
/**
 * Updates the ChunkManager given the player's interpolated position.
 * Update includes shifting the render cube and generating new terrain.
 * Returns a position that is in the center of the render cube.
 */
Vector3D ChunkManager::update(Vector3D position)
{
    std::cout << "\tEntering ChunkManager.update()" << std::endl;
    std::cout << "\tOld coords: " << position.x << " " << position.y << " " << position.z << std::endl;

    // Original position
    float distx = position.x;
    float disty = position.y;
    float distz = position.z;

    int xShift = 0;
    int yShift = 0;
    int zShift = 0;

    // Translate to a position within center Chunk
    // And update current render cube coordinate
    while (distx >= _width)
    {
        distx -= _width;
        _x++;
        xShift++;
    }
    while (distx < 0) 
    {
        distx += _width;
        _x--;
        xShift--;
    }
    while (disty >= _height) 
    {
        disty -= _height;
        _y++;
        yShift++;
    }
    while (disty < 0) 
    {
        disty += _height;
        _y--;
        yShift--;
    }
    while (distz >= _length) 
    {
        distz -= _length;
        _z++;
        zShift++;
    }
    while (distz < 0) 
    {
        distz += _length;
        _z--;
        zShift--;
    }

    // Has player left the center of render cube?
    if (xShift!= 0 || zShift!= 0 || yShift!= 0)
    {
        std::cout << "\tShift: " << xShift << " " << yShift << " " << zShift << std::endl;

        // Calculate in-place swap limits
        int xShiftStart = xShift >= 0 ? 0 : -xShift;
        int xShiftStop = xShift <= 0 ? RENDER_SIZE : RENDER_SIZE - xShift;

        int yShiftStart = yShift >= 0 ? 0 : -yShift;
        int yShiftStop = yShift <= 0 ? RENDER_SIZE : RENDER_SIZE - yShift;

        int zShiftStart = zShift >= 0 ? 0 : -zShift;
        int zShiftStop = zShift <= 0 ? RENDER_SIZE : RENDER_SIZE - zShift;

        // Shift render cube
        // In-place swap
        Chunk* swap;
        for (int x = xShiftStart; x < xShiftStop; x++)
            for (int y = yShiftStart; y < yShiftStop; y++)
                for (int z = zShiftStart; z < zShiftStop; z++)
                {
                    int xSwap = xShift == 0 ? x : xShift > 0 ? x+xShift : x % (-xShift);
                    int ySwap = yShift == 0 ? y : yShift > 0 ? y+yShift : y % (-yShift);
                    int zSwap = zShift == 0 ? z : zShift > 0 ? z+zShift : x % (-zShift);

                    std::cout << "\tSwapping " << x << "," << y << "," << z
                        << " with " << xSwap << "," << ySwap << "," << zSwap << std::endl; 

                    swap =  _chunks[x][y][z];
                    _chunks[x][y][z] =  _chunks[xSwap][ySwap][zSwap];
                    _chunks[xSwap][ySwap][zSwap] = swap; 
                }

        // TODO move into previous loop?
        // Generate new terrain and create mesh
        // Only for the terrain that needs to be generated
        int xGenStart = xShift <= 0 ? 0 : RENDER_SIZE-xShift;
        int xGenStop = xShift < 0 ? -xShift : RENDER_SIZE;

        int yGenStart = yShift <= 0 ? 0 : RENDER_SIZE-yShift;
        int yGenStop = yShift < 0 ? -yShift : RENDER_SIZE;

        int zGenStart = zShift <= 0 ? 0 : RENDER_SIZE-zShift;
        int zGenStop = zShift < 0 ? -zShift : RENDER_SIZE;


        for (int x = xGenStart; x < xGenStop; x++)
            for (int y = yGenStart; y < yGenStop; y++)
                for (int z = zGenStart; z < zGenStop; z++)
                {
                    std::cout << "\tGenerating " 
                        << x+_x-_offset << "," << y+_y-_offset << "," << z+_z-_offset
                        << " at " << x << "," << y << "," << z << std::endl; 
                    _chunks[x][y][z]->reset();
                    _chunks[x][y][z]->generate(x+_x-_offset,y+_y-_offset,z+_z-_offset);
                    _chunks[x][y][z]->createMesh();
                }
    }

    Vector3D newPosition(distx, disty, distz);

    std::cout << "\tRender coords " << _x << " " << _y << " " << _z << std::endl;
    std::cout << "\tNew coords" << distx << " " << disty << " " << distz << std::endl;
    std::cout << "\tExiting ChunkManager.update()" << std::endl;

    return newPosition;
}

/**
 * Returns true if the Block at the position is active.
 */
bool ChunkManager::isActive(Vector3D position)
{
    std::cout << "\tEntering ChunkManager.isActive()" << std::endl;

    // Offsets from the center Chunk
    // Block # / CHUNK_SIZE = Chunk offset
    int offsetx = int(position.x / BLOCK_WIDTH) / CHUNK_WIDTH; 
    int offsety = int(position.y / BLOCK_HEIGHT) / CHUNK_HEIGHT;
    int offsetz = int(position.z / BLOCK_LENGTH) / CHUNK_LENGTH; 

    // Distance from (0,0) in center Chunk
    // int(position.x / BLOCK_WIDTH) = Distance offset
    int distx = int(position.x / BLOCK_WIDTH);
    int disty = int(position.y / BLOCK_HEIGHT);
    int distz = int(position.z / BLOCK_LENGTH);

    // Translate to a position within Chunk
    while (distx >= CHUNK_WIDTH) distx -= CHUNK_WIDTH;
    while (distx < 0) distx += CHUNK_WIDTH;
    while (disty >= CHUNK_HEIGHT) disty -= CHUNK_HEIGHT;
    while (disty < 0) disty += CHUNK_HEIGHT;
    while (distz >= CHUNK_LENGTH) distz -= CHUNK_LENGTH;
    while (distz < 0) distz += CHUNK_LENGTH;

    std::cout << "\tOffsets: " 
        << offsetx << " " << offsety << " " << offsetz << std::endl;
    std::cout << "\tChunk: " 
        << _offset+offsetx << " " << _offset+offsety << " "
        << _offset+offsetz << std::endl;
    std::cout << "\tCoord: " 
        << distx << " " << disty << " "
        << distz << std::endl;

    bool ret = _chunks[_offset+offsetx][_offset+offsety][_offset+offsetz]->isActive(distx, disty, distz);

    std::cout << "\tExiting ChunkManager.isActive()" << std::endl;

    return ret; 
}


#endif
