/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>

/**
 * @brief MainWindow::MainWindow Constructor that sets up the connections for the signals and slots for the
 * model to communicate with the view
 * @param model
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // changeCurrentFrame slot being called when framScrollBar value changed
    connect(ui->frameScrollBar,
            &QScrollBar::valueChanged,
            this,
            &MainWindow::changeCurrentFrame);

    // createNewProject slot being called when newProjectButton is clicked
    connect(ui->newProjectButton,
            &QPushButton::clicked,
            this,
            &MainWindow::createNewProject);

    // colorChange slot being called when current color have changed
    connect(&this->colorPicker,
            &QColorDialog::currentColorChanged,
            this,
            &MainWindow::colorChange);

    // rotateGrid slot being called when rotateButton is clicked
    connect(ui->rotateButton, &QPushButton::clicked,
            this, &MainWindow::rotateGrid);

    // flipGrid slot is being called when flipButton is clicked
    connect(ui->flipButton, &QPushButton::clicked,
            this, &MainWindow::flipGrid);

    // saveProject slot is being called when saveButton is clicked
    connect(ui->saveButton, &QPushButton::clicked,
            this, &MainWindow::saveProject);

    // initializing model by information of the canvas
    model.init(16, 600, 270, 10, canvas);
    current = QColor(0, 0, 0);

    // showing icons on the ui
    ui -> undoButton -> setIcon(QIcon(":/icons/undo.png"));
    ui -> redoButton -> setIcon(QIcon(":/icons/redo.png"));
    ui -> colorDialogButton -> setIcon(QIcon(":/icons/color.png"));
    ui -> Eraser -> setIcon(QIcon(":/icons/eraser.png"));
    ui -> rotateButton -> setIcon(QIcon(":/icons/rotation.png"));
    ui -> flipButton -> setIcon(QIcon(":/icons/flip.png"));
    ui -> shapeButton -> setIcon(QIcon(":/icons/geometric-shape.png"));
    ui -> addFrameBut -> setIcon(QIcon(":/icons/frame.png"));
    ui -> previewButton -> setIcon(QIcon(":/icons/preview.png"));
    ui -> paintBucketButton -> setIcon(QIcon(":/icons/paint-bucket.png"));
    ui -> gridCheckBox -> setIcon(QIcon(":/icons/grid.png"));
    ui -> newProjectButton -> setIcon(QIcon(":/icons/new-project.png"));
}

/**
 * @brief MainWindow::~MainWindow destuctor to delete the ui
 */
MainWindow::~MainWindow() {
    delete ui;
}

/**
 * @brief MainWindow::closeEvent method for closing the canvas
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event) {
    if (canvas.canvasChanged()) {
        QMessageBox::StandardButton result;
        // show warning message when canvas is being closed
        result = QMessageBox::question(this, "Warning", "You have unsaved changes. Would you like to save?",
                                       QMessageBox::Yes|QMessageBox::No);

        // if user wants to save project
        if (result == QMessageBox::Yes) {
            saveProject();
        }
    }
    event->accept();
}

/**
 * @brief MainWindow::rotateGrid method for rotating the canvas
 */
void MainWindow::rotateGrid() {
    canvas.rotateGrid();

    // add the canvas state to undoStack
    model.addToUndo();
    // clear the redoStack
    model.bypassRedo();

    update();
}

/**
 * @brief MainWindow::flipGrid method for flipping the canvas
 */
void MainWindow::flipGrid() {
    canvas.flipHorizontally();

    model.addToUndo();
    model.bypassRedo();

    update();
}

/**
 * @brief MainWindow::saveProject method for saving the canvas drawing
 */
void MainWindow::saveProject() {
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Project"), "",
            tr("Project Type (*.ssp);;All Files (*)"));

    model.saveFile(fileName);
}

/**
 * @brief MainWindow::on_gridCheckBox_stateChanged method for gridCheckBox state
 * @param arg1
 */
void MainWindow::on_gridCheckBox_stateChanged(int arg1) {
    if(arg1 != 0) {
        gridChecked = true;
    }
    else {
        gridChecked = false;
    }
    update();
}

/**
 * @brief MainWindow::createNewProject method for creating new canvas project
 */
void MainWindow::createNewProject() {
    if (canvas.canvasChanged()) {
        saveClauseCatch();
    }
    // if paintBucket was previously clicked, cancel it
    paintBucket = false;
    ui -> paintBucketButton -> setEnabled(true);

    // reset the canvas
    reset();
    int options = ui ->sizeOptions->currentIndex();
    size = model.gridSize(options);

    // re-initialize the state of the model
    model.init(size, 600, 270, 10, canvas);
}

