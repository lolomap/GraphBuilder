#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings(APP_NAME);
    ui->lineEdit->setText(settings.value(TABLE_STANDART_DIR, "").toString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadTable(QString file)
{
    try
    {
        QFile loadTableFile;
        if(!file.isEmpty())
            loadTableFile.setFileName(file);
        else return false;

        if(loadTableFile.open(QIODevice::ReadOnly))
        {
            QString solubility = loadTableFile.readAll();
            QStringList solubilityLines= solubility.split("\r\n");
            QStringList head = solubilityLines[0].split('\t');
            for(int i = 1; i < solubilityLines.size(); i++)
            {
                QStringList line = solubilityLines[i].split('\t');
                for (int j = 1; j < line.size(); j++)
                {
                    table[line[0]][head[j]] = line[j].toInt();
                }
            }
        }
        else return false;

        loadTableFile.close();
    }
    catch(QException)
    {
        return false;
    }

    return true;
}

void MainWindow::on_pushButton_clicked()
{
    if(!loadTable(ui->lineEdit->text()))
        QMessageBox::warning(this, "Ошибка",  "Таблица не загруженна");

    newGraph = new GraphicsView(&table);
    ui->gridLayout->addWidget(newGraph);
}

void MainWindow::on_toolButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Открыть файл", QDir::currentPath(), "Таблица (*.txt)");
    ui->lineEdit->setText(file);

    QSettings settings(APP_NAME);
    settings.setValue(TABLE_STANDART_DIR, file);
}
