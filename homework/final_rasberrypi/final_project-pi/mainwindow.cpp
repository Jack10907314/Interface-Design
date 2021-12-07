#include "mainwindow.h"
#include "ui_mainwindow.h"
int c = 100;
int b = 100;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(field_size_x, field_size_y); //Form size

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimeTick()));
    timer->start(30);

    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(TimeTick_Pin()));
    timer2->start(30);

    InitialObject(&player_1, &player_2, &vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2, &vector_props);
    wiringPiSetup();

    pinMode(Pin_player1_up,INPUT);
    pinMode(Pin_player1_down,INPUT);
    pinMode(Pin_player1_left,INPUT);
    pinMode(Pin_player1_right,INPUT);
    pinMode(Pin_player1_bomb,INPUT);

    pinMode(Pin_player2_up,INPUT);
    pinMode(Pin_player2_down,INPUT);
    pinMode(Pin_player2_left,INPUT);
    pinMode(Pin_player2_right,INPUT);
    pinMode(Pin_player2_bomb,INPUT);

    QFile Fout(":/new/prefix1/sound/backsound.wav");

    if(!Fout.exists())
    {
        qDebug()<<"no file";
    }
    else
    {
        qDebug()<<"have file";
    }

    //QSound::play(":/new/prefix1/sound/backsound.wav");
    /*QSound *bell = new QSound(":/new/prefix1/sound/backsound.wav");
    bell->setLoops(QSound::Infinite);
    bell->play();*/
    QSoundEffect *effect=new QSoundEffect;
    effect->setSource(QUrl::fromLocalFile(":/new/prefix1/sound/backsound.wav"));
    effect->setLoopCount(QSoundEffect::Infinite);  //循环次数
    effect->setVolume(1.00f); //音量  0~1之间
    effect->play();

}

MainWindow::~MainWindow()
{
    delete ui;

}



void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);

    QImage field = BuildField(&player_1, &player_2, &vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2, &vector_props);
    painter.drawImage(QPoint(0, 0), field);
    //painter.drawImage(QPoint(player_1.X, player_1.Y), player_1.Image);

    if(decision == PLAYER1)
    {
        timer->stop();

        QImage img_win = QImage(":/new/prefix1/picture/win.png");
        img_win = img_win.scaled(object_size,object_size*2/3);
        painter.drawImage(QPoint(player_1.X, player_1.Y - object_size*2/3), img_win);

        QImage img_sad = QImage(":/new/prefix1/picture/sad.png");
        img_sad = img_sad.scaled(object_size,object_size);
        painter.drawImage(QPoint(player_2.X, player_2.Y - object_size), img_sad);
    }
    if(decision == PLAYER2)
    {
        timer->stop();

        QImage img_win = QImage(":/new/prefix1/picture/win.png");
        img_win = img_win.scaled(object_size,object_size*2/3);
        painter.drawImage(QPoint(player_2.X, player_2.Y - object_size*2/3), img_win);

        QImage img_sad = QImage(":/new/prefix1/picture/sad.png");
        img_sad = img_sad.scaled(object_size,object_size);
        painter.drawImage(QPoint(player_1.X, player_1.Y - object_size), img_sad);
    }
    if(decision == TIE)
    {
        timer->stop();

        QImage img_laugh = QImage(":/new/prefix1/picture/laugh.png");
        img_laugh = img_laugh.scaled(object_size*3,object_size*3);
        painter.drawImage(QPoint(player_1.X - object_size, player_1.Y - object_size*3), img_laugh);
        painter.drawImage(QPoint(player_2.X - object_size, player_2.Y - object_size*3), img_laugh);
    }
}

