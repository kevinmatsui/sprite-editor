/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#include "sprite.h"

Sprite::Sprite() {
//we dont know if we neeed to do anything in here yet. (dependent on canvas implementation)
}
Sprite::Sprite(QVector<QPixmap> inputCycle) {
    frameCycle = inputCycle;
    framePosition = 0;
}

void Sprite::addImage(QPixmap inputImg){
    frameCycle.append(inputImg);
    //incase we use default constructor.
    //if(frameCycle.length==1){
        //curFrame = inputImg;
   // }
}

QPixmap Sprite::getImage() {
    QPixmap curFrame = frameCycle[framePosition];
    framePosition++;
    return curFrame;
}

void Sprite::setFramePosition(int position) {
    framePosition = position;
}



