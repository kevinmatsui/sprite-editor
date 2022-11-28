/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#ifndef CANVAS_H
#define CANVAS_H

#include <QObject>

class Canvas : public QObject
{
public:
    Canvas();
    Canvas(QVector<QVector<QColor>> grid);
    QVector<QVector<QVector<QColor>>> frames;
    //Important Methods
    void initialize(int _height, int _width, int _offsetX, int _offsetY, int _pixelSize);
    void initializeFromJson(int gridX, int gridY, int numFrame);
    void colRowsToAbsolutePos(int& x, int& y);
    void addFrame();
    bool boundsDetection(int x, int y);
    void fillArea(int absoluteX, int absoluteY, QColor newColor);
    void resetCanvasChange();

    //Getters and Setters
    int getPixelSize();
    int getCurrentFrame();
    int getTotalFrame();
    QVector<QVector<QColor>>& getGrid();
    int currentFrame;
    QVector<QVector<QVector<QColor>>>& getFrames();
    void changeFrame(int frame);
    void changeBackground(QColor background);
    void setGrid(QVector<QVector<QColor>> newGrid);
    void setGrid(QVector<QVector<QColor>> newGrid, int frameNum);
    QColor getColor(QPoint point);
    bool canvasChanged();

    int getOffsetX();
    int getOffsetY();

public slots:
    void updateGrid(QPoint absolutePosition, QColor col);
    void rotateGrid();
    void flipHorizontally();

private:
    bool changed;
    int height, width, numCols, numRows, offsetX, offsetY, pixelSize;

    void resetCanvas();
    void absoluteToRowsCols(int& x, int& y);
    void floodFillRecursive(int x, int y, QColor prevColor, QColor newColor);

};

#endif // CANVAS_H
