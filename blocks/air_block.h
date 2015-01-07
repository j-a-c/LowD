#ifndef BLOCKS_AIRBLOCK_H
#define BLOCKS_AIRBLOCK_H

#include "block.h"

class AirBlock : public Block
{
    public:
        static AirBlock* getBlock();
        ~AirBlock();

    private:
        AirBlock();

    protected:
        void _setFrontColor() const override;
        void _setBackColor() const override;
        void _setLeftColor() const override;
        void _setRightColor() const override;
        void _setTopColor() const override;
        void _setBottomColor() const override;
};

AirBlock::AirBlock()
{
    _blockType = BlockType_Air;
}

AirBlock::~AirBlock()
{

}

AirBlock* AirBlock::getBlock()
{
    static AirBlock instance;
    return &instance;
}

void AirBlock::_setFrontColor() const
{

}

void AirBlock::_setBackColor() const
{

}

void AirBlock::_setLeftColor() const
{

}

void AirBlock::_setRightColor() const
{

}

void AirBlock::_setTopColor() const
{

}

void AirBlock::_setBottomColor() const
{

}

#endif
