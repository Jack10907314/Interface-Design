#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

#include "object.h"
#include "bomb.h"
#include "wall.h"
#include "block.h"

class Player : public Object
{
public:
    Player();
    Player(QString path, int x, int y);

    int id = 1;

    void Move_up(QVector<Wall> *vector_wall, QVector<Block> *vector_block, QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2);
    void Move_down(QVector<Wall> *vector_wall, QVector<Block> *vector_block, QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2);
    void Move_left(QVector<Wall> *vector_wall, QVector<Block> *vector_block, QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2);
    void Move_right(QVector<Wall> *vector_wall, QVector<Block> *vector_block, QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2);
    void Put_bomb(QVector<Bomb> *vector_bomb);

    int direct_count = 0;

    int Speed = 8;
    int bomb_power = 1;
    int bomb_number = 1;
    int bomb_order = 0;

    bool direct_up = false;
    bool direct_down = false;
    bool direct_left = false;
    bool direct_right = false;
    int action_up = 0;
    int action_down = 0;
    int action_left = 0;
    int action_right = 0;
    QVector<QImage> image_up;
    QVector<QImage> image_down;
    QVector<QImage> image_left;
    QVector<QImage> image_right;

    int time_count = 0;

    void TimeTick();
    bool isExploded(Bomb bomb);
    bool isOverlapBomb(Bomb* bomb);
};

#endif // PLAYER_H
