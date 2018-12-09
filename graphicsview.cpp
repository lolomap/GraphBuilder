#include "graphicsview.h"

GraphicsView::GraphicsView(QMap<QString, QMap<QString, int>>* table, QWidget *parent)
    : QGraphicsView(parent)
{
    this->table = table;

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setMinimumSize(100, 100);

    scene = new QGraphicsScene;
    this->setScene(scene);

    group = new QGraphicsItemGroup;
    scene->addItem(group);

    timer = new QTimer;
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));
    timer->start(50);
}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::slotAlarmTimer()
{
    this->deleteItemsFromGroup(group);

    int height = this->height(), width = this->width();
    //scene->setSceneRect(0, 0, height, width);

    QPen pen(Qt::black);

    /*
     * ЗДЕСЬ РИСУЕМ
     */
    //group->addToGroup(scene->addEllipse(X_MIN, Y_MIN, 5, 5, pen, QBrush(Qt::black)));
    //group->addToGroup(scene->addEllipse(X_MAX, Y_MAX, 5, 5, pen, QBrush(Qt::black)));

    QList<QGraphicsTextItem*> points;
    for(int i = 0; i < table->size(); i++)
    {
        QGraphicsTextItem* pointName = new QGraphicsTextItem(table->keys()[i]);

        if(i%2 == 0)
            pointName->setPos(X_MAX/2 + i*100, Y_MAX/2);
        else
            pointName->setPos(X_MAX/2 + i*100, Y_MAX/2 + 100);

        scene->addItem(pointName);
        group->addToGroup(pointName);
        points.push_back(pointName);
    }

    foreach(auto point1, points)
    {
        foreach(auto point2, points)
        {
            if(table->value(point1->toPlainText()).value(point2->toPlainText()) != 0)
            {
                group->addToGroup(scene->addLine(point1->x(), point1->y(),
                                                 point2->x(), point2->y(), pen));
                double midleX = (point1->x()+point2->x())/2;
                double midleY = (point1->y()+point2->y())/2;
                QGraphicsTextItem* lineWeight = new QGraphicsTextItem(
                            QString::number(table->value(point1->toPlainText()).value(point2->toPlainText())));
                lineWeight->setPos(midleX, midleY);
                scene->addItem(lineWeight);
                group->addToGroup(lineWeight);
            }
        }
    }
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    timer->start(50);   // Как только событие произошло стартуем таймер для отрисовки
    QGraphicsView::resizeEvent(event);  // Запускаем событие родителького класса
}

void GraphicsView::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
    /* Перебираем все элементы сцены, и если они принадлежат группе,
     * переданной в метод, то удаляем их
     * */
    foreach( QGraphicsItem *item, scene->items(group->boundingRect())) {
       if(item->group() == group ) {
          delete item;
       }
    }
}
