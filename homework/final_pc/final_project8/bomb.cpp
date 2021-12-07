#include "bomb.h"

Bomb::Bomb()
{
    setImage(":/new/prefix1/picture/bomb.png");
    setImageScaled(size, size);
}

Bomb::Bomb(QString path, int x, int y):Object(path, x, y)
{

}

void Bomb::TimeTick(QVector<Bomb*> *vector_bomb, int index, QVector<Wall> *vector_wall, QVector<Block> *vector_block)
{
    if(put)
    {
        time_count++;
        if(time_count == 50)
        {
            setExplosion(vector_bomb, index, vector_wall, vector_block);
        }
    }

    if(explosion)
    {
        time_count++;
        if(time_count == 8)
        {
            setBomb();
        }
    }

}

void Bomb::setBomb()
{
    player1_block = false;
    player2_block = false;
    time_count = 0;
    explosion = false;
    setImage(":/new/prefix1/picture/bomb.png");
    setImageScaled(size, size);
}

void Bomb::setExplosion(QVector<Bomb*> *vector_bomb, int index, QVector<Wall> *vector_wall, QVector<Block> *vector_block)
{
    QSoundEffect *effect=new QSoundEffect;
    effect->setSource(QUrl::fromLocalFile(":/new/prefix1/sound/explosion.wav"));
    effect->setLoopCount(1);  //循环次数
    effect->setVolume(1.00f); //音量  0~1之间
    effect->play();
    put = false;
    explosion = true;
    once_explosion = true;
    time_count = 0;
    setImage(":/new/prefix1/picture/fire.png");
    setImageScaled(size, size);
    up_fire = bomb_power;
    down_fire = bomb_power;
    left_fire = bomb_power;
    right_fire = bomb_power;

    for(int j = 0; j < vector_block->size(); ++j)
    {
        (*vector_bomb)[index]->isExplode((*vector_block)[j]);
    }

    foreach(Wall wall, *vector_wall)
    {
        (*vector_bomb)[index]->isExplode(wall);
    }

    for(int i = 0; i < vector_bomb->size(); ++i)
    {
        if(i != index && (*vector_bomb)[i]->put)
        {
            bool exploded = (*vector_bomb)[index]->isExplode((*(*vector_bomb)[i]));
            if(exploded) (*vector_bomb)[i]->setExplosion(vector_bomb,i, vector_wall, vector_block);
        }
    }
}

bool Bomb::isExplode(Object object)
{
    bool whether = false;
    int original_x = X;
    int original_y = Y;

    if(this->isOverlap(&object))
        whether = true;

    for(int i = 1; i <= up_fire ; ++i)
    {
        this->Y -= object_size;
        if(this->isOverlap(&object))
        {
            up_fire = min(up_fire, i-1);
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    for(int i = 1; i <= down_fire ; ++i)
    {
        this->Y += object_size;
        if(this->isOverlap(&object))
        {
            down_fire = min(down_fire, i-1);
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    for(int i = 1; i <= left_fire ; ++i)
    {
        this->X -= object_size;
        if(this->isOverlap(&object))
        {
            left_fire = min(left_fire, i-1);
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    for(int i = 1; i <= right_fire ; ++i)
    {
        this->X += object_size;
        if(this->isOverlap(&object))
        {
            right_fire = min(right_fire, i-1);
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    return whether;
}


bool Bomb::isExplode_unchange(Object object)
{
    bool whether = false;
    int original_x = X;
    int original_y = Y;

    if(this->isOverlap(&object))
        whether = true;

    for(int i = 1; i <= up_fire ; ++i)
    {
        this->Y -= object_size;
        if(this->isOverlap(&object))
        {
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    for(int i = 1; i <= down_fire ; ++i)
    {
        this->Y += object_size;
        if(this->isOverlap(&object))
        {
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    for(int i = 1; i <= left_fire ; ++i)
    {
        this->X -= object_size;
        if(this->isOverlap(&object))
        {
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    for(int i = 1; i <= right_fire ; ++i)
    {
        this->X += object_size;
        if(this->isOverlap(&object))
        {
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    return whether;
}
/*bool Bomb::isExplodePlayer(Player player)
{
    bool whether = false;
    int original_x = X;
    int original_y = Y;

    if(this->isOverlap(&object))
        whether = true;

    for(int i = 1; i <= up_fire ; ++i)
    {
        this->Y -= object_size;
        if(this->isOverlap(&object))
        {
            up_fire = min(up_fire, i-1);
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    for(int i = 1; i <= down_fire ; ++i)
    {
        this->Y += object_size;
        if(this->isOverlap(&object))
        {
            down_fire = min(down_fire, i-1);
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    for(int i = 1; i <= left_fire ; ++i)
    {
        this->X -= object_size;
        if(this->isOverlap(&object))
        {
            left_fire = min(left_fire, i-1);
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    for(int i = 1; i <= right_fire ; ++i)
    {
        this->X += object_size;
        if(this->isOverlap(&object))
        {
            right_fire = min(right_fire, i-1);
            whether = true;
            break;
        }
    }
    this->X = original_x;
    this->Y = original_y;

    return whether;
}*/

int Bomb::min(int a, int b)
{
    if(b < a)
        return b;

    return a;
}
