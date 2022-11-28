/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#ifndef PREVIEWFORM_H
#define PREVIEWFORM_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPainter>
#include "canvas.h"

namespace Ui {
class PreviewForm;
}

class PreviewForm : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewForm(QWidget *parent = nullptr);
    ~PreviewForm();
    void previewSetup(QVector<QVector<QVector<QColor>>> canvasFrames, Canvas& canvas, int size);
    void startDrawing();

private slots:
    void on_CloseWindow_clicked();
    void displayFrame();
    void displayNextFrame();
    void on_spinBox_valueChanged(int fps);
    void on_actualSizeCheckBox_stateChanged(int arg1);

private:
    int startMiliSeconds;
    int selectedFPS;
    int vectorIndex;
    int imageSize;
    int xOffSet;
    int yOffSet;
    bool activated;
    bool actualSize;
    Ui::PreviewForm *ui;
    QVector<QVector<QVector<QColor>>> spriteFrames;
    Canvas* canvas;
    void drawSprite(QPainter *painter, QVector<QVector<QColor>> &grid);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // PREVIEWFORM_H
