/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#ifndef TOOLBAR_H
#define TOOLBAR_H
#include <QUndoStack>
#include <QColor>


class ToolBar : public QObject
{
public:
    ToolBar();

public slots:
    void paintBucket(QColor rgb);
    void rotateFrame(QVector<QVector<QColor>>& grid);
    void mirrorFrameHorizontal(QVector<QVector<QColor>>& grid);
    void mirrorFrameVertical(QVector<QVector<QColor>>& grid);
    void previewSprite();

private:
    QUndoStack undoStack;
    QColor previousColor;
    bool actualSize= false;
};

#endif // TOOLBAR_H
