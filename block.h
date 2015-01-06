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
        void setRenderOffset(float dx, float dy, float dz);

    private:
        // BlockType default is Air
        BlockType _blockType;

        // Face coloring methods.
        void _setFrontColor();
        void _setBackColor();
        void _setLeftColor();
        void _setRightColor();
        void _setTopColor();
        void _setBottomColor();
        void _setDefaultColor();

        // Render offsets.
        float _dx = 0.0f;
        float _dy = 0.0f;
        float _dz = 0.0f;

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

    _setFrontColor();

    // Front
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f-height,                                  _dz + 0.0f+length); //BR
    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f+length); //TR
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f-height,                                  _dz + 0.0f+length); // BL

    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f+length); // TL
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f-height,                                  _dz + 0.0f+length); // BL
    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f+length); //TR
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

    _setBackColor();

    // Back
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f-height,                                  _dz + 0.0f-length); // BR
    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f-length); //TR
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f-height,                                  _dz + 0.0f-length); // BL

    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f-length); // TL
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f-height,                                  _dz + 0.0f-length); // BL
    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f-length); //TR
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

    _setLeftColor();

    // Left
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f-height,                                  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); //FB
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); //FT
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f-height,                                  _dz + 0.0f-length); //BB

    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f-length); //BT
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f-height,                                  _dz + 0.0f-length); //BB
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); //FT

}

void Block::createRight()
{
    createRight(1, 1);
}

void Block::createRight(int heightMultiplier, int lengthMultiplier)
{
    heightMultiplier -= 1;
    lengthMultiplier -= 1;

    _setRightColor();

    // Right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(_dx + 0.0f+width,                                _dy + 0.0f-height,                                  _dz + 0.0f-length); // BB
    glVertex3f(_dx + 0.0f+width,                                _dy + 0.0f-height,                                  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // BT
    glVertex3f(_dx + 0.0f+width,                                _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FT

    glVertex3f(_dx + 0.0f+width,                                _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FT
    glVertex3f(_dx + 0.0f+width,                                _dy + 0.0f+height + heightMultiplier*BLOCK_HEIGHT,  _dz + 0.0f-length); // FB
    glVertex3f(_dx + 0.0f+width,                                _dy + 0.0f-height,                                  _dz + 0.0f-length); // BB
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

    _setTopColor();

    // Top
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f+height,                                  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FL
    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f+height,                                  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FR
    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f+height,                                  _dz + 0.0f-length); //BR

    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f+height,                                  _dz + 0.0f-length); //BR
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f+height,                                  _dz + 0.0f-length); // BL
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f+height,                                  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FL
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

    _setBottomColor();

    // Bottom
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f-height,                                  _dz + 0.0f-length); // BL
    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f-height,                                  _dz + 0.0f-length); // BR
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f-height,                                  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FL


    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f-height,                                  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FR
    glVertex3f(_dx + 0.0f-width,                                _dy + 0.0f-height,                                  _dz + 0.0f+length + lengthMultiplier*BLOCK_LENGTH); // FL
    glVertex3f(_dx + 0.0f+width + widthMultiplier*BLOCK_WIDTH,  _dy + 0.0f-height,                                  _dz + 0.0f-length); // BR
}

void Block::setType(BlockType blockType)
{
    _blockType = blockType;
}

BlockType Block::getType()
{
    return _blockType;
}

void Block::setRenderOffset(float dx, float dy, float dz)
{
    _dx = dx;
    _dy = dy;
    _dz = dz;
}

void Block::_setFrontColor()
{
    switch(_blockType)
    {
        case BlockType_Grass:
            glColor3f(0.0f , 0.7647f, 0.1725f);
            break;
        default:
            _setDefaultColor();
    }
}

void Block::_setBackColor()
{
    switch(_blockType)
    {
        case BlockType_Grass:
            glColor3f(0.0, 0.7647, 0.1725);
            break;
        default:
            _setDefaultColor();
    }
}

void Block::_setLeftColor()
{
    switch(_blockType)
    {
        case BlockType_Grass:
            glColor3f(0.0, 0.7647, 0.1725);
            break;
        default:
            _setDefaultColor();
    }
}

void Block::_setRightColor()
{
    switch(_blockType)
    {
        case BlockType_Grass:
            glColor3f(0.0, 0.7647, 0.1725);
            break;
        default:
            _setDefaultColor();
    }
}

void Block::_setTopColor()
{
    switch(_blockType)
    {
        case BlockType_Grass:
            glColor3f(0.0, 0.8588, 0.1137);
            break;
        default:
            _setDefaultColor();
    }
}

void Block::_setBottomColor()
{
    switch(_blockType)
    {
        case BlockType_Grass:
           glColor3f(0.0, 0.8588, 0.1137);
           break;
        default:
            _setDefaultColor();
    }
}

void Block::_setDefaultColor()
{
    glColor3f(1.0f, 1.0f, 1.0f);
}


#endif
