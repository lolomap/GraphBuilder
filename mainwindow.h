#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMap>
#include <QException>
#include <QDebug>
#include <QMessageBox>
#include <ActiveQt/qaxobject.h>

#include "graphicsview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_open_button_clicked();


    void on_pushButton_clicked();

    void on_openGraph_button_clicked();

    void on_create_button_clicked();

    void on_addPoint_button_clicked();

    void on_deletePoint_button_clicked();

    void on_addLine_button_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QString, QMap<QString, QString>> table;
    GraphicsView* newGraph;
    bool graphAgain;

    void showEditingGraph();
    void hideEditingGraph();
};

#endif // MAINWINDOW_H
