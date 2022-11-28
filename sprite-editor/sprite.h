/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#ifndef SPRITE_H
#define SPRITE_H
#include <QPixmap>
#include <QVector>

class Sprite
{
public:
    QVector<QPixmap> frameCycle;
    int FPS;
    Sprite();
    Sprite(QVector<QPixmap> inputCycle);
    void addImage(QPixmap newImg);
    void playPreviousCycle();
    QPixmap getImage();
    void setFramePosition(int position);
private:
    int framePosition;


};

#endif // SPRITE_H
