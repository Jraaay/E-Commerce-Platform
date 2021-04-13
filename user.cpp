#include "user.h"


string userClass::getJson()
{
    json j;
    j["uid"] = this->uid;
    j["name"] = this->name;
    j["balance"] = this->balance;
    j["type"] = this->type;
    j["password"] = this->password;
    return j.dump();
}
