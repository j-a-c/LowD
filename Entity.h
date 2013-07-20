#ifndef ENTITY_H
#define ENTITY_H

#include "math/Vector3D.h"

/**
 * Class to represent a movable entity in the world
 */
class Entity
{
    public:
        Entity(){}
        virtual ~Entity(){}

        virtual bool isFalling() = 0;
        virtual void setFalling(bool) = 0;
        virtual Vector3D getPosition() = 0;
        virtual void setPosition(Vector3D) = 0;
};

#endif
