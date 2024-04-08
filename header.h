#pragma once

#include "fce.h"




//fce

int inBounds(int posX, int posY, ImgSize boundary);


void changePixel(int posX, int posY, int red, int green, int blue, double alpha, Pixel* array, int width);


void changePixel2(int posX, int posY, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary);


void drawRectangleFull(int posX, int posY, int rectWidth, int rectHeight, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary);


void drawLine(int startX, int startY, int endX, int endY, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary);


void drawRectangle(int posX, int posY, int rectHeight, int rectWidth, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary);


void drawCircle(int centerX, int centerY, int radius, int red, int green, int blue, double alpha, Pixel* array, ImgSize boundary);


void drawTriangle(int centerX, int centerY, int edge, int red, int green, int blue, int alpha, Pixel* array, ImgSize boundary);


Point rotatePoint(Point p, Point c, int angle);


void drawRotatedRectangle(int angle, int centerX, int centerY, int width, int height, int red, int green, int blue, int alpha, Pixel* array, ImgSize boundary);