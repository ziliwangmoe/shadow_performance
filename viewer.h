#ifndef EX03V_H
#define EX03V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "opengl.h"

class viewer : public QWidget
{
	Q_OBJECT
private:
	QSlider* Xpos;
	QSlider* Ypos;
	QSlider*        Zpos;
	QPushButton*    light;
	QCheckBox*      antiAlias;
	opengl*     ogl;
	private slots:
	void reset();        //  Reset angles
	void lmove();        //  Pause/animate light
	void izoom(int iz);  //  Zoom level (percent)
public:
	viewer();
};

#endif
