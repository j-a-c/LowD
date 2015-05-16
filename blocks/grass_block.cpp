#include <GL/gl.h>

#include "grass_block.h"

GrassBlock::GrassBlock()
{
    _blockType = BlockType_Grass;
}

GrassBlock::~GrassBlock()
{

}

GrassBlock* GrassBlock::getBlock()
{
    static GrassBlock instance;
    return &instance;
}

void GrassBlock::_setFrontColor() const
{
    glColor3f(0.0f , 0.7647f, 0.1725f);
}

void GrassBlock::_setBackColor() const
{
    glColor3f(0.0, 0.7647, 0.1725);
}

void GrassBlock::_setLeftColor() const
{
    glColor3f(0.0, 0.7647, 0.1725);
}

void GrassBlock::_setRightColor() const
{
    glColor3f(0.0, 0.7647, 0.1725);
}

void GrassBlock::_setTopColor() const
{
    glColor3f(0.0, 0.8588, 0.1137);
}

void GrassBlock::_setBottomColor() const
{
   glColor3f(0.0, 0.8588, 0.1137);
}
