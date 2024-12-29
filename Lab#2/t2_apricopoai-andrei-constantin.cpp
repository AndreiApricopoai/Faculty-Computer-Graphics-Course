/**********************************

  The reader of this program should not limit
  himself/herself to the comments of this
  program.

  If one wants to read more about the syntax and
  the semantics of OpenGL functions used in this
  program, one should read the beginning of the
  paragraph 2.6 "Begin/End Paradigm", the
  paragraph 2.6.1 "Begin and End Objects" from the
  file glspec15.pdf at page 25/333 and the index
  from the end of that file. One could also
  read the references to the GLUT functions from
  the file glut-3.spec.pdf.



  H O W  T O  R E A D  T H I S  P R O G R A M ?

  Start from the function "main" and follow the
  instruction flow, paying attention to the fact that
  this program belongs to the event-driven programming
  paradigm. So pay attention to what happens when
  the user presses a key, moves the mouse or presses a
  mouse button. There are also some special events: the
  redrawing of the application window, etc.
  Identify what happens in the program when one of these
  events occurs.

  **********************************/


  /**********************************
	With respect to the include-file directives, see the
	example program from homework 1; in the following it
	was made the assumption that you are using GLUT locally,
	in your project and you didn't install it as an
	Administrator
	**********************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// the graph of the Conchoid of Nicomedes
void Display1() {
	double xmax, ymax, xmin, ymin;
	double a = 1, b = 2;
	double pi = 4 * atan(1.0);
	double ratia = 0.05;
	double t;

	/**********************************
	   Maximum/minimum values of x and y are computed.
	   These values will be further used in scaling the
	   original graph of the curve.
	   These values are stored in the variables
	   xmax, ymax, xmin, ymin: e.g., xmax is initialized
	   with a value smaller than any of possible values
	   of x; if in doubt or you cannot estimate it, use
	   DBL_MIN instead (or DBL_MAX for minimum values).
	   These DBL_* constants are found in <float.h> which
	   need to be included.
	   E.g., xmax is initialized with a - b - 1 because
	   x(t) = a +/- b * cos(t) and for t in (-pi/2, pi/2),
	   cos(t) is in (0, 1), so b * cos(t) is in (0, b),
	   so +/- b * cos(t) is in (-b, b), so x(t) is in
	   (a-b, a+b) and one can safely choose a-b-1 because
	   a-b-1 < a-b.
	   For y(t) we see that in its expression appears
	   tan(t) which varies in (-inf,+inf) when t
	   varies in (-pi/2, pi/2).
	  **********************************/
	xmax = a - b - 1;
	xmin = a + b + 1;
	ymax = ymin = 0;
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = a + b * cos(t);
		xmax = (xmax < x1) ? x1 : xmax;
		xmin = (xmin > x1) ? x1 : xmin;

		x2 = a - b * cos(t);
		xmax = (xmax < x2) ? x2 : xmax;
		xmin = (xmin > x2) ? x2 : xmin;

		y1 = a * tan(t) + b * sin(t);
		ymax = (ymax < y1) ? y1 : ymax;
		ymin = (ymin > y1) ? y1 : ymin;

		y2 = a * tan(t) - b * sin(t);
		ymax = (ymax < y2) ? y2 : ymax;
		ymin = (ymin > y2) ? y2 : ymin;
	}

	/**********************************
	   At this line, we have found that the graph of the Conchoid
	   is included in the rectangle having the edges x = xmin,
	   x = xmax, y = ymin and y = ymax.
	   We would like that the rectangle should be symmetric with
	   respect to the Ox and Oy axes.
	   We store in xmax and ymax the maximum of absolute values
	   max(|xmax|,|xmin|) and max(|ymax|,|ymin|).
	   Now we know that the graph of the Conchoid is included in
	   the rectangle [-xmax, xmax] x [-ymax, ymax].
	  **********************************/

	xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
	ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

	/**********************************
	   At this line, we can perform the scaling. All the points
	   scaled are visible (i.e., in the rectangle [-1,1]x[-1,1]).
	   Because we have -xmax <= x <= xmax we get
	   -1 <= x / xmax <= 1. Idem for y.
	   In order to correctly display the graph of the Conchoid,
	   one should use the exact same points that were used in
	   the computation of the scaling factor.
	  **********************************/

	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x2, y2);
	}
	glEnd();
}

