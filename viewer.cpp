//
//  viewer Widget
//

#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "viewer.h"

//
//  Constructor
//
viewer::viewer()
{
	//  Set window title
	setWindowTitle(tr("Assignment 3: Zili Wang"));

	//  Create new Triangle widget
	ogl = new opengl;

	//  Select shader
	QComboBox* shader = new QComboBox();
	shader->addItem("Shadow");
	shader->addItem("Depth");
	shader->addItem("Performance");

	//  Select projection
	QComboBox* projection = new QComboBox();
	projection->addItem("Float");
	projection->addItem("Int");

	//  Select object
	QComboBox* object = new QComboBox();
	object->addItem("Func");
	object->addItem("Inline");

	//  Center position
	Xpos = new QSlider(Qt::Horizontal);
	Ypos = new QSlider(Qt::Horizontal);
	Zpos = new QSlider(Qt::Horizontal);
	Xpos->setRange(0, 100); Xpos->setValue(50);
	Ypos->setRange(0, 100); Ypos->setValue(50);
	Zpos->setRange(1, 100); Zpos->setValue(10);

	//  View angle and zoom
	QLabel* angles = new QLabel();
	QLabel* fps_Lable = new QLabel();
	QLabel* info1 = new QLabel();
	QLabel* info2 = new QLabel();

	//  Pause/resume button
	light = new QPushButton("Pause");

	antiAlias = new QCheckBox("Use Antialias");
	antiAlias->setChecked(true);


	//  Reset
	QPushButton* rst = new QPushButton("Reset");
	//  Quit
	QPushButton* quit = new QPushButton("Quit");

	//  Set layout of child widgets
	QGridLayout* layout = new QGridLayout;
	layout->addWidget(ogl, 0, 0, 13, 1);
	layout->addWidget(new QLabel("Shader"), 0, 1);
	layout->addWidget(shader, 0, 2);
	layout->addWidget(new QLabel("Int Test"), 1, 1);
	layout->addWidget(projection, 1, 2);
	layout->addWidget(new QLabel("Func Test"), 2, 1);
	layout->addWidget(object, 2, 2);
	layout->addWidget(new QLabel("X Area"), 3, 1);
	layout->addWidget(Xpos, 3, 2);
	layout->addWidget(new QLabel("Y Area"), 4, 1);
	layout->addWidget(Ypos, 4, 2);
	layout->addWidget(new QLabel("Loops"), 5, 1);
	layout->addWidget(Zpos, 5, 2);
	layout->addWidget(new QLabel("Angles"), 6, 1);
	layout->addWidget(angles, 6, 2);
	layout->addWidget(new QLabel("FPS"), 7, 1);
	layout->addWidget(fps_Lable, 7, 2);
	layout->addWidget(new QLabel("Info1"), 8, 1);
	layout->addWidget(fps_Lable, 8, 2);
	layout->addWidget(new QLabel("Info2"), 9, 1);
	layout->addWidget(fps_Lable, 9, 2);
	layout->addWidget(new QLabel("Light"), 10, 1);
	layout->addWidget(light, 10, 2);
	//layout->addWidget(new QLabel("Light"), 9, 1);
	layout->addWidget(antiAlias, 11, 2);
	layout->addWidget(rst, 13, 1);
	layout->addWidget(quit, 13, 2);
	//  Manage resizing
	layout->setColumnStretch(0, 100);
	layout->setColumnMinimumWidth(0, 100);
	layout->setRowStretch(12, 100);
	setLayout(layout);

	connect(shader, SIGNAL(currentIndexChanged(int)), ogl, SLOT(setShader(int)));
	connect(object, SIGNAL(currentIndexChanged(int)), ogl, SLOT(setObject(int)));
	connect(projection, SIGNAL(currentIndexChanged(int)), ogl, SLOT(setPerspective(int)));
	connect(Xpos, SIGNAL(valueChanged(int)), ogl, SLOT(setX(int)));
	connect(Ypos, SIGNAL(valueChanged(int)), ogl, SLOT(setY(int)));
	connect(Zpos, SIGNAL(valueChanged(int)), ogl, SLOT(setZoom(int)));
	connect(ogl, SIGNAL(angles(QString)), angles, SLOT(setText(QString)));
	connect(ogl, SIGNAL(fps(QString)), fps_Lable, SLOT(setText(QString)));
	connect(ogl, SIGNAL(info1(QString)), fps_Lable, SLOT(setText(QString)));
	connect(ogl, SIGNAL(info2(QString)), fps_Lable, SLOT(setText(QString)));
	connect(rst, SIGNAL(pressed()), this, SLOT(reset()));
	connect(light, SIGNAL(pressed()), this, SLOT(lmove()));
	connect(quit, SIGNAL(pressed()), qApp, SLOT(quit()));

	connect(antiAlias, SIGNAL(stateChanged(int)), ogl, SLOT(setAntiAlias(int)));
}

//
//  Reset view
//
void viewer::reset()
{
	Xpos->setValue(0);
	Ypos->setValue(0);
	Zpos->setValue(0);
	ogl->reset();
}

//
//  Light pause/move
//
void viewer::izoom(int iz)
{
	//  Map coordinates
	float z = powf(2.0, 0.2*iz);
	ogl->setZoom(z);
	//  Set Increments
	int dec = iz / 16 + 1;
	float step = powf(10.0, -dec);
	Xpos->setSingleStep(step);
	Ypos->setSingleStep(step);
}

//
//  Light pause/move
//
void viewer::lmove()
{
	bool pause = (light->text() == "Pause");
	if (pause)
		light->setText("Animate");
	else
		light->setText("Pause");
	ogl->setLightMove(!pause);
}
