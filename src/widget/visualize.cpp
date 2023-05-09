#include "visualize.h"

#include <QtOpenGL>

#if defined(Q_OS_LINUX)
  #include <termios.h>
  #include <unistd.h>
  #include <GL/gl.h>  // sudo apt install mesa-common-dev
  #include <GL/glu.h> // sudo apt install libglu1-mesa-dev freeglut3-dev
#elif defined(Q_OS_WIN)
  #include <windows.h>
  #include <GL/gl.h>
  #include <GL/glu.h>
  #define random() rand()
#elif defined(Q_OS_MACOS)
  #include <termios.h>
  #include <unistd.h>
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#endif

static const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
static const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
static const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

static const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
static const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
static const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
static const GLfloat high_shininess[] = { 100.0f };

static GLuint spherelist;
static GLuint spherelowreslist;

VisualizeWidget::VisualizeWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
	
}

VisualizeWidget::~VisualizeWidget()
{

}

void VisualizeWidget::initializeGL()
{
	initializeOpenGLFunctions();

	GLUquadric *sphere;

	glClearColor(0.5, 0.5, 0.5, 1.0); // glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHT0);
	//glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	spherelist = glGenLists(1);
	glNewList(spherelist, GL_COMPILE);
	gluSphere(sphere, 0.08, 16, 14);
	glEndList();
	spherelowreslist = glGenLists(1);
	glNewList(spherelowreslist, GL_COMPILE);
	gluSphere(sphere, 0.08, 12, 10);
	glEndList();
	gluDeleteQuadric(sphere);
}

void VisualizeWidget::resizeGL(int width, int height)
{
	const float ar = (float) width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity() ;
}

void VisualizeWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 0, 0);	// set current color to red
	glLoadIdentity();

	Matrix3x3d rMatrix = m_orientation.toRotation();

	// for (point : points) // uhh ohh, we need the points here as well
}

void VisualizeWidget::mousePressEvent(QMouseEvent *event)
{

}

void VisualizeWidget::mouseMoveEvent(QMouseEvent *event)
{
	
}

