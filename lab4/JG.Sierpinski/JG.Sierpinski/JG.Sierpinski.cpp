#include <GL/glut.h>

// Clears the current window and draws a triangle.
void DrawSingleTriangle() {

    // Set every pixel in the frame buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing is done by specifying a sequence of vertices
    glBegin(GL_TRIANGLES);
        
        glColor3f(1.0, 1.0, 1.0);
        
        const int numberOfVectors = 3;
        const int dimensions = 2;

        //dlugosc boku AB
        const float sideLength = 0.6;

        //poczatek ukladu wspolrzednych
        const GLfloat begginingOfCoordinateSystem[2] = { -0.5, 0 };

        const GLfloat triangleVertices[numberOfVectors][dimensions] = { {begginingOfCoordinateSystem[0] + (-0.2), begginingOfCoordinateSystem[1]}, {begginingOfCoordinateSystem[0]  +(-0.2), begginingOfCoordinateSystem[1] + sideLength}, {begginingOfCoordinateSystem[0] + 0.7, begginingOfCoordinateSystem[1]} };
        glVertex2fv(triangleVertices[0]);
        glVertex2fv(triangleVertices[1]);
        glVertex2fv(triangleVertices[2]);

        const float sideLength2 = 0.3;
        glColor3f(1.0, 0, 0);
        const GLfloat triangleVertices2[numberOfVectors][dimensions] = { {begginingOfCoordinateSystem[0] + (-0.2), begginingOfCoordinateSystem[1]}, {begginingOfCoordinateSystem[0] + (-0.2), begginingOfCoordinateSystem[1] + sideLength2}, {begginingOfCoordinateSystem[0] + 0.7, begginingOfCoordinateSystem[1]} };
        glVertex2fv(triangleVertices2[0]);
        glVertex2fv(triangleVertices2[1]);
        glVertex2fv(triangleVertices2[2]);
        //draw smaller one



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
    glutDisplayFunc(DrawSingleTriangle);

    // Tell GLUT to start reading and processing events.  This function
    // never returns; the program only exits when the user closes the main
    // window or kills the process.
    glutMainLoop();
}