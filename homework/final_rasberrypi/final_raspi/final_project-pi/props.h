#ifndef PROPS_H
#define PROPS_H

#include "object.h"

#define POWER 1
#define BOMB 2
#define SPEED 3
class Props : public Object
{
public:
    Props();
    Props(int props);
    Props(QString path, int x, int y);

    int props_id = POWER;
};

#endif // PROPS_H
