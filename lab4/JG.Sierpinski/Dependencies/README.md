 How to setup open GL with VS 2019

 __1.__ Paste glut.h to C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\{x.x.x.x number of your version}\include\GL

__1.1__ Create the GL folder if not present.

__2.__ Paste glut.lib to C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\{x.x.x.x number of your version}\lib\x64

__2.1__ Paste glut32.lib in C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\{x.x.x.x number of your version}\lib\x86

__3.__ Paste glut.dll and glut32.dll to C:\Windows\SysWOW64

__3.1__ Copy glut32.dll to C:\Windows\System32

__4.__ In Visual Studio:

__4.1__ right click on project => C/C++ => precompiled headers => precompilder header => set to 'not using precompiled headers'

__4.2__ right click on project => Linker => Input => Additional Dependencies => Edit... => add each on newline: 
opengl32.lib
glu32.lib
glut32.lib