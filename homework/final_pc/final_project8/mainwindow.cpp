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

    InitialObject(&player_1, &player_2, &vector_wall, &vector_block, &vector_bomb_1, &vector_bomb_2, &vector_props);
    //QSound::play(":/new/prefix1/sound/explosion.wma");
//    QSound *bells =new QSound(":/new/prefix1/sound/explosion.wma");
//    bells->play();
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
    /*auto player = new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile(":/new/prefix1/sound/explosion.wma"));
        player->setVolume(50);*/
    /*player = new QMediaPlayer(this);
   playlist = new QMediaPlaylist(this);
   playlist->setPlaybackMode(QMediaPlaylist::Loop);//设置循环模式
   player->setPlaylist(playlist);//获取将播放列表要播放的文件
///
//添加音频文件 槽函数添加如下代码，上面出现的代码已省略
   //for(int i = 0;i<fileList.count();i++)
   //{
       QString aFile = ":/new/prefix1/sound/explosion.wma";
       playlist->addMedia(QUrl::fromLocalFile(aFile));
       //QFileInfo fileInfo(aFile);
       //ui->listWidget->addItem(fileInfo.fileName());  //用于显示
       //ui->plainTextEdit->appendPlainText(fileList.at(i)); //用于显示
   //}
   if(player->state()!=QMediaPlayer::PlayingState)
   {
       playlist->setCurrentIndex(0);
   }
   player->play();*/
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
