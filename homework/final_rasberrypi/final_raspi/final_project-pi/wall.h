#ifndef WALL_H
#define WALL_H

#include <QString>

#include "object.h"


class Wall : public Object
{
public:
    Wall();
    Wall(QString path, int x, int y);
};

#endif // WALL_H
