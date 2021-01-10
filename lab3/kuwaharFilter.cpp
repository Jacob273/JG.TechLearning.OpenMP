#include <stdio.h>
#include "EasyBMP.h"
#include <omp.h>
 


//Klasa realizujaca filtrowanie kuwahara dla obrazu kolorowego
//Obraz wczytywany jest przy pomocy EasyBMP
class Kuwahar
{ 
    //TODO:
    BMP img;
    const char* _inputFileName;
    const char* _outputFileName;

    bool imgRead;
    bool isFiltered;

    private:
        void ReadFile(const char* fileName)
        {
            img.ReadFromFile(fileName);
        }

    public: 
        Kuwahar(const char* inputFileName,  const char* outputFileName)
        {
            _inputFileName = inputFileName;
            _outputFileName = outputFileName;
            imgRead = false;
            isFiltered = false;
        }

        void Read()
        {
            if(!imgRead)
            {
                img.ReadFromFile(_inputFileName);
                imgRead = true;
            }
        }

        void Write()
        {
            if(imgRead && isFiltered)
            {
                img.WriteToFile(_outputFileName); 
            }
            else
            {
                printf("Could not create a filtered image. Make sure the Read() and Filter() methods were executed.");
            }
        }

        void FilterParallel() 
        { 
                if(!imgRead || isFiltered)
                {
                    return;
                }

                //WIELKOSC PLIKU marbles.bmp: WIDTH: 1419 X HEIGHT:1001
                unsigned char **red = (unsigned char**)malloc(img.TellWidth() * sizeof(char*));
                unsigned char **green = (unsigned char**)malloc(img.TellWidth() * sizeof(char*));
                unsigned char **blue = (unsigned char**)malloc(img.TellWidth() * sizeof(char*));
                
                
                for (int i=0; i<img.TellWidth(); i++)
                {
                    red[i] = (unsigned char*)malloc(img.TellHeight());
                    green[i] = (unsigned char*)malloc(img.TellHeight());
                    blue[i] = (unsigned char*)malloc(img.TellHeight());
            
                    for (int j=0; j<img.TellHeight(); j++)
                    {
                        //SETTING RGB values from image (masked)
                        red[i][j] = (img(i,j)->Red & 0xFF); // 0xFF = 11111111 = 255
                        green[i][j] = (img(i,j)->Green & 0xFF);
                        blue[i][j] = (img(i,j)->Blue & 0xFF);
                    }
                }
                            
                double redAverage[4], greenAverage[4], blueAverage[4];  //FOR AVERAGES
                double redVariant[4], greenVariant[4], blueVariant[4];  //FOR VARIANT

                int m, lowestRed, lowestGreen, lowestBlue;
                int Size = 5;
                int margin = (( Size - 1) / 2);
            
                int i, j, k, l;


            #pragma omp parallel for private(i, j, k, l, redAverage, greenAverage , blueAverage , redVariant, greenVariant, blueVariant ) num_threads(8)
            for (i = margin; i < img.TellWidth() - margin; i++)
                for ( j = margin;  j < img.TellHeight() - margin; j++)
                {

                    //SETTING AVERAGEs to 0
                    for (k=0; k < 4; k++)
                    {
                        redAverage[k] = 0;
                        greenAverage[k] = 0;
                        blueAverage[k] = 0;
                    }

                    //Calculating AVERAGE
                    for (k=0;  k < 3; k++)
                        for (l=0;  l < 3; l++)
                        {
                            const float numberOfPixels = 9.0;
                            redAverage[0] += red[i + k - margin][j + l - margin] / numberOfPixels;
                            redAverage[1] += red[i + k ][ j + l - margin] / numberOfPixels;
                            redAverage[2] += red[i + k - margin][j + l] / numberOfPixels;
                            redAverage[3] += red[i + k][ j + l] / numberOfPixels;
            
                            greenAverage[0] += green[ i + k - margin][ j + l - margin] / numberOfPixels;
                            greenAverage[1] += green[ i + k][ j + l - margin] / numberOfPixels;
                            greenAverage[2] += green[i + k - margin][j + l] / numberOfPixels;
                            greenAverage[3] += green[i + k][j + l] / numberOfPixels;
            
                            blueAverage[0] += blue[i + k - margin][j + l - margin] / numberOfPixels;
                            blueAverage[1] += blue[i + k][j + l - margin] / numberOfPixels;
                            blueAverage[2] += blue[i + k - margin][j + l] / numberOfPixels;
                            blueAverage[3] += blue[i + k][j + l] / numberOfPixels;
                        }
            
                    //SETTINGS VARIANTS TO 0
                    for (k=0; k < 4; k++)
                    {
                        redVariant[k] = 0;
                        greenVariant[k] = 0;
                        blueVariant[k] = 0;
                    }

                    //CALCULATING VARIANTS
                    for (k=0; k < 3; k++)
                        for (l=0; l < 3; l++)
                        {
                            redVariant[0] += (red[i + k - margin][j + l - margin] - redAverage[0]) * (red[i + k - margin][j + l - margin] - redAverage[0]);
                            redVariant[1] += (red[i + k][j + l - margin] - redAverage[1]) * (red[i + k][j + l - margin] - redAverage[1]);
                            redVariant[2] += (red[i + k - margin][j + l] - redAverage[2]) * (red[i + k - margin][j + l] - redAverage[2]);
                            redVariant[3] += (red[i + k][j + l] - redAverage[3]) * (red[i + k][j + l] - redAverage[3]);
            
                            greenVariant[0] += (green[i + k - margin][j + l - margin] - greenAverage[0]) * (green[i + k - margin][j + l - margin] - greenAverage[0]);
                            greenVariant[1] += (green[i + k][j + l - margin] - greenAverage[1]) * (green[i + k][j + l - margin] - greenAverage[1]);
                            greenVariant[2] += (green[i + k - margin][j + l] - greenAverage[2]) * (green[i + k - margin][j + l] - greenAverage[2]);
                            greenVariant[3] += (green[i + k][j + l] - greenAverage[3]) * (green[i + k][j + l] - greenAverage[3]);
            
                            blueVariant[0] += (blue[i + k - margin][j + l - margin] - blueAverage[0]) * (blue[i + k - margin][j + l - margin] - blueAverage[0]);
                            blueVariant[1] += (blue[i + k][j + l - margin] - blueAverage[1]) * (blue[i + k][j + l - margin] - blueAverage[1]);
                            blueVariant[2] += (blue[i + k - margin][j + l] - blueAverage[2]) * (blue[i + k - margin][j + l] - blueAverage[2]);
                            blueVariant[3] += (blue[i + k][j + l] - blueAverage[3]) * (blue[i + k][j + l] - blueAverage[3]);
                        }
            
                    //FINDING LOWEST VARIATIONS FOR RED, GREEN AND BLUE
                    lowestRed=0;
                    for (k=1; k < 4; k++)
                    {
                                if (redVariant[k] < redVariant[lowestRed])
                                {
                                lowestRed = k;
                                }
                    }

                    lowestGreen=0;
                    for (k=1; k < 4; k++)
                    {
                                if (greenVariant[k] < greenVariant[lowestGreen])
                                {
                                    lowestGreen = k;
                                }
                    }
            
                    lowestBlue=0;
                    for (k=1; k < 4; k++)
                    {
                            if (blueVariant[k] < blueVariant[lowestBlue])
                            {
                                lowestBlue = k;
                            }
                    }
                    
                    //SETTING the i, j Pixel on img
                    img(i,j)->Red  = (int)redAverage[lowestRed];
                    img(i,j)->Green = (int)greenAverage[lowestGreen];
                    img(i,j)->Blue = (int)blueAverage[lowestBlue];
            }
            isFiltered = true;
        } 
}; 

int main()
{
    const char* fileName = "marbles.bmp";
    const char* outputFileName = "marbles_new.bmp";
    Kuwahar* kuwahar = new Kuwahar(fileName, outputFileName);
    kuwahar->Read();

    double start = omp_get_wtime();
    kuwahar->FilterParallel();
    double stop = omp_get_wtime();
    printf("TIME: %.3f \n", stop - start);

    kuwahar->Write();
}

// CZASY dla Size = 5, 
//1 WATEK TIME: 0.461 Size 5
//2 WATKI TIME 0.243 Size 5
//4 WATKI TIME 0.136 Size 5
//8 WATKOW TIME: 0.094  Size 5
 //Wielkosc pliku testowego marbles.bmp: WIDTH: 1419 X HEIGHT:1001