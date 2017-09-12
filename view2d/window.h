#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <G4UImanager.hh>
#include "GRayGun.h"
#include "GRaySteppingAction.h"

class Render2D;

class Window : public QWidget
{
    Q_OBJECT

public:
	Window(G4UImanager*, GRayGun*, GRaySteppingAction*);
	void setFrame(double ,double ,double, double);

private:
	Render2D *r2d;
	QLineEdit* wminEdit;
	QLineEdit* wmaxEdit;
	QLineEdit* hminEdit;
	QLineEdit* hmaxEdit;
	QPushButton* unzoomButton;

private slots:
	void updateFrame();
	void resetFrame();
};

#endif // WINDOW_H
