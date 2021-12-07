#ifndef BOMB_H
#define BOMB_H


#include <QString>

#include "object.h"
#include "wall.h"
#include "block.h"
#include <QtMultimedia/QSoundEffect>

class Bomb : public Object
{
public:
    Bomb();
    Bomb(QString path, int x, int y);

    int bomb_power = 1;
    int up_fire = bomb_power;
    int down_fire = bomb_power;
    int left_fire = bomb_power;
    int right_fire = bomb_power;

    bool put = false;
    bool explosion = false;
    bool once_explosion = false;
    int time_count = 0;
    
    bool player1_block = false;
    bool player2_block = false;

    void TimeTick(QVector<Bomb*> *vector_bomb, int index, QVector<Wall> *vector_wall, QVector<Block> *vector_block);
    void setExplosion(QVector<Bomb*> *vector_bomb, int index, QVector<Wall> *vector_wall, QVector<Block> *vector_block);
    void setBomb();
    bool isExplode(Object object);
    bool isExplode_unchange(Object object);
    int min(int a, int b);
};


#endif // BOMB_H
