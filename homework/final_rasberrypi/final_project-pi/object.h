#ifndef OBJECT_H
#define OBJECT_H

#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QDebug>

const int object_size = 40;

class Object
{
public:
    Object();
    Object(QString path, int x, int y);

    int size = object_size;

    int X;
    int Y;

    QImage Image;

    void setImage(QString path);
    void setImageScaled(int x,int y);
    bool isOverlap(Object* object);

protected:

};

#endif // OBJECT_H
