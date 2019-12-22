#include "common.h"

//#include "nlohmann/json.hpp"

class Player
{
private:
    Vector3 position;
    Vector3 rotation;
    float hp;
    int teamId;
    
public:
    Player();
    ~Player();
    void setPosition(Vector3 posi);
    void setRotation(Vector3 rota);
    json toJson();
};