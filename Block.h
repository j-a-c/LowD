#ifndef BLOCK_H
#define BLOCK_H

#include "Constants.h"

/**
 * A block is a voxel and is represents data for a single 3D cube.
 *
 * @author Joshua A. Campbell
 *
*/

// TODO
enum BlockType
{
	BlockType_Default = 0,

	BlockType_Grass,
	BlockType_Dirt,
	BlockType_Water,
	BlockType_Stone,
    BlockType_Wood,
    BlockType_Sand,

    BlockType_NumTypes,
};

class Block
{
    public:
        Block();
        ~Block();

        void createBlock();

        bool isActive();
        void setActive(bool active);

    private:
        // Is block active
        bool _active;

        BlockType _blockType;
};

Block::Block()
{
    // Each block is by default inactive until the ChunkManager
    // sets it to active
    _active = false;
}

Block::~Block()
{

}

/**
 * Creates an instance of a single cube.
 * Centered around (0,0,0)
 */
void Block::createBlock()
{
    // Half the length, width, height
    float length = BLOCK_LENGTH / 2.0f;
    float height = BLOCK_HEIGHT / 2.0f;
    float width = BLOCK_WIDTH / 2.0f;
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBegin(GL_QUADS);
    
    glColor3f(0.0, 1.0, 0.0);


    // Back
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f+width, 0.0f-height, 0.0f-length); // BR
    glVertex3f(0.0f+width, 0.0f+height, 0.0f-length); //TR
    glVertex3f(0.0f-width, 0.0f+height, 0.0f-length); // TL
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); // BL

    // Front
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f+width, 0.0f-height, 0.0f+length); //BR
    glVertex3f(0.0f+width, 0.0f+height, 0.0f+length); //TR
    glVertex3f(0.0f-width, 0.0f+height, 0.0f+length); // TL
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length); // BL

    // Right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f+width, 0.0f-height, 0.0f-length); // BB
    glVertex3f(0.0f+width, 0.0f-height, 0.0f+length); // BT
    glVertex3f(0.0f+width, 0.0f+height, 0.0f+length); // FT
    glVertex3f(0.0f+width, 0.0f+height, 0.0f-length); // FB

    // Left
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length); //FB
    glVertex3f(0.0f-width, 0.0f+height, 0.0f+length); //FT
    glVertex3f(0.0f-width, 0.0f+height, 0.0f-length); //BT
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); //BB

    // Bottom
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); // BL
    glVertex3f(0.0f+width, 0.0f-height, 0.0f-length); // BR
    glVertex3f(0.0f+width, 0.0f-height, 0.0f+length); // FR
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length); // FL

    // Top
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f-width, 0.0f+height, 0.0f+length); // FL
    glVertex3f(0.0f+width, 0.0f+height, 0.0f+length); // FR
    glVertex3f(0.0f+width, 0.0f+height, 0.0f-length); //BR
    glVertex3f(0.0f-width, 0.0f+height, 0.0f-length); // BL

    glEnd();

}

void Block::setActive(bool active)
{
    _active = active;
}

bool Block::isActive()
{
    return _active;
}

#endif
