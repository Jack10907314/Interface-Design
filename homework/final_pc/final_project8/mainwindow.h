#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QPoint>
#include <QBitmap>
#include <QVector>
#include <QDebug>
#include <QtMultimedia/QSound>
#include <QtMultimedia/QSoundEffect>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>

#include "fieldfunction.h"
#include "object.h"
#include "wall.h"
#include "player.h"
#include "block.h"
#include "bomb.h"

#include <iostream>
#include <cstdlib>

using namespace std;

#define NON_DECISION 0
#define PLAYER1 1
#define PLAYER2 2
#define TIE 3

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
int count = 0;
    QTimer* timer;
    Player player_1;
    Player player_2;
    QVector<Wall> vector_wall;
    QVector<Block> vector_block;
    QVector<Bomb> vector_bomb_1;
    QVector<Bomb> vector_bomb_2;
    QVector<Props> vector_props;

    int decision = NON_DECISION;
    bool player1_win = false;
    bool player2_win = false;

    bool key_player1_up = false;
    bool key_player1_down = false;
    bool key_player1_left = false;
    bool key_player1_right = false;
    bool key_player1_bomb = false;

    bool key_player2_up = false;
    bool key_player2_down = false;
    bool key_player2_left = false;
    bool key_player2_right = false;
    bool key_player2_bomb = false;

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void player1Move();
    void player2Move();
    void addPower(int player_num);
    void addBomb(int player_num);
    void addSpeed(int player_num);

    QMediaPlayer * player;
    QMediaPlaylist * playlist;

private slots:
    void TimeTick();
};

#endif // MAINWINDOW_H
