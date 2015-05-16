#ifndef BLOCKS_BLOCK_H
#define BLOCKS_BLOCK_H

#include "block_type.h"
#include "../constants.h"

/**
 * A block is a voxel and is represents data for a single 3D cube.
 *
 * @author Joshua A. Campbell
 *
*/



class Block
{
    public:
        virtual ~Block();

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
        void setRenderOffset(float dx, float dy, float dz);

    private: 
        // Render offsets.
        float _dx = 0.0f;
        float _dy = 0.0f;
        float _dz = 0.0f;

        // Half the length, width, height
        static constexpr float length = BLOCK_LENGTH / 2.0f;
        static constexpr float height = BLOCK_HEIGHT / 2.0f;
        static constexpr float width = BLOCK_WIDTH / 2.0f;

    protected:
        // BlockType default is Air
        BlockType _blockType = BlockType_Air;

        // Face coloring methods.
        virtual void _setFrontColor() const = 0;
        virtual void _setBackColor() const = 0;
        virtual void _setLeftColor() const = 0;
        virtual void _setRightColor() const = 0;
        virtual void _setTopColor() const = 0;
        virtual void _setBottomColor() const = 0;

};

#endif
