/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 *
 * Handles logic that is not directly related to drawing on the canvas
 *
 * CS 3505
 * April 4, 2022
 */
#include "model.h"
#include <QMouseEvent>
#include <QStack>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

/**
 * @brief Model::Model constructor for the Model class
 */
Model::Model()
{
    currentColor = QColor(0, 0, 0);
    previousColor = QColor(0, 0, 0);
}

/**
 * @brief Model::init setting up initial variables for the Model class
 * @param gridSize - amount of columns for the canvas object
 * @param canvasSize - the area in which a user can actively draw
 * @param offsetX - sets the correct position of the canvas onto the whole window
 * @param offsetY - sets the correct position of the canvas onto the whole window
 * @param _canvas - reference of Canvas object
 */
void Model::init(int gridSize, int canvasSize, int offsetX, int offsetY, Canvas& _canvas) {
    PIXELSIZE = canvasSize / gridSize;
    actualSize = gridSize;
    canvas = &_canvas;
    canvas->initialize(canvasSize, canvasSize, offsetX, offsetY, PIXELSIZE);
    currentGrid = canvas->getGrid();  

    addUndoStack();
    addRedoStack();
}

/**
 * @brief Model::addUndoStack - adds a stack to hold grids for undo functions. Directly corresponds
 * to frame count.
 */
void Model::addUndoStack() {
    QStack<QVector<QVector<QColor>>> undoStack;
    undoStacks.push_back(undoStack);
}

/**
 * @brief Model::addRedoStack - adds a stack to hold grids for redo functions. Directly corresponds
 * to frame count.
 */
void Model::addRedoStack() {
    QStack<QVector<QVector<QColor>>> undoStack;
    redoStacks.push_back(undoStack);
}

/**
 * @brief Model::addToUndo pushes the previous grid to the stack and updates to the current state
 */
void Model::addToUndo() {
    int frameNum = canvas -> getCurrentFrame();
    QStack<QVector<QVector<QColor>>>& undoStack = undoStacks[frameNum];

    if (undoStack.count() > 20) {
        undoStack.pop_back();
    }
    undoStack.push(currentGrid);
    currentGrid = canvas->getGrid();
}

/**
 * @brief Model::bypassRedo clear the redo stack if a change is added after undoing
 */
void Model::bypassRedo() {
    int frameNum = canvas -> getCurrentFrame();
    QStack<QVector<QVector<QColor>>>& redoStack = redoStacks[frameNum];
    redoStack.clear();
}

/**
 * @brief Model::redoDraw retrieves the previous redo grid and updates it to the current grid
 */
void Model::redoDraw() {
    int frameNum = canvas -> getCurrentFrame();
     QStack<QVector<QVector<QColor>>>& undoStack = undoStacks[frameNum];
     QStack<QVector<QVector<QColor>>>& redoStack = redoStacks[frameNum];
    if (!redoStack.isEmpty()) {
        if (undoStack.count() > 20) {
            undoStack.pop_back();
        }
        undoStack.push(currentGrid);
        currentGrid = redoStack.pop();
        canvas->setGrid(currentGrid);
    }
}

/**
 * @brief Model::undoDraw retrieves the previous undo grid and updates it to the current grid
 */
void Model::undoDraw() {
    int frameNum = canvas -> getCurrentFrame();
    QStack<QVector<QVector<QColor>>>& undoStack = undoStacks[frameNum];
    QStack<QVector<QVector<QColor>>>& redoStack = redoStacks[frameNum];


    if (!undoStack.isEmpty()) {
        if (redoStack.count() > 20) {
            redoStack.pop_back();
        }
        redoStack.push(currentGrid);
        currentGrid = undoStack.pop();
        canvas->setGrid(currentGrid);
    }
}

/**
 * @brief Model::resetCurrent - refreshes the current grid to the most up to date version
 * held by the canvas
 */
void Model::resetCurrent() {
    currentGrid = canvas -> getGrid();
}

/**
 * @brief Model::resetUndoRedoStacks - clear the undo and redo stacks
 */
void Model::resetUndoRedoStacks() {
    undoStacks.clear();
    redoStacks.clear();
}


/**
 * @brief Model::gridSize changes the size of the grid and pixels based on the users selection
 * @param options - options from the spin box of the project size
 * @return - int to be sent to the view to update the canvas
 */
int Model::gridSize(int options) {
    int size = 0;
    switch (options) {
    case 0:
        size = 8;
        break;
    case 1:
        size = 16;
        break;
    case 2:
        size = 32;
        break;
    case 3:
        size = 64;
        break;
    }
    return size;
}

/**
 * @brief Model::selectColor updates and tracks the current and previous QColor used
 * @param newColor - the current color the user has selected
 */
void Model::selectColor(QColor newColor) {
    previousColor = currentColor;
    currentColor = newColor;
}

/**
 * @brief Model::drawShape - Calculates the positions/coordinates necessary to draw a triangle
 * or rectangle to the view
 *
 * @param shapePos - final position of the shape
 * @param shapeStr - type of shape
 * @param color - color to draw
 */
