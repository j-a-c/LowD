#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "biome_manager.h"
#include "chunk.h"
#include "constants.h"
#include "ds/vector3D.h"

#include <iostream>
#include <math.h>
#include <utility>
#include <vector>


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
        ChunkManager(BiomeManager* biome_manager);
        ~ChunkManager();

        void initializeWorld();

        void render();

        Vector3D update(Vector3D position);

        bool isActive(Vector3D position);

    private:
        BiomeManager* _biome_manager = nullptr;

        //Chunks
        std::vector<Chunk> _nchunks;

        // Current chunk
        // Player starts off in chunk 0, 0, 0 - before being moved due to
        // terrain
        int _x;
        int _y;
        int _z;

        // Offset bc the player is assumed to be in the center of render cube
        const int _offset = RENDER_SIZE / 2;

        // Render offset bc player is in center of render cube
        const int _renderOffsetX = int(RENDER_SIZE / 2) * CHUNK_WIDTH * BLOCK_HEIGHT;
        const int _renderOffsetY = int(RENDER_SIZE / 2) * CHUNK_HEIGHT * BLOCK_LENGTH;
        const int _renderOffsetZ = int(RENDER_SIZE / 2) * CHUNK_LENGTH * BLOCK_WIDTH;


        // Height, length, width in units
        const int _height = CHUNK_HEIGHT* BLOCK_HEIGHT;
        const int _length = CHUNK_LENGTH * BLOCK_LENGTH;
        const int _width = CHUNK_WIDTH * BLOCK_WIDTH;

        // Number of triangles currently rendered.
        int _numberOfTrianglesRendered = 0;


};

ChunkManager::ChunkManager(BiomeManager* biome_manager) : _x{0}, _y{0}, _z{0}
{
    _biome_manager = biome_manager;

    // Initialize the chunks
    for(int i = 0; i < RENDER_SIZE*RENDER_SIZE*RENDER_SIZE; i++)
        _nchunks.push_back(Chunk());
}

ChunkManager::~ChunkManager()
{

}


// TODO
void ChunkManager::initializeWorld()
{
    for (int x = 0; x < RENDER_SIZE; x++)
    {
        for (int y = 0; y < RENDER_SIZE; y++)
        {
            for (int z = 0; z < RENDER_SIZE; z++)
            {
                int eff_x = x+_x-_offset;
                int eff_y = y+_y-_offset;
                int eff_z = z+_z-_offset;


                // Get the biome for this chunk.
                Biome* biome = _biome_manager->get_biome(eff_x, eff_y, eff_x);
                // Generate the biome.
                _nchunks[toChunkIndex(x,y,z)].generate(biome, eff_x, eff_y, eff_x);                

                _nchunks[toChunkIndex(x,y,z)].createMesh();
            }
        }
    }
}

/**
 * Renders all Chunks in the ChunkManager.
 */
void ChunkManager::render()
{
    int numTrianglesJustRendered = 0;

    // TODO render spherically from center outwards
    // render visible
    for (int x = 0; x < RENDER_SIZE; x++)
    {
        for (int y = 0; y < RENDER_SIZE; y++)
        {
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
                _nchunks[toChunkIndex(x,y,z)].render();

                if (DEBUG)
                {
                    numTrianglesJustRendered += _nchunks[toChunkIndex(x,y,z)].getNumberOfTriangles();
                }
                
                glPopMatrix();
            }
        }
    }

    if (DEBUG && (numTrianglesJustRendered != _numberOfTrianglesRendered))
    {
        _numberOfTrianglesRendered = numTrianglesJustRendered;
        std::cout << "Number of triangles rendered: " << _numberOfTrianglesRendered << std::endl; 
    }
}
/**
 * Updates the ChunkManager given the player's interpolated position.
 * Update includes shifting the render cube and generating new terrain.
 * Returns a position that is in the center of the render cube.
 */
Vector3D ChunkManager::update(Vector3D position)
{
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
        // Calculate in-place swap limits
        int xShiftStart = xShift >= 0 ? 0 : -xShift;
        int xShiftStop = xShift <= 0 ? RENDER_SIZE : RENDER_SIZE - xShift;

        int yShiftStart = yShift >= 0 ? 0 : -yShift;
        int yShiftStop = yShift <= 0 ? RENDER_SIZE : RENDER_SIZE - yShift;

        int zShiftStart = zShift >= 0 ? 0 : -zShift;
        int zShiftStop = zShift <= 0 ? RENDER_SIZE : RENDER_SIZE - zShift;

        // Shift render cube
        for (int x = xShiftStart; x < xShiftStop; x++)
            for (int y = yShiftStart; y < yShiftStop; y++)
                for (int z = zShiftStart; z < zShiftStop; z++)
                {
                    int xSwap = xShift == 0 ? x : xShift > 0 ? x+xShift : x % (-xShift);
                    int ySwap = yShift == 0 ? y : yShift > 0 ? y+yShift : y % (-yShift);
                    int zSwap = zShift == 0 ? z : zShift > 0 ? z+zShift : x % (-zShift);

                    //swap =  _chunks[x][y][z];
                    //_chunks[x][y][z] =  _chunks[xSwap][ySwap][zSwap];
                    //_chunks[xSwap][ySwap][zSwap] = swap;
                    std::swap(_nchunks[toChunkIndex(x,y,z)], _nchunks[toChunkIndex(xSwap,ySwap,zSwap)]);
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
                    _nchunks[toChunkIndex(x,y,z)].reset();

                    int eff_x = x+_x-_offset;
                    int eff_y = y+_y-_offset;
                    int eff_z = z+_z-_offset;


                    // Get the biome for this chunk.
                    Biome* biome = _biome_manager->get_biome(eff_x, eff_y, eff_x);
                    // Generate the biome.
                    _nchunks[toChunkIndex(x,y,z)].generate(biome, eff_x, eff_y, eff_x);
                    // Create the mesh.
                    _nchunks[toChunkIndex(x,y,z)].createMesh();
                }
    }

    Vector3D newPosition(distx, disty, distz);

    return newPosition;
}

/**
 * Returns true if the Block at the position is active.
 */
bool ChunkManager::isActive(Vector3D position)
{
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

    bool ret = _nchunks[toChunkIndex(_offset+offsetx,_offset+offsety,_offset+offsetz)].isActive(distx, disty, distz);

    return ret; 
}


#endif
