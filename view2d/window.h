#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <G4UImanager.hh>
#include "GRayGun.h"
#include "GRaySteppingAction.h"

class Render2D;

class Window : public QWidget
{
    Q_OBJECT

public:
	Window(G4UImanager*, GRayGun*, GRaySteppingAction*);

private:
	Render2D *r2d;
};

#endif // WINDOW_H