void Model::drawShape(QPoint shapePos, QString shapeStr, QColor color) {
    double smallX, bigX;
    int smallY, bigY;

    // switches the draw point based on final position
    if(shapePos.x() < initShapePos.x()) {
        smallX = shapePos.x();
        bigX = initShapePos.x();
    }else {
        smallX = initShapePos.x();
        bigX = shapePos.x();
    }

    if(shapePos.y() < initShapePos.y()){
        smallY = shapePos.y();
        bigY = initShapePos.y();
    }else {
        smallY = initShapePos.y();
        bigY = shapePos.y();
    }
    if(shapeStr == "triangle") {
        // for vertical line
        for(int y = smallY; y < bigY; y++) {
            paintPixel(initShapePos.x(), y, color);
        }

        // for horizontal line
        for(int x = smallX; x < bigX; x++) {
            paintPixel(x, initShapePos.y(), color);
        }

        double xDiff = bigX - smallX;
        double yDiff = bigY - smallY;

        // for diagonal line
        if((shapePos.x() == bigX && shapePos.y() == smallY) || (shapePos.x() == smallX && shapePos.y() == bigY)) {
            for(int x =0; x < xDiff; x++){
                double y = getYCoord(yDiff/xDiff, x, smallY);
                paintPixel(x + smallX, y, color);
            }
        } else {
            for(int x =0; x < xDiff; x++){
                double y = getYCoord(yDiff/xDiff, x, smallY);
                paintPixel(bigX - x, y, color);
            }
        }
    }
    else if(shapeStr == "rectangle"){

        // horizontal lines
        for(int x = smallX; x < bigX; x++) {
            paintPixel(x, initShapePos.y(), color);
            paintPixel(x, shapePos.y(), color);
        }

        // vertical lines
        for(int y = smallY; y < bigY; y++) {
            paintPixel(initShapePos.x(), y, color);
            paintPixel(shapePos.x(), y, color);
        }
    }
}

/**
 * @brief Model::getYCoord - gets y coordinate based on slope. Specifically used for
 * triangle.
 *
 * @param slope - slope of the line
 * @param x - x position
 * @param b - height
 * @return corresponding y coordinate to x position
 */
double Model::getYCoord(double slope, int x, int b) {
    return slope*x + b;
}

/**
 * @brief Model::paintPixel - sets pixel in canvas to color
 * @param x - x position
 * @param y - y position
 * @param color - color to set pixel
 */
void Model::paintPixel(int x, int y, QColor color) {
        QPoint pixel(x, y);
        canvas->updateGrid(pixel, color);
}

/**
 * @brief Model::initializeShapePos - initialize the starting position of shape
 * @param _initShapePos - starting position
 */
void Model::initializeShapePos(QPoint _initShapePos) {
    initShapePos = _initShapePos;
}

/**
 * @brief Model::saveFile Saving sprites using json and .ssp
 * @param fileName - the name of the file to be saved under
 */
void Model::saveFile(QString fileName) {
    if (fileName.isEmpty())
        return;

    int totalFrame = canvas->getTotalFrame();
    QVector<QVector<QVector<QColor>>> frames = canvas->getFrames();
    QJsonObject project;

    // add constants to the json obj
    project.insert("height", actualSize);
    project.insert("width", actualSize);
    project.insert("numberOfFrames", totalFrame);

    QJsonObject frame;
    for (int n = 0; n < totalFrame; n ++) {
        QJsonObject rows;
        for (int y = 0 ; y < actualSize; y++) {
            QJsonArray cols;
            for (int x = 0; x < actualSize; x++) {
                // push row values into the JsonArray Cols
                QColor color = frames[n][x][y];
                QJsonArray colVals{color.red(), color.green(), color.blue(), color.alpha()};
                cols.push_back(colVals);
            }
            rows.insert(QString("row%1").arg(y), cols);
        }
        frame.insert(QString("frame%1").arg(n), rows);
    }

    //Convert JsonObj to document and write it in file
    project.insert("frames", frame);
    QJsonDocument document;
    document.setObject(project);
    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        file.write(document.toJson());
        file.close();
    }
    canvas->resetCanvasChange();
}

/**
 * @brief Model::loadFile method to load .ssp files into our sprite editor
 * @param file_path name of the file to be loaded
 * @return true if loaded correctly, false otherwise
 */
bool Model::loadFile(QString file_path){
   QFile file_obj(file_path);

   // if failed to open return false
   if (file_obj.open(QIODevice::ReadOnly)) {
       QByteArray bytes = file_obj.readAll();
       file_obj.close();

       //return false for any errors in converting file bytes to json
       QJsonParseError jsonError;
       QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
       if (jsonError.error != QJsonParseError::NoError) {
           return false;
       }

       if (document.isObject()) {
           //Assumes all files are correctly formatted and assumes that all canvases are square
           try{
               QJsonObject docObj = document.object();

               // Grab constants from file
               int gridX, gridY, numberOfFrames;
               gridX = docObj.value("width").toInt();
               gridY = docObj.value("height").toInt();
               numberOfFrames = docObj.value("numberOfFrames").toInt();

               // reinitialize vars
               actualSize = gridX;
               resetUndoRedoStacks();
               canvas->initializeFromJson(gridX, gridY, numberOfFrames);

               QJsonObject frameObj = docObj.value("frames").toObject();
               for (int n = 0; n < numberOfFrames; n++) {
                   QVector<QVector<QColor>> grid;
                   grid.resize(gridX, QVector<QColor>(gridY));

                   QString frameNum = QString("frame%1").arg(n);
                   QJsonObject curFrame = frameObj[frameNum].toObject();
                   for (int y = 0; y < gridY; y++) {
                       QString rowNum = QString("row%1").arg(y);
                       QJsonArray row = curFrame.take(rowNum).toArray();
                       for (int x = 0; x < gridX; x++) {
                           QJsonArray rgba = row[x].toArray();

                           QColor pix(rgba[0].toInt(), rgba[1].toInt(), rgba[2].toInt(), rgba[3].toInt());
                           grid[x][y] = pix;
                       }
                   }
                   canvas->setGrid(grid, n);
                   addUndoStack();
                   addRedoStack();
               }
               currentGrid = canvas->getGrid();
               return true;
           } catch (...) {
               return false;
           }
       }
   }
   return false;
}

