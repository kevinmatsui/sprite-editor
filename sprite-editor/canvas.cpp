/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 *
 * Canvas class handles all logic directly related to drawing functions
 *
 * CS 3505
 * April 4, 2022
 */
#include "canvas.h"
#include <QPoint>
#include <QColor>
#include <QDebug>
/**
 * @brief Canvas::Canvas constructor for a canvas object
 */
Canvas::Canvas() {}

/**
 * @brief Canvas::initialize - Initialize variables necessary for canvas functions
 *
 * @param _height - height (in pixels) of the drawable area
 * @param _width - width (in pixels) of the drawable area
 * @param _offsetX - x position of the draw area in main window
 * @param _offsetY - y position of the draw area in main window
 * @param _pixelSize - pixel size relative to the drawable area
 */
void Canvas::initialize(int _height, int _width, int _offsetX, int _offsetY, int _pixelSize) {
    height = _height;
    width = _width;
    offsetX = _offsetX;
    offsetY = _offsetY;
    pixelSize = _pixelSize;
    changed = false;

    currentFrame = 0;
    frames.clear();
    // actual size of sprite -- not dimensions of drawable area
    numCols = width / pixelSize;
    numRows = height / pixelSize;

    addFrame();
}

/**
 * @brief Canvas::initializeFromJson - Initializes variables needed for drawing calculations
 *
 * @param gridX - number of columns
 * @param gridY - number of rows
 * @param numFrame - number of frames
 */
void Canvas::initializeFromJson(int gridX, int gridY, int numFrame) {
    frames.resize(numFrame);

    changed = false;
    currentFrame = 0;

    numCols = gridX;
    numRows = gridY;

    //assumes grid will always be square
    pixelSize = height / gridX;
}

/**
 * @brief Canvas::changeBackground - Fills the whole canvas to inputed
 * color
 *
 * @param background - new background
 */
void Canvas::changeBackground(QColor background) {
    QVector<QVector<QColor>>& currentGrid = frames[currentFrame];

    for(int x = 0; x < numCols; x ++) {
        currentGrid[x].fill(background);
    }
}

/**
 * @brief Canvas::rotateGrid - rotate the grid 90 degrees clockwise
 */
void Canvas::rotateGrid() {
    QVector<QVector<QColor>>& grid = frames[currentFrame];

    for (int x = 0; x < numCols; x++) {
        for (int y = x; y < numRows; y++) {
            //Transpose grid
            QColor temp = grid[x][y];
            grid[x][y] = grid[y][x];
            grid[y][x] = temp;
        }
    }

    flipHorizontally();
    changed = true;
}

/**
 * @brief Canvas::flipHorizontally - flips the canvas along middle vertical
 * axis
 */
void Canvas::flipHorizontally() {
    QVector<QVector<QColor>>& grid = frames[currentFrame];

    for (int x = 0; x < numCols / 2; x ++) {
        QVector<QColor> row = grid[x];

        int otherSide = numCols - 1 - x;
        grid[x] = grid[otherSide];
        grid[otherSide] =  row;
    }
    changed = true;
}

/**
 * @brief Canvas::setGrid - Sets the current grid/frame to the inputted grid
 * @param newGrid - new grid
 * @param frameNum - index of grid to change
 */
void Canvas::setGrid(QVector<QVector<QColor>> newGrid, int frameNum) {
    frames[frameNum] = newGrid;
}

/**
 * @brief Canvas::resetCanvasChange - Resets the changed variable to false - use for
 * saves and loads.
 */
void Canvas::resetCanvasChange() {
    changed = false;
}

/**
 * @brief Canvas::canvasChanged - gets changed variable - called to check
 * if canvas is changed
 *
 * @return if canvas was changed
 */
bool Canvas::canvasChanged() {
    return changed;
}

/**
 * @brief Canvas::updateGrid - update the current grid's values
 * @param absolutePosition - click position
 * @param col - new color
 */
void Canvas::updateGrid(QPoint absolutePosition, QColor col) {
    int xPosActual = absolutePosition.x() - offsetX;
    int yPosActual = absolutePosition.y() - offsetY;

    if (boundsDetection(xPosActual, yPosActual)) {
        int column = xPosActual / pixelSize;
        int row = yPosActual / pixelSize;

        QVector<QVector<QColor>>& currentGrid = frames[currentFrame];

        //brush size here
        currentGrid[column][row] = col;
        changed = true;
    }
}

/**
 * @brief Canvas::getTotalFrame - gets the total frames in the canvas
 * @return number of frames
 */
int Canvas::getTotalFrame() {
    return frames.count();
}

/**
 * @brief Canvas::getColor - Gets color at point
 * @param point - mouse click position
 * @return - color at point
 */
QColor Canvas::getColor(QPoint point) {
    int x = point.x() - offsetX;
    int y = point.y() - offsetY;

    if (boundsDetection(x, y)) {
        x = x/pixelSize;
        y = y/pixelSize;

        QVector<QVector<QColor>> currentGrid = frames[currentFrame];
        return currentGrid[x][y];
    }
    return QColor("White");
}

