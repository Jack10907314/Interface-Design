#include "wall.h"

Wall::Wall()
{
    setImage(":/new/prefix1/picture/wall.png");
    setImageScaled(size, size);
}

Wall::Wall(QString path, int x, int y):Object(path, x, y)
{

}
