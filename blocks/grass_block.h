#ifndef BLOCKS_GRASSBLOCK_H
#define BLOCKS_GRASSBLOCK_H

#include "block.h"

class GrassBlock : public Block
{
    public:
        static GrassBlock* getBlock();
        ~GrassBlock();

    private:
        GrassBlock();

    protected:
        void _setFrontColor() const override;
        void _setBackColor() const override;
        void _setLeftColor() const override;
        void _setRightColor() const override;
        void _setTopColor() const override;
        void _setBottomColor() const override;
};

#endif
