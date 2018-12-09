#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QWidget>
#include <QMap>

#define X_MIN 0
#define X_MAX width - 10
#define Y_MIN 0
#define Y_MAX height - 10

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QMap<QString, QMap<QString, int> > *table, QWidget *parent = nullptr);
    ~GraphicsView();

private slots:
    void slotAlarmTimer();

private:
    QGraphicsScene* scene;
    QGraphicsItemGroup* group;

    QTimer* timer;


    void resizeEvent(QResizeEvent *event);
    void deleteItemsFromGroup(QGraphicsItemGroup *group);

    QMap<QString, QMap<QString, int>>* table;
};

#endif // GRAPHICSVIEW_H