void MainWindow::TimeTick()
{
    player_1.TimeTick();
    player1Move();
    player_2.TimeTick();
    player2Move();



    int remove_index_1 = -1;
    for(int i = 0; i < vector_props.size();++i)
    {
        if(player_1.isOverlap(&vector_props[i]))
        {
            if(vector_props[i].props_id == POWER)
                addPower(PLAYER1);
            if(vector_props[i].props_id == BOMB)
                addBomb(PLAYER1);
            if(vector_props[i].props_id == SPEED)
                addSpeed(PLAYER1);

            remove_index_1 = i;
        }
    }
    if(remove_index_1 != -1)
        vector_props.removeAt(remove_index_1);

    int remove_index_2 = -1;
    for(int i = 0; i < vector_props.size();++i)
    {
        if(player_2.isOverlap(&vector_props[i]))
        {
            if(vector_props[i].props_id == POWER)
                addPower(PLAYER2);
            if(vector_props[i].props_id == BOMB)
                addBomb(PLAYER2);
            if(vector_props[i].props_id == SPEED)
                addSpeed(PLAYER2);

            remove_index_2 = i;
        }
    }
    if(remove_index_2 != -1)
        vector_props.removeAt(remove_index_2);




    QVector<Bomb*> vector_bomb_total;
    for(int i = 0; i < vector_bomb_1.size();++i)
    {
        vector_bomb_total.append(&vector_bomb_1[i]);
    }
    for(int i = 0; i < vector_bomb_2.size();++i)
    {
        vector_bomb_total.append(&vector_bomb_2[i]);
    }
    for(int i = 0; i < vector_bomb_total.size();++i)
    {
        vector_bomb_total[i]->TimeTick(&vector_bomb_total, i, &vector_wall, &vector_block);
    }


    for(int i = 0; i < vector_bomb_1.size();++i)
    {
        //vector_bomb_1[i].TimeTick(&vector_bomb_1, i, &vector_wall, &vector_block);

        //reduce explosion region
        if(vector_bomb_1[i].once_explosion)
        {
            for(int j = 0; j < vector_block.size(); ++j)
            {
                vector_bomb_1[i].isExplode(vector_block[j]);
            }

            foreach(Wall wall, vector_wall)
            {
                vector_bomb_1[i].isExplode(wall);
            }

            //extend region +1
            if(vector_bomb_1[i].up_fire < vector_bomb_1[i].bomb_power)
                vector_bomb_1[i].up_fire++;

            if(vector_bomb_1[i].down_fire < vector_bomb_1[i].bomb_power)
                vector_bomb_1[i].down_fire++;

            if(vector_bomb_1[i].left_fire < vector_bomb_1[i].bomb_power)
                vector_bomb_1[i].left_fire++;

            if(vector_bomb_1[i].right_fire < vector_bomb_1[i].bomb_power)
                vector_bomb_1[i].right_fire++;
        }
        if(vector_bomb_1[i].explosion)
        {
            for(int j = 0; j < vector_block.size(); ++j)
            {
                bool exploded = vector_bomb_1[i].isExplode(vector_block[j]);
                if(exploded) vector_block.removeAt(j);
            }

            foreach(Wall wall, vector_wall)
            {
                vector_bomb_1[i].isExplode(wall);
            }


            for(int j = 0; j < vector_props.size(); ++j)
            {
                bool exploded = vector_bomb_1[i].isExplode_unchange(vector_props[j]);
                if(exploded) vector_props.removeAt(j);
            }

            vector_bomb_1[i].once_explosion = false;

            player2_win |= player_1.isExploded(vector_bomb_1[i]);
            player1_win |= player_2.isExploded(vector_bomb_1[i]);
        }
    }

    for(int i = 0; i < vector_bomb_2.size();++i)
    {
        //vector_bomb_2[i].TimeTick(&vector_bomb_2, i, &vector_wall, &vector_block);

        //reduce explosion region
        if(vector_bomb_2[i].once_explosion)
        {
            for(int j = 0; j < vector_block.size(); ++j)
            {
                vector_bomb_2[i].isExplode(vector_block[j]);
            }

            foreach(Wall wall, vector_wall)
            {
                vector_bomb_2[i].isExplode(wall);
            }

            //extend region +2
            if(vector_bomb_2[i].up_fire < vector_bomb_2[i].bomb_power)
                vector_bomb_2[i].up_fire++;

            if(vector_bomb_2[i].down_fire < vector_bomb_2[i].bomb_power)
                vector_bomb_2[i].down_fire++;

            if(vector_bomb_2[i].left_fire < vector_bomb_2[i].bomb_power)
                vector_bomb_2[i].left_fire++;

            if(vector_bomb_2[i].right_fire < vector_bomb_2[i].bomb_power)
                vector_bomb_2[i].right_fire++;
        }
        if(vector_bomb_2[i].explosion)
        {
            for(int j = 0; j < vector_block.size(); ++j)
            {
                bool exploded = vector_bomb_2[i].isExplode(vector_block[j]);
                if(exploded) vector_block.removeAt(j);
            }

            foreach(Wall wall, vector_wall)
            {
                vector_bomb_2[i].isExplode(wall);
            }

            for(int j = 0; j < vector_props.size(); ++j)
            {
                bool exploded = vector_bomb_2[i].isExplode_unchange(vector_props[j]);
                if(exploded) vector_props.removeAt(j);
            }

            vector_bomb_2[i].once_explosion = false;


            player2_win |= player_1.isExploded(vector_bomb_2[i]);
            player1_win |= player_2.isExploded(vector_bomb_2[i]);
        }
    }


    if(player1_win) decision=PLAYER1;
    if(player2_win) decision=PLAYER2;
    if(player1_win & player2_win) decision=TIE;
    //qDebug()<<player.Y;
    update();
}