/**
 * @brief MainWindow::reset method to reset current canvas
 */
void MainWindow::reset() {
    // re-initialize current frame and the frameScrollBar state
    changeCurrentFrame(0);
    ui->frameScrollBar->setMaximum(0);
}

/**
 * @brief MainWindow::mouseMoveEvent method for when mouse moves
 * @param event
 */
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    // if mouse is pressed and moving and user is not drawing shape
    if (event->type() == QEvent::MouseMove && pressed && !shapeChecked) {
        QPoint pixelPos = event -> pos();
        int x = pixelPos.x() - canvas.getOffsetX();
        int y = pixelPos.y() - canvas.getOffsetY();

        // if inside canvas
        if(canvas.boundsDetection(x, y) && !shapeChecked && !paintBucket){
            // update on current pixel position
            canvas.updateGrid(pixelPos, current);
            update();
        }
    }
}

/**
 * @brief MainWindow::mouseReleaseEvent method for when mouse is released
 * @param event
 */
void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    // if mouse is pressed
    if(pressed){
        // if user is drawing shape
        if(shapeChecked){
            bool isTriangle = ui -> shapeComboBox -> currentIndex() == 0;
            if(isTriangle) {
                // draw shape for triangle
                model.drawShape(event -> pos(), "triangle", current);
            }
            else if(!isTriangle) {
                // for rectangle
                model.drawShape(event -> pos(), "rectangle", current);
            }
            update();
            shapeChecked = false;
        }

        model.addToUndo();
        model.bypassRedo();
    }
    pressed = false;
}

/**
 * @brief MainWindow::mousePressEvent method for mouse being pressed
 * @param event
 */
void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPoint pixelPos = event -> pos();
    int x = pixelPos.x() - canvas.getOffsetX();
    int y = pixelPos.y() - canvas.getOffsetY();

    // if inside canvas
    if(canvas.boundsDetection(x, y)){
        pressed = true;
        // drawing shape
        if(shapeChecked){
            // initialize current pixel position on shape drawing
            model.initializeShapePos(pixelPos);
        }
        // using paint bucket
        else if(paintBucket) {
            // fill the area with the paint bucket
            canvas.fillArea(pixelPos.x(), pixelPos.y(), current);
            paintBucket = false;
            ui -> paintBucketButton -> setEnabled(true);
        }
        // regular drawing
        else {
            canvas.updateGrid(pixelPos, current);
        }
        update();
    }
}

/**
 * @brief MainWindow::paintEvent paintEvent method for drawing on the UI
 */
void MainWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    drawSprite(&painter, canvas.getGrid());

    int xLoc = ui -> frameLabel -> x();
    xLoc -= 10;
    int yLoc = ui -> frameLabel -> y();

    //if grid check box is checked
    if(gridChecked){
        int gridSize = (ui -> frameLabel ->width()) / 7;

        // draw vertical lines
        for(int i =0; i < 6; i++) {
            xLoc += gridSize;
            painter.drawLine(xLoc, yLoc, xLoc, yLoc + (ui -> frameLabel ->height()));
        }

        // re-initializing variables for grid
        xLoc = ui -> frameLabel -> x();
        xLoc -= 10;

        // draw horizontal lines
        for(int i = 0; i < 6; i++) {
            yLoc += gridSize;
            painter.drawLine(xLoc, yLoc, xLoc + (ui -> frameLabel -> width()), yLoc);
        }
    }
}

/**
 * @brief MainWindow::drawSprite method that draws the current state
 * @param painter
 * @param grid
 */
void MainWindow::drawSprite(QPainter *painter, QVector<QVector<QColor>>& grid) {
    for (int x = 0; x < grid.count(); x++) {
        for (int y = 0; y < grid[x].count(); y++) {
            // initializing values for drawing on the canvas
            QColor color = grid[x][y];
            int xCord = x, yCord = y;
            canvas.colRowsToAbsolutePos(xCord, yCord);
            int pixelSize = canvas.getPixelSize();
            QRect rectangle(xCord, yCord , pixelSize, pixelSize);
            // draws the pixel with corresponding pixelSize
            painter->fillRect(rectangle, color);
        }
    }
}

/**
 * @brief MainWindow::changeCurrentFrame method for when user chaning the frame
 * @param frame
 */
void MainWindow::changeCurrentFrame(int frame) {
    // change frame by frame number
    canvas.changeFrame(frame);
    // reset current frame
    model.resetCurrent();
    QString text = QString("Current Frame: %1").arg(canvas.getCurrentFrame());
    ui->frameCounter->setText(text);

    update();
}

/**
 * @brief MainWindow::on_previewButton_clicked method to open the preview window
 */
