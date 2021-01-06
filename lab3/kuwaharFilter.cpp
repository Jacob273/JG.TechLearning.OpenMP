#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <iostream>

#define RGB_MAX_VAL 255

class Pixel 
{
    public:
        char Red;
        char Green;
        char Blue;
};

class Image
{
    public:
        int x;
        int y;
        Pixel *data;
};

class ImageReaderWriter
{
    public:

        Image *Read(const char *filename)
        {
            char buff[16];
            Image *img;
            FILE *fp;
            int c, rgb_comp_color;

            fp = fopen(filename, "rb");
            if (!fp) 
            {
                fprintf(stderr, "Unable to open file '%s'\n", filename);
                exit(1);
            }

            //read image format
            if (!fgets(buff, sizeof(buff), fp)) 
            {
                perror(filename);
                exit(1);
            }

            //Image format check
            if (buff[0] != 'P' || buff[1] != '6') 
            {
                fprintf(stderr, "Invalid image format (must be 'P6')\n");
                exit(1);
            }

            //Allocation
            img = (Image*)malloc(sizeof(Image));
            if (!img) 
            {
                fprintf(stderr, "Unable to allocate memory\n");
                exit(1);
            }

            //Image size information
            if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) 
            {
                fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
                exit(1);
            }

            //Read RGB component
            if (fscanf(fp, "%d", &rgb_comp_color) != 1) 
            {
                fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
                exit(1);
            }

            //Check RGB component depth
            if (rgb_comp_color!= RGB_MAX_VAL) 
            {
                fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
                exit(1);
            }

            while (fgetc(fp) != '\n');

            //Allocation
            img->data = (Pixel*)malloc(img->x * img->y * sizeof(Pixel));

            if (!img) 
            {
                fprintf(stderr, "Unable to allocate memory\n");
                exit(1);
            }

            //Read data from file into img->data
            size_t size = 3 * img->x;
            size_t count = img->y;

            int totalNumberOfElementsReturned = fread(img->data, size, count, fp);
            if (totalNumberOfElementsReturned != img->y) 
            {
                fprintf(stderr, "Error loading image '%s'\n", filename);
                exit(1);
            }

            fclose(fp);
            return img;
        }

        void Write(const char *filename, Image *img)
        {
            FILE *fp;
            fp = fopen(filename, "wb");
            if (!fp) {
                fprintf(stderr, "Unable to open file '%s'\n", filename);
                exit(1);
            }

            //write the header file image format
            fprintf(fp, "P6\n");

            //comments
            fprintf(fp, "# Created by %s\n", "Jakub Gmur");

            //image size
            fprintf(fp, "%d %d\n",img->x,img->y);

            // rgb component depth
            fprintf(fp, "%d\n", RGB_MAX_VAL);

            // pixel data
            fwrite(img->data, 3 * img->x, img->y, fp);
            fclose(fp);
        }

        void ChangeColor(Image *img)
        {
            int i;
            if(img){

                for(i=0;i<img->x*img->y;i++){
                    img->data[i].Red= RGB_MAX_VAL - img->data[i].Red;
                    img->data[i].Green= RGB_MAX_VAL - img->data[i].Green;
                    img->data[i].Blue= RGB_MAX_VAL - img->data[i].Blue;
                }
            }
        }
};

class Kuwahar
{ 
    //TODO:
    
    public: 
        void Filter() 
        { 
        std::cout << "Filtering...";
        } 
}; 

int main(int argc, char **argv){

     std::cout << argv[0];
     ImageReaderWriter *imgReaderWrite = new ImageReaderWriter();
     Image *img = imgReaderWrite->Read(".//sample_5184×3456.ppm");
     imgReaderWrite->ChangeColor(img);
     imgReaderWrite->Write(".//sample_5184×3456_new.ppm", img);

    return 0;
}