void MainWindow::TimeTick_Pin()
{
    //positive edge
    if(previous_read_player1_up == 0 && digitalRead(Pin_player1_up) == 1)
    {
        key_player1_up=true;
        player_1.direct_count++;
    }
    if(previous_read_player1_down == 0 && digitalRead(Pin_player1_down) == 1)
    {
        key_player1_down=true;
        player_1.direct_count++;
    }
    if(previous_read_player1_left == 0 && digitalRead(Pin_player1_left) == 1)
    {
        key_player1_left=true;
        player_1.direct_count++;
    }
    if(previous_read_player1_right == 0 && digitalRead(Pin_player1_right) == 1)
    {
        key_player1_right=true;
        player_1.direct_count++;
    }
    if(previous_read_player1_bomb == 0 && digitalRead(Pin_player1_bomb) == 1)
    {
        player_1.Put_bomb(&vector_bomb_1);
        if(decision != NON_DECISION)
        {
            InitialObject(&player_1, &player_2, &vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2, &vector_props);
            decision = NON_DECISION;
            player1_win = false;
            player2_win = false;
            timer->start(30);
        }
    }

    if(previous_read_player2_up == 0 && digitalRead(Pin_player2_up) == 1)
    {
        key_player2_up=true;
        player_2.direct_count++;
    }
    if(previous_read_player2_down == 0 && digitalRead(Pin_player2_down) == 1)
    {
        key_player2_down=true;
        player_2.direct_count++;
    }
    if(previous_read_player2_left == 0 && digitalRead(Pin_player2_left) == 1)
    {
        key_player2_left=true;
        player_2.direct_count++;
    }
    if(previous_read_player2_right == 0 && digitalRead(Pin_player2_right) == 1)
    {
        key_player2_right=true;
        player_2.direct_count++;
    }
    if(previous_read_player2_bomb == 0 && digitalRead(Pin_player2_bomb) == 1)
    {
        player_2.Put_bomb(&vector_bomb_2);
        if(decision != NON_DECISION)
        {
            InitialObject(&player_1, &player_2, &vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2, &vector_props);
            decision = NON_DECISION;
            player1_win = false;
            player2_win = false;
            timer->start(30);
        }
    }


    //negative edge
    if(previous_read_player1_up == 1 && digitalRead(Pin_player1_up) == 0)
    {
        key_player1_up=false;
        player_1.direct_count--;
    }
    if(previous_read_player1_down == 1 && digitalRead(Pin_player1_down) == 0)
    {
        key_player1_down=false;
        player_1.direct_count--;
    }
    if(previous_read_player1_left == 1 && digitalRead(Pin_player1_left) == 0)
    {
        key_player1_left=false;
        player_1.direct_count--;
    }
    if(previous_read_player1_right == 1 && digitalRead(Pin_player1_right) == 0)
    {
        key_player1_right=false;
        player_1.direct_count--;
    }

    if(previous_read_player2_up == 2 && digitalRead(Pin_player2_up) == 0)
    {
        key_player2_up=false;
        player_2.direct_count--;
    }
    if(previous_read_player2_down == 2 && digitalRead(Pin_player2_down) == 0)
    {
        key_player2_down=false;
        player_2.direct_count--;
    }
    if(previous_read_player2_left == 2 && digitalRead(Pin_player2_left) == 0)
    {
        key_player2_left=false;
        player_2.direct_count--;
    }
    if(previous_read_player2_right == 2 && digitalRead(Pin_player2_right) == 0)
    {
        key_player2_right=false;
        player_2.direct_count--;
    }

    previous_read_player1_up = digitalRead(Pin_player1_up);
    previous_read_player1_down = digitalRead(Pin_player1_down);
    previous_read_player1_left = digitalRead(Pin_player1_left);
    previous_read_player1_right = digitalRead(Pin_player1_right);
    previous_read_player1_bomb = digitalRead(Pin_player1_bomb);

    previous_read_player2_up = digitalRead(Pin_player2_up);
    previous_read_player2_down = digitalRead(Pin_player2_down);
    previous_read_player2_left = digitalRead(Pin_player2_left);
    previous_read_player2_right = digitalRead(Pin_player2_right);
    previous_read_player2_bomb = digitalRead(Pin_player2_bomb);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
   switch (e->key())
   {
       case Qt::Key_W : key_player1_up=true;    player_1.direct_count++;  break;
       case Qt::Key_S : key_player1_down=true;  player_1.direct_count++;  break;
       case Qt::Key_A : key_player1_left=true;  player_1.direct_count++;  break;
       case Qt::Key_D : key_player1_right=true; player_1.direct_count++;  break;
       case Qt::Key_Space :
            player_1.Put_bomb(&vector_bomb_1);
            if(decision != NON_DECISION)
            {
                InitialObject(&player_1, &player_2, &vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2, &vector_props);
                decision = NON_DECISION;
                player1_win = false;
                player2_win = false;
                timer->start(30);
            }
            break;

       case Qt::Key_I : key_player2_up=true;    player_2.direct_count++;  break;
       case Qt::Key_K : key_player2_down=true;  player_2.direct_count++;  break;
       case Qt::Key_J : key_player2_left=true;  player_2.direct_count++;  break;
       case Qt::Key_L : key_player2_right=true; player_2.direct_count++;  break;
       case Qt::Key_Enter :
            player_2.Put_bomb(&vector_bomb_2);
            if(decision != NON_DECISION)
            {
                InitialObject(&player_1, &player_2, &vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2, &vector_props);
                decision = NON_DECISION;
                player1_win = false;
                player2_win = false;
                timer->start(30);
            }
            break;
   }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_W : key_player1_up=false;    player_1.direct_count--;  break;
        case Qt::Key_S : key_player1_down=false;  player_1.direct_count--;  break;
        case Qt::Key_A : key_player1_left=false;  player_1.direct_count--;  break;
        case Qt::Key_D : key_player1_right=false; player_1.direct_count--;  break;

        case Qt::Key_I : key_player2_up=false;    player_2.direct_count--;  break;
        case Qt::Key_K : key_player2_down=false;  player_2.direct_count--;  break;
        case Qt::Key_J : key_player2_left=false;  player_2.direct_count--;  break;
        case Qt::Key_L : key_player2_right=false; player_2.direct_count--;  break;
    }
}

