#include "player.h"

Player::Player()
{
    setImage(":/new/prefix1/picture/player1_down_1.png");
    setImageScaled(size, size);

    image_up.append(QImage(":/new/prefix1/picture/player1_up_1.png"));
    image_up.append(QImage(":/new/prefix1/picture/player1_up_2.png"));
    image_up.append(QImage(":/new/prefix1/picture/player1_up_3.png"));

    image_down.append(QImage(":/new/prefix1/picture/player1_down_1.png"));
    image_down.append(QImage(":/new/prefix1/picture/player1_down_2.png"));
    image_down.append(QImage(":/new/prefix1/picture/player1_down_3.png"));

    image_left.append(QImage(":/new/prefix1/picture/player1_left_1.png"));
    image_left.append(QImage(":/new/prefix1/picture/player1_left_2.png"));
    image_left.append(QImage(":/new/prefix1/picture/player1_left_3.png"));

    image_right.append(QImage(":/new/prefix1/picture/player1_right_1.png"));
    image_right.append(QImage(":/new/prefix1/picture/player1_right_2.png"));
    image_right.append(QImage(":/new/prefix1/picture/player1_right_3.png"));
}

Player::Player(QString path, int x, int y):Object(path, x, y)
{

}

void Player::TimeTick()
{
    if(direct_count > 0)
    {
        time_count++;
        if(time_count == 5)
        {
            time_count = 0;

            if(direct_up)
            {
                Image = image_up[action_up];
                setImageScaled(size, size);
                action_up++;
                if(action_up == image_up.size())
                    action_up = 0;
            }
            else if(direct_down)
            {
                Image = image_down[action_down];
                setImageScaled(size, size);
                action_down++;
                if(action_down == image_down.size())
                    action_down = 0;
            }
            else if(direct_left)
            {
                Image = image_left[action_left];
                setImageScaled(size, size);
                action_left++;
                if(action_left == image_left.size())
                    action_left = 0;
            }
            else if(direct_right)
            {
                Image = image_right[action_right];
                setImageScaled(size, size);
                action_right++;
                if(action_right == image_right.size())
                    action_right = 0;
            }
        }
    }
}

void Player::Move_up(QVector<Wall> *vector_wall, QVector<Block> *vector_block, QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2)
{
    bool cancel = false;

    Y -= Speed;

    foreach(Wall wall, *vector_wall)
    {
        cancel |= isOverlap(&wall);
    }

    foreach(Block blcok, *vector_block)
    {
        cancel |= isOverlap(&blcok);
    }

    for(int i = 0; i < vector_bomb_1->size(); ++i)
    {
        cancel |= isOverlapBomb(&(*vector_bomb_1)[i]);
    }

    for(int i = 0; i < vector_bomb_2->size(); ++i)
    {
        cancel |= isOverlapBomb(&(*vector_bomb_2)[i]);
    }

    if(cancel)
    {
        Y += Speed;

        //optimize user experience
        if(direct_count == 1)
        {
            int original_x = X;
            int original_y = Y;

            if( (X % object_size) <= (object_size / 2) )
                X = (X / object_size) * object_size;
            else if( (X % object_size) > (object_size / 2) )
                X = (X / object_size + 1) * object_size;

            Y -= Speed;

            cancel = false;
            foreach(Wall wall, *vector_wall)
            {
                cancel |= isOverlap(&wall);
            }
            foreach(Block blcok, *vector_block)
            {
                cancel |= isOverlap(&blcok);
            }
            for(int i = 0; i < vector_bomb_1->size(); ++i)
            {
                cancel |= isOverlapBomb(&(*vector_bomb_1)[i]);
            }
            for(int i = 0; i < vector_bomb_2->size(); ++i)
            {
                cancel |= isOverlapBomb(&(*vector_bomb_2)[i]);
            }
            if(cancel)
            {
                X = original_x;
                Y = original_y;
            }
        }
        direct_up = false;
        action_up = 0;
    }
    else
    {
        direct_up = true;
        direct_down = false;
        direct_left = false;
        direct_right = false;
    }

}

