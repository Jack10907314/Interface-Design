#ifndef BLOCK_H
#define BLOCK_H

#include <QString>

#include "object.h"

class Block : public Object
{
public:
    Block();
    Block(QString path, int x, int y);
};

#endif // BLOCK_H
