#ifndef BLOCK_H
#define BLOCK_H

#include "block_type.h"
#include "constants.h"

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

        // Default face creation.
        void createFront();
        void createBack();
        void createLeft();
        void createRight();
        void createTop();
        void createBottom();

        // Increase width/height of faces using the specified multiplier.
        void createFront(int, int);
        void createBack(int, int);
        void createLeft(int, int);
        void createRight(int, int);
        void createTop(int, int);
        void createBottom(int, int);


        BlockType getType();
        void setType(BlockType blockType);

    private:
        // BlockType default is Air
        BlockType _blockType;

        // Half the length, width, height
        static constexpr float length = BLOCK_LENGTH / 2.0f;
        static constexpr float height = BLOCK_HEIGHT / 2.0f;
        static constexpr float width = BLOCK_WIDTH / 2.0f;
};

Block::Block()
{
    // Each block is by default Air until the ChunkManager changes its
    _blockType = BlockType_Air;

}

Block::~Block()
{

}

void Block::createFront()
{
    createFront(1, 1);
}

void Block::createFront(int heightMultiplier, int widthMultiplier)
{
    // Decrement so we don't count this block when drawing the extensions.
    widthMultiplier -= 1;
    heightMultiplier -= 1;

    glColor3f(0.0, 1.0, 0.0);

    // Front
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f-height, 0.0f+length); //BR
    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f+length); //TR
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length); // BL

    glVertex3f(0.0f-width, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f+length); // TL
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length); // BL
    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f+length); //TR
}

void Block::createBack()
{
    createBack(1, 1);
}

void Block::createBack(int heightMultiplier, int widthMultiplier)
{
    // Decrement so we don't count this block when drawing the extensions.
    widthMultiplier -= 1;
    heightMultiplier -= 1;

    glColor3f(0.0, 1.0, 0.0);

    // Back
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f-height, 0.0f-length); // BR
    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f-length); //TR
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); // BL

    glVertex3f(0.0f-width, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f-length); // TL
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); // BL
    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f-length); //TR
}

void Block::createLeft()
{
    createLeft(1, 1);
}

void Block::createLeft(int heightMultiplier, int lengthMultiplier)
{
    // Decrement so we don't count this block when drawing the extensions.
    heightMultiplier -= 1;
    lengthMultiplier -= 1;

    glColor3f(0.0, 1.0, 0.0);

    // Left
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); //FB
    glVertex3f(0.0f-width, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); //FT
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); //BB

    glVertex3f(0.0f-width, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f-length); //BT
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); //BB
    glVertex3f(0.0f-width, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); //FT

}

void Block::createRight()
{
    createRight(1, 1);
}

void Block::createRight(int heightMultiplier, int lengthMultiplier)
{
    heightMultiplier -= 1;
    lengthMultiplier -= 1;

    glColor3f(0.0, 1.0, 0.0);

    // Right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f+width, 0.0f-height, 0.0f-length); // BB
    glVertex3f(0.0f+width, 0.0f-height, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // BT
    glVertex3f(0.0f+width, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FT

    glVertex3f(0.0f+width, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FT
    glVertex3f(0.0f+width, 0.0f+height + heightMultiplier*BLOCK_HEIGHT, 0.0f-length); // FB
    glVertex3f(0.0f+width, 0.0f-height, 0.0f-length); // BB
}

void Block::createTop()
{
    createTop(1, 1);
}

void Block::createTop(int widthMultiplier, int lengthMultiplier)
{

    // Decrement so we don't count this block when drawing the extensions.
    widthMultiplier -= 1;
    lengthMultiplier -= 1;

    glColor3f(0.0, 1.0, 0.0);

    // Top
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f-width, 0.0f+height, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FL
    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f+height, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FR
    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f+height, 0.0f-length); //BR

    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f+height, 0.0f-length); //BR
    glVertex3f(0.0f-width, 0.0f+height, 0.0f-length); // BL
    glVertex3f(0.0f-width, 0.0f+height, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FL
}

void Block::createBottom()
{
    createBottom(1, 1);
}

void Block::createBottom(int widthMultiplier, int lengthMultiplier)
{

    // Decrement so we don't count this block when drawing the extensions.
    widthMultiplier -= 1;
    lengthMultiplier -= 1;

    glColor3f(0.0, 1.0, 0.0);

    // Bottom
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f-width, 0.0f-height, 0.0f-length); // BL
    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f-height, 0.0f-length); // BR
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FL


    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f-height, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FR
    glVertex3f(0.0f-width, 0.0f-height, 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FL
    glVertex3f(0.0f+width + widthMultiplier*BLOCK_WIDTH, 0.0f-height, 0.0f-length); // BR
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