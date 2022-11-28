/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#include "previewform.h"
#include "ui_previewform.h"
#include <QImage>

/**
 * @brief PreviewForm::PreviewForm constructor to initialize all variables needed for the class.
 * @param parent inherits from QWidget
 */
PreviewForm::PreviewForm(QWidget *parent) : QWidget(parent), ui(new Ui::PreviewForm) {
    ui->setupUi(this);
    vectorIndex = 0;
    selectedFPS = 0;
    xOffSet = ui -> spriteLabel -> x();
    yOffSet = ui -> spriteLabel -> y();
    activated = true;
}

/**
 * @brief PreviewForm::~PreviewForm destructor for proper memory management
 */
PreviewForm::~PreviewForm() {
    delete ui;
}

/**
 * @brief PreviewForm::on_CloseWindow_clicked slot to close the preview window when the close button is clicked.
 */
void PreviewForm::on_CloseWindow_clicked() {
    this->hide();
}

/**
 * @brief PreviewForm::startDrawing displays the first frame of the sprite when the preview window is opened
 */
void PreviewForm::startDrawing() {
    update();
}

/**
 * @brief PreviewForm::previewSetup communication between mainwindow and previewform containing sprite frames and Canvas object
 * @param canvasFrames - contains the frames of the sprite to be displayed
 * @param _canvas - reference of the Canvas object to retrieve the pixel size later
 */
void PreviewForm::previewSetup(QVector<QVector<QVector<QColor>>> canvasFrames, Canvas& _canvas, int _imageSize) {
    spriteFrames = canvasFrames;
    canvas = &_canvas;
    imageSize = _imageSize;
}

/**
 * @brief PreviewForm::on_spinBox_valueChanged slot to update the number of frames per second the user selected
 * @param fps - value from the spin box representing frames per second
 */
void PreviewForm::on_spinBox_valueChanged(int fps) {
    // starts off the loop for displaying the sprite's frames
    if (activated) {
        QTimer::singleShot(0, this, &PreviewForm::displayFrame);
        activated = false;
    }
    selectedFPS = fps;
}

/**
 * @brief PreviewForm::displayFrame displays the sprite's frame using a QTimer to schedule when
 * the next frame will be shown
 */
void PreviewForm::displayFrame() {
    QTimer::singleShot((1000 / selectedFPS), this, &PreviewForm::displayNextFrame);
    update();
}

/**
 * @brief PreviewForm::displayNextFrame updates the vector index to display the next frame
 */
void PreviewForm::displayNextFrame() {
    // resets the vector index to 0 if at the end of the vector, otherwise increments by 1
    if (vectorIndex == spriteFrames.size() - 1) {
        vectorIndex = 0;
    } else {
        vectorIndex++;
    }
    update();
    displayFrame();
}

/**
 * @brief PreviewForm::paintEvent sets up the painter to draw each frame
 */
void PreviewForm::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    drawSprite(&painter, spriteFrames[vectorIndex]);
}

/**
 * @brief PreviewForm::drawSprite method method to draw the frame onto the preview form
 * @param painter - object to draw on widgets
 * @param grid - contains the location of each pixel and color of the pixel
 */
void PreviewForm::drawSprite(QPainter *painter, QVector<QVector<QColor>> &grid) {
    for (int x = 0; x < grid.count(); x++) {
        for (int y = 0; y < grid[x].count(); y++) {
            QColor color = grid[x][y];
            int pixelSize = canvas->getPixelSize();
            int xCord = (x * pixelSize) + xOffSet;
            int yCord = (y * pixelSize) + yOffSet;
            // if the user selects to view the sprite at actual size,
            // the following displays the sprite at pixel size 1
            if (actualSize) {
                pixelSize = 1;
                xCord = (x * pixelSize) + xOffSet + (ui->spriteLabel->width() / 2);
                yCord = (y * pixelSize) + yOffSet + (ui->spriteLabel->height() / 2);
            }
            // fills the designated rectangle with the sprite frame
            QRect rectangle(xCord, yCord, pixelSize, pixelSize);
            painter->fillRect(rectangle, color);
        }
    }

}

/**
 * @brief PreviewForm::on_actualSizeCheckBox_stateChanged method alters the size of the sprite in the preview window
 * @param spriteSize integer representing true (1) or false (0)
 */
void PreviewForm::on_actualSizeCheckBox_stateChanged(int spriteSize) {
    actualSize = spriteSize;
    update();
}