void Player::Move_down(QVector<Wall> *vector_wall, QVector<Block> *vector_block, QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2)
{
    bool cancel = false;

    Y += Speed;

    foreach(Wall wall, *vector_wall)
    {
        cancel |= isOverlap(&wall);
    }

    foreach(Block blcok, *vector_block)
    {
        cancel |= isOverlap(&blcok);
    }

    for(int i = 0; i < vector_bomb_1->size(); ++i)
    {
        cancel |= isOverlapBomb(&(*vector_bomb_1)[i]);
    }

    for(int i = 0; i < vector_bomb_2->size(); ++i)
    {
        cancel |= isOverlapBomb(&(*vector_bomb_2)[i]);
    }

    if(cancel)
    {
        Y -= Speed;

        //optimize user experience
        if(direct_count == 1)
        {
            int original_x = X;
            int original_y = Y;

            if( (X % object_size) <= (object_size / 2) )
                X = (X / object_size) * object_size;
            else if( (X % object_size) > (object_size / 2) )
                X = (X / object_size + 1) * object_size;

            Y += Speed;

            cancel = false;
            foreach(Wall wall, *vector_wall)
            {
                cancel |= isOverlap(&wall);
            }
            foreach(Block blcok, *vector_block)
            {
                cancel |= isOverlap(&blcok);
            }
            for(int i = 0; i < vector_bomb_1->size(); ++i)
            {
                cancel |= isOverlapBomb(&(*vector_bomb_1)[i]);
            }
            for(int i = 0; i < vector_bomb_2->size(); ++i)
            {
                cancel |= isOverlapBomb(&(*vector_bomb_2)[i]);
            }
            if(cancel)
            {
                X = original_x;
                Y = original_y;
            }
        }
        direct_down = false;
        action_down = 0;
    }
    else
    {
        direct_up = false;
        direct_down = true;
        direct_left = false;
        direct_right = false;
    }
}

void Player::Move_left(QVector<Wall> *vector_wall, QVector<Block> *vector_block, QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2)
{
    bool cancel = false;

    X -= Speed;

    foreach(Wall wall, *vector_wall)
    {
        cancel |= isOverlap(&wall);
    }

    foreach(Block blcok, *vector_block)
    {
        cancel |= isOverlap(&blcok);
    }

    for(int i = 0; i < vector_bomb_1->size(); ++i)
    {
        cancel |= isOverlapBomb(&(*vector_bomb_1)[i]);
    }

    for(int i = 0; i < vector_bomb_2->size(); ++i)
    {
        cancel |= isOverlapBomb(&(*vector_bomb_2)[i]);
    }

    if(cancel)
    {
        X += Speed;

        //optimize user experience
        if(direct_count == 1)
        {
            int original_x = X;
            int original_y = Y;

            if( (Y % object_size) <= (object_size / 2) )
                Y = (Y / object_size) * object_size;
            else if( (Y % object_size) > (object_size / 2) )
                Y = (Y / object_size + 1) * object_size;

            X -= Speed;

            cancel = false;
            foreach(Wall wall, *vector_wall)
            {
                cancel |= isOverlap(&wall);
            }
            foreach(Block blcok, *vector_block)
            {
                cancel |= isOverlap(&blcok);
            }
            for(int i = 0; i < vector_bomb_1->size(); ++i)
            {
                cancel |= isOverlapBomb(&(*vector_bomb_1)[i]);
            }
            for(int i = 0; i < vector_bomb_2->size(); ++i)
            {
                cancel |= isOverlapBomb(&(*vector_bomb_2)[i]);
            }
            if(cancel)
            {
                X = original_x;
                Y = original_y;
            }
        }
        direct_left = false;
        action_left = 0;
    }
    else
    {
        direct_up = false;
        direct_down = false;
        direct_left = true;
        direct_right = false;
    }
}

void Player::Move_right(QVector<Wall> *vector_wall, QVector<Block> *vector_block, QVector<Bomb> *vector_bomb_1, QVector<Bomb> *vector_bomb_2)
{
    bool cancel = false;

    X += Speed;

    foreach(Wall wall, *vector_wall)
    {
        cancel |= isOverlap(&wall);
    }

    foreach(Block blcok, *vector_block)
    {
        cancel |= isOverlap(&blcok);
    }

    for(int i = 0; i < vector_bomb_1->size(); ++i)
    {
        cancel |= isOverlapBomb(&(*vector_bomb_1)[i]);
    }

    for(int i = 0; i < vector_bomb_2->size(); ++i)
    {
        cancel |= isOverlapBomb(&(*vector_bomb_2)[i]);
    }

    if(cancel)
    {
        X -= Speed;

        //optimize user experience
        if(direct_count == 1)
        {
            int original_x = X;
            int original_y = Y;

            if( (Y % object_size) <= (object_size / 2) )
                Y = (Y / object_size) * object_size;
            else if( (Y % object_size) > (object_size / 2) )
                Y = (Y / object_size + 1) * object_size;

            X += Speed;

            cancel = false;
            foreach(Wall wall, *vector_wall)
            {
                cancel |= isOverlap(&wall);
            }
            foreach(Block blcok, *vector_block)
            {
                cancel |= isOverlap(&blcok);
            }
            for(int i = 0; i < vector_bomb_1->size(); ++i)
            {
                cancel |= isOverlapBomb(&(*vector_bomb_1)[i]);
            }
            for(int i = 0; i < vector_bomb_2->size(); ++i)
            {
                cancel |= isOverlapBomb(&(*vector_bomb_2)[i]);
            }
            if(cancel)
            {
                X = original_x;
                Y = original_y;
            }
        }
        direct_right = false;
        action_right = 0;
    }
    else
    {
        direct_up = false;
        direct_down = false;
        direct_left = false;
        direct_right = true;
    }
}

