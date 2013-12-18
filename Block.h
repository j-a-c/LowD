#ifndef BLOCK_H
#define BLOCK_H

#include "BlockType.h"
#include "Constants.h"

/**
 * A block is a voxel and is represents data for a single 3D cube.
 *
 * @author Joshua A. Campbell
 *
*/



class Block
{
    public:
        Block();
        ~Block();

        void createBlock();

        void createFront();
        void createBack();
        void createLeft();
        void createRight();
        void createTop();
        void createBottom();


        BlockType getType();
        void setType(BlockType blockType);

    private:
        // BlockType default is Air
        BlockType _blockType;
};

Block::Block()
{
    // Each block is by default Air until the ChunkManager changes its
    _blockType = BlockType_Air;
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

void Block::createFront()
{
    // Half the length, width, height
    float length = BLOCK_LENGTH / 2.0f;
    float height = BLOCK_HEIGHT / 2.0f;
    float width = BLOCK_WIDTH / 2.0f;
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //glBegin(GL_QUADS);
    
    glColor3f(0.0, 1.0, 0.0);

    // Front
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f+width, 0.0f-height, 0.0f+length); //BR
    glVertex3f(0.0f+width, 0.0f+height, 0.0f+length); //TR
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length); // BL

    glVertex3f(0.0f-width, 0.0f+height, 0.0f+length); // TL
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length); // BL
    glVertex3f(0.0f+width, 0.0f+height, 0.0f+length); //TR


    //glEnd();
}

void Block::createBack()
{
    // Half the length, width, height
    float length = BLOCK_LENGTH / 2.0f;
    float height = BLOCK_HEIGHT / 2.0f;
    float width = BLOCK_WIDTH / 2.0f;
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //glBegin(GL_QUADS);
    
    glColor3f(0.0, 1.0, 0.0);

    // Back
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f+width, 0.0f-height, 0.0f-length); // BR
    glVertex3f(0.0f+width, 0.0f+height, 0.0f-length); //TR
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); // BL

    glVertex3f(0.0f-width, 0.0f+height, 0.0f-length); // TL
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); // BL
    glVertex3f(0.0f+width, 0.0f+height, 0.0f-length); //TR

    //glEnd();
}

void Block::createLeft()
{
    // Half the length, width, height
    float length = BLOCK_LENGTH / 2.0f;
    float height = BLOCK_HEIGHT / 2.0f;
    float width = BLOCK_WIDTH / 2.0f;
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //glBegin(GL_QUADS);
    
    glColor3f(0.0, 1.0, 0.0);

    // Left
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length); //FB
    glVertex3f(0.0f-width, 0.0f+height, 0.0f+length); //FT
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); //BB

    glVertex3f(0.0f-width, 0.0f+height, 0.0f-length); //BT
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); //BB
    glVertex3f(0.0f-width, 0.0f+height, 0.0f+length); //FT

    //glEnd();
}

void Block::createRight()
{
    // Half the length, width, height
    float length = BLOCK_LENGTH / 2.0f;
    float height = BLOCK_HEIGHT / 2.0f;
    float width = BLOCK_WIDTH / 2.0f;
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //glBegin(GL_QUADS);
    
    glColor3f(0.0, 1.0, 0.0);

    // Right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f+width, 0.0f-height, 0.0f-length); // BB
    glVertex3f(0.0f+width, 0.0f-height, 0.0f+length); // BT
    glVertex3f(0.0f+width, 0.0f+height, 0.0f+length); // FT

    glVertex3f(0.0f+width, 0.0f+height, 0.0f+length); // FT
    glVertex3f(0.0f+width, 0.0f+height, 0.0f-length); // FB
    glVertex3f(0.0f+width, 0.0f-height, 0.0f-length); // BB

    //glEnd();
}

void Block::createTop()
{
    // Half the length, width, height
    float length = BLOCK_LENGTH / 2.0f;
    float height = BLOCK_HEIGHT / 2.0f;
    float width = BLOCK_WIDTH / 2.0f;
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //glBegin(GL_QUADS);
    
    glColor3f(0.0, 1.0, 0.0);

    // Top
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f-width, 0.0f+height, 0.0f+length); // FL
    glVertex3f(0.0f+width, 0.0f+height, 0.0f+length); // FR
    glVertex3f(0.0f+width, 0.0f+height, 0.0f-length); //BR

    glVertex3f(0.0f+width, 0.0f+height, 0.0f-length); //BR
    glVertex3f(0.0f-width, 0.0f+height, 0.0f-length); // BL
    glVertex3f(0.0f-width, 0.0f+height, 0.0f+length); // FL

    //glEnd();
}

void Block::createBottom()
{
    // Half the length, width, height
    float length = BLOCK_LENGTH / 2.0f;
    float height = BLOCK_HEIGHT / 2.0f;
    float width = BLOCK_WIDTH / 2.0f;
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //glBegin(GL_QUADS);
    
    glColor3f(0.0, 1.0, 0.0);

    // Bottom
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); // BL
    glVertex3f(0.0f+width, 0.0f-height, 0.0f-length); // BR
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length); // FL


    glVertex3f(0.0f+width, 0.0f-height, 0.0f+length); // FR
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length); // FL
    glVertex3f(0.0f+width, 0.0f-height, 0.0f-length); // BR

    //glEnd();
}

void Block::setType(BlockType blockType)
{
    _blockType = blockType;
}

BlockType Block::getType()
{
    return _blockType;
}

#endif
