#include "mainwindow.h"
#include "ui_mainwindow.h"

#define DEBUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(this->width(), this->height());
    graphAgain = false;
    ui->pushButton->setEnabled(false);

    hideEditingGraph();
    ui->pointX_input->setText("0");
    ui->pointY_input->setText("0");

    connect(this, &MainWindow::converting_ready, this, &MainWindow::build_converted);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_button_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), "Table (*.txt *.xlsx)");
    QtConcurrent::run(this, &MainWindow::converting, fileName);
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save file", QDir::currentPath(), "Graph (*.gph)");
    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly))
    {
        for(auto i : newGraph->points)
        {
            file.write(QString::number(i->pos().x()).toStdString().c_str());
            file.write(";");
            file.write(QString::number(i->pos().y()).toStdString().c_str());
            file.write("\t");
            file.write(i->toPlainText().toStdString().c_str());
            file.write("\n");


        }

        file.write("/\n");

        for(auto i : newGraph->weights)
        {
            file.write(QString::number(i->pos().x()).toStdString().c_str());
            file.write(";");
            file.write(QString::number(i->pos().y()).toStdString().c_str());
            file.write("\t");
            file.write(i->toPlainText().toStdString().c_str());
            file.write("\n");
        }

        file.write("@");

        for(auto i : newGraph->lines)
        {
            file.write("\n");
            file.write(QString::number(i->line().p1().x()).toStdString().c_str());
            file.write(";");
            file.write(QString::number(i->line().p1().y()).toStdString().c_str());
            file.write(",");
            file.write(QString::number(i->line().p2().x()).toStdString().c_str());
            file.write(";");
            file.write(QString::number(i->line().p2().y()).toStdString().c_str());
        }

        file.flush();
    }
    else
        QMessageBox::warning(this, "Error", "Fail saving file");

    if(file.isOpen())
    {
        file.close();
    }
}

void MainWindow::on_openGraph_button_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), "Graph (*.gph)");
    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly))
    {
        QString data = file.readAll();
        QStringList dataSplited = data.split("\n");
        QStringList points;
        for(int i(0); i < dataSplited.indexOf("/"); i++)
        {
            points.push_back(dataSplited[i]);
        }
        QStringList weights;
        for(int i(dataSplited.indexOf("/")+1); i < dataSplited.indexOf("@"); i++)
        {
            weights.push_back(dataSplited[i]);
        }
        QStringList lines;
        for(int i(dataSplited.indexOf("@")+1); i < dataSplited.size(); i++)
        {
            lines.push_back(dataSplited[i]);
        }

        newGraph = new GraphicsView();
        newGraph->buildGraphByFile(points, weights, lines);
        if(graphAgain)
        {
            QWidget* widget = ui->gridLayout->itemAtPosition(0,0)->widget();
            ui->gridLayout->removeWidget(widget);
            delete widget;
        }
        ui->gridLayout->addWidget(newGraph, 0, 0);
        graphAgain = true;

        showEditingGraph();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Fail openning file");
    }
}

void MainWindow::on_create_button_clicked()
{
    if(graphAgain)
    {
        QWidget* widget = ui->gridLayout->itemAtPosition(0,0)->widget();
        ui->gridLayout->removeWidget(widget);
        delete widget;
    }

    newGraph = new GraphicsView();
    ui->gridLayout->addWidget(newGraph, 0, 0);



    graphAgain = true;

    showEditingGraph();

    ui->pushButton->setEnabled(true);
}

void MainWindow::on_addPoint_button_clicked()
{
    if(graphAgain)
    {
        QString pointName = ui->pontName_input->text();
        double x = ui->pointX_input->text().toDouble();
        double y = ui->pointY_input->text().toDouble();
        newGraph->addPoint(x, y, pointName);


        ui->pointX_input->setText("0");
        ui->pointY_input->setText("0");
    }
}

void MainWindow::showEditingGraph()
{
    ui->label_2->show();
    ui->pontName_input->show();
    ui->addPoint_button->show();
    ui->label_3->show();
    ui->pointX_input->show();
    ui->pointY_input->show();
    ui->pointNameDelete_input->show();
    ui->label_4->show();
    ui->deletePoint_button->show();
    ui->line_2->show();
    ui->label_5->show();
    ui->label_6->show();
    ui->label_7->show();
    ui->linePoint_input1->show();
    ui->linePoint_input2->show();
    ui->weight_input->show();
    ui->line_3->show();
    ui->addLine_button->show();
}

void MainWindow::hideEditingGraph()
{
    ui->label_2->hide();
    ui->pontName_input->hide();
    ui->addPoint_button->hide();
    ui->label_3->hide();
    ui->pointX_input->hide();
    ui->pointY_input->hide();
    ui->pointNameDelete_input->hide();
    ui->label_4->hide();
    ui->deletePoint_button->hide();
    ui->line_2->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->linePoint_input1->hide();
    ui->linePoint_input2->hide();
    ui->weight_input->hide();
    ui->line_3->hide();
    ui->addLine_button->hide();
}

