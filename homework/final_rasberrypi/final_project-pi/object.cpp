#include "object.h"

Object::Object()
{
    X = 0;
    Y = 0;
}

Object::Object(QString path, int x, int y)
{
    Image = QImage(path);
    setImageScaled(size,size);

    X = x;
    Y = y;
}

void Object::setImage(QString path)
{
    Image = QImage(path);
}

void Object::setImageScaled(int x,int y)
{
    Image = Image.scaled(x,y);
}

bool Object::isOverlap(Object* object)
{
    bool whether = false;

    if( (object->X <= this->X) && (this->X <= object->X+object->size-1) &&
        (object->Y <= this->Y) && (this->Y <= object->Y+object->size-1))
        whether = true;

    if( (object->X <= this->X+this->size-1) && (this->X+this->size-1 <= object->X+object->size-1) &&
        (object->Y <= this->Y) && (this->Y <= object->Y+object->size-1))
        whether = true;

    if( (object->X <= this->X) && (this->X <= object->X+object->size-1) &&
        (object->Y <= this->Y+this->size-1) && (this->Y+this->size-1 <= object->Y+object->size-1))
        whether = true;

    if( (object->X <= this->X+this->size-1) && (this->X+this->size-1 <= object->X+object->size-1) &&
        (object->Y <= this->Y+this->size-1) && (this->Y+this->size-1 <= object->Y+object->size-1))
        whether = true;

    //printf("this.x = %d  this.y = %d\n",this->X,this->Y);
    return whether;
}


