#include <GL/glut.h>

// Clears the current window and draws a triangle.
void DrawSingleTriangle() {

    // Set every pixel in the frame buffer to the current clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing is done by specifying a sequence of vertices
    glBegin(GL_TRIANGLES);
        
        glColor3f(3, 5, 155);
        
        const int numberOfVectors = 3;
        const int dimensions = 2;
        const GLfloat triangleVertices[numberOfVectors][dimensions] = { {-0.5, -0.7}, {0.5, -0.7}, {0.0, 0.5} };
        glVertex2fv(triangleVertices[0]);
        glVertex2fv(triangleVertices[1]);
        glVertex2fv(triangleVertices[2]);
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