#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h> // pri prekladu parametr -lm
#include "header.h"

typedef unsigned char byte;

typedef struct {
    byte id_length;
    byte color_map_type;
    byte image_type;
    byte color_map[5];
    byte x_origin[2];
    byte y_origin[2];
    byte width[2];
    byte height[2];
    byte depth;
    byte descriptor;
} TGAHeader;

typedef struct {
    byte blue;
    byte green;
    byte red;
} Pixel;

typedef struct{
    int width;
    int height;
} ImgSize;

typedef struct{
    int x;
    int y;
} Point;



// fce zjisti jestli jsou zadane pozice uvnitr rozmeru obrazku
int inBounds(int posX, int posY, ImgSize boundary){
    if(posX > boundary.width || posY > boundary.height || posX < 1 || posY < 1){
        return 0;
    }
    else{
        return 1;
    }
}

// fce zmeni pixel na pozici posX, posY. bere RGB a Alpha hodnotu. width = parametr width zadany uzivatelem
void changePixel(int posX, int posY, int red, int green, int blue, double alpha, Pixel* array, int width){
    int index = width * (posY - 1) + posX - 1;
    alpha = alpha / 255;
    int r1 = array[index].red;
    int g1 = array[index].green;
    int b1 = array[index].blue;
    array[index].red = (int) (alpha * red + (1.0 - alpha) * r1);
    array[index].green = (int) (alpha * green + (1.0 - alpha) * g1);
    array[index].blue = (int) (alpha * blue + (1.0 - alpha) * b1);
}


// fce changePixel ktera zaroven overi jestli je pixel uvnitr rozmeru obrazku
void changePixel2(int posX, int posY, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary){
    if(inBounds(posX, posY, boundary) == 1){
        changePixel(posX, posY, red, green, blue, alpha, array, boundary.width);
    }
}

// vykresli vyplneni obdelnik
void drawRectangleFull(int posX, int posY, int rectWidth, int rectHeight, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary){
    for(int i = 0; i < rectHeight; i++){
        for(int j = 0; j < rectWidth; j++){
            if(inBounds(posX+j, posY+i, boundary) == 1){
                changePixel(posX+j, posY+i, red, green, blue, alpha, array, boundary.width);
            }
        }
    }
}

// vykresli caru, Bresenhamuv alg
void drawLine(int startX, int startY, int endX, int endY, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary){
    int dx =  abs (endX - startX), sx = startX < endX ? 1 : -1;
    int dy = -abs (endY - startY), sy = startY < endY ? 1 : -1; 
    int err = dx + dy;
    int e2; 
    int x = startX;
    int y = startY;

    if(startX == endX && startY == endY){ //
        changePixel2(startX,startY,red,green,blue,alpha,array,boundary);
    }
    else{
        do{
            if(inBounds(x,y,boundary) == 1){changePixel(x,y,red,green,blue,alpha,array,boundary.width);}
            e2 = 2 * err;
            if (e2 >= dy){
                err += dy; 
                x += sx; 
            }
            if (e2 <= dx){ 
                err += dx; 
                y += sy; 
            }
        } while(!(x == endX && y == endY));
    }

    /*
    do{
        if(inBounds(x,y,boundary) == 1){changePixel(x,y,red,green,blue,alpha,array,boundary.width);}
        e2 = 2 * err;
        if (e2 >= dy){
            err += dy; 
            x += sx; 
        }
        if (e2 <= dx){ 
            err += dx; 
            y += sy; 
        }
    } while(!(x == endX && y == endY));
    */
    
}

// fce vykresli hrany obdelniku
void drawRectangle(int posX, int posY, int rectHeight, int rectWidth, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary){
    drawLine(posX, posY, posX+rectWidth, posY, red,green,blue,alpha, array, boundary); //horni hrana
    drawLine(posX, posY, posX, posY+rectHeight, red,green,blue,alpha, array, boundary); // leva hrana
    drawLine(posX, posY+rectHeight, posX+rectWidth, posY+rectHeight, red,green,blue,alpha, array, boundary); // dolni hrana
    drawLine(posX+rectWidth, posY, posX+rectWidth, posY+rectHeight, red,green,blue,alpha, array, boundary); // prava hrana
    changePixel2(posX+rectWidth, posY+rectHeight,red,green,blue,alpha,array,boundary);
}


// fce vykresli kruh, Mid-point alg
void drawCircle(int centerX, int centerY, int radius, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary){
    int x = radius;
    int y = 0;

    int circleBound = 1 - radius;

    while(x>y){
        y++;

        if(circleBound <= 0){
            circleBound = circleBound + 2*y + 1;
        }
        else{
            x--;
            circleBound = circleBound + 2*y - 2*x + 1;
        }

        if(x<y){
            break;
        }
        
        changePixel2(x+centerX, y+centerY,red,green,blue,alpha,array,boundary);
        changePixel2(-x+centerX, y+centerY,red,green,blue,alpha,array,boundary);
        changePixel2(x+centerX, -y+centerY,red,green,blue,alpha,array,boundary);
        changePixel2(-x+centerX, -y+centerY,red,green,blue,alpha,array,boundary);


        if(x != y){
            changePixel2(y+centerX, x+centerY,red,green,blue,alpha,array,boundary);
            changePixel2(-y+centerX, x+centerY,red,green,blue,alpha,array,boundary);
            changePixel2(y+centerX, -x+centerY,red,green,blue,alpha,array,boundary);
            changePixel2(-y+centerX, -x+centerY,red,green,blue,alpha,array,boundary);
        }
    }

    changePixel2(radius+centerX, centerY,red,green,blue,alpha,array,boundary);
    changePixel2(-radius+centerX, centerY,red,green,blue,alpha,array,boundary);
    changePixel2(centerX, radius+centerY,red,green,blue,alpha,array,boundary);
    changePixel2(centerX, -radius+centerY,red,green,blue,alpha,array,boundary);
    
}