void MainWindow::player1Move()
{
    if(key_player1_up)      player_1.Move_up(&vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2);
    if(key_player1_down)    player_1.Move_down(&vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2);
    if(key_player1_left)    player_1.Move_left(&vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2);
    if(key_player1_right)   player_1.Move_right(&vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2);
}

void MainWindow::player2Move()
{
    if(key_player2_up)      player_2.Move_up(&vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2);
    if(key_player2_down)    player_2.Move_down(&vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2);
    if(key_player2_left)    player_2.Move_left(&vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2);
    if(key_player2_right)   player_2.Move_right(&vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2);
}

void MainWindow::addPower(int player_num)
{
    if(player_num == PLAYER1)
    {
        player_1.bomb_power++;
        for(int i = 0; i < vector_bomb_1.size(); ++i)
        {
            vector_bomb_1[i].bomb_power = player_1.bomb_power;
        }
    }

    if(player_num == PLAYER2)
    {
        player_2.bomb_power++;
        for(int i = 0; i < vector_bomb_2.size(); ++i)
        {
            vector_bomb_2[i].bomb_power = player_2.bomb_power;
        }
    }
}

void MainWindow::addBomb(int player_num)
{
    if(player_num == PLAYER1)
    {
        player_1.bomb_number++;
        Bomb bomb;
        bomb.bomb_power = player_1.bomb_power;
        vector_bomb_1.append(bomb);
    }

    if(player_num == PLAYER2)
    {
        player_2.bomb_number++;
        Bomb bomb;
        bomb.bomb_power = player_2.bomb_power;
        vector_bomb_2.append(bomb);
    }
}

void MainWindow::addSpeed(int player_num)
{
    if(player_num == PLAYER1)
    {
        player_1.Speed = 10;
    }

    if(player_num == PLAYER2)
    {
        player_2.Speed = 10;
    }
}
