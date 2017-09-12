#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <G4UImanager.hh>

class Render2D;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(G4UImanager*);

private:
    Render2D *r2d;
};

#endif // WINDOW_H
