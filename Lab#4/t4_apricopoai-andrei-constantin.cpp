#define _CRT_SECURE_NO_WARNINGS

#define LINES 15
#define COLUMNS 15
#define margin 0.1
#define WINDOW_SIZE_PX  300
#define M_PI 3.14159265358979323846

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include "glut.h"

int radius = 10;
int width = WINDOW_SIZE_PX;
int height = WINDOW_SIZE_PX;

double columns = 15.0;
double lines = 15.0;

void Init(void);
void Display(void);
void Reshape(int w, int h);
void Reshape(int w, int h);
void KeyboardFunc(unsigned char key, int x, int y);
void MouseFunc(int button, int state, int x, int y);

unsigned char prevKey;

class Point
{
private:
	int coordX, coordY;

public:
	Point(int initialX, int initialY) : coordX(initialX), coordY(initialY) {}

	void updateX(int newX)
	{
		coordX = newX;
	}

	int retrieveX()
	{
		return coordX;
	}

	void updateY(int newY)
	{
		coordY = newY;
	}

	int retrieveY()
	{
		return coordY;
	}
};


class GridDisplay {
private:
    int gridRows;
    int gridColumns;

    void getGridCoordinates(int row, int column, double& coordX, double& coordY) {
        double minimumDimension = std::min(width, height);
        double xMax = static_cast<double>(width) / minimumDimension + margin;
        double yMax = static_cast<double>(height) / minimumDimension + margin;

        coordX = (2.0 * row / gridColumns - 1) / xMax;
        coordY = (2.0 * column / gridRows - 1) / yMax;
    }

public:
    GridDisplay(int rows, int columns) : gridRows(rows), gridColumns(columns) {}

    void render() {
        glColor3f(0.0, 0.0, 0.0);
        double minDim = std::min(width, height);
        double xLimit = static_cast<double>(width) / minDim + margin;
        double yLimit = static_cast<double>(height) / minDim + margin;

        int counter = 0;
        while(counter <= gridColumns){
            glBegin(GL_LINE_STRIP);
            glVertex2f(-1.0 / xLimit, (2.0 * counter / gridColumns - 1) / yLimit);
            glVertex2f(1.0 / xLimit, (2.0 * counter / gridColumns - 1) / yLimit);
            glEnd();
            counter++;
        }

        counter = 0;
        while (counter <= gridRows) {
            glBegin(GL_LINE_STRIP);
            glVertex2f((2.0 * counter / gridRows - 1) / xLimit, -1.0 / yLimit);
            glVertex2f((2.0 * counter / gridRows - 1) / xLimit, 1.0 / yLimit);
            glEnd();
            counter++;
        }
    }

    void plotPixel(int row, int column) {
        if (row < 0 || row > gridRows || column < 0 || column > gridColumns) {
            return;
        }

        glColor3f(0.3, 0.3, 0.3);
        double minDimension = std::min(width, height);
        double xMax = static_cast<double>(width) / minDimension + margin;
        double yMax = static_cast<double>(height) / minDimension + margin;

        double xCenter = (2.0 * row / gridColumns - 1) / xMax;
        double yCenter = (2.0 * column / gridRows - 1) / yMax;

        double minXY = std::min(xMax, yMax);
        double radius = (gridRows > gridColumns) ? minXY / (gridColumns * 2) : minXY / (gridRows * 2);

        glBegin(GL_POLYGON);
        int counter = 0;
        while(counter < 25){
            double angle = 2 * M_PI * counter / 25;
            double x = radius * cos(angle);
            double y = radius * sin(angle);
            glVertex2f(x / xMax + xCenter, y / yMax + yCenter);
            counter++;
        }
        glEnd();
    }

