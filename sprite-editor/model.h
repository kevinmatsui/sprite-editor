/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#ifndef MODEL_H
#define MODEL_H
#include <QObject>
#include <QMouseEvent>
#include "sprite.h"
#include "toolbar.h"
#include "canvas.h"
#include <QStack>

class Model : public QObject
{
public:
    Model();
    void init(int gridSize, int canvasSize,
          int offsetX, int offsetY, Canvas& canvas);

    void addToUndo();
    void undoDraw();
    void redoDraw();
    void bypassRedo();
    QVector<QVector<QColor>>& getCurrentGrid();
    QColor getColor(QPoint pos);
    QColor currentColor;
    QColor previousColor;
    int getPixelSize();
    int gridSize(int option);

    void addUndoStack();
    void addRedoStack();
    void resetCurrent();

    void initializeShapePos(QPoint _initShapePos);
    void drawShape(QPoint shapePos, QString shapeStr, QColor color);
private:
    int brushSize;
    int PIXELSIZE;
    int actualSize;
    QImage currentFrame;


public slots:
    void selectColor(QColor newColor);
    void selectBrushSize(int size);
    void rotateCanvas();
    void flipCanvas();
    void saveFile(QString fileName);
    bool loadFile(QString file_path);

private:
    QColor current;
    QVector<QVector<QColor>> currentGrid;
    Canvas* canvas;
    QVector<QStack<QVector<QVector<QColor>>>> undoStacks;
     QVector<QStack<QVector<QVector<QColor>>>> redoStacks;

     QPoint initShapePos;

     void resetUndoRedoStacks();

     void paintPixel(int x, int y, QColor color);
     double getYCoord(double slope, int x, int b);





};

#endif // MODEL_H