// The graph of the function 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
	double pi = 4 * atan(1.0);
	double xmax = 8 * pi;
	double ymax = exp(1.1);
	double ratia = 0.05;


	/**********************************
	   For this example, the computation of the scaling factors
	   is not needed. Because x is in the interval [0, 8pi]
	   the scaling factor for x is 8pi because x/(8pi) is in
	   [0,1]. In the case of the exponential function we know
	   that if x is [0,8pi] then sin x is in [-1,1] so
	   e^(-sin x) is in [1/e, e] and thus it is safe to use
	   e^(1.1) as a scaling factor.
	  **********************************/
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double x = 0; x < xmax; x += ratia) {
		double x1, y1;
		x1 = x / xmax;
		y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();
}

/**********************************
 Display3: Draws the graph of a function that represents the distance to the nearest integer.

 The function iterates over the x-axis from 0 to 100 and computes the distance of each x
 to the nearest integer, scaled to fit within the predefined window boundaries. The resulting
 points are connected to form a line strip, displaying the wave-like nature of the function.
**********************************/
void Display3()
{
	double x, x1, y; // coordonatele punctului de pe graficul functiei
	double ratia = 0.05; // pasul de discretizare

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (x = 0; x <= 100; x += ratia)
	{ 
		// precizia masinii ca la calcul numeric :-?
		if (fabs(x - 0) <= 1 / 2) 
		{
			x1 = x; 
			y = 1; 
		}
		else {
			// x maxim e 100 , x1 maxim e 1  : impartim la xmax
			// y max : d(x) max e 0.5 , xmin - e 0.5 pe ramura asta ; y maxim 1
			x1 = x / 100; 
			y = (fabs(x - floorf(x)) / x); 
		}
		glVertex2f(x1 * 0.8, y * 0.9); // scalare pentru a incadra graficul in fereastra

	}
	glEnd();
}


/**********************************
 Display4: Renders the Trisectrix of Longchamps curve using a parametric equation.

 It draws the curve by calculating the x and y coordinates based on the parametric equations
 given, skipping undefined points. It fills the area between the curve and a boundary line,
 using GL_TRIANGLES to create a filled effect. The loop creates a line strip at the edges
 to define the curve's boundaries clearly.
**********************************/
void Display4() {
	double x, y, t; // coordonatele punctului de pe trisectrix
	double a = 0.20; // raza cercului
	double ratie = 0.020; // pasul de discretizare
	const double pi = 4 * atan(1.0); // valoarea lui pi

	for (t = (-pi) / 2 + ratie; t < pi / 2; t += ratie) { // jumatate de trisectrix
		if (t != (-pi) / 6 && t != pi / 6) { // evitam punctele de pe axa Ox
			x = a / (4 * pow(cos(t), 2) - 3); // formula trisectrix
			y = (a * tan(t)) / (4 * pow(cos(t), 2) - 3); // formula trisectrix


			double x2, y2, t2; // coordonatele punctului de pe trisectrix
			t2 = t + 0.005; // pasul de discretizare
			x2 = a / (4 * pow(cos(t2), 2) - 3); // formula trisectrix
			y2 = (a * tan(t2)) / (4 * pow(cos(t2), 2) - 3); // formula trisectrix

			// x max = 0.2 
			// y max 0.2
			if (x < 0.2 && y > 0.2 && x > -0.85 && y < 0.85) { // verificam daca punctul se afla in fereastra
				glColor3f(1, 0.1, 0.1); // rosu
				glBegin(GL_TRIANGLES); 
				glVertex2f(-0.95, 0.95); // originea
				glVertex2f(x, y); // punctul de pe trisectrix
				glVertex2f(x2, y2); // punctul de pe trisectrix
				glEnd();
			}

		}
	}

	glColor3f(0.2, 0.15, 0.88); // albastru

	glBegin(GL_LINE_LOOP);
	//originea
	glVertex2f(-0.95, 0.95);  

	t = -(pi) / 2;  // punctul de pe trisectrix
	x = a / (4 * pow(cos(t), 2) - 3);  // formula trisectrix
	glVertex2f(x, 0.95);

	for (t = (-pi) / 2 + ratie; t < pi / 2; t += ratie) { // cealalta jumatate de trisectrix
		if (t != (-pi) / 6 && t != pi / 6) { // evitam punctele de pe axa Ox
			x = a / (4 * pow(cos(t), 2) - 3); // formula trisectrix
			y = (a * tan(t)) / (4 * pow(cos(t), 2) - 3); // formula trisectrix

			if (x < 0 && y > 0 && x >= -1 && y <= 1)
			{ // verificam daca punctul se afla in fereastra
				glVertex2f(x, y);
			}


			double x2, y2, t2; // coordonatele punctului de pe trisectrix
			t2 = t + 0.005; // pasul de discretizare
			x2 = a / (4 * pow(cos(t2), 2) - 3); // formula trisectrix
			y2 = (a * tan(t2)) / (4 * pow(cos(t2), 2) - 3); // formula trisectrix

			if (x2 < 0 && y2 > 0 && x2 >= -1 && y2 <= 1) { // verificam daca punctul se afla in fereastra
				glVertex2f(x2, y2);
			}

		}
	}
	//glVertex2f(-0.95, y3);
	glEnd();
}

