#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QSound>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QKeyEvent>
#include <QDebug>
#include <QPixmap>
#include <QTransform>
#include <QMatrix>
#include <QMessageBox>

#include "player.h"
#include "alien.h"
#include "boss.h"
#include "game.h"
#include "enemyspawn.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTimer *timer = new QTimer(this);
    //creating these now so they don't lag up the game to death
    QSound *pewSound = new QSound(":/images/pew.wav");
    QSound *riperinoPlayerino = new QSound(":/images/ripplayer.wav");
    QSound *ripAsteroid = new QSound(":/images/asteroidexlpode.wav");
    QTcpServer* server;
    QTcpSocket* socket;
     QSound *levelUp = new QSound(":/images/levelAccomplished.wav");
     QTimer *congratsLabelTimer = new QTimer(this);
     QLabel *congratsLabel;

    bool upKeyPressed = false;
    bool downKeyPressed = false;
    bool rightKeyPressed = false;
    bool leftKeyPressed = false;
    bool spacebarKeyPressed = false;

    bool synching = false;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void resetGUI();

    void makeEnemies(int);
    bool noEnemiesLeft();
    void advanceLevel();
    void hideGUI();
    void showGUI();
    QString shipSelect();

    void sendGameData(QTcpSocket *sock);

private slots:
    void timerHit();
    void hideMessage();
    void on_btnPlay_clicked();

    void clientConnected();
    void dataReceived();
    void clientDisconnected();
    void serverDisconnected();

private:
    Ui::MainWindow *ui;
};

// A basic QLabel that also contains a pointer to
// a player object and an original pixmap (for
// rotation purposes)
class PlayerLabel : public QLabel
{
    Q_OBJECT

    int offsetX, offsetY; // The offset (based on image rotation and scaling) between the image's coordinates and the player's coordinates

    Player *myPlayer;

    QPixmap *orig_pixmap;

public:
    explicit PlayerLabel(QWidget *parent): QLabel(parent) {
        if (this->pixmap() != nullptr)
        {
            orig_pixmap = new QPixmap(*this->pixmap());
        }
        offsetX = 0;
        offsetY = 0;
    }
    void playerGen();

    int getOffsetX() { return offsetX; }
    int getOffsetY() { return offsetY; }
    Player *getPlayer() { return myPlayer; }
    void setPlayer(Player *player) { myPlayer = player; }
    void setOrigPixmap(QPixmap &pixmap) { orig_pixmap = new QPixmap(pixmap); }

    ~PlayerLabel()
    {
        delete myPlayer;
        delete orig_pixmap;
    }

    void rotate(int angle);

};

class AlienLabel : public QLabel
{
    Q_OBJECT

    Alien *myAlien;

public:
    explicit AlienLabel(QWidget *parent): QLabel(parent) { }
    Alien *getAlien() { return myAlien; }
    void setAlien(Alien *alien) { myAlien = alien; }

    ~AlienLabel()
    {
        //delete myAlien;
    }

    void alienGen(QPixmap pixmap);

};

class BossLabel : public QLabel
{
    Q_OBJECT

    Boss *myBoss;

public:
    explicit BossLabel(QWidget *parent): QLabel(parent) { }
    Boss *getBoss() const { return myBoss; }
    void setBoss(Boss *boss) { myBoss = boss; }

    ~BossLabel()
    {
        delete myBoss;
    }

    void bossGen(QPixmap pixmap);

};

class ShotLabel : public QLabel
{
    Q_OBJECT

    Shot *myShot;

public:
    explicit ShotLabel(QWidget *parent): QLabel(parent) { }
    Shot *getShot() { return myShot; }
    void setShot(Shot *shot) { myShot = shot; }

    ~ShotLabel()
    {
        // Taken care of manually every time the
        // label is deleted.
        //delete myShot;
    }

    void shotGen();

};

#endif // MAINWINDOW_H
