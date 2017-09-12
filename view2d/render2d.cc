#include "render2d.h"
#include <cstdlib>
#include <QPainter>
#include <QMouseEvent>
#include <iostream>
#include "window.h"

Render2D::Render2D(Window* parent, G4UImanager* _uiman, GRaySteppingAction* _stpAction, GRayGun* _gPen):QWidget(parent)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
	stpAction = _stpAction;
	gPen = _gPen;
	uiman = _uiman;

	pressed = false;

	mainwin = parent;
}

void Render2D::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	double hh = height();
	double ww = width();

	for(int iv=0;iv<stpAction->points.size();iv++){
		int iw = (stpAction->points[iv].z()-w0)/(w1-w0)*ww;
		int ih = hh-(stpAction->points[iv].x()-h0)/(h1-h0)*hh;
		painter.drawPoint(iw, ih);
	}
}


void Render2D::setFrame(double _w0, double _w1, double _h0, double _h1)
{
	if(_w0!=w0 || _h0!=h0 || _w1!=w1 || _h1!=h1){
		std::cout<<w0<<" "<<w1<<" "<<h0<<" "<<h1<<std::endl;

		w0=_w0;
		w1=_w1;
		h0=_h0;
		h1=_h1;

		mainwin->setFrame(w0, w1, h0, h1);

//		scan();
		update();

		std::cout<<"finished"<<std::endl;
	}
}


void Render2D::scan()
{
	double ww = w1-w0;
	double hh = h1-h0;
	double nsteps = 5000.0;

	for(int ih=0;ih<nsteps;ih++){
		G4ThreeVector vv(hh*ih/nsteps + h0, 0, 0);
		gPen->vertices.push_back(vv);
		G4ThreeVector vd(0,0,1);
		gPen->dirmoms.push_back(vd);
	}

	for(int iw=0;iw<nsteps;iw++){
		G4ThreeVector vv(h0, 0, ww*iw/nsteps + w0);
		gPen->vertices.push_back(vv);
		G4ThreeVector vd(1,0,0);
		gPen->dirmoms.push_back(vd);
	}

	int nlen = gPen->vertices.size();

	char cmd[100];
	sprintf(cmd, "/run/beamOn %d", nlen);
	uiman->ApplyCommand(cmd);
}


void Render2D::mousePressEvent(QMouseEvent *event)
{
	x0 = event->x();
	y0 = event->y();
	pressed = true;
}

void Render2D::mouseMoveEvent(QMouseEvent *event)
{
	x1 = event->x();
	y1 = event->y();
}

void Render2D::mouseReleaseEvent(QMouseEvent *event)
{
	x1 = event->x();
	y1 = event->y();

	double neww0 = x0/width()*(w1-w0) + w0;
	double neww1 = x1/width()*(w1-w0) + w0;
	double newh0 = (height()-y0)/height()*(h1-h0) + h0;
	double newh1 = (height()-y1)/height()*(h1-h0) + h0;

	setFrame(std::min(neww0, neww1), std::max(neww0, neww1),
		std::min(newh0, newh1), std::max(newh0, newh1));

	pressed = false;
}
