#include "render2d.h"
#include <cstdlib>
#include <QPainter>
#include <iostream>

Render2D::Render2D(G4UImanager* _uiman, GRayGun* _gPen, GRaySteppingAction* _stpAction)
    : QWidget(0), uiman(_uiman), gPen(_gPen), stpAction(_stpAction)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	wrange = 3000;
	w0 = 0;
	hrange = 600;
	h0 = -300;

	scan();
}

void Render2D::scan()
{
	int ww = width();
	int hh = height();

	for(int ih=0;ih<hh*10;ih++){
		G4ThreeVector vv(hrange*ih/10/hh+h0,0,0);
		gPen->vertices.push_back(vv);
		G4ThreeVector vd(0,0,1);
		gPen->dirmoms.push_back(vd);
	}

	for(int iw=0;iw<ww*10;iw++){
		G4ThreeVector vv(h0,0,wrange*iw/10/ww);
		gPen->vertices.push_back(vv);
		G4ThreeVector vd(1,0,0);
		gPen->dirmoms.push_back(vd);
	}

	int nlen = gPen->vertices.size();

	char cmd[100];
	sprintf(cmd, "/run/beamOn %d", nlen);
	uiman->ApplyCommand(cmd);
}

void Render2D::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

	for(int iv=0;iv<stpAction->points.size();iv++){
		int iw = stpAction->points[iv].z()/wrange*width();
		int ih = (stpAction->points[iv].x()-h0)/hrange*height();
		painter.drawPoint(iw, ih);
	}
}
