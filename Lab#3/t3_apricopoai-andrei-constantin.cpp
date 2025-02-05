#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include "glut.h"

#define WINDOW_SIZE_PX 300
// the maximum number of iterations for the Julia-Fatou set membership testing 
#define NRITER_JF 5000
// the maximum value of M for the Julia-Fatou set membership testing 
#define MODMAX_JF 10000000
// increments used in CJuliaFatou
#define RX_JF 0.01
#define RY_JF 0.01

unsigned char prevKey;
int nivel_display3 = 0;
int nivel_display4 = 0;
int nivel_display5 = 0;
int nivel_display6 = 0;
int nivel_display8 = 0;
int nivel_display9 = 0;
int nivel_display10 = 0;

void Display();
void Display1();
void Display2();
void Display3();
void Display4();
void Display5();
void Display6();
void Display7();
void Display8();
void Display9();
void Display10();
void clear_levels();
void Reshape(int w, int h);
void Init(void);
void KeyboardFunc(unsigned char key, int x, int y);
void MouseFunc(int button, int state, int x, int y);
void display_characters(char c[]);


class CComplex {
public:
	CComplex() : re(0.0), im(0.0) {}
	CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
	CComplex(const CComplex& c) : re(c.re), im(c.im) {}
	~CComplex() {}

	CComplex& operator=(const CComplex& c)
	{
		re = c.re;
		im = c.im;
		return *this;
	}

	double getRe() { return re; }
	void setRe(double re1) { re = re1; }

	double getIm() { return im; }
	void setIm(double im1) { im = im1; }

	double getModul() { return sqrt(re * re + im * im); }

	int operator==(CComplex& c1)
	{
		return ((re == c1.re) && (im == c1.im));
	}

	CComplex pow2()
	{
		CComplex rez;
		rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
		rez.im = 2.0 * re * im;
		return rez;
	}

	friend CComplex operator+(const CComplex& c1, const CComplex& c2);
	friend CComplex operator*(CComplex& c1, CComplex& c2);

	void print(FILE* f)
	{
		fprintf(f, "%.20f%+.20f i", re, im);
	}

private:
	double re, im;
};

CComplex operator+(const CComplex& c1, const CComplex& c2)
{
	CComplex rez(c1.re + c2.re, c1.im + c2.im);
	return rez;
}

CComplex operator*(CComplex& c1, CComplex& c2)
{
	CComplex rez(c1.re * c2.re - c1.im * c2.im,
		c1.re * c2.im + c1.im * c2.re);
	return rez;
}

class CJuliaFatou {
public:
	CJuliaFatou()
	{
		// m.c is initialized by default with 0+0i
		m.nriter = NRITER_JF;
		m.modmax = MODMAX_JF;
	}

	CJuliaFatou(CComplex& c)
	{
		m.c = c;
		m.nriter = NRITER_JF;
		m.modmax = MODMAX_JF;
	}

	~CJuliaFatou() {}

	void setmodmax(double v) { assert(v <= MODMAX_JF); m.modmax = v; }
	double getmodmax() { return m.modmax; }

	void setnriter(int v) { assert(v <= NRITER_JF); m.nriter = v; }
	int getnriter() { return m.nriter; }

	// it tests if x belongs to the Julia-Fatou set Jc
	// it returns 0 if yes, -1 for finite convergence, +1 for infinite convergence
	int isIn(CComplex& x)
	{
		int rez = 0;
		// an array for storing the values for computing z_n+1 = z_n * z_n + c
		CComplex z0, z1;

		z0 = x;
		for (int i = 1; i < m.nriter; i++)
		{
			z1 = z0 * z0 + m.c;
			if (z1 == z0)
			{
				// x does not belong to the J-F set because the 
				// iterative process converges finitely
				rez = -1;
				break;
			}
			else if (z1.getModul() > m.modmax)
			{
				// x does not belong to the J-F set because the 
				// iterative process converges infinitely
				rez = 1;
				break;
			}
			z0 = z1;
		}

		return rez;
	}

