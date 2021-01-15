#include <GL/glut.h>
#include <cmath>
#include <iostream>

/**
    X = (A + B )/ 2
    Y = (A + C) / 2
**/
GLfloat GetMidpoint(GLfloat coord1, GLfloat coord2)
{
    return (coord1 + coord2) / 2;
}

class VerticesGenerator {

public: 
    GLfloat* Generate(GLfloat a, GLfloat b)
    {
        GLfloat* tmp = new GLfloat[2];
        tmp[0] = a;
        tmp[1] = b;
        return tmp;
    }
};

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

// Clears the current window and draws a triangle.
void DrawTwoTriangles() {

    VerticesGenerator generator = VerticesGenerator();

    // Set every pixel in the frame buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing is done by specifying a sequence of vertices
    glBegin(GL_TRIANGLES);
        
        glColor3f(1.0, 1.0, 1.0);
        
        const int numberOfVectors = 3;
        const int dimensions = 2;

        //dlugosc boku AB
        GLfloat sideLength = 0.6;

        //poczatek ukladu wspolrzednych
        GLfloat begginingOfCoordinateSystem[2] = { -0.5, 0 };
        
        GLfloat* A = generator.Generate(begginingOfCoordinateSystem[0] + (GLfloat)(-0.2), begginingOfCoordinateSystem[1]);
        GLfloat* B = generator.Generate(begginingOfCoordinateSystem[0] + (GLfloat)(-0.2), begginingOfCoordinateSystem[1] + sideLength);
        GLfloat* C = generator.Generate(begginingOfCoordinateSystem[0] + (GLfloat)0.7, begginingOfCoordinateSystem[1]);

        Triangle* mainTriangle = new Triangle(A, B ,C);
        mainTriangle->PrintMyVertices();
        mainTriangle->PrintMidpoints();

        // Rysowanie trojkata glownego
        glVertex2fv(mainTriangle->A);
        glVertex2fv(mainTriangle->B);
        glVertex2fv(mainTriangle->C);

        GLfloat* Aprim = mainTriangle->GetMidPointsAB();
        GLfloat* Bprim = mainTriangle->GetMidPointsBC();
        GLfloat* Cprim = mainTriangle->GetMidPointsAC();


        Triangle* subTriangle = new Triangle(Aprim, Bprim, Cprim);
        
        glColor3f(1.0, 0, 0);
        glVertex2fv(subTriangle->A);
        glVertex2fv(subTriangle->B);
        glVertex2fv(subTriangle->C);

    glEnd();

    // Flush drawing command buffer to make drawing happen as soon as possible.
    glFlush();
}



// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv) {

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
    glutDisplayFunc(DrawTwoTriangles);

    // Tell GLUT to start reading and processing events.  This function
    // never returns; the program only exits when the user closes the main
    // window or kills the process.
    glutMainLoop();
}