/**
 * @brief Canvas::fillArea - header method for filling an area with a color
 *
 * @param absoluteX - x position of mouse click
 * @param absoluteY - y position of mouse click
 * @param newColor - color filling the area
 */
void Canvas::fillArea(int absoluteX, int absoluteY, QColor newColor) {
    QPoint point = QPoint(absoluteX,absoluteY);
    QColor prevColor = getColor(point);
    absoluteToRowsCols(absoluteX, absoluteY);
    floodFillRecursive(absoluteX, absoluteY, prevColor, newColor);
}

/**
 * @brief Canvas::floodFillRecursive - recursive call to calculate positions that
 * need to be filled with color
 * @param x - x component (column)
 * @param y - y component (row)
 * @param prevColor - old color to fill
 * @param newColor - new color to replace old color
 */
void Canvas::floodFillRecursive(int x, int y, QColor prevColor, QColor newColor)
{
    QVector<QVector<QColor>> &currentGrid = frames[currentFrame];
    if (x < 0 || x >= currentGrid.size() || y < 0 || y >= currentGrid[x].size()) {
        return;
    }
    if (currentGrid[x][y] == newColor) {
        return;
    }
    if (currentGrid[x][y] != prevColor){
        return;
    }
    currentGrid[x][y] = newColor;
    floodFillRecursive(x-1, y, prevColor, newColor);
    floodFillRecursive(x+1, y, prevColor, newColor);
    floodFillRecursive(x, y-1, prevColor, newColor);
    floodFillRecursive(x, y+1, prevColor, newColor);
}

/**
 * @brief Canvas::boundsDetection - detects if x and y position is in the valid drawable
 * position
 * @param x - x position of the mouse position
 * @param y - y position of the mouse position
 * @return - if (x, y) is a valid drawable position
 */
bool Canvas::boundsDetection(int x, int y) {
    // offset the width and height to ensure everything is inside
    return (x > 0 && y > 0) && (x < width - 10 && y < height - 10);
}

/**
 * @brief Canvas::changeFrame - Change the current frame
 * @param frame - index of frame
 */
void Canvas::changeFrame(int frame) {
    if (frame < frames.count()) {
        currentFrame = frame;
    }
}

/**
 * @brief Canvas::getCurrentFrame - gets the index of current frame
 * @return - index of current frame
 */
int Canvas::getCurrentFrame() {
    return currentFrame;
}

/**
 * @brief Canvas::getPixelSize - gets the pixel size
 * @return - pixel size
 */
int Canvas::getPixelSize() {
    return pixelSize;
}

/**
 * @brief Canvas::addFrame - adds a new grid to the frame
 * Sets the current frame to the new frame
 */
void Canvas::addFrame() {
    QVector<QVector<QColor>> temp;
    temp.resize(numCols, QVector<QColor>(numRows));

    frames.push_back(temp);

    currentFrame = frames.size() - 1;

    changeBackground(QColor("white"));
}

/**
 * @brief Canvas::resetCanvas - resets the canvas to default
 */
void Canvas::resetCanvas() {
    //resets the grid to default
    frames[currentFrame].resize(numCols, QVector<QColor>(numRows));
}

/**
 * @brief Canvas::colRowsToAbsolutePos - calculates the column and rows to absolute positions
 * in the main window.
 *
 * @param x - column to be converted
 * @param y - row to be converted
 */
void Canvas::colRowsToAbsolutePos(int &x, int &y) {
    x = (x * pixelSize) + offsetX;
    y = (y * pixelSize) + offsetY;
}

/**
 * @brief Canvas::absoluteToRowsCols - calculates the absolute positions in the main window to
 * columns and rows that's usuable by the grid
 *
 * @param x - x position of the mouse input
 * @param y - y position of the mouse input
 */
void Canvas::absoluteToRowsCols(int &x, int &y) {
    x = (x - offsetX) / pixelSize;
    y = (y - offsetY) / pixelSize;
}

/**
 * @brief Canvas::setGrid - sets the current frame to the new grid
 * @param newGrid - new grid to input into frame
 */
void Canvas::setGrid(QVector<QVector<QColor>> newGrid) {
    frames[currentFrame] = newGrid;
}

/**
 * @brief Canvas::getGrid - Gets the grid at current frame
 * @return - return the current grid representing drawing
 */
QVector<QVector<QColor>>& Canvas::getGrid() {
    return frames[currentFrame];
}

/**
 * @brief Canvas::getFrames - gets the frames of canvas
 * @return frames of canvas
 */
QVector<QVector<QVector<QColor>>>& Canvas::getFrames() {
    return frames;
}

int Canvas::getOffsetX(){
    return offsetX;
}
int Canvas::getOffsetY(){
    return offsetY;
}
