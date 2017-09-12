#include "render2d.h"
#include "window.h"

#include <QtWidgets>

const int IdRole = Qt::UserRole;

Window::Window(G4UImanager *uiman, GRayGun *gGun, GRaySteppingAction *stpAction)
{
	r2d = new Render2D(uiman, gGun, stpAction);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(3, 1);
	mainLayout->addWidget(r2d, 0, 0, 1, 4);
	setLayout(mainLayout);

	setWindowTitle(tr("GEMC 2D Drawing"));
}

