#include "fieldfunction.h"

int RandomInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

bool Probability(float probability)
{
    bool whether = false;
    float random = (float)RandomInt(0, 100) / 100.0;

    if(random <= probability) whether = true;

    return whether;
}

void InitialObject(Player *player_1, Player *player_2, QVector<Wall> *vector_wall, QVector<Block> *vector_block,
                   QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2,QVector<Props> *vector_props)
{
    // Create seed for the random
    // That is needed only once on application startup
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    Player player;
    *player_1 = player;
    *player_2 = player;

    player_1->Image = QImage(":/new/prefix1/picture/player1_down_1.png");
    player_1->setImageScaled(object_size, object_size);
    time = QTime::currentTime();
    qsrand((uint)time.msec()*2);
    player_1->X = ( RandomInt(0,field_cell_x/2-1) * 2 + 1) * object_size;
    time = QTime::currentTime();
    qsrand((uint)time.msec()*2);
    player_1->Y = ( RandomInt(0,field_cell_y/2-1) * 2 + 1) * object_size;
    player_1->id = 1;

    Bomb bomb;
    vector_bomb_1->clear();
    vector_bomb_1->append(bomb);
    vector_bomb_2->clear();
    vector_bomb_2->append(bomb);

    vector_props->clear();
    vector_wall->clear();
    vector_block->clear();
    Block block;
    Wall wall;

    player_2->Image = QImage(":/new/prefix1/picture/player1_down_1.png");
    player_2->setImageScaled(object_size, object_size);
    time = QTime::currentTime();
    qsrand((uint)time.msec()*2);
    player_2->X = ( RandomInt(0,field_cell_x/2-1) * 2 + 1) * object_size;
    time = QTime::currentTime();
    qsrand((uint)time.msec()*2);
    player_2->Y = ( RandomInt(0,field_cell_y/2-1) * 2 + 1) * object_size;
    player_2->id = 2;

    for(int i = 0; i < field_cell_x; ++i)
    {
        for(int j = 0; j < field_cell_y; ++j)
        {
            if(i == 0 || i == field_cell_x - 1 || j == 0 || j == field_cell_y - 1)
            {
                wall.X = i * object_size;
                wall.Y = j * object_size;
                vector_wall->append(wall);
            }

            else if(i % 2 == 0 && j % 2 == 0)
            {
                wall.X = i * object_size;
                wall.Y = j * object_size;
                vector_wall->append(wall);
            }
            \
            else if(i == player_1->X / object_size && j == player_1->Y / object_size) {}
            else if(i == player_1->X / object_size - 1 && j == player_1->Y / object_size) {}
            else if(i == player_1->X / object_size + 1 && j == player_1->Y / object_size) {}
            else if(i == player_1->X / object_size && j == player_1->Y / object_size - 1) {}
            else if(i == player_1->X / object_size && j == player_1->Y / object_size + 1) {}

            else if(i == player_2->X / object_size && j == player_2->Y / object_size) {}
            else if(i == player_2->X / object_size - 1 && j == player_2->Y / object_size) {}
            else if(i == player_2->X / object_size + 1 && j == player_2->Y / object_size) {}
            else if(i == player_2->X / object_size && j == player_2->Y / object_size - 1) {}
            else if(i == player_2->X / object_size && j == player_2->Y / object_size + 1) {}

            else if( (i==1 && j==1) || (i==1 && j==2) || (i==2 && j==1)) //player1 space
            {

            }

            else if( (i==field_cell_x-2 && j==field_cell_y-2) || (i==field_cell_x-2 && j==field_cell_y-3) || (i==field_cell_x-3 && j==field_cell_y-2)) //player2 space
            {

            }

            else if(Probability(0.70))
            {
                block.X = i * object_size;
                block.Y = j * object_size;
                vector_block->append(block);

                //build props
                if(Probability(0.30))
                {
                    if(Probability(0.50))
                    {
                        Props props(BOMB);
                        props.X = i * object_size;
                        props.Y = j * object_size;
                        vector_props->append(props);
                    }
                    else if(Probability(0.80))
                    {
                        Props props(POWER);
                        props.X = i * object_size;
                        props.Y = j * object_size;
                        vector_props->append(props);
                    }
                    else
                    {
                        Props props(SPEED);
                        props.X = i * object_size;
                        props.Y = j * object_size;
                        vector_props->append(props);
                    }
                }
            }
        }
    }

}

QImage BuildField(Player *player_1, Player *player_2, QVector<Wall> *vector_wall, QVector<Block> *vector_block,
                  QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2, QVector<Props> *vector_props)
{
    QImage field = QImage(":/new/prefix1/picture/grass.png");
    field = field.scaled(field_size_x,field_size_y);

    QPainter painter(&field);

    foreach(Props props, *vector_props)
    {
        painter.drawImage(QPoint(props.X, props.Y), props.Image);
    }

    foreach(Wall wall, *vector_wall)
    {
        painter.drawImage(QPoint(wall.X, wall.Y), wall.Image);
    }

    foreach(Block blcok, *vector_block)
    {
        painter.drawImage(QPoint(blcok.X, blcok.Y), blcok.Image);
    }

    foreach(Bomb bomb, *vector_bomb_1)
    {
        if(bomb.put)
            painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);

        if(bomb.explosion)
        {
            int original_x = bomb.X;
            int original_y = bomb.Y;

            painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);

            for(int i = 1; i <= bomb.up_fire ; ++i)
            {
                bomb.Y -= object_size;
                painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);
            }
            bomb.X = original_x;
            bomb.Y = original_y;

            for(int i = 1; i <= bomb.down_fire ; ++i)
            {
                bomb.Y += object_size;
                painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);
            }
            bomb.X = original_x;
            bomb.Y = original_y;

            for(int i = 1; i <= bomb.left_fire ; ++i)
            {
                bomb.X -= object_size;
                painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);
            }
            bomb.X = original_x;
            bomb.Y = original_y;

            for(int i = 1; i <= bomb.right_fire ; ++i)
            {
                bomb.X += object_size;
                painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);
            }
            bomb.X = original_x;
            bomb.Y = original_y;
        }
    }

    foreach(Bomb bomb, *vector_bomb_2)
    {
        if(bomb.put)
            painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);

        if(bomb.explosion)
        {
            int original_x = bomb.X;
            int original_y = bomb.Y;

            painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);

            for(int i = 1; i <= bomb.up_fire ; ++i)
            {
                bomb.Y -= object_size;
                painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);
            }
            bomb.X = original_x;
            bomb.Y = original_y;

            for(int i = 1; i <= bomb.down_fire ; ++i)
            {
                bomb.Y += object_size;
                painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);
            }
            bomb.X = original_x;
            bomb.Y = original_y;

            for(int i = 1; i <= bomb.left_fire ; ++i)
            {
                bomb.X -= object_size;
                painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);
            }
            bomb.X = original_x;
            bomb.Y = original_y;

            for(int i = 1; i <= bomb.right_fire ; ++i)
            {
                bomb.X += object_size;
                painter.drawImage(QPoint(bomb.X, bomb.Y), bomb.Image);
            }
            bomb.X = original_x;
            bomb.Y = original_y;
        }
    }

    painter.drawImage(QPoint(player_1->X, player_1->Y), player_1->Image);
    painter.drawImage(QPoint(player_2->X, player_2->Y), player_2->Image);

    return field;
}
