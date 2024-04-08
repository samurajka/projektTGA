#include "header.h"


int main(int argc, char** argv){

    int width = 0;
    int height = 0;
    char outputPath[50] = {};

    int widthCheck = 0;
    int heightCheck = 0;
    int outputCheck = 0;

    if(argc != 7){ // je spravny poceet parametru??
        printf("Nespravny pocet parametru");
        return 1;
    }

    // cteni parametru
    for(int i = 0; i < 3; i++){
        if(strcmp(argv[i*2+1], "--width") == 0){
            width = atoi(argv[i*2+2]);
            widthCheck++;
        }
        if(strcmp(argv[i*2+1], "--height") == 0){
            height = atoi(argv[i*2+2]);
            heightCheck++;
        }
        if(strcmp(argv[i*2+1], "--output") == 0){
            strcpy(outputPath, argv[i*2+2]);
            outputCheck++;
        }
    }



    // error check
    if(widthCheck != 1){
        printf("Spatny parametr width");
        return 1;
    }
    if(heightCheck != 1){
        printf("Spatny parametr height");
        return 1;
    }
    if(outputCheck != 1){
        printf("Spatny parametr output");
        return 1;
    }




    // nastaveni headeru
    TGAHeader head = {};
    head.image_type = 2;
    head.depth = 24;

    head.width[0] = width % 256;
    head.width[1] = width / 256;

    head.height[0] = height % 256;
    head.height[1] = height / 256;

    head.descriptor = 32; // razeni pixelu  32 znamena shora-dolu zleva-doprava


    ImgSize boundary = {};
    boundary.width = width;
    boundary.height = height;


    Pixel* pixelArray = malloc(width * height * sizeof(Pixel));

    FILE* output;


    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            Pixel* pixel = pixelArray + (row * width + col);
            pixel->red = 0;
            pixel->blue = 0;
            pixel->green = 0;
        }
    }

    char buffer[100];
    char* token;

    while(1){
        fgets(buffer, sizeof(buffer), stdin);
        token = strtok(buffer, " ");
        
        if(strcmp(token, "line") == 0){
            int x1 = atoi(strtok(NULL, ","));
            int y1 = atoi(strtok(NULL, ","));
            int x2 = atoi(strtok(NULL, ","));
            int y2 = atoi(strtok(NULL, ","));
            int red = atoi(strtok(NULL, ","));
            int green = atoi(strtok(NULL, ","));
            int blue = atoi(strtok(NULL, ","));
            int alpha = atoi(strtok(NULL, ","));
            drawLine(x1,y1,x2,y2,red,green,blue,alpha,pixelArray,boundary);
        }

        if(strcmp(token, "rectangle") == 0){
            int x = atoi(strtok(NULL, ","));
            int y = atoi(strtok(NULL, ","));
            int widthTemp = atoi(strtok(NULL, ","));
            int heightTemp = atoi(strtok(NULL, ","));
            int red = atoi(strtok(NULL, ","));
            int green = atoi(strtok(NULL, ","));
            int blue = atoi(strtok(NULL, ","));
            int alpha = atoi(strtok(NULL, ","));
            drawRectangle(x,y,heightTemp,widthTemp,red,green,blue,alpha,pixelArray,boundary);
        }

        if(strcmp(token, "circle") == 0){
            int x = atoi(strtok(NULL, ","));
            int y = atoi(strtok(NULL, ","));
            int p = atoi(strtok(NULL, ","));
            int red = atoi(strtok(NULL, ","));
            int green = atoi(strtok(NULL, ","));
            int blue = atoi(strtok(NULL, ","));
            int alpha = atoi(strtok(NULL, ","));
            drawCircle(x,y,p,red,green,blue,alpha,pixelArray,boundary);
        }

        if(strcmp(token, "triangle") == 0){
            int x = atoi(strtok(NULL, ","));
            int y = atoi(strtok(NULL, ","));
            int s = atoi(strtok(NULL, ","));
            int red = atoi(strtok(NULL, ","));
            int green = atoi(strtok(NULL, ","));
            int blue = atoi(strtok(NULL, ","));
            int alpha = atoi(strtok(NULL, ","));
            drawTriangle(x,y,s,red,green,blue,alpha,pixelArray,boundary);
        }

        if(strcmp(token, "rotated-rectangle") == 0){
            int angle = atoi(strtok(NULL, ","));
            int x = atoi(strtok(NULL, ","));
            int y = atoi(strtok(NULL, ","));
            int widthTemp = atoi(strtok(NULL, ","));
            int heightTemp = atoi(strtok(NULL, ","));
            int red = atoi(strtok(NULL, ","));
            int green = atoi(strtok(NULL, ","));
            int blue = atoi(strtok(NULL, ","));
            int alpha = atoi(strtok(NULL, ","));
            drawRotatedRectangle(angle,x,y,widthTemp,heightTemp,red,green,blue,alpha,pixelArray,boundary);
        }

        if(strcmp(token, "save") == 0 || strcmp(token, "save\n") == 0){
            //ulozit obrazek
            output = fopen(outputPath, "wb");
            assert(output);
            fwrite(&head, sizeof(head), 1, output);
            fwrite(pixelArray, sizeof(Pixel), height * width, output);
            fclose(output);
        }

        
        if(strcmp(token, "exit") == 0 || strcmp(token, "exit\n") == 0){
            //ulozit soubor a ukoncit program
            output = fopen(outputPath, "wb");
            assert(output);
            fwrite(&head, sizeof(head), 1, output);
            fwrite(pixelArray, sizeof(Pixel), height * width, output);
            fclose(output);
            break;
        }


        if(strcmp(token, "rectangle-full") == 0){
            int x = atoi(strtok(NULL, ","));
            int y = atoi(strtok(NULL, ","));
            int widthTemp = atoi(strtok(NULL, ","));
            int heightTemp = atoi(strtok(NULL, ","));
            int red = atoi(strtok(NULL, ","));
            int green = atoi(strtok(NULL, ","));
            int blue = atoi(strtok(NULL, ","));
            int alpha = atoi(strtok(NULL, ","));
            drawRectangleFull(x,y,heightTemp,widthTemp,red,green,blue,alpha,pixelArray,boundary);
        }

        if(strcmp(token, "circle-full") == 0){
            int x = atoi(strtok(NULL, ","));
            int y = atoi(strtok(NULL, ","));
            int p = atoi(strtok(NULL, ","));
            int red = atoi(strtok(NULL, ","));
            int green = atoi(strtok(NULL, ","));
            int blue = atoi(strtok(NULL, ","));
            int alpha = atoi(strtok(NULL, ","));
            drawCircleFull(x,y,p,red,green,blue,alpha,pixelArray,boundary);
        }

        
        if(strcmp(token, "triangle-full") == 0){
            int x = atoi(strtok(NULL, ","));
            int y = atoi(strtok(NULL, ","));
            int s = atoi(strtok(NULL, ","));
            int red = atoi(strtok(NULL, ","));
            int green = atoi(strtok(NULL, ","));
            int blue = atoi(strtok(NULL, ","));
            int alpha = atoi(strtok(NULL, ","));
            drawTriangleFull(x,y,s,red,green,blue,alpha,pixelArray,boundary);
        }
        
    
    }





    free(pixelArray);

    return 0;
}

