/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#include "toolbar.h"
#include <QUndoStack>
#include <QTransform>

/**
 * @brief ToolBar::ToolBar constructor for the ToolBar class, sets the initial color for drawing
 */
ToolBar::ToolBar()
{
    // the first previous color starts as white, updates after changing colors
    previousColor = QColor(255, 255, 255);
}

/**
 * Added Feature
 * @brief ToolBar::paintBucket fills an closed area with the selected color
 * @param rgb - color the user selected
 */
void ToolBar::paintBucket(QColor rgb) {
//    QImage image(256, 256, QImage::Format_Mono);

//    image.setColorCount(2);
//    image.setColor(0, qRgba(255, 0, 0, 255));   // index 0 = red
//    image.setColor(1, qRgba(0, 0, 0, 0));       // index 1 = transparent

//    for(short x = 0; x < 256; ++x) {
//        for(short y = 0; y < 256; ++y) {
//            if (y < 128) {
//                // fill the part of the image with red color #0
//                image.setPixel(x, y, 0);
//            } else {
//                // fil the part of the image with transparent color #1
//                image.setPixel(x, y, 1);
//            }
//        }
//    }
}

/**
 * Added Feature
 * @brief ToolBar::mirrorFrameHorizontal flips each pixel of the sprite frame horizontally
 * @param grid - object holds all the pixels and each color of the pixel
 */
void ToolBar::mirrorFrameHorizontal(QVector<QVector<QColor>>& grid) {
    int gridSize = grid.size();

    for (int x = 0; x < gridSize / 2; x ++) {
        QVector<QColor> row = grid[x];
        int otherSide = gridSize - x;
        grid[x] = grid[otherSide];
        grid[otherSide] =  row;
    }
}

/**
 * Added Feature
 * @brief ToolBar::mirrorFrameVertical flips each pixel of the sprite frame vertically
 * @param grid - object holds all the pixels and each color of the pixel
 */
void ToolBar::mirrorFrameVertical(QVector<QVector<QColor>>& grid) {
    int gridSize = grid.size();

    for (int x = 0; x < gridSize; x ++) {
        for (int y = 0; y < gridSize/2; y ++) {
            QColor tempColor = grid[x][y];
            int otherSide = gridSize - y;
            grid[x][y] = grid[x][otherSide];
            grid[x][otherSide] = tempColor;
        }
    }
}

/**
 * Added Feature
 * @brief ToolBar::rotateFrame rotates the sprite frame 90 degrees clockwise
 * @param grid - object holds all the pixels and each color of the pixel
 */
void ToolBar::rotateFrame(QVector<QVector<QColor>>& grid) {
    int gridSize = grid.size();

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize/2; j++) {
            //Transpose grid
            QColor temp = grid[i][j];
            grid[i][j] = grid[j][i];
            grid[j][i] = temp;
        }
    }
    mirrorFrameHorizontal(grid);
}

void ToolBar::previewSprite() {

}