/**********************************
 Display5: Visualizes the cycloid curve, defined parametrically with parameters a and b.

 The function plots the cycloid by calculating the parametric equations over a range of t values.
 Each (x, y) point is scaled and then plotted as a line strip to create the cycloid curve on the screen.
**********************************/
void Display5() {
	double x, y, t; // coordonatele punctului de pe cicloida
	double ratie = 0.05; // pasul de discretizare
	double a = 0.1, b = 0.2; // raza cercului si distanta de la punctul de rulare la punctul de referinta

	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = -10; t <= 10; t += ratie) { // cicloida are o singura bucla
		// x max  = 0.1 * t - 0.2 = 0.8
		x = a * t - b * sin(t); // formula cicloidei
		y = a - b * cos(t); // formula cicloidei
		x = x * 0.8; // scalare pentru a incadra cicloida in fereastra
		glVertex2f(x, y); 
	}
	glEnd();
}


/**********************************
 Display6: Depicts the Lemniscate of Bernoulli, which is defined with a polar equation.

 The lemniscate is rendered by transforming polar coordinates to Cartesian coordinates
 over a range of angles. Points are connected using a line strip to illustrate the figure-eight shape.
**********************************/
void Display6() {
	double a = 0.4; // distanta de la origine la mijlocul figurii
	double ratia = 0.01; // pasul de discretizare
	double pi = 4 * atan(1.0); // valoarea lui pi
	double x, y, t; // coordonatele punctului de pe lemniscata

	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP); // trasarea cu linie
	for (t = -pi / 4; t < pi / 4; t += ratia) { // jumatate de lemniscata
		x = a * sqrt(2 * cos(2 * t)) * cos(t); // formula lemniscatei
		y = a * sqrt(2 * cos(2 * t)) * sin(t); // formula lemniscatei

		glVertex2f(x, y); 
	}

	for (t = -pi / 4 - ratia; t < pi / 4; t += ratia) { // cealalta jumatate de lemniscata

		x = -a * sqrt(2 * cos(2 * t)) * cos(t); // formula lemniscatei
		y = -a * sqrt(2 * cos(2 * t)) * sin(t); // formula lemniscatei

		glVertex2f(x, y);
	}
	glEnd();
}

/**********************************
 Display7: Illustrates the logarithmic spiral, also defined using polar coordinates.

 The function generates the spiral by plotting points in polar coordinates converted to Cartesian.
 The points are scaled and plotted as a line strip, visualizing the exponential growth of the spiral.
**********************************/
void Display7() {
	double x, y, t; // coordonatele punctului de pe spirala
	double a = 0.02; // rata de crestere a spiralei
	double ratie = 0.05; // pasul de discretizare
	const double pi = 4 * atan(1.0); // valoarea lui pi

	double xmax = -1000; // initializare cu o valoare mai mica decat orice valoare posibila
	double ymax = -1000; // initializare cu o valoare mai mica decat orice valoare posibila


	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = 0; t <= pi; t += ratie) { // o singura spira
		x = a * exp(1 + t) * cos(t);  // formula spiralei
		y = a * exp(1 + t) * sin(t); // formula spiralei
		glVertex2f(x * 0.75, y); // scalare pentru a incadra spirala in fereastra
	}
	glEnd();
}


void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	//   glPointSize(4);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '1':
		Display1();
		break;
	case '2':
		Display2();
		break;
	case '3':
		Display3();
		break;
	case '4':
		Display4();
		break;
	case '5':
		Display5();
		break;
	case '6':
		Display6();
		break;
	case '7':
		Display7();
		break;
	default:
		break;
	}

	glFlush();
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}