// fce vykresli rovnostranny trojuhelnik
void drawTriangle(int centerX, int centerY, int edge, int red, int green, int blue, int alpha, Pixel* array, ImgSize boundary){
    int cx = centerX;
    int cy = centerY - ((edge * sqrt(3)) / 3);
    
    int ax = centerX - (edge / 2);
    int ay = centerY + ((edge * sqrt(3)) / 6);

    int bx = centerX + (edge / 2);
    int by = centerY + ((edge * sqrt(3)) / 6);

    drawLine(cx, cy, ax, ay, red, green, blue, alpha, array, boundary);
    drawLine(cx, cy, bx, by, red, green, blue, alpha, array, boundary);
    drawLine(bx, by, ax, ay, red, green, blue, alpha, array, boundary);
}


Point rotatePoint(Point p, Point c, int angle){
    Point result;
    p.x = p.x - c.x;
    p.y = p.y - c.y;

    double rad = angle * (3.14 / 180);

    result.x = p.x * cos(rad) - p.y * sin(rad);
    result.y = p.y * cos(rad) + p.x * sin(rad);

    result.x = result.x + c.x;
    result.y = result.y + c.y;

    return result;
}


//fce vykresli obdelnik otoceny o angle stupnu
void drawRotatedRectangle(int angle, int centerX, int centerY, int width, int height, int red, int green, int blue, int alpha, Pixel* array, ImgSize boundary){
    int halfW = (int) floor(width/2);
    int halfH = (int) floor(height/2);
    Point a = {.x = centerX-halfW, .y = centerY-halfH};
    Point b = {.x = centerX+halfW, .y = centerY-halfH};
    Point d = {.x = centerX-halfW, .y = centerY+halfH};
    Point c = {.x = centerX+halfW, .y = centerY+halfH};

    Point center = {.x = centerX, .y = centerY};

    a = rotatePoint(a, center, angle);
    b = rotatePoint(b, center, angle);
    c = rotatePoint(c, center, angle);
    d = rotatePoint(d, center, angle);

    drawLine(a.x, a.y, d.x, d.y, red,green,blue,alpha, array, boundary);
    drawLine(b.x, b.y, c.x, c.y, red,green,blue,alpha, array, boundary);
    drawLine(c.x, c.y, d.x, d.y, red,green,blue,alpha, array, boundary);
    drawLine(b.x, b.y, a.x, a.y, red,green,blue,alpha, array, boundary);
}


//fce vykresli plny kruh
void drawCircleFull(int centerX, int centerY, int radius, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary){
    int x = radius;
    int y = 0;

    int circleBound = 1 - radius;

    while(x>y){
        y++;

        if(circleBound <= 0){
            circleBound = circleBound + 2*y + 1;
        }
        else{
            x--;
            circleBound = circleBound + 2*y - 2*x + 1;
        }

        if(x<y){
            break;
        }
        
        drawLine(x+centerX, y+centerY, -x+centerX, y+centerY, red,green,blue,alpha,array,boundary);
        drawLine(x+centerX, -y+centerY, -x+centerX, -y+centerY, red,green,blue,alpha,array,boundary);

        if(x != y){
            drawLine(y+centerX, x+centerY, -y+centerX, x+centerY, red,green,blue,alpha,array,boundary);
            drawLine(y+centerX, -x+centerY, -y+centerX, -x+centerY, red,green,blue,alpha,array,boundary);
        }
    }

    drawLine(radius+centerX, centerY, -radius+centerX, centerY, red,green,blue,alpha,array,boundary);
    drawLine(centerX, radius+centerY, centerX, -radius+centerY, red,green,blue,alpha,array,boundary);
}


//fce vykresli plny trojuhelnik
void drawTriangleFull(int centerX, int centerY, int edge, int red, int green, int blue, int alpha, Pixel* array, ImgSize boundary){
    int cx = centerX;
    int cy = centerY - ((edge * sqrt(3)) / 3);
    
    int ax = centerX - (edge / 2);
    int ay = centerY + ((edge * sqrt(3)) / 6);

    int bx = centerX + (edge / 2);
    int by = centerY + ((edge * sqrt(3)) / 6);

    int x0 = ax;
    int y0 = ay;
    int x1 = cx;
    int y1 = cy;

    int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2; 

    int x2 = bx;
    int y2 = by;
 
    while(1){
        //printf("x0 %d - y0 %d \n", x0, y0);
        //printf("x2 %d - y2 %d \n", x2, y2);

        drawLine(x0, y0, x2, y2, red,green,blue,alpha,array,boundary);
        
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; x2 -= sx;} 
        if (e2 <= dx) { err += dx; y0 += sy; y2 += sy;} 
    }

}
