#include <GL/gl.h>

#include "block.h"

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