void MainWindow::on_previewButton_clicked() {
    previewWindow.show();
    previewWindow.previewSetup(canvas.getFrames(), canvas, size);
    previewWindow.startDrawing();
}

/**
 * @brief MainWindow::on_undoButton_clicked method for undo feature
 */
void MainWindow::on_undoButton_clicked() {
    model.undoDraw();
    update();
}

/**
 * @brief MainWindow::on_redoButton_clicked method for redo feature
 */
void MainWindow::on_redoButton_clicked() {
    model.redoDraw();
    update();
}

/**
 * @brief MainWindow::on_addFrameBut_clicked method for adding frames
 */
void MainWindow::on_addFrameBut_clicked() {
    canvas.addFrame();
    // add new undoStack and redoStack for added frame
    model.addUndoStack();
    model.addRedoStack();

    paintBucket = false;
    ui -> paintBucketButton -> setEnabled(true);

    // update scrollBar with added frame
    QScrollBar* bar = ui->frameScrollBar;
    int frame = canvas.getCurrentFrame();
    bar->setMaximum(frame);
    bar->setValue(frame);

    update();
}

/**
 * @brief MainWindow::on_shapeButton_clicked method for drawing shape feature
 */
void MainWindow::on_shapeButton_clicked() {
    shapeChecked = true;

    paintBucket = false;
    ui -> paintBucketButton -> setEnabled(true);

}

/**
 * @brief MainWindow::on_paintBucketButton_clicked method for paint bucket feature
 */
void MainWindow::on_paintBucketButton_clicked() {
    paintBucket = true;
    ui -> paintBucketButton->setEnabled(false);
}

/**
 * @brief MainWindow::on_colorDialogButton_clicked method for picking color feature
 */
void MainWindow::on_colorDialogButton_clicked() {
   colorPicker.setVisible(true);
}

/**
 * @brief MainWindow::colorChange method for changing color feature
 * @param color
 */
void MainWindow::colorChange(QColor color) {
   int red = current.red();
   int green = current.green();
   int blue = current.blue();
   ui->previousColorButton->setStyleSheet( QString("QPushButton {background-color: rgb("+QString::number(red)+","+QString::number(green)+","+QString::number(blue)+");} QPushButton:pressed {background-color: rgb(255,150,150);}"));

   // update color for model and canvas
   model.selectColor(color);
   current = color;
}

/**
 * @brief MainWindow::on_previousColorButton_clicked method for picking previous color feature
 */
void MainWindow::on_previousColorButton_clicked() {
    // update color with previous color for model and canvas
    current = model.previousColor;
    model.selectColor(model.previousColor);

    int red = model.previousColor.red();
    int green = model.previousColor.green();
    int blue = model.previousColor.blue();
    ui->previousColorButton->setStyleSheet( QString("QPushButton {background-color: rgb("+QString::number(red)+","+QString::number(green)+","+QString::number(blue)+");} QPushButton:pressed {background-color: rgb(255,150,150);}"));
}

/**
 * @brief MainWindow::on_loadButton_clicked method for loading a canvas project
 */
void MainWindow::on_loadButton_clicked() {
    if (canvas.canvasChanged()) {
        saveClauseCatch();
    }
    paintBucket = false;
    ui -> paintBucketButton -> setEnabled(true);

    // show UI to choose file and open file
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Sprite Project"), "",
            tr("Project Type (*.ssp);;All Files (*)"));

    if (!model.loadFile(fileName)) {
        QMessageBox::warning(this, tr("Load Failed"),
                             tr("Check if file is valid or file structure is correct"));
    }

    ui->frameScrollBar->setMaximum(canvas.getTotalFrame());
    ui->frameScrollBar->setValue(canvas.getCurrentFrame());

    update();
}

/**
 * @brief MainWindow::saveClauseCatch method for saving the canvas project
 */
void MainWindow::saveClauseCatch() {
    QMessageBox::StandardButton result;
    // show message box for saving the file
    result = QMessageBox::question(this, "Unsaved Changes", "Save Changes?",
                                   QMessageBox::Yes|QMessageBox::No);

    // if user wants to save the canvas
    if (result == QMessageBox::Yes) {
        saveProject();
    }
}

/**
 * @brief MainWindow::on_Eraser_clicked method for erasing feature
 * @param checked
 */
void MainWindow::on_Eraser_clicked(bool checked) {
    // if user wants to erase
    if (checked) {
        eraserHold = current;
        // change the current color to white
        current = QColor("white");

        paintBucket = false;
        ui -> paintBucketButton -> setEnabled(true);
    }
    // if user don't want to erase
    else {
        // change the eraser color to user's picked color
        current = eraserHold;
    }
}

