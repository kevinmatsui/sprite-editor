/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canvas.h"
#include "model.h"
#include <QColorDialog>
#include <previewform.h>
#include <QPoint>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    int size;

signals:
    void undoDraw();

private slots:
    void on_gridCheckBox_stateChanged(int arg1);
    void on_previewButton_clicked();
    void on_undoButton_clicked();
    void on_redoButton_clicked();
    void on_addFrameBut_clicked();
    void on_shapeButton_clicked();
    void on_paintBucketButton_clicked();
    void on_colorDialogButton_clicked();
    void on_previousColorButton_clicked();
    void on_loadButton_clicked();
    void on_Eraser_clicked(bool checked);

    void colorChange(QColor color);
    void changeCurrentFrame(int frame);
    void createNewProject();
    void rotateGrid();
    void flipGrid();
    void saveProject();

private:
    Ui::MainWindow *ui;
    Canvas canvas;
    Model model;
    QColorDialog colorPicker;
    QColor current, eraserHold;
    PreviewForm previewWindow;

    bool gridChecked = false;
    bool pressed = false;
    bool paintBucket = false;
    bool shapeChecked = false;

    void reset();
    void saveClauseCatch();

    void closeEvent(QCloseEvent* event);
    void drawSprite(QPainter * painter, QVector<QVector<QColor>>& grid);

protected:
    void paintEvent(QPaintEvent *event);
};
#endif // MAINWINDOW_H
