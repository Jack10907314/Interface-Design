#include "block.h"

Block::Block()
{
    setImage(":/new/prefix1/picture/block.png");
    setImageScaled(size, size);
}

Block::Block(QString path, int x, int y):Object(path, x, y)
{

}

