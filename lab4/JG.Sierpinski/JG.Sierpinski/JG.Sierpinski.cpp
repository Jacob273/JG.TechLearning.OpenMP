#include "SierpinskiOpenGL.h"
#include "SierpinskiPPM.h"

int main(int argc, char** argv)
{
    //SierpinskiOpenGL::Run(argc, argv);
    SierpinskiPPM::Sierpinski* s = new SierpinskiPPM::Sierpinski(800, 800, 3);
    s->Run();
}