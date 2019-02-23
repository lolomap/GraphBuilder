#include "graphicsview.h"

GraphicsView::GraphicsView(QMap<QString, QMap<QString, QString>>* table, QWidget *parent)
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

    buildGraphByTable();
}

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setMinimumSize(100, 100);

    scene = new QGraphicsScene;
    this->setScene(scene);

    group = new QGraphicsItemGroup;
    scene->addItem(group);
}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::buildGraphByFile(QStringList points, QStringList weights, QStringList lines)
{
    QPen pen(Qt::black);

    for(auto i : points)
    {
        if(!i.isEmpty() && i.contains(';') && i.contains('\t'))
        {
            QStringList s = i.split("\t");
            QString position = s[0];
            QString name = s[1];

            QStringList k = position.split(";");
            double posX = k[0].toDouble();
            double posY = k[1].toDouble();

            QGraphicsTextItem* pointName = new QGraphicsTextItem(name);
            pointName->setPos(posX, posY);
            scene->addItem(pointName);
            group->addToGroup(pointName);
            this->points.push_back(pointName);
        }
    }

    for(auto i : weights)
    {
        if(!i.isEmpty() && i.contains(';') && i.contains('\t'))
        {
            QStringList s = i.split("\t");
            QString position = s[0];
            QString name = s[1];

            QStringList k = position.split(";");
            double posX = k[0].toDouble();
            double posY = k[1].toDouble();

            QGraphicsTextItem* pointName = new QGraphicsTextItem(name);
            pointName->setPos(posX, posY);
            scene->addItem(pointName);
            group->addToGroup(pointName);
            this->points.push_back(pointName);
        }
    }

    for(auto i : lines)
    {
        QStringList linePoints = i.split(",");
        QStringList point1 = linePoints[0].split(";");
        QStringList point2 = linePoints[1].split(";");

        QGraphicsLineItem* line = scene->addLine(point1[0].toDouble(), point1[1].toDouble(),
                                                 point2[0].toDouble(), point2[1].toDouble(), pen);
        group->addToGroup(line);
        this->lines.push_back(line);
    }
}

void GraphicsView::buildGraphByTable()
{
    this->deleteItemsFromGroup(group);

    int height = this->height(), width = this->width();

    QPen pen(Qt::black);


    for(int i = 0; i < table->size(); i++)
    {
        QGraphicsTextItem* pointName = new QGraphicsTextItem(table->keys()[i]);

        if(i%2 == 0)
            pointName->setPos(X_MAX/2 + i*100, Y_MAX/2);
        else
            pointName->setPos(X_MAX/2 + i*100, Y_MAX/2 + 100);

        if(pointName->pos().x() > X_MAX || pointName->pos().y() > Y_MAX)
        {
            pointName->setPos(rand()%X_MAX, rand()%Y_MAX);
        }

        scene->addItem(pointName);
        group->addToGroup(pointName);
        points.push_back(pointName);
    }

    foreach(auto point1, points)
    {
        foreach(auto point2, points)
        {
            if(!table->value(point1->toPlainText()).value(point2->toPlainText()).isEmpty())
            {
                QGraphicsLineItem* line = scene->addLine(point1->x(), point1->y(),
                                                   point2->x(), point2->y(), pen);
                group->addToGroup(line);
                this->lines.push_back(line);
                double midleX = (point1->x()+point2->x())/2;
                double midleY = (point1->y()+point2->y())/2;
                QGraphicsTextItem* lineWeight = new QGraphicsTextItem(
                            table->value(point1->toPlainText()).value(point2->toPlainText()));
                lineWeight->setPos(midleX, midleY);
                scene->addItem(lineWeight);
                group->addToGroup(lineWeight);
                weights.push_back(lineWeight);
            }
        }
    }
}

void GraphicsView::addLine(double x1, double y1, double x2, double y2)
{
    QPen pen(Qt::black);
    QGraphicsLineItem* line = scene->addLine(x1, y1, x2, y2, pen);
    group->addToGroup(line);
    this->lines.push_back(line);
}

void GraphicsView::addPoint(double x, double y, QString name)
{
    QPen pen(Qt::black);
    QGraphicsTextItem* point = scene->addText(name);
    point->setPos(x,y);
    group->addToGroup(point);
    this->points.push_back(point);
}

void GraphicsView::addWeight(double x, double y, QString name)
{
    QPen pen(Qt::black);
    QGraphicsTextItem* weight = scene->addText(name);
    weight->setPos(x,y);
    group->addToGroup(weight);
    this->weights.push_back(weight);
}

void GraphicsView::removePoint(QGraphicsTextItem *item)
{
    if(points.contains(item))
    {
        scene->removeItem(item);
        group->removeFromGroup(item);
        points.removeOne(item);
    }
}

void GraphicsView::removeLine(QGraphicsLineItem *item)
{
    if(lines.contains(item))
    {
        scene->removeItem(item);
        group->removeFromGroup(item);
        lines.removeOne(item);
    }
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

