#ifndef CANVAS2D_H
#define CANVAS2D_H

#include <QWidget>
#include <G4UImanager.hh>
#include "GRaySteppingAction.h"
#include "GRayGun.h"
#include "window.h"

class Render2D : public QWidget
{
    Q_OBJECT

public:
	Render2D(Window*, G4UImanager*, GRaySteppingAction*, GRayGun*);
	QSize minimumSizeHint() const override {return QSize(800,600);};
	QSize sizeHint() const override {return QSize(800,600);};

	void setFrame(double, double, double, double);
	void scan();

protected:
	void paintEvent(QPaintEvent *event) override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	double w1, h1, w0, h0;
	double x0, y0, x1, y1;
	bool pressed;
	Window* mainwin;

	G4UImanager *uiman;
	GRayGun *gPen;
	GRaySteppingAction *stpAction;
};

#endif // CANVAS2D_H
