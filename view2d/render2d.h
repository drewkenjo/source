#ifndef CANVAS2D_H
#define CANVAS2D_H

#include <QWidget>
#include <G4UImanager.hh>
#include "GRaySteppingAction.h"
#include "GRayGun.h"

class Render2D : public QWidget
{
    Q_OBJECT

public:
	Render2D(G4UImanager*, GRayGun*, GRaySteppingAction*);
	QSize minimumSizeHint() const override {return QSize(800,600);};
	QSize sizeHint() const override {return QSize(800,600);};

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void scan();
	void rescale();

	double wrange, hrange, w0, h0;

	G4UImanager *uiman;
	GRayGun* gPen;
	GRaySteppingAction *stpAction;
};

#endif // CANVAS2D_H