	// it displays a J-F set
	void display(double xmin, double ymin, double xmax, double ymax)
	{
		glPushMatrix();
		glLoadIdentity();

		glBegin(GL_POINTS);
		for (double x = xmin; x <= xmax; x += RX_JF)
			for (double y = ymin; y <= ymax; y += RY_JF)
			{
				CComplex z(x, y);
				int r = isIn(z);
				if (r == 0)
				{
					glVertex3d(x, y, 0);
				}
				else if (r == -1)
				{
				}
				else if (r == 1)
				{
				}
			}
		fprintf(stdout, "STOP\n");
		glEnd();

		glPopMatrix();
	}

private:
	struct SDate {
		CComplex c;
		// number of iterations
		int nriter;
		// the maximum value of M
		double modmax;
	} m;
};


class C2coord
{
public:
	C2coord()
	{
		m.x = m.y = 0;
	}

	C2coord(double x, double y)
	{
		m.x = x;
		m.y = y;
	}

	C2coord(const C2coord& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
	}

	C2coord& operator=(C2coord& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	int operator==(C2coord& p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

protected:
	struct SDate
	{
		double x, y;
	} m;
};

class CPunct : public C2coord
{
public:
	CPunct() : C2coord(0.0, 0.0)
	{}

	CPunct(double x, double y) : C2coord(x, y)
	{}

	CPunct& operator=(const CPunct& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	void getxy(double& x, double& y)
	{
		x = m.x;
		y = m.y;
	}

	int operator==(CPunct& p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

	void marcheaza()
	{
		glBegin(GL_POINTS);
		glVertex2d(m.x, m.y);
		glEnd();
	}

	void print(FILE* fis)
	{
		fprintf(fis, "(%+f,%+f)", m.x, m.y);
	}

};

class CVector : public C2coord
{
public:
	CVector() : C2coord(0.0, 0.0)
	{
		normalizare();
	}

	CVector(double x, double y) : C2coord(x, y)
	{
		normalizare();
	}

	CVector& operator=(CVector& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	int operator==(CVector& p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

	CPunct getDest(CPunct& orig, double lungime)
	{
		double x, y;
		orig.getxy(x, y);
		CPunct p(x + m.x * lungime, y + m.y * lungime);
		return p;
	}

	void rotatie(double grade)
	{
		double x = m.x;
		double y = m.y;
		double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
		m.x = x * cos(t) - y * sin(t);
		m.y = x * sin(t) + y * cos(t);
		normalizare();
	}

	void draw_graphic(CPunct p, double lungime)
	{
		double x, y;
		p.getxy(x, y);
		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x + m.x * lungime, y + m.y * lungime);
		glEnd();
	}

	void draw_graphicPatrat(CPunct p, double lungime) {
		double x, y;
		p.getxy(x, y);
		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINE_LOOP);
		glVertex2d(x - lungime / 2, y - lungime / 2);
		glVertex2d(x - lungime / 2, y + lungime / 2);
		glVertex2d(x + lungime / 2, y + lungime / 2);
		glVertex2d(x + lungime / 2, y - lungime / 2);

		glEnd();
	}

	void print(FILE* fis)
	{
		fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
	}

private:
	// it is used for normalizing a vector
	void normalizare()
	{
		double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
		if (d != 0.0)
		{
			C2coord::m.x = C2coord::m.x * 1.0 / d;
			C2coord::m.y = C2coord::m.y * 1.0 / d;
		}
	}
};

class TriangleFractal {
public:
	void generateFractal(CPunct point, CVector direction, double size, int depth, int angleShift) {
		int rotationFactor = depth % 2 == 0 ? -1 : 1;

		if (depth == 0) {
			plotDetailedLines(point, direction, size, angleShift);
			return;
		}

		double halfSize = size / 2;

		CPunct newPoint = direction.getDest(point, halfSize);
		generateFractal(newPoint, direction, halfSize, depth - 1, angleShift);

		direction.rotatie(120);
		newPoint = direction.getDest(point, halfSize);
		generateFractal(newPoint, direction, halfSize, depth - 1, angleShift + 120 * rotationFactor);

		direction.rotatie(120);
		newPoint = direction.getDest(point, halfSize);
		generateFractal(newPoint, direction, halfSize, depth - 1, angleShift - 120 * rotationFactor);
	}

	void constructTriangle(CPunct point, CVector direction, double size) {
		double x, y;

		CPunct firstPoint = direction.getDest(point, size);
		direction.rotatie(120);
		CPunct secondPoint = direction.getDest(point, size);
		direction.rotatie(120);
		CPunct thirdPoint = direction.getDest(point, size);

		glColor3f(0.1, 1.0, 0.1);
		glBegin(GL_LINE_LOOP);

		firstPoint.getxy(x, y);
		glVertex2d(x, y);
		secondPoint.getxy(x, y);
		glVertex2d(x, y);
		thirdPoint.getxy(x, y);
		glVertex2d(x, y);
		glEnd();
	}

	void plotDetailedLines(CPunct point, CVector direction, double size, int angleShift) {
		double x, y;
		direction.rotatie(angleShift);

		direction.rotatie(-60);
		CPunct firstPoint = direction.getDest(point, size / 2);

		direction.rotatie(120);
		CPunct secondPoint = direction.getDest(point, size / 2);

		direction.rotatie(60);
		CPunct thirdPoint = direction.getDest(point, size);

		direction.rotatie(120);
		CPunct fourthPoint = direction.getDest(point, size);

		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINE_STRIP);
		fourthPoint.getxy(x, y);
		glVertex2d(x, y);
		firstPoint.getxy(x, y);
		glVertex2d(x, y);
		secondPoint.getxy(x, y);
		glVertex2d(x, y);
		thirdPoint.getxy(x, y);
		glVertex2d(x, y);
		glEnd();
	}

	void renderGraphic(double size, int depth) {
		glPushMatrix();
		glRotated(-90.0, 0.0, 0.0, 1.0);
		CVector direction(0.0, 1.0);
		CPunct point(0.0, 0);
		generateFractal(point, direction, size, depth, 0);
		glPopMatrix();
	}
};

class CCurbaKoch
{
public:
	void segmentKoch(double lungime, int nivel, CPunct& p, CVector v)
	{
		CPunct p1;
		if (nivel == 0)
		{
			v.draw_graphic(p, lungime);
		}
		else
		{
			segmentKoch(lungime / 3.0, nivel - 1, p, v);
			p1 = v.getDest(p, lungime / 3.0);
			v.rotatie(60);
			segmentKoch(lungime / 3.0, nivel - 1, p1, v);
			p1 = v.getDest(p1, lungime / 3.0);
			v.rotatie(-120);
			segmentKoch(lungime / 3.0, nivel - 1, p1, v);
			p1 = v.getDest(p1, lungime / 3.0);
			v.rotatie(60);
			segmentKoch(lungime / 3.0, nivel - 1, p1, v);
		}
	}

	void print_graphic(double lungime, int nivel)
	{
		CVector v1(sqrt(3.0) / 2.0, 0.5);
		CPunct p1(-1.0, 0.0);

		CVector v2(0.0, -1.0);
		CPunct p2(0.5, sqrt(3.0) / 2.0);

		CVector v3(-sqrt(3.0) / 2.0, 0.5);
		CPunct p3(0.5, -sqrt(3.0) / 2.0);

		segmentKoch(lungime, nivel, p1, v1);
		segmentKoch(lungime, nivel, p2, v2);
		segmentKoch(lungime, nivel, p3, v3);
	}
};

class CArboreBinar
{
public:
	void arboreBinar(double lungime, int nivel, CPunct& p, CVector v)
	{
		CPunct p1;
		if (nivel == 0)
		{
			v.draw_graphic(p, lungime);
		}
		else
		{
			arboreBinar(lungime, nivel - 1, p, v);
			p1 = v.getDest(p, lungime);

			v.rotatie(-45);
			arboreBinar(lungime / 2.0, nivel - 1, p1, v);

			v.rotatie(90);
			arboreBinar(lungime / 2.0, nivel - 1, p1, v);
		}
	}

	void print_graphic(double lungime, int nivel)
	{
		CVector v(0.0, -1.0);
		CPunct p(0.0, 1.0);

		arboreBinar(lungime, nivel, p, v);
	}
};

class CArborePerron
{
public:
	void arborePerron(double lungime,
		int nivel,
		double factordiviziune,
		CPunct p,
		CVector v)
	{
		assert(factordiviziune != 0);
		CPunct p1, p2;
		if (nivel == 0)
		{
		}
		else
		{
			v.rotatie(30);
			v.draw_graphic(p, lungime);
			p1 = v.getDest(p, lungime);
			arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

			v.rotatie(-90);
			v.draw_graphic(p, lungime);
			p1 = v.getDest(p, lungime);
			p2 = p1;

			v.rotatie(-30);
			v.draw_graphic(p1, lungime);
			p1 = v.getDest(p1, lungime);
			arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

			p1 = p2;
			v.rotatie(90);
			v.draw_graphic(p1, lungime);
			p1 = v.getDest(p1, lungime);
			p2 = p1;

			v.rotatie(30);
			v.draw_graphic(p1, lungime);
			p1 = v.getDest(p1, lungime);
			arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

			p1 = p2;
			v.rotatie(-90);
			v.draw_graphic(p1, lungime);
			p1 = v.getDest(p1, lungime);
			arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
		}
	}

	void print_graphic(double lungime, int nivel)
	{
		CVector v(0.0, 1.0);
		CPunct p(0.0, -1.0);

		v.draw_graphic(p, 0.25);
		p = v.getDest(p, 0.25);
		arborePerron(lungime, nivel, 0.4, p, v);
	}
};

class PatternDesign {
public:
	void renderDesign(double size, int depth) {
		CVector direction(0.0, 1.0);
		CPunct origin(0.0, 0.0);
		direction.draw_graphicPatrat(origin, 1.8);
		generatePattern(0.6, depth, origin, direction);
	}

	void generatePattern(double size, int depth, CPunct point, CVector vector) {
		CPunct newPoint;
		if (depth == 0) {
			vector.draw_graphicPatrat(point, size);
		}
		else {
			generatePattern(size, depth - 1, point, vector);
			for (int i = 0; i < 4; ++i) {
				vector.rotatie(i * 90);
				newPoint = vector.getDest(point, size);
				vector.rotatie(-i * 90);
				generatePattern(size / 3, depth - 1, newPoint, vector);
			}
			for (int i = 0; i < 4; ++i) {
				vector.rotatie(45 + i * 90);
				newPoint = vector.getDest(point, size * sqrt(2));
				vector.rotatie(-45 - i * 90);
				generatePattern(size / 3, depth - 1, newPoint, vector);
			}
		}
	}
};

class CCurbaHilbert
{
public:
	void curbaHilbert(double lungime, int nivel, CPunct& p, CVector& v, int d)
	{
		if (nivel == 0)
		{
		}
		else
		{
			v.rotatie(d * 90);
			curbaHilbert(lungime, nivel - 1, p, v, -d);

			v.draw_graphic(p, lungime);
			p = v.getDest(p, lungime);

			v.rotatie(-d * 90);
			curbaHilbert(lungime, nivel - 1, p, v, d);

			v.draw_graphic(p, lungime);
			p = v.getDest(p, lungime);

			curbaHilbert(lungime, nivel - 1, p, v, d);

			v.rotatie(-d * 90);
			v.draw_graphic(p, lungime);
			p = v.getDest(p, lungime);

			curbaHilbert(lungime, nivel - 1, p, v, -d);

			v.rotatie(d * 90);
		}
	}

	void print_graphic(double lungime, int nivel)
	{
		CVector v(0.0, 1.0);
		CPunct p(0.0, 0.0);

		curbaHilbert(lungime, nivel, p, v, 1);
	}
};

class FractalRenderer {

private:
	struct FractalData {
		CComplex complex;
		int iterCount;
		double maxMagnitude;
	} fractalParams;

public:
	void renderFractal(double left, double bottom, double right, double top) {
		glPushMatrix();
		glLoadIdentity();

		glTranslated((left + right) * 2.0 / (left - right), (bottom + top) * 2.0 / (bottom - top), 0);
		glScaled(2.0 / (right - left), 2.0 / (top - bottom), 1);

		glBegin(GL_POINTS);
		for (double x = left; x <= right; x += RX_JF)
			for (double y = bottom; y <= top; y += RY_JF) {
				CComplex z(x, y);
				int iterationResult = calculatePointMembership(z, CComplex(x, y));
				if (iterationResult == 0) {
					glColor3f(1.0, 0.1, 0.1);
					glVertex3d(x, y, 0);
				}
			}
		glEnd();
		glPopMatrix();
	}

	FractalRenderer() {
		fractalParams.iterCount = NRITER_JF;
		fractalParams.maxMagnitude = MODMAX_JF;
	}

	explicit FractalRenderer(CComplex& complexVar) {
		fractalParams.complex = complexVar;
		fractalParams.iterCount = NRITER_JF;
		fractalParams.maxMagnitude = MODMAX_JF;
	}

	void updateMaxMagnitude(double newValue) {
		assert(newValue <= MODMAX_JF);
		fractalParams.maxMagnitude = newValue;
	}

	void updateIterationCount(int newValue) {
		assert(newValue <= MODMAX_JF);
		fractalParams.iterCount = newValue;
	}

	int calculatePointMembership(CComplex& point, const CComplex& constant) {
		CComplex next, current = point;

		for (int i = 1; i < fractalParams.iterCount; ++i) {
			next = current * current + constant;
			if (next == current || next.getModul() > fractalParams.maxMagnitude) {
				return i;
			}
			current = next;
		}
		return 0;
	}
};


class ComplexFigure {
public:
	void visualizePattern(double size, int depth) {
		CVector direction(0.0, -1.0);
		CPunct origin(0.0, 2.0);

		direction.draw_graphic(origin, 0.25);
		origin = direction.getDest(origin, 0.25);
		drawFigure(origin, direction, depth, 0.4, size);
	}

	void drawFigure(CPunct point, CVector vector, int depth, double scale, double size) {
		CPunct point1, point2;
		if (depth != 0) {
			vector.rotatie(-45);
			vector.draw_graphic(point, size);
			point1 = vector.getDest(point, size);
			drawFigure(point1, vector, depth - 1, scale, size * scale);

			vector.rotatie(90);
			vector.draw_graphic(point, size);
			point1 = vector.getDest(point, size);
			point2 = point1;

			vector.rotatie(15);
			vector.draw_graphic(point1, size);
			point1 = vector.getDest(point1, size);
			drawFigure(point1, vector, depth - 1, scale, size * scale);

			point1 = point2;
			vector.rotatie(-60);
			vector.draw_graphic(point1, size);
			point1 = vector.getDest(point1, size);
			point2 = point1;

			vector.rotatie(-90);
			vector.draw_graphic(point1, size / 2);
			point1 = vector.getDest(point1, size / 2);
			drawFigure(point1, vector, depth - 1, scale, size * scale);

			point1 = point2;
			vector.rotatie(120);
			vector.draw_graphic(point1, size / 2);
			point1 = vector.getDest(point1, size / 2);
			drawFigure(point1, vector, depth - 1, scale, size * scale);
		}
	}
};

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(WINDOW_SIZE_PX, WINDOW_SIZE_PX);
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


void display_characters(char c[])
{
	glRasterPos2d(-0.98, -0.98);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);
}

// Julia-Fatou set for z0 = 0 and c = -0.12375+0.056805i
void Display1() {
	CComplex c(-0.12375, 0.056805);
	CJuliaFatou cjf(c);

	glColor3f(1.0, 0.1, 0.1);
	cjf.setnriter(30);
	cjf.display(-0.8, -0.4, 0.8, 0.4);
}

// Julia-Fatou set for z0 = 0 and c = -0.012+0.74i
void Display2() {
	CComplex c(-0.012, 0.74);
	CJuliaFatou cjf(c);

	glColor3f(1.0, 0.1, 0.1);
	cjf.setnriter(30);
	cjf.display(-1, -1, 1, 1);
}

// displays the Koch snowflake
void Display3() {
	CCurbaKoch cck;
	cck.print_graphic(sqrt(3.0), nivel_display3);

	char c[3];
	sprintf(c, "%2d", nivel_display3);
	display_characters(c);

	glRasterPos2d(-1.0, 0.9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'K');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'h');

	nivel_display3++;
}

// displays a binary tree
void Display4() {
	CArboreBinar cab;
	cab.print_graphic(1, nivel_display4);

	char c[3];
	sprintf(c, "%2d", nivel_display4);
	display_characters(c);

	glRasterPos2d(-1.0, 0.9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');

	nivel_display4++;
}

// displays a fractal tree
void Display5() {
	CArborePerron cap;

	char c[3];
	sprintf(c, "%2d", nivel_display5);
	display_characters(c);

	glRasterPos2d(-1.0, -0.9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');

	glPushMatrix();
	glLoadIdentity();
	glScaled(0.4, 0.4, 1);
	glTranslated(-0.5, -0.5, 0.0);
	cap.print_graphic(1, nivel_display5);
	glPopMatrix();
	nivel_display5++;
}

// displays the Hilbert curve
void Display6() {
	CCurbaHilbert cch;
	cch.print_graphic(0.05, nivel_display6);

	char c[3];
	sprintf(c, "%2d", nivel_display6);
	display_characters(c);

	glRasterPos2d(-1.0, -0.9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'H');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');

	nivel_display6++;
}

// multimea Mandelbrot
void Display7() {
	FractalRenderer cmb;
	cmb.updateIterationCount(20);
	cmb.updateMaxMagnitude(2.0);
	cmb.renderFractal(-2, -2, 2, 2);
}

void Init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glLineWidth(1);
	glPointSize(1);
	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display8() {
	PatternDesign firstShape;
	firstShape.renderDesign(1, nivel_display8);

	char c[3];
	sprintf(c, "%2d", nivel_display8);
	display_characters(c);

	nivel_display8++;
}

void Display9() {
	ComplexFigure secondShape;

	char c[3];
	sprintf(c, "%2d", nivel_display9);
	display_characters(c);

	glPushMatrix();
	glLoadIdentity();
	glScaled(0.4, 0.4, 1);
	glTranslated(-0.5, -0.5, 0.0);
	secondShape.visualizePattern(1, nivel_display9);
	glPopMatrix();
	nivel_display9++;
}

void Display10() {
	TriangleFractal thirdShape;

	char c[3];
	sprintf(c, "%2d", nivel_display10);
	display_characters(c);

	glPushMatrix();
	glLoadIdentity();
	glScaled(0.4, 0.4, 1);
	glTranslated(-0.5, -0.5, 0.0);
	thirdShape.renderGraphic(1, nivel_display10);
	glPopMatrix();
	nivel_display10++;
}

void clear_levels() {
	nivel_display3 = 0;
	nivel_display4 = 0;
	nivel_display5 = 0;
	nivel_display6 = 0;
	nivel_display8 = 0;
	nivel_display9 = 0;
	nivel_display10 = 0;
}

void Display(void) {
	printf("Call Display\n");
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '0':
		glClear(GL_COLOR_BUFFER_BIT);
		clear_levels();
		break;
	case '1':
		glClear(GL_COLOR_BUFFER_BIT);
		Display1();
		break;
	case '2':
		glClear(GL_COLOR_BUFFER_BIT);
		Display2();
		break;
	case '3':
		glClear(GL_COLOR_BUFFER_BIT);
		Display3();
		break;
	case '4':
		glClear(GL_COLOR_BUFFER_BIT);
		Display4();
		break;
	case '5':
		glClear(GL_COLOR_BUFFER_BIT);
		Display5();
		break;
	case '6':
		glClear(GL_COLOR_BUFFER_BIT);
		Display6();
		break;
	case '7':
		glClear(GL_COLOR_BUFFER_BIT);
		Display7();
		break;
	case '8':
		glClear(GL_COLOR_BUFFER_BIT);
		Display8();
		break;
	case '9':
		glClear(GL_COLOR_BUFFER_BIT);
		Display9();
		break;
	case 'x':
		glClear(GL_COLOR_BUFFER_BIT);
		Display10();
		break;
	default:
		break;
	}

	glFlush();
}

void Reshape(int w, int h) {
	printf("Call Reshape : width = %d, height = %d\n", w, h);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	printf("You have pressed <%c>. The mouse has the position %d, %d.\n",
		key, x, y);
	prevKey = key;
	if (key == 27)
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
	printf("Call MouseFunc : you have %s the %s button in the position %d %d\n",
		(state == GLUT_DOWN) ? "pressed" : "released",
		(button == GLUT_LEFT_BUTTON) ?
		"left" :
		((button == GLUT_RIGHT_BUTTON) ? "right" : "middle"),
		x, y);
}
