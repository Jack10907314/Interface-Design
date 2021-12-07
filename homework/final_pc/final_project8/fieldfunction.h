#ifndef FIELDFUNCTION_H
#define FIELDFUNCTION_H

#include <QVector>
#include <QImage>
#include <QTime>

#include "wall.h"
#include "player.h"
#include "block.h"
#include "bomb.h"
#include "props.h"

//const int field_cell_x = 31;
//const int field_cell_y = 19;
const int field_cell_x = 17;
const int field_cell_y = 11;

const int field_size_x = field_cell_x * object_size; //Form x size
const int field_size_y = field_cell_y * object_size; //Form y size

int RandomInt(int low, int high);
bool Probability(float probability);
void InitialObject(Player *player_1, Player *player_2, QVector<Wall> *vector_wall, QVector<Block> *vector_block,
                   QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2, QVector<Props> *vector_props);
QImage BuildField(Player *player_1, Player *player_2, QVector<Wall> *vector_wall, QVector<Block> *vector_block,
                  QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2, QVector<Props> *vector_props);


#endif // FIELDFUNCTION_H

