#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QWidget>
#include <QMap>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QLineEdit>

#define X_MIN 0
#define X_MAX width - 10
#define Y_MIN 0
#define Y_MAX height - 10

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QMap<QString, QMap<QString, QString> > *table, int &mode, QWidget *parent = nullptr);
    explicit GraphicsView(int &mode, QWidget *parent = nullptr);
    ~GraphicsView();

    QList<QGraphicsTextItem*> points;
    QList<QGraphicsTextItem*> weights;
    QList<QGraphicsLineItem*> lines;

    void buildGraphByFile(QStringList points, QStringList weights, QStringList lines);

    void buildGraphByTable();


    void addLine(double x1, double y1, double x2, double y2);
    void addPoint(double x, double y, QString name);
    void addWeight(double x, double y, QString name);

    void removePoint(QGraphicsTextItem* item);
    void removeLine(QGraphicsLineItem* item);

private:
    QGraphicsScene* scene;
    QGraphicsItemGroup* group;

    int* mode;

    void deleteItemsFromGroup(QGraphicsItemGroup *group);

    void mouseReleaseEvent(QMouseEvent *event);

    QMap<QString, QMap<QString, QString>>* table = nullptr;

    bool secondClick = false;
    QPoint saveClick;
};

#endif // GRAPHICSVIEW_H
