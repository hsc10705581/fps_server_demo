#include "player.h"

Player::Player()
{
    //position = Vector3(500.0f, 400.0f, 0.0f);
    position = Vector3(0.0f, 0.0f, 0.0f);
    rotation = Vector3(0.0f, 0.0f, 0.0f);
    hp = 100.0f;
    teamId = 0;
}

Player::~Player()
{
    
}


void Player::setPosition(Vector3 posi)
{
    position = posi;
}

void Player::setRotation(Vector3 rota)
{
    rotation = rota;
}

json Player::toJson()
{
    json ret;
    ret["position"] = this->position.toJson();
    ret["rotation"] = this->rotation.toJson();
    ret["hp"] = hp;
    return ret;
}
