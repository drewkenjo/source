#include "render2d.h"
#include <cstdlib>
#include <QPainter>
#include <iostream>

Render2D::Render2D(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void Render2D::paintEvent(QPaintEvent*)
{
    QRect rect(10, 20, 80, 60);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

	for(int i=0;i<1000000;i++){
		int xx = rand()%width();
		int yy = rand()%height();
		painter.drawPoint(xx, yy);
	}
//    painter.drawLine(0,0, 800,1600);
/*
    for (int x = 0; x < width(); x += 200) {
        for (int y = 0; y < height(); y += 200) {
            painter.save();
            painter.translate(x, y);

            painter.drawLine(rect.bottomLeft(), rect.topRight());
            painter.restore();
        }
    }
*/
}
