#ifndef PLAYER_H 
#define PLAYER_H 

#include "Entity.h"
#include "datastructures/Vector3D.h"

static const float _maxVerticalAngle = 80.0f;

// Player is 1u wide, 2u tall, 1u long.
// Player camera is 1.75u from bottom of player

/**
 * Class to represent the player
 */
class Player : public Entity 
{
    public:
        Player();
        ~Player();

        bool isFalling();
        void setFalling(bool isFalling);
        Vector3D getPosition();
        void setPosition(Vector3D position);

        void offsetOrientation(float vertAngle, float horizAngle);

        float up(void);
        float side(void);



    private:
        // Angle side, and up
        float _horizontalAngle, _verticalAngle;
        // x, y, z coordinates
        Vector3D _position;

        bool _isFalling;

};

Player::Player()
{
    _horizontalAngle = 0.0f;
    _verticalAngle = 0.0f;
    _isFalling = false;
}

Player::~Player()
{

}

void Player::setFalling(bool isFalling)
{
    _isFalling = isFalling;
}

bool Player::isFalling()
{
    return _isFalling;
}

Vector3D Player::getPosition()
{
    return _position;
}

void Player::setPosition(Vector3D position)
{
    _position = position;
}

void Player::offsetOrientation(float hAngle, float vAngle)
{
    _horizontalAngle += hAngle;
    while(_horizontalAngle > 360.0f) _horizontalAngle -= 360.0;
    while(_horizontalAngle < 0.0f) _horizontalAngle += 360.0;

    _verticalAngle += vAngle;
    if(_verticalAngle > _maxVerticalAngle) _verticalAngle = _maxVerticalAngle;
    if(_verticalAngle < -_maxVerticalAngle) _verticalAngle = -_maxVerticalAngle;
}

float Player::up()
{
    return _verticalAngle;
}

float Player::side()
{
    return _horizontalAngle;
}


#endif
