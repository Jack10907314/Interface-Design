#include "props.h"

Props::Props()
{
    setImage(":/new/prefix1/picture/props_power.png");
    setImageScaled(size, size);
}

Props::Props(int props)
{
    switch(props)
    {
        case POWER:
            setImage(":/new/prefix1/picture/props_power.png");
            props_id = POWER;
        break;
        case BOMB:
            setImage(":/new/prefix1/picture/props_bomb.png");
            props_id = BOMB;
        break;
        case SPEED:
            setImage(":/new/prefix1/picture/props_speed.png");
            props_id = SPEED;
        break;
    }
    setImageScaled(size, size);
}

Props::Props(QString path, int x, int y):Object(path, x, y)
{

}
