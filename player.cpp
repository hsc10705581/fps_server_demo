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

float Player::getHp()
{
    return this->hp;
}

void Player::setHp(float hp)
{
    this->hp = hp;
}

json Player::toJson()
{
    json ret;
    json position = this->position.toJson();
    json rotation = this->rotation.toJson();
    ret["position_x"] = position["x"];
    ret["position_y"] = position["y"];
    ret["position_z"] = position["z"];
    ret["rotation_x"] = rotation["x"];
    ret["rotation_y"] = rotation["y"];
    ret["rotation_z"] = rotation["z"];
    ret["hp"] = hp;
    return ret;
}

json Player::toJson(int id)
{
    json ret;
    json position = this->position.toJson();
    json rotation = this->rotation.toJson();
    ret["position_x"] = position["x"];
    ret["position_y"] = position["y"];
    ret["position_z"] = position["z"];
    ret["rotation_x"] = rotation["x"];
    ret["rotation_y"] = rotation["y"];
    ret["rotation_z"] = rotation["z"];
    ret["hp"] = hp;
    ret["id"] = id;
    return ret;
}