void MainWindow::converting(QString fileName)
{
    QString fileType = fileName.split('.').last();
    if(fileType == "txt")
    {
        QFile tableFile(fileName);
        if(tableFile.open(QIODevice::ReadOnly))
        {
            try {
                QString data = tableFile.readAll();
                QStringList lines = data.split("\r\n");
                QStringList head = lines[0].split('\t');
                for(int i = 1; i < lines.size(); i++)
                {
                    QStringList line = lines[i].split('\t');
                    for(int j = 1; j < line.size(); j++)
                    {
                        table[line[0]][head[j]] = line[j];
                    }
                }

            } catch (QException ex) {
            #ifdef DEBUG
                qDebug() << ex.what();
            #endif
              QMessageBox::warning(this, "Error", "Fail opening file");
            }
        }
        else QMessageBox::warning(this, "Error", "Fail opening file");
        if(tableFile.isOpen()) tableFile.close();

    }
    else if(fileType == "xlsx")
    {
        // получаем указатель на Excel
        QAxObject *mExcel = new QAxObject("Excel.Application",this);
        // на книги
        QAxObject *workbooks = mExcel->querySubObject("Workbooks");
        // на директорию, откуда грузить книг
        QAxObject *workbook = workbooks->querySubObject( "Open(const QString&)", fileName );
        // на листы
        QAxObject *mSheets = workbook->querySubObject( "Sheets" );
        // указываем, какой лист выбрать
        QAxObject *StatSheet = mSheets->querySubObject( "Item(const QVariant&)", QVariant("mainSheet") );



        // получение указателя на ячейку [строка][столбец] ((!)нумерация с единицы)
        //QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 1);

        QStringList head;
        QAxObject* cell = nullptr;
        int i = 2;
        do
        {
            cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, i);
            head.push_back(cell->property("Value").toString());
            i++;
        }while(!cell->property("Value").toString().isEmpty());

        i = 2;
        do
        {
            cell = StatSheet->querySubObject("Cells(Qvariant,Qvariant)", i, 1);
            QString lineName = cell->property("Value").toString();

            for (int j = 2; j < head.size(); j++) {
                cell = StatSheet->querySubObject("Cells(QVariant,Qvariant)", i, j);
                table[lineName][head[j-2]] = cell->property("Value").toString();
            }
            cell = StatSheet->querySubObject("Cells(Qvariant,Qvariant)", i, 1);
            i++;
        }while(!cell->property("Value").toString().isEmpty());

        // вставка значения переменной data (любой тип, приводимый к QVariant) в полученную ячейку
        //cell->setProperty("Value", QVariant("Some value"));



        // освобождение памяти
        delete cell;
        delete StatSheet;
        delete mSheets;
        workbook->dynamicCall("Save()");
        workbook->dynamicCall("Close()");
        delete workbook;
        //закрываем книги
        delete workbooks;
        //закрываем Excel
        mExcel->dynamicCall("Quit()");
        delete mExcel;
    }
    else QMessageBox::warning(this, "Error", "Fail opening file");


    emit converting_ready();
}

void MainWindow::on_deletePoint_button_clicked()
{
    QString pointName = ui->pointNameDelete_input->text();

    foreach(auto point, newGraph->points)
    {
        if(point->toPlainText() == pointName)
            newGraph->removePoint(point);
    }
}

void MainWindow::on_addLine_button_clicked()
{
    QString point1 = ui->linePoint_input1->text();
    QString point2 = ui->linePoint_input2->text();
    QString weight = ui->weight_input->text();

    double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    bool finded1 = false, finded2 = false;

    foreach(auto i, newGraph->points)
    {
        if(i->toPlainText() == point1)
        {
            x1 = i->pos().x();
            y1 = i->pos().y();
            finded1 = true;
        }
        if(i->toPlainText() == point2)
        {
            x2 = i->pos().x();
            y2 = i->pos().y();
            finded2 = true;
        }
    }

    if(!(finded1 && finded2))
        return;

    newGraph->addLine(x1, y1, x2, y2);
    newGraph->addWeight((x1+x2)/2, (y1+y2)/2, weight);
}

void MainWindow::build_converted()
{
    newGraph = new GraphicsView(&table);
    if(graphAgain)
    {
        QWidget* widget = ui->gridLayout->itemAtPosition(0,0)->widget();
        ui->gridLayout->removeWidget(widget);
        delete widget;
    }
    ui->gridLayout->addWidget(newGraph, 0, 0);
    graphAgain = true;

    ui->pushButton->setEnabled(true);
    showEditingGraph();
}
