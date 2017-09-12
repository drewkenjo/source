#ifndef CANVAS2D_H
#define CANVAS2D_H

#include <QWidget>

class Render2D : public QWidget
{
    Q_OBJECT

public:
    Render2D(QWidget *parent = 0);
    QSize minimumSizeHint() const override {return QSize(800,600);};
    QSize sizeHint() const override {return QSize(800,600);};

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // CANVAS2D_H