    void drawLine(int xStart, int yStart, int xEnd, int yEnd, int points) {
        double x1, y1, x2, y2;
        glColor3f(1.0, 0.1, 0.1);
        glLineWidth(4.0f);
        getGridCoordinates(xStart, yStart, x1, y1);
        getGridCoordinates(xEnd, yEnd, x2, y2);

        glBegin(GL_LINE_STRIP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
        glLineWidth(1.0f);

        auto linePoints = calculateLinePoints(xStart, yStart, xEnd, yEnd, points);
        for (auto& point : linePoints)
            plotPixel(point.retrieveX(), point.retrieveY());
    }

    std::vector<Point> calculateLinePoints(int xStart, int yStart, int xEnd, int yEnd, int points) {
        double slope = (static_cast<double>(yEnd - yStart)) / (xEnd - xStart);
        if (slope > 0)
            return (xStart > xEnd) ? calculateUpperLine(xEnd, yEnd, xStart, yStart, points)
            : calculateUpperLine(xStart, yStart, xEnd, yEnd, points);
        else
            return (xStart > xEnd) ? calculateLowerLine(xEnd, yEnd, xStart, yStart, points)
            : calculateLowerLine(xStart, yStart, xEnd, yEnd, points);
    }

    std::vector<Point> calculateUpperLine(int xStart, int yStart, int xEnd, int yEnd, int density) {
        std::vector<Point> points;
        int deltaY = yEnd - yStart;
        int deltaX = xEnd - xStart;

        int decision = 2 * deltaY - deltaX;
        int incrementEast = 2 * deltaY;
        int incrementNorthEast = 2 * (deltaY - deltaX);
        int x = xStart, y = yStart;

        points.emplace_back(x, y);

        int counter = 0;
        while (counter <= density / 2) {
			points.emplace_back(x, y - counter);
			points.emplace_back(x, y + counter);
			counter++;
		}

        while (x < xEnd) {
            if (decision <= 0) {
                decision = decision + incrementEast;
                x++;
            }
            else {
                decision = decision + incrementNorthEast;
                x++;
                y++;
            }

            points.emplace_back(x, y);
            counter = 0;
            while (counter <= density / 2) {
                points.emplace_back(x, y - counter);
                points.emplace_back(x, y + counter);
                counter++;
            }
        }
        return points;
    }

    std::vector<Point> calculateLowerLine(int xStart, int yStart, int xEnd, int yEnd, int density) {
        std::vector<Point> points;
        int deltaY = yEnd - yStart;
        int deltaX = xEnd - xStart;

        int decision = 2 * deltaY + deltaX;
        int incrementEast = 2 * deltaY;
        int incrementSouthEast = 2 * (deltaY + deltaX);
        int x = xStart, y = yStart;

        points.emplace_back(x, y);
        int counter = 0;
        while (counter <= density / 2) {
            points.emplace_back(x, y - counter);
            points.emplace_back(x, y + counter);
            counter++;
        }

        while (x < xEnd) {
            if (decision >= 0) {
                decision = decision + incrementEast;
                x++;
            }
            else {
                decision = decision + incrementSouthEast;
                x++;
                y--;
            }

            points.emplace_back(x, y);
            counter = 0;
            while (counter <= density / 2) {
                points.emplace_back(x, y - counter);
                points.emplace_back(x, y + counter);
                counter++;
            }
        }
        return points;
    }

    std::vector<Point> ScanConvertCircle4(float R)
    {
        std::vector<Point>  puncte;
        std::vector<Point>  puncteSecundare;
        int x = R;
        int y = 0;

        puncte.push_back(Point(x, y));
        puncteSecundare.push_back(Point(x, y));
        puncteSecundare.push_back(Point(x - 1, y));
        puncteSecundare.push_back(Point(x + 1, y));
        puncte.insert(std::end(puncte), std::begin(puncteSecundare), std::end(puncteSecundare));

        int d = 1 - R;
        int dN = 3;
        int dNW = -2 * R + 5;
        while (x > y)
        {
            if (d > 0)
            {
                d += dNW;
                dN += 2;
                dNW += 4;
                x--;
            }
            else
            {
                d += dN;
                dN += 2;
                dNW += 2;
            }
            y++;

            puncte.push_back(Point(x, y));
            puncteSecundare.push_back(Point(x, y));
            puncteSecundare.push_back(Point(x - 1, y));
            puncteSecundare.push_back(Point(x + 1, y));
            puncte.insert(std::end(puncte), std::begin(puncteSecundare), std::end(puncteSecundare));

        }
        return puncte;
    }


    void renderCircle(float  R, int width) {
        std::vector<Point> pixeli = ScanConvertCircle4(R);
        for (int index = 0; index < pixeli.size(); index++)
        {
            plotPixel(pixeli[index].retrieveX(), pixeli[index].retrieveY());
        }

        glColor3f(1.0, 0.1, 0.1);
        glLineWidth(4.f);

        double rx = (2.0 / (double)15 * R - 1) / 1.1;
        double ry = - 1 / 1.1;

        glBegin(GL_LINE_LOOP);

        double counter = 0;
        while (counter < 1000)
        {
            double theta = 2.0 * M_PI * counter / 1000.0;
            double x = (rx + 0.9) * cosf(theta);
            double y = (rx + 0.9) * sinf(theta);

            glVertex2f(x - 0.9, y - 0.9);
            counter++;
        }

        glEnd();
        glLineWidth(1.f);
    }

};
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(width, height);
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
void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glLineWidth(1);
	glPolygonMode(GL_FRONT, GL_FILL);
}
void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	GridDisplay cartesianGrid(lines, columns);

	glRasterPos2d(-0.98, -0.98);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '0');

	glRasterPos2d(-0.98, 0.92);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'Y');

	glRasterPos2d(0.92, -0.98);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'X');

	switch (prevKey)
	{
	case '1':
		cartesianGrid.render();
		cartesianGrid.renderCircle(13, 3);
		break;
	case '2':
		cartesianGrid.render();
		cartesianGrid.drawLine(0, 15, 15, 10, 3);
		cartesianGrid.drawLine(0, 0, 15, 7, 1);
		break;
	default:
		break;
	}
	glFlush();
}
void Reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}
void KeyboardFunc(unsigned char key, int x, int y)
{
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}
void MouseFunc(int button, int state, int x, int y)
{
	printf("Call MouseFunc : you have %s the %s button in the position %d %d\n",
		(state == GLUT_DOWN) ? "pressed" : "released",
		(button == GLUT_LEFT_BUTTON) ?
		"left" :
		((button == GLUT_RIGHT_BUTTON) ? "right" : "middle"),
		x, y);
}
