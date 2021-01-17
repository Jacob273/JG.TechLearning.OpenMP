#include <math.h>
#include <stdio.h>
#include <thread>
#include <iostream>
#include <chrono>

#pragma warning(disable:4996)

namespace SierpinskiPPM
{

	class Sierpinski
	{

	private:

		//unsigned char rgbImage[800][800][3];
		unsigned char*** rgbImage;
		const int maxLevel = 9;
		int _iYmax;
		int _iXmax;
		int _colors;
		const int MAIN_SIZE = 512;
		const char* filename = "sierpinskiOutput.ppm";
		const char* fileComment = "# ";

	public:

		Sierpinski(int iXmax, int iYmax, int colors)
		{
			_iXmax = iXmax;
			_iYmax = iYmax;
			_colors = colors;
			rgbImage = new unsigned char**[_iXmax];
			for (int i = 0; i < _iXmax;i++)
			{
				rgbImage[i] = new unsigned char*[_iYmax];
				for (int j = 0; j < _iYmax; j++)
				{
					rgbImage[i][j] = new unsigned char[colors];
				}
			}
		}

		void DrawTriangleToArray(int x, int y, int size_a, int level, int r, int g, int b)
		{
			unsigned int myLength = MAIN_SIZE / pow(2, level);

			for (int i = y; i < myLength + y; i++)  //VERTICAL
			{
				rgbImage[i][x][0] = r;
				rgbImage[i][x][1] = g;
				rgbImage[i][x][2] = b;
			}

			for (int i = x; i < myLength + x; i++) //BIAS
			{
				if (x == 0)
				{
					rgbImage[i + y][i][0] = r;
					rgbImage[i + y][i][1] = g;
					rgbImage[i + y][i][2] = b;
				}
				else if (y > x)
				{
					int diffYandX = y - x;
					rgbImage[i + diffYandX][i][0] = r;
					rgbImage[i + diffYandX][i][1] = g;
					rgbImage[i + diffYandX][i][2] = b;
				}
				else if (x > y)
				{
					int diffXandY = x - y;
					rgbImage[i - diffXandY][i][0] = r;
					rgbImage[i - diffXandY][i][1] = g;
					rgbImage[i - diffXandY][i][2] = b;
				}
				else
				{
					rgbImage[i][i][0] = r;
					rgbImage[i][i][1] = g;
					rgbImage[i][i][2] = b;
				}
			}

			for (int i = x; i < myLength + x; i++) // HORIZONTAL
			{
				rgbImage[myLength + y][i][0] = r;
				rgbImage[myLength + y][i][1] = g;
				rgbImage[myLength + y][i][2] = b;
			}

			level++;

			if (level == maxLevel)
			{
				return;
			}
			else
			{
				DrawTriangleToArray(x, y, size_a / 2, level, r, g, b);
				DrawTriangleToArray(x, (size_a / 4) + y, size_a / 2, level, r, g, b);
				DrawTriangleToArray((size_a / 4) + x, (size_a / 4) + y, size_a / 2, level, r, g, b);
			}
		}


		void MakePixelsBlack()
		{
			int blackColor = 255;
			for (int iX = 0; iX < _iXmax; iX++)
			{
				for (int iY = 0; iY < _iYmax; iY++)
				{
					rgbImage[iY][iX][0] = blackColor;
					rgbImage[iY][iX][1] = blackColor;
					rgbImage[iY][iX][2] = blackColor;
				}
			}
		}

	public:
		void Run()
		{
			MakePixelsBlack();

			std::thread* t1 = new std::thread([this]
				{
					int x1 = 0, y1 = 256, size1 = 512, level_1 = 1;
					int r1 = 140;
					int g1 = 200;
					int b1 = 251;
					DrawTriangleToArray(x1, y1, size1, level_1, r1, g1, b1);
				});

			std::thread* t2 = new std::thread([this]
				{
					int x2 = 0, y2 = 0, size2 = 512, level_2 = 1;
					int r2 = 160;
					int g2 = 190;
					int b2 = 230;
					DrawTriangleToArray(x2, y2, size2, level_2, r2, g2, b2);
				});

			std::thread* t3 = new std::thread([this]
				{
					int x3 = 256, y3 = 256, size3 = 512, level_3 = 1;
					int r3 = 150;
					int g3 = 180;
					int b3 = 270;
					this->DrawTriangleToArray(x3, y3, size3, level_3, r3, g3, b3);
				});

			//join - Blokuje wątek wywołujący do momentu przerwania wątku reprezentowanego przez to wystąpienie, 
			t1->join();
			t2->join();
			t3->join();

			SaveRgbImageToFile();
			std::cout << "Program has finished" << std::endl;
		}

		void SaveRgbImageToFile()
		{
			FILE* outputFile = fopen(filename, "wb");
			int maxColorValue = 255;
			fprintf(outputFile, "P6\n %s\n %d\n %d\n %d\n", fileComment, _iXmax, _iYmax, maxColorValue);


			for (int i = 0; i < _iXmax; i++)
			{
				for (int j = 0; j < _iYmax; j++)
				{
					fwrite(rgbImage[i][j], _colors, 1, outputFile);
				}
			}
			fclose(outputFile);
		}
	};
}