bool Player::isExploded(Bomb bomb)
{
    bool whether = false;
    int original_plyaer_x = X;
    int original_plyaer_y = Y;

    if( (X % object_size) <= (object_size / 2) )
        X = (X / object_size) * object_size;
    else if( (X % object_size) > (object_size / 2) )
        X = (X / object_size + 1) * object_size;

    if( (Y % object_size) <= (object_size / 2) )
        Y = (Y / object_size) * object_size;
    else if( (Y % object_size) > (object_size / 2) )
        Y = (Y / object_size + 1) * object_size;;


    int original_x = bomb.X;
    int original_y = bomb.Y;

    if(this->isOverlap(&bomb))
        whether = true;

    for(int i = 1; i <= bomb.up_fire ; ++i)
    {
        bomb.Y -= object_size;
        if(this->isOverlap(&bomb))
        {
            whether = true;
            break;
        }
    }
    bomb.X = original_x;
    bomb.Y = original_y;

    for(int i = 1; i <= bomb.down_fire ; ++i)
    {
        bomb.Y += object_size;
        if(this->isOverlap(&bomb))
        {
            whether = true;
            break;
        }
    }
    bomb.X = original_x;
    bomb.Y = original_y;

    for(int i = 1; i <= bomb.left_fire ; ++i)
    {
        bomb.X -= object_size;
        if(this->isOverlap(&bomb))
        {
            whether = true;
            break;
        }
    }
    bomb.X = original_x;
    bomb.Y = original_y;

    for(int i = 1; i <= bomb.right_fire ; ++i)
    {
        bomb.X += object_size;
        if(this->isOverlap(&bomb))
        {
            whether = true;
            break;
        }
    }
    bomb.X = original_x;
    bomb.Y = original_y;


    X = original_plyaer_x;
    Y = original_plyaer_y;

    return whether;
}

bool Player::isOverlapBomb(Bomb* bomb)
{
    if(bomb->put == false)
        return false;

    bool whether = false;

    if( (bomb->X <= this->X) && (this->X <= bomb->X+bomb->size-1) &&
        (bomb->Y <= this->Y) && (this->Y <= bomb->Y+bomb->size-1))
        whether = true;

    if( (bomb->X <= this->X+this->size-1) && (this->X+this->size-1 <= bomb->X+bomb->size-1) &&
        (bomb->Y <= this->Y) && (this->Y <= bomb->Y+bomb->size-1))
        whether = true;

    if( (bomb->X <= this->X) && (this->X <= bomb->X+bomb->size-1) &&
        (bomb->Y <= this->Y+this->size-1) && (this->Y+this->size-1 <= bomb->Y+bomb->size-1))
        whether = true;

    if( (bomb->X <= this->X+this->size-1) && (this->X+this->size-1 <= bomb->X+bomb->size-1) &&
        (bomb->Y <= this->Y+this->size-1) && (this->Y+this->size-1 <= bomb->Y+bomb->size-1))
        whether = true;

    if(this->id == 1)
    {
        if(whether == false && bomb->player1_block == false)
        {
            bomb->player1_block = true;
        }
        if(bomb->player1_block == false)
        {
            whether = false;
        }
    }
    if(this->id == 2)
    {
        if(whether == false && bomb->player2_block == false)
        {
            bomb->player2_block = true;
        }
        if(bomb->player2_block == false)
        {
            whether = false;
        }
    }
    return whether;
}

void Player::Put_bomb(QVector<Bomb> *vector_bomb)
{
    bomb_order++;
    if(bomb_order >= (*vector_bomb).count())
        bomb_order = 0;

    Bomb* bomb = &(*vector_bomb)[bomb_order];
    if(bomb->put == false && bomb->explosion == false)
    {
        if(X % object_size <= object_size / 2)
            bomb->X = (X/object_size) * object_size;

        else if(X % object_size > object_size / 2)
            bomb->X = (X/object_size + 1) * object_size;

        if(Y % object_size <= object_size / 2)
            bomb->Y = (Y/object_size) * object_size;

        else if(Y % object_size > object_size / 2)
            bomb->Y = (Y/object_size + 1) * object_size;

        bomb->put = true;
    }

    bool cancel = false;
    for(int i = 0; i < vector_bomb->size(); ++i)
    {
        if(i != bomb_order)
        {
            if((*vector_bomb)[i].explosion || (*vector_bomb)[i].put)
            {
                cancel |= bomb->isOverlap(&(*vector_bomb)[i]);
            }
        }
    }
    if(cancel)
    {
        bomb->put = false;

        bomb_order--;
        if(bomb_order < 0)
            bomb_order = vector_bomb->size() - 1;
    }

}
