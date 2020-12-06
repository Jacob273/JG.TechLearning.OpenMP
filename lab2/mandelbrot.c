 /* 
 c program:
 --------------------------------
  1. draws Mandelbrot set for Fc(z)=z*z +c
  using Mandelbrot algorithm ( boolean escape time )
 -------------------------------         
 2. technique of creating ppm file is  based on the code of Claudio Rocchini
 http://en.wikipedia.org/wiki/Image:Color_complex_plot.jpg
 create 24 bit color graphic file ,  portable pixmap file = PPM 
 see http://en.wikipedia.org/wiki/Portable_pixmap
 to see the file use external application ( graphic viewer)
  */
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define iXmax 3000
#define iYmax 3000
unsigned char color[iYmax][iXmax][3];

 int main()
 {
        /* screen ( integer) coordinate */
        int iX,iY;
        /* world ( double) coordinate = parameter plane*/
        double Cx,Cy;
        const double CxMin=-2.5;
        const double CxMax=1.5;
        const double CyMin=-2.0;
        const double CyMax=2.0;
        /* */
        double PixelWidth=(CxMax-CxMin)/iXmax;
        double PixelHeight=(CyMax-CyMin)/iYmax;
        /* color component ( R or G or B) is coded from 0 to 255 */
        /* it is 24 bit color RGB file */
        const int MaxColorComponentValue=255; 
        FILE * fp;
        char *filename="new1.ppm";
        char *comment="# ";/* comment should start with # */
        /* Z=Zx+Zy*i  ;   Z0 = 0 */
        double Zx, Zy;
        double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
        /*  */
        int Iteration;
        const int IterationMax=200;
        /* bail-out value , radius of circle ;  */
        const double EscapeRadius=2;
        double ER2=EscapeRadius*EscapeRadius;
        /*create new file,give it a name and open it in binary mode  */
        fp= fopen(filename,"wb"); /* b -  binary mode */
        /*write ASCII header to the file*/
        fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,iXmax, iYmax, MaxColorComponentValue);
        /* compute and write image data bytes to the file*/
        double start = omp_get_wtime();
        double stop;
        int threadId = 0;
        int sums[4] = {0, 0, 0, 0};

        #pragma omp parallel private(threadId) num_threads(4)
        {
         threadId = omp_get_thread_num();
         #pragma omp for private(iY, iX, Iteration, Cy, Cx, Zx, Zy, Zx2, Zy2) schedule(static , 10)

            for(iY=0;iY<iYmax;iY++)
            {
                  Cy=CyMin + iY*PixelHeight;
                  if (fabs(Cy)< PixelHeight/2) Cy=0.0; /* Main antenna */
                  for(iX=0;iX<iXmax;iX++)
                  {         
                              Cx=CxMin + iX*PixelWidth;
                              /* initial value of orbit = critical point Z= 0 */
                              Zx=0.0;
                              Zy=0.0;
                              Zx2=Zx*Zx;
                              Zy2=Zy*Zy;
                              /* */
                              for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
                              {
                                 Zy=2*Zx*Zy + Cy;
                                 Zx=Zx2-Zy2 +Cx;
                                 Zx2=Zx*Zx;
                                 Zy2=Zy*Zy;
                              };

                              sums[threadId] += Iteration;
                              /* compute  pixel color (24 bit = 3 bytes) */
                              if (Iteration==IterationMax)
                              { 
                                 color[iY][iX][0]=0;
                                 color[iY][iX][1]=0;
                                 color[iY][iX][2]=0;                           
                              }
                           else 
                              { 
                                 color[iY][iX][0]= threadId + 1 * 60;
                                 color[iY][iX][1]= threadId + 1 * 60; 
                                 color[iY][iX][2]= threadId + 1 * 60;  
                              };
                     }
            }
        }
        stop = omp_get_wtime();
        fwrite(color,1, 3 * iXmax * iYmax, fp);
        printf("Czas %.3f \n", stop - start);

      for(int i = 0 ; i < 4; i++)
      {
         printf("Wartosc sum[%d] = %d \n", i, sums[i]);
      }

        fclose(fp);
        return 0;
 }



















 //TODO:

 /**

Zmniejszajcie ja od 100 do 10 az do 1
Jak wyrownuja sie liczby iteracje.


//wykresy dla przyspieszenia
//dla roznej liczby watkow
//zmienic obrazek na 2000x2000 pixeli! (800x600 to loozik)


//jak schedal rozklada im prace

 **/