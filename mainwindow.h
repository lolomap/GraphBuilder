#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphicsview.h"
#include <QMap>
#include <QFile>
#include <QException>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>

#define APP_NAME "LMM", "GraphBuilder"
#define TABLE_STANDART_DIR "standart dir"

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
    void on_pushButton_clicked();

    void on_toolButton_clicked();

private:
    Ui::MainWindow *ui;

    GraphicsView* newGraph;

    QMap<QString, QMap<QString, int>> table;
    bool loadTable(QString file);
};

#endif // MAINWINDOW_H
