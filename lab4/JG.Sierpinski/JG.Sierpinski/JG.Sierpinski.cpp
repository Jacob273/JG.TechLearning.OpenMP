#include <GL/glut.h>
#include <cmath>
#include <iostream>


GLfloat begginingOfCoordinateSystem[2] = { 0, 0 };
GLfloat sideLength = 0.5;

class Triangle {

private: 

    GLfloat GetMidPointAB_x()
    {
        return (A[0] + B[0]) / 2;
    }

    GLfloat GetMidPointAB_y()
    {
        return (A[1] + B[1]) / 2;
    }

    GLfloat GetMidPointAC_x()
    {
        return (A[0] + C[0]) / 2;
    }

    GLfloat GetMidPointAC_y()
    {
        return (A[1] + C[1]) / 2;
    }

    GLfloat GetMidPointBC_x()
    {
        return (B[0] + C[0]) / 2;
    }


    GLfloat GetMidPointBC_y()
    {
        return (B[1] + C[1]) / 2;
    }

public:

    GLfloat* A;
    GLfloat* B;
    GLfloat* C;

    Triangle(GLfloat* vertexA, GLfloat* vertexB, GLfloat* vertexC)
    {
        A = vertexA;
        B = vertexB;
        C = vertexC;
    }

    GLfloat* GetMidPointsAB() 
    {
        GLfloat* tmp = new GLfloat[2];
        tmp[0] = GetMidPointAB_x();
        tmp[1] = GetMidPointAB_y();
        return tmp;
    }

    GLfloat* GetMidPointsAC()
    {
        GLfloat* tmp = new GLfloat[2];
        tmp[0] = GetMidPointAC_x();
        tmp[1] = GetMidPointAC_y();
        return tmp;
    }

    GLfloat* GetMidPointsBC()
    {
        GLfloat* tmp = new GLfloat[2];
        tmp[0] = GetMidPointBC_x();
        tmp[1] = GetMidPointBC_y();
        return tmp;
    }

    void PrintMyVertices() {
        std::cout << "Ax = " << A[0] << " Ay= " << A[1] << std::endl;
        std::cout << "Bx = " << B[0] << " By= " << B[1] << std::endl;
        std::cout << "Cx = " << C[0] << " Cy= " << C[1] << std::endl;
        std::cout << std::endl;
    }

    void PrintMidpoints() {
        std::cout << "ABx = " << GetMidPointsAB()[0] << " ABy= " << GetMidPointsAB()[1] << std::endl;
        std::cout << "ACx = " << GetMidPointsAC()[0] << " ACy= " << GetMidPointsAC()[1] << std::endl;
        std::cout << "BCx = " << GetMidPointsBC()[0] << " BCy= " << GetMidPointsAB()[1] << std::endl;
        std::cout << std::endl;
    }
};

class GeometryHelper {

public:

    GLfloat* GenerateVertices(GLfloat a, GLfloat b)
    {
        GLfloat* tmp = new GLfloat[2];
        tmp[0] = a;
        tmp[1] = b;
        return tmp;
    }

    GLfloat* DivideVertices(GLfloat* vertices, GLfloat divider)
    {
        vertices[0] /= divider;
        vertices[1] /= divider;
        return vertices;
    }

    Triangle* GenerateTriangle() {
        GLfloat* A = GenerateVertices(begginingOfCoordinateSystem[0] + (GLfloat)(-0.2), begginingOfCoordinateSystem[1]);
        GLfloat* B = GenerateVertices(begginingOfCoordinateSystem[0] + (GLfloat)(-0.2), begginingOfCoordinateSystem[1] + sideLength);
        GLfloat* C = GenerateVertices(begginingOfCoordinateSystem[0] + (GLfloat)0.7, begginingOfCoordinateSystem[1]);
        return new Triangle(A, B, C);
    }

};


class DrawingObject
{

public:
    static int counter;
    int _id;

    DrawingObject() 
    {
        _id = 0;
    }

    void DrawTriangle(Triangle* triangle, GLfloat r, GLfloat g, GLfloat b) {
        glColor3f(r, g, b);
        glVertex2fv(triangle->A);
        glVertex2fv(triangle->B);
        glVertex2fv(triangle->C);
    }




    void RecursiveDrawing(Triangle* firstTriangle, int level)
    {
        _id = counter++;

        GLfloat r = rand() / ((float)RAND_MAX + 1);
        GLfloat g = 0 + rand() / ((float)RAND_MAX + 1);
        GLfloat b = 0 + rand() / ((float)RAND_MAX + 1);

        std::cout << "level: " << (GLfloat)level << std::endl;
        std::cout << "triangleID: >>" << _id << ">>" << std::endl << std::endl;
        firstTriangle->PrintMyVertices();
        counter++;
        DrawTriangle(firstTriangle, r, g, b);
        if (level > 0)
        {
            GeometryHelper helper = GeometryHelper();

            GLfloat* ABMid = firstTriangle->GetMidPointsAB();
            GLfloat* BCMid = firstTriangle->GetMidPointsBC();
            GLfloat* ACMid = firstTriangle->GetMidPointsAC();

            Triangle* second = new Triangle(ABMid, BCMid, ACMid);

            GLfloat* aPrim = second->GetMidPointsAC();
            GLfloat* bPrim = helper.GenerateVertices(firstTriangle->A[0], firstTriangle->A[1] + firstTriangle->B[1] / 4);
            GLfloat* cPrim = helper.GenerateVertices(second->GetMidPointsAC()[0], firstTriangle->C[1]);

            Triangle* third = new Triangle(aPrim, bPrim, cPrim);

            RecursiveDrawing(second, 0);
            RecursiveDrawing(third, 0);
            //RecursiveDrawing(new Triangle(ABMid, BCMid, ACMid), 0);
        }
    }

    // Clears the current window and draws a triangle.
    void Trampoline_MainDrawing() 
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
        GeometryHelper helper = GeometryHelper();
        Triangle* main = helper.GenerateTriangle();
        int level = 3;
        RecursiveDrawing(main, level);
        glEnd();
        glFlush();
    }
};
int DrawingObject::counter = 0;


DrawingObject* drawObj = new DrawingObject();
extern "C" void Trampoline_MainDrawing()
{
    return drawObj->Trampoline_MainDrawing();
}


// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv) 
{
    // Use a single buffered window in RGB mode
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    const int windowPositionX = 500;
    const int windowPositionY = 200;
    glutInitWindowPosition(windowPositionX, windowPositionY);

    const int windowWidth = 400;
    const int windowHeight = 500;

    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("A Simple Triangle");

    // Tell GLUT that whenever the main window needs to be repainted that it
    // should call the function display().
    glutDisplayFunc(Trampoline_MainDrawing);

    // Tell GLUT to start reading and processing events.  This function
    // never returns; the program only exits when the user closes the main
    // window or kills the process.
    glutMainLoop();
}