#ifndef BLOCKS_GRASSBLOCK_H
#define BLOCKS_GRASSBLOCK_H

#include "block.h"

class GrassBlock : public Block
{
    public:
        GrassBlock();
        ~GrassBlock();

    private:

    protected:
        void _setFrontColor() const override;
        void _setBackColor() const override;
        void _setLeftColor() const override;
        void _setRightColor() const override;
        void _setTopColor() const override;
        void _setBottomColor() const override;
};

GrassBlock::GrassBlock()
{

}

GrassBlock::~GrassBlock()
{

}

#include <iostream>
void GrassBlock::_setFrontColor() const
{
    std::cout << "6.1.2" << std::endl;
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

#endif
