#include "render2d.h"
#include "window.h"

#include <QtWidgets>

const int IdRole = Qt::UserRole;

Window::Window(G4UImanager *uiman, GRayGun *gGun, GRaySteppingAction *stpAction)
{
	setWindowTitle(tr("GEMC 2D Drawing"));

	wminEdit = new QLineEdit;
	wminEdit->setText("-1000");
	wmaxEdit = new QLineEdit;
	wmaxEdit->setText("10000");

	hminEdit = new QLineEdit;
	hminEdit->setText("-5000");
	hmaxEdit = new QLineEdit;
	hmaxEdit->setText("5000");

	unzoomButton = new QPushButton;
	unzoomButton->setText("unzoom frame");

	connect(wminEdit, SIGNAL(editingFinished()), this, SLOT(updateFrame()));
	connect(wmaxEdit, SIGNAL(editingFinished()), this, SLOT(updateFrame()));
	connect(hminEdit, SIGNAL(editingFinished()), this, SLOT(updateFrame()));
	connect(hmaxEdit, SIGNAL(editingFinished()), this, SLOT(updateFrame()));
	connect(unzoomButton, SIGNAL(clicked()), this, SLOT(resetFrame()));

	r2d = new Render2D(this, uiman, stpAction, gGun);
	updateFrame();

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setRowStretch(3, 1);
	mainLayout->addWidget(r2d, 0, 0, 4, 4);
	mainLayout->addWidget(unzoomButton,0,4);
	mainLayout->addWidget(wminEdit,1,4,Qt::AlignRight);
	mainLayout->addWidget(wmaxEdit,1,5,Qt::AlignRight);
	mainLayout->addWidget(hminEdit,2,4,Qt::AlignRight);
	mainLayout->addWidget(hmaxEdit,2,5,Qt::AlignRight);
	setLayout(mainLayout);

}



void Window::updateFrame()
{
	double w0 = wminEdit->text().toDouble();
	double w1 = wmaxEdit->text().toDouble();
	double h0 = hminEdit->text().toDouble();
	double h1 = hmaxEdit->text().toDouble();

	r2d->setFrame(w0, w1, h0, h1);
}


void Window::setFrame(double w0, double w1, double h0, double h1)
{
	wminEdit->setText(QString::number(w0));
	wmaxEdit->setText(QString::number(w1));
	hminEdit->setText(QString::number(h0));
	hmaxEdit->setText(QString::number(h1));
}

void Window::resetFrame()
{
	setFrame(-1000,10000,-5000,5000);
	updateFrame();
}
