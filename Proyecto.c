#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include "qdbmp.h"
#include "qdbmp.c"
#include "stack.c"

float RESX;
float RESY;

int mouseleft_down=0;
int left_down_x=0;
int left_down_y=0;
int mousecurr_x=0;
int mousecurr_y=0;
int tool_type=10;
int polysides=3;
int curvestate=1;
int curve[8];
int dragstate=1;
int dragsquarev[4];
int dragcut=0;
int squaresize=10;
int drawmode=0;
GLubyte currentcolor[3];
GLubyte pixelcolor[3];
GLubyte auxcolor[3];
GLubyte *gdata;



#define ROUND(a) ((int) (a + 0.5))

#define PI 3.141592653589
#define MAXCOORD 2000
#define DRAW_PARTIAL 0
#define DRAW_TOTAL 1
#define PENCIL 0
#define LINE 1
#define CIRCLE 2
#define ELLIPSE 3
#define POLYGON 4
#define CURVE 5
#define ERASER 6
#define SPRAY 7
#define FILL 8
#define DRAG 9


#define COLOR_BLACK 0
#define COLOR_DGRAY 1
#define COLOR_DBLUE 2
#define COLOR_DGREEN 3
#define COLOR_DRED 4
#define COLOR_VIOLET 5
#define COLOR_YELLOW 6
#define COLOR_TURQOISE 7
#define COLOR_BLUE 8
#define COLOR_GREEN 9
#define COLOR_RED 10
#define COLOR_BROWN 11
#define COLOR_ORANGE 12
#define COLOR_PINK 13
#define COLOR_GRAY 14
#define COLOR_WHITE 15

void wait(int miliseconds);
double xvalueto1(double pixel);
double yvalueto1(double pixel);
void setColor(int color);
void setPixel(int xcoord,int ycoord);
void drawLine(int xa, int ya, int xb, int yb);
void drawGUI();
void drawCircle(int xc, int yc, int r);
void ellipsePlotPoints(int xCenter, int yCenter, int x, int y);
void drawEllipse(int xCenter, int yCenter, int Rx, int Ry);
void drawPolygon (int xc, int yc, int x, int y, int sides);
void drawCurve(int x0,int y0,int x1,int y1,int x2,int y2,int x3,int y3);
void setFSquare(float xcoord,float ycoord);
void drawEraser(int xa, int ya, int xb, int yb);
void drawSpray(int x,int y);
int issameColor(GLubyte pixel1[3],GLubyte pixel2[3]);
void drawFillgpu(int x,int y,GLubyte seedColor[3],GLubyte fillColor[3]);
void getPixelRAM(int x,int y,GLubyte data[(int)(3*RESX*RESY)],GLubyte **aux);
void putPixelRAM(int x,int y,GLubyte data[(int)(3*RESX*RESY)],GLubyte *aux);
void drawFill(int x,int y,GLubyte seedColor[3]);
void getFilename(char *filename);
int savetoBMP();
int loadfromBMP();
void display();
void idle();
void mousefunc(int button,int state,int x,int y);
void mousemotion(int x,int y);
void keypressed(unsigned char key,int x,int y);


void wait(int miliseconds){
  clock_t endwait;
  endwait=clock()+miliseconds*(CLOCKS_PER_SEC/1000);
  while(clock()<endwait){}
}


double xvalueto1(double pixel){
  if(drawmode==DRAW_PARTIAL){
    if(pixel<0.0){
      return -1.0;
    }
    if(pixel<((RESX-RESX*.1)/2.0)){
      return ((-1.0/((RESX-RESX*.1)/2))*(((RESX-RESX*.1)/2)-pixel));
    }else if(pixel>((RESX-RESX*.1)/2.0)){
      return ((1.0/((RESX-RESX*.1)/2))*(pixel-((RESX-RESX*.1)/2)));
    }else{
      return 0;
    }
  }else if(drawmode==DRAW_TOTAL){
    if(pixel<0.0){
      return -1.0;
    }
    if(pixel<(RESX/2.0)){
      return ((-1.0/(RESX/2))*((RESX/2)-pixel));
    }else if(pixel>(RESX/2.0)){
      return ((1.0/(RESX/2))*(pixel-(RESX/2)));
    }else{
      return 0;
    }
  } 
}

double yvalueto1(double pixel){
  if(drawmode==DRAW_PARTIAL){
    if(pixel<0.0){
      return -1.0;
    }
    if(pixel<((RESY-RESY*.1)/2.0)){
      return ((-1.0/((RESY-RESY*.1)/2))*(((RESY-RESY*.1)/2)-pixel));
    }else if(pixel>((RESY-RESY*.1)/2.0)){
      return ((1.0/((RESY-RESY*.1)/2))*(pixel-((RESY-RESY*.1)/2)));
    }else{
      return 0;
    }
  }else if(drawmode==DRAW_TOTAL){
    if(pixel<0.0){
      return -1.0;
    }
    if(pixel<(RESY/2.0)){
      return ((-1.0/(RESY/2))*((RESY/2)-pixel));
    }else if(pixel>(RESY/2.0)){
      return ((1.0/(RESY/2))*(pixel-(RESY/2)));
    }else{
      return 0;
    }
  }
} 

void setColor(int color){
  switch(color){
  case COLOR_BLACK:
    currentcolor[0]=0;
    currentcolor[1]=0;
    currentcolor[2]=0;
    break;
  case COLOR_DGRAY:
    currentcolor[0]=128;
    currentcolor[1]=128;
    currentcolor[2]=128;
    break;
  case COLOR_DBLUE:
    currentcolor[0]=33;
    currentcolor[1]=0;
    currentcolor[2]=171;
    break;
  case COLOR_DGREEN:
    currentcolor[0]=184;
    currentcolor[1]=207;
    currentcolor[2]=48;
    break;
  case COLOR_DRED:
    currentcolor[0]=184;
    currentcolor[1]=0;
    currentcolor[2]=0;
    break;
  case COLOR_VIOLET:
    currentcolor[0]=110;
    currentcolor[1]=18;
    currentcolor[2]=128;
    break;
  case COLOR_YELLOW:
    currentcolor[0]=255;
    currentcolor[1]=255;
    currentcolor[2]=0;
    break;
  case COLOR_TURQOISE:
    currentcolor[0]=0;
    currentcolor[1]=255;
    currentcolor[2]=255;
    break;
  case COLOR_BLUE:
    currentcolor[0]=0;
    currentcolor[1]=0;
    currentcolor[2]=255;
    break;
  case COLOR_GREEN:
    currentcolor[0]=0;
    currentcolor[1]=255;
    currentcolor[2]=0;
    break;
  case COLOR_RED:
    currentcolor[0]=255;
    currentcolor[1]=0;
    currentcolor[2]=0;
    break;
  case COLOR_BROWN:
    currentcolor[0]=110;
    currentcolor[1]=43;
    currentcolor[2]=13;
    break;
  case COLOR_ORANGE:
    currentcolor[0]=214;
    currentcolor[1]=179;
    currentcolor[2]=0;
    break;
  case COLOR_PINK:
    currentcolor[0]=255;
    currentcolor[1]=0;
    currentcolor[2]=155;
    break;
  case COLOR_GRAY:
    currentcolor[0]=204;
    currentcolor[1]=204;
    currentcolor[2]=204;
    break;
  case COLOR_WHITE:
    currentcolor[0]=255;
    currentcolor[1]=255;
    currentcolor[2]=255;
    break;
  }
  glColor3ubv(currentcolor);
}


void setPixel(int xcoord,int ycoord){
  glBegin(GL_POINTS);
  glVertex2d(xvalueto1((double)xcoord),yvalueto1((double)ycoord));
  glEnd();
}


void drawLine(int xa, int ya, int xb, int yb){
  int dx,dy,sx,sy,err,e2;
  dx=abs(xb-xa);sx=xa<xb?1:-1;
  dy=-abs(yb-ya);sy=ya<yb?1:-1;
  err=dx+dy,e2;
  while(1){
    setPixel((double)xa,(double)ya);
    if(xa==xb && ya==yb){
      break;
    }
    e2=2*err;
    if(e2>dy){
      err+=dy;
      xa+=sx;
    }
    if(e2<dx){
      err+=dx;
      ya+=sy;
    }
  }
}



void drawGUI(){
  int i=0;
  auxcolor[0]=currentcolor[0];
  auxcolor[1]=currentcolor[1];
  auxcolor[2]=currentcolor[2];

  
  glViewport(0,0,RESX,RESY);
  drawmode=DRAW_TOTAL;
  
  glBegin(GL_QUADS);

  setColor(COLOR_BLACK);
  glVertex2f(xvalueto1(0.0),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(0.0),yvalueto1(RESY));

  setColor(COLOR_DGRAY);
  glVertex2f(xvalueto1(RESX/16),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*2)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*2)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16),yvalueto1(RESY));
  
  setColor(COLOR_DBLUE);
  glVertex2f(xvalueto1(RESX/16*2),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*3)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*3)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*2),yvalueto1(RESY));
  
  setColor(COLOR_DGREEN);
  glVertex2f(xvalueto1(RESX/16*3),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*4)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*4)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*3),yvalueto1(RESY));

  setColor(COLOR_DRED);
  glVertex2f(xvalueto1(RESX/16*4),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*5)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*5)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*4),yvalueto1(RESY));
  
  setColor(COLOR_VIOLET);
  glVertex2f(xvalueto1(RESX/16*5),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*6)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*6)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*5),yvalueto1(RESY));

  setColor(COLOR_YELLOW);
  glVertex2f(xvalueto1(RESX/16*6),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*7)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*7)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*6),yvalueto1(RESY));

  setColor(COLOR_TURQOISE);
  glVertex2f(xvalueto1(RESX/16*7),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*8)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*8)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*7),yvalueto1(RESY));
  
  setColor(COLOR_BLUE);
  glVertex2f(xvalueto1(RESX/16*8),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*9)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*9)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*8),yvalueto1(RESY));

  setColor(COLOR_GREEN);
  glVertex2f(xvalueto1(RESX/16*9),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*10)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*10)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*9),yvalueto1(RESY));

  setColor(COLOR_RED);
  glVertex2f(xvalueto1(RESX/16*10),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*11)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*11)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*10),yvalueto1(RESY));

  setColor(COLOR_BROWN);
  glVertex2f(xvalueto1(RESX/16*11),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*12)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*12)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*11),yvalueto1(RESY));

  setColor(COLOR_ORANGE);
  glVertex2f(xvalueto1(RESX/16*12),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*13)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*13)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*12),yvalueto1(RESY));

  setColor(COLOR_PINK);
  glVertex2f(xvalueto1(RESX/16*13),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*14)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*14)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*13),yvalueto1(RESY));

  setColor(COLOR_GRAY);
  glVertex2f(xvalueto1(RESX/16*14),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*15)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX/16*15)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*14),yvalueto1(RESY));

  setColor(COLOR_WHITE);
  glVertex2f(xvalueto1(RESX/16*15),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX)-1),yvalueto1(RESY-RESY*.08));
  glVertex2f(xvalueto1((RESX)-1),yvalueto1(RESY));
  glVertex2f(xvalueto1(RESX/16*15),yvalueto1(RESY));

  glEnd();

  for(i=12;i>=1;i--){
    glBegin(GL_LINE_STRIP);
    
    setColor(COLOR_BLACK);
    glVertex2f(xvalueto1(RESX-RESX*.08),yvalueto1(RESY/15*i));
    glVertex2f(xvalueto1(RESX-RESX*.02),yvalueto1(RESY/15*i));
    glVertex2f(xvalueto1(RESX-RESX*.02),yvalueto1(RESY/15*(i+1)));
    glVertex2f(xvalueto1(RESX-RESX*.08),yvalueto1(RESY/15*(i+1)));
    glVertex2f(xvalueto1(RESX-RESX*.08),yvalueto1(RESY/15*i));    
    glEnd();
    
  }
    
    
  
  glBegin(GL_LINES);
  glVertex2f(xvalueto1(RESX-RESX*.08),yvalueto1(RESY/15*12));
  glVertex2f(xvalueto1(RESX-RESX*.02),yvalueto1(RESY/15*13));
  glEnd();

  drawCircle(RESX-RESX*.05,RESY/15*11.5,15);

  drawEllipse(RESX-RESX*.05,RESY/15*10.5,20,10);

  glBegin(GL_QUADS);
  setColor(COLOR_WHITE);
  glVertex2f(xvalueto1(RESX-RESX*.021),yvalueto1(RESY/15*9.05));
  glVertex2f(xvalueto1(RESX-RESX*.079),yvalueto1(RESY/15*9.05));
  glVertex2f(xvalueto1(RESX-RESX*.079),yvalueto1(RESY/15*9.95));
  glVertex2f(xvalueto1(RESX-RESX*.021),yvalueto1(RESY/15*9.95));
  glEnd();
  setColor(COLOR_BLACK);
  drawPolygon(RESX-RESX*.05,RESY/15*9.5,RESX-RESX*.05,RESY/15*9.9,polysides);

  drawCurve(RESX-RESX*.07,RESY/15*8.1,RESX-RESX*.07,RESY/15*8.4,RESX-RESX*.04,RESY/15*8.1,RESX-RESX*.03,RESY/15*8.9);

  glBegin(GL_QUADS);
  setColor(COLOR_WHITE);
  glVertex2f(xvalueto1(RESX-RESX*.021),yvalueto1(RESY/15*7.05));
  glVertex2f(xvalueto1(RESX-RESX*.079),yvalueto1(RESY/15*7.05));
  glVertex2f(xvalueto1(RESX-RESX*.079),yvalueto1(RESY/15*7.95));
  glVertex2f(xvalueto1(RESX-RESX*.021),yvalueto1(RESY/15*7.95));
  glEnd();
  setColor(COLOR_BLACK);
  for(i=0;i<500;i++){
    setPixel(rand()%25+RESX-RESX*.065,rand()%25+RESY/15*7.2);
  }


  glBegin(GL_QUADS);
  setColor(COLOR_DBLUE);
  glVertex2f(xvalueto1(RESX-RESX*.07),yvalueto1(RESY/15*6.2));
  glVertex2f(xvalueto1(RESX-RESX*.055),yvalueto1(RESY/15*6.2));
  glVertex2f(xvalueto1(RESX-RESX*.055),yvalueto1(RESY/15*6.8));
  glVertex2f(xvalueto1(RESX-RESX*.07),yvalueto1(RESY/15*6.8));

  setColor(COLOR_DRED);
  glVertex2f(xvalueto1(RESX-RESX*.055),yvalueto1(RESY/15*6.2));
  glVertex2f(xvalueto1(RESX-RESX*.03),yvalueto1(RESY/15*6.2));
  glVertex2f(xvalueto1(RESX-RESX*.03),yvalueto1(RESY/15*6.8));
  glVertex2f(xvalueto1(RESX-RESX*.055),yvalueto1(RESY/15*6.8));
  glEnd();

  
  glBegin(GL_QUADS);
  setColor(COLOR_BLACK);
  glVertex2f(xvalueto1(RESX-RESX*.07),yvalueto1(RESY/15*5.2));
  glVertex2f(xvalueto1(RESX-RESX*.03),yvalueto1(RESY/15*5.2));
  glVertex2f(xvalueto1(RESX-RESX*.03),yvalueto1(RESY/15*5.8));
  glVertex2f(xvalueto1(RESX-RESX*.07),yvalueto1(RESY/15*5.8));
  glEnd();

  setColor(COLOR_BLACK);
  drawCircle(RESX-RESX*.06,RESY/15*4.25,3);
  drawCircle(RESX-RESX*.04,RESY/15*4.25,3);
  drawLine(RESX-RESX*.055,(RESY/15*4.25)+3,RESX-RESX*.04,RESY/15*4.7);
  drawLine(RESX-RESX*.045,(RESY/15*4.25)+3,RESX-RESX*.06,RESY/15*4.7);

  drawCurve(RESX-RESX*.07,RESY/15*3.1,RESX-RESX*.07,RESY/15*3.4,RESX-RESX*.04,RESY/15*3.1,RESX-RESX*.04,RESY/15*3.4);
  drawLine(RESX-RESX*.04,RESY/15*3.4,RESX-RESX*.07,RESY/15*3.7);
  drawLine(RESX-RESX*.07,RESY/15*3.7,RESX-RESX*.04,RESY/15*3.7);

  
  glBegin(GL_QUADS);
  setColor(COLOR_BLACK);
  glVertex2f(xvalueto1(RESX-RESX*.075),yvalueto1(RESY/15*2.2));
  glVertex2f(xvalueto1(RESX-RESX*.045),yvalueto1(RESY/15*2.2));
  glVertex2f(xvalueto1(RESX-RESX*.045),yvalueto1(RESY/15*2.8));
  glVertex2f(xvalueto1(RESX-RESX*.075),yvalueto1(RESY/15*2.8));

  setColor(COLOR_DGRAY);
  glVertex2f(xvalueto1(RESX-RESX*.07),yvalueto1(RESY/15*2.25));
  glVertex2f(xvalueto1(RESX-RESX*.05),yvalueto1(RESY/15*2.25));
  glVertex2f(xvalueto1(RESX-RESX*.05),yvalueto1(RESY/15*2.4));
  glVertex2f(xvalueto1(RESX-RESX*.07),yvalueto1(RESY/15*2.4));

  setColor(COLOR_GRAY);
  glVertex2f(xvalueto1(RESX-RESX*.065),yvalueto1(RESY/15*2.65));
  glVertex2f(xvalueto1(RESX-RESX*.055),yvalueto1(RESY/15*2.65));
  glVertex2f(xvalueto1(RESX-RESX*.055),yvalueto1(RESY/15*2.75));
  glVertex2f(xvalueto1(RESX-RESX*.065),yvalueto1(RESY/15*2.75));
  
  glEnd();
  
  setColor(COLOR_BLACK);
  drawLine(RESX-RESX*.03,RESY/15*2.2,RESX-RESX*.03,RESY/15*2.8);
  drawLine(RESX-RESX*.03,RESY/15*2.2,RESX-RESX*.035,RESY/15*2.3);
  drawLine(RESX-RESX*.03,RESY/15*2.2,RESX-RESX*.024,RESY/15*2.3);


  glBegin(GL_QUADS);
  setColor(COLOR_BLACK);
  glVertex2f(xvalueto1(RESX-RESX*.075),yvalueto1(RESY/15*1.2));
  glVertex2f(xvalueto1(RESX-RESX*.045),yvalueto1(RESY/15*1.2));
  glVertex2f(xvalueto1(RESX-RESX*.045),yvalueto1(RESY/15*1.8));
  glVertex2f(xvalueto1(RESX-RESX*.075),yvalueto1(RESY/15*1.8));

  setColor(COLOR_DGRAY);
  glVertex2f(xvalueto1(RESX-RESX*.07),yvalueto1(RESY/15*1.25));
  glVertex2f(xvalueto1(RESX-RESX*.05),yvalueto1(RESY/15*1.25));
  glVertex2f(xvalueto1(RESX-RESX*.05),yvalueto1(RESY/15*1.4));
  glVertex2f(xvalueto1(RESX-RESX*.07),yvalueto1(RESY/15*1.4));

  setColor(COLOR_GRAY);
  glVertex2f(xvalueto1(RESX-RESX*.065),yvalueto1(RESY/15*1.65));
  glVertex2f(xvalueto1(RESX-RESX*.055),yvalueto1(RESY/15*1.65));
  glVertex2f(xvalueto1(RESX-RESX*.055),yvalueto1(RESY/15*1.75));
  glVertex2f(xvalueto1(RESX-RESX*.065),yvalueto1(RESY/15*1.75));
  
  glEnd();
  
  setColor(COLOR_BLACK);
  drawLine(RESX-RESX*.03,RESY/15*1.2,RESX-RESX*.03,RESY/15*1.8);
  drawLine(RESX-RESX*.03,RESY/15*1.8,RESX-RESX*.035,RESY/15*1.7);
  drawLine(RESX-RESX*.03,RESY/15*1.8,RESX-RESX*.024,RESY/15*1.7);


  
  glViewport(0,0,RESX-RESX*.1,RESY-RESY*.1);
  drawmode=DRAW_PARTIAL;
  glColor3ubv(auxcolor);
}  




void drawCircle(int xc, int yc, int r){
  int f=1-r;
  int ddF_x=1;
  int ddF_y=-2*r;
  int x=0;
  int y=r;  
  setPixel(xc,yc+r);
  setPixel(xc,yc-r);
  setPixel(xc+r,yc);
  setPixel(xc-r,yc);
  while(x<y){
    if(f>=0){
      y--;
      ddF_y+=2;
      f+=ddF_y;
    }
    x++;
    ddF_x+=2;
    f+=ddF_x;
    setPixel(xc+x,yc+y);
    setPixel(xc-x,yc+y);
    setPixel(xc+x,yc-y);
    setPixel(xc-x,yc-y);
    setPixel(xc+y,yc+x);
    setPixel(xc-y,yc+x);
    setPixel(xc+y,yc-x);
    setPixel(xc-y,yc-x);
  }
  glFlush();
}

void ellipsePlotPoints(int xCenter, int yCenter, int x, int y){
  setPixel(xCenter+x,yCenter+y);
  setPixel(xCenter-x,yCenter+y);
  setPixel(xCenter+x,yCenter-y);
  setPixel(xCenter-x,yCenter-y);
}

void drawEllipse(int xCenter, int yCenter, int Rx, int Ry){
  long long Rx2=Rx*Rx;
  long long Ry2=Ry*Ry;
  long long twoRx2=2*Rx2;
  long long twoRy2=2*Ry2;
  long long p;
  long long x=0;
  long long y=Ry;
  long long px=0;
  long long py=twoRx2*y;
  ellipsePlotPoints(xCenter,yCenter,x,y);

  p=ROUND(Ry2-(Rx2*Ry)+(0.25)*Rx2);
  while(px<py){
    x++;
    px+=twoRy2;
    if(p<0){
      p+=Ry2+px;
    }else{
      y--;
      py-=twoRx2;
      p+=Ry2+px-py;
    }
    ellipsePlotPoints((int)xCenter,(int)yCenter,(int)x,(int)y);
  }

  p=ROUND(Ry2*(x+0.5)*(x+0.5)+Rx2*(y-1)*(y-1)-Rx2*Ry2);
  while(y>0){
    y--;
    py-=twoRx2;
    if(p>0){
      p+=Rx2-py;
    }else{
      x++;
      px+=twoRy2;
      p+=Rx2-py+px;
    }
    ellipsePlotPoints((int)xCenter,(int)yCenter,(int)x,(int)y);
  }
}

void drawPolygon (int xc, int yc, int x, int y, int sides) {
  int lastx, lasty;
  double r=sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc));
  double a=atan2(y-yc,x-xc);
  int i;
  for (i=1;i<=sides;i++){
    lastx=x;lasty=y;
    a=a+PI*2/sides;
    x=ROUND((double)xc+(double)r*cos(a));
    y=ROUND((double)yc+(double)r*sin(a));
    drawLine(lastx,lasty,x,y);
  }
}

void drawCurve(int x0,int y0,int x1,int y1,int x2,int y2,int x3,int y3){
  double time;
  int x,y;
  for(time=0;time<=1.0;time+=0.001){
    x=pow(1-time,3)*x0+
      3*time*pow(1-time,2)*x1+
      3*pow(time,2)*(1-time)*x2+
      pow(time,3)*x3;

    y=pow(1-time,3)*y0+
      3*time*pow(1-time,2)*y1+
      3*pow(time,2)*(1-time)*y2+
      pow(time,3)*y3;
    
    setPixel(x,y);
  }
}

void setFSquare(float xcoord,float ycoord){
  glBegin(GL_QUADS);
  
  glVertex2f(xvalueto1(xcoord),yvalueto1(ycoord));
  glVertex2f(xvalueto1(xcoord),yvalueto1(ycoord+squaresize));
  glVertex2f(xvalueto1(xcoord+squaresize),yvalueto1(ycoord+squaresize));
  glVertex2f(xvalueto1(xcoord+squaresize),yvalueto1(ycoord));
  
  glEnd();
}

void drawEraser(int xa, int ya, int xb, int yb){
  int dx,dy,sx,sy,err,e2;
  dx=abs(xb-xa);sx=xa<xb?1:-1;
  dy=-abs(yb-ya);sy=ya<yb?1:-1;
  err=dx+dy,e2;
  while(1){
    setFSquare((float)xa,(float)ya);
    if(xa==xb&&ya==yb){
      break;
    }
    e2=2*err;
    if(e2>dy){
      err+=dy;
      xa+=sx;
    }
    if(e2<dx){
      err+=dx;
      ya+=sy;
    }
  }
}

void drawSpray(int x,int y){
  setPixel(rand()%squaresize+x,rand()%squaresize+y);
  glutSwapBuffers();
  wait(10); //miliseconds
}

int issameColor(GLubyte pixel1[3],GLubyte pixel2[3]){
  if(pixel1[0]==pixel2[0]&&pixel1[1]==pixel2[1]&&pixel1[2]==pixel2[2])
    return 1;
  return 0;
}

void getPixelRAM(int x,int y,GLubyte data[(int)(3*RESX*RESY)],GLubyte **aux){
  *aux=data+(3*(x+y*(int)RESX));
}

void putPixelRAM(int x,int y,GLubyte data[(int)(3*RESX*RESY)],GLubyte *aux){
  data[(int)(3*(x+y*RESX))]=*aux;
  data[(int)(3*(x+y*RESX)+1)]=*(aux+1);
  data[(int)(3*(x+y*RESX)+2)]=*(aux+2);
}


void drawFill(int x,int y,GLubyte seedColor[3]){
  if(issameColor(seedColor,currentcolor)){
    return;
  }
  GLubyte data[(int)(3*((RESX+1)*(RESY+1)))];
  GLubyte *aux=NULL;
  if(!data)
    return;
  glReadPixels(0,0,RESX,RESY,GL_RGB,GL_UNSIGNED_BYTE,data);
  stackEmpty();
  top=stackPush(x,y);
  while(!stackIsEmpty()){
    x=top->x;
    y=top->y;
    stackPop();
    getPixelRAM(x,y,data,&aux);
    if(issameColor(aux,seedColor)&&x>=0&&x<=(RESX-RESX*.1)&&y>=0&&y<=(RESY-RESY*.1)){
      putPixelRAM(x,y,data,currentcolor);
      top=stackPush(x,y+1);
      top=stackPush(x,y-1);
      top=stackPush(x-1,y);
      top=stackPush(x+1,y);
    }
  }
  glWindowPos2i(0,0);
  glDrawPixels(RESX,RESY,GL_RGB,GL_UNSIGNED_BYTE,data);
  glutSwapBuffers();
  return;
}




void getFilename(char *filename){
  fgets(filename,99,stdin);
  strcpy(filename+(strlen(filename)-1),"\0");
  if(strcmp(filename+(strlen(filename)-4),".bmp"))
    strcat(filename,".bmp");
}

int savetoBMP(){
  int x,y;
  unsigned int z=0;
  BMP *bmp;
  GLubyte *data=malloc(sizeof(GLubyte)*3*RESX*RESY);
  char filename[100];
  printf("\n  Save to file: ");
  getFilename(filename);
  bmp=BMP_Create(RESX,RESY,24);
  BMP_CHECK_ERROR(stdout,-1);
  glReadPixels(0,0,RESX,RESY,GL_RGB,GL_UNSIGNED_BYTE,data);
  printf("  Saving");
  for(y=0;y<RESY;y++){
    for(x=0;x<RESX;x++){
      if(x<=(RESX-RESX*.1)&&y<=(RESY-RESY*.1))
	BMP_SetPixelRGB(bmp,x,RESY-y,*(data+z),*(data+z+1),*(data+z+2));
      else
	BMP_SetPixelRGB(bmp,x,RESY-y,255,255,255);
      z+=3;
    }
  }
  BMP_WriteFile(bmp,filename);
  BMP_CHECK_ERROR(stdout,-1);
  wait(300);
  fflush(stdout);
  printf(".");
  wait(300);
  fflush(stdout);
  printf(".");
  wait(300);
  fflush(stdout);
  printf(".");
  wait(300);
  fflush(stdout);
  printf(" %s wrote successfully.\n\n",filename);
  fflush(stdout);
  BMP_Free(bmp);
  free(data);
}

int loadfromBMP(){
  int x,y;
  unsigned int z=0;
  BMP *bmp;
  GLubyte *data=malloc(sizeof(GLubyte)*3*RESX*RESY);
  char filename[100];
  printf("\n  Load from file: ");
  getFilename(filename);
  bmp=BMP_ReadFile(filename);
  BMP_CHECK_ERROR(stdout,-1);
  printf("  Loading");
  for(y=0;y<BMP_GetHeight(bmp);y++){
    for(x=0;x<BMP_GetWidth(bmp);x++){
      BMP_GetPixelRGB(bmp,x,RESY-y,data+z,data+z+1,data+z+2);
      z+=3;
    }
  }
  wait(300);
  fflush(stdout);
  printf(".");
  wait(300);
  fflush(stdout);
  printf(".");
  wait(300);
  fflush(stdout);
  printf(".");
  wait(300);
  fflush(stdout);
  glDrawPixels(RESX,RESY,GL_RGB,GL_UNSIGNED_BYTE,data);
  printf(" %s loaded successfully.\n\n",filename);
  BMP_Free(bmp);
  free(data);
  drawGUI();
  glutSwapBuffers();
}



void display(){ 
  
  drawGUI();
  setColor(COLOR_BLACK);
  glutSwapBuffers();

}




void idle(){
  if(mouseleft_down){
    switch(tool_type){     
    case SPRAY:
      drawSpray(mousecurr_x-squaresize/2,mousecurr_y-squaresize/2);
      break;
    }
  }
}



void checkGUIButtons(int x,int y){
  printf("Checando botones\n");
  if(x>=(RESX-RESX*.08)&&x<=(RESX-RESX*.02)){
    if(y<=RESY/15*13&&y>=RESY/15*12){
      tool_type=LINE;
      printf("#Line Selected");
    }else if(y<=RESY/15*12&&y>=RESY/15*11){
      tool_type=CIRCLE;
      printf("#Circle Selected");
    }else if(y<=RESY/15*11&&y>=RESY/15*10){
      tool_type=ELLIPSE;
      printf("#Ellipse Selected");
    }else if(y<=RESY/15*10&&y>=RESY/15*9){
      tool_type=POLYGON;
      printf("#Polygon Selected");
    }else if(y<=RESY/15*9&&y>=RESY/15*8){
      tool_type=CURVE;
      printf("#Curve Selected");
    }else if(y<=RESY/15*8&&y>=RESY/15*7){
      tool_type=SPRAY;
      printf("#Spray Selected");
    }else if(y<=RESY/7*12&&y>=RESY/15*6){
      glColor3f(1.0,1.0,1.0);
      tool_type=ERASER;
      printf("#Eraser Selected");
    }else if(y<=RESY/15*6&&y>=RESY/15*5){
      tool_type=FILL;
      printf("#Fill Selected");
    }else if(y<=RESY/15*5&&y>=RESY/15*4){
      tool_type=DRAG;
      printf("#Drag Selected");
    }else if(y<=RESY/15*4&&y>=RESY/15*3){
      tool_type=PENCIL;
      printf("#Pencil Selected");
    }else if(y<=RESY/15*3&&y>=RESY/15*2){
      savetoBMP();
    }else if(y<=RESY/15*2&&y>=RESY/15*1){
      loadfromBMP();
    }
  }if(y>=RESY-RESY*.08){
    if(x<=(RESX/16*1-1)){
      setColor(COLOR_BLACK);
    }else if(x>=RESX/16*1&&x<=(RESX/16*2-1)){
      setColor(COLOR_DGRAY);
    }else if(x>=RESX/16*2&&x<=(RESX/16*3-1)){
      setColor(COLOR_DBLUE);
    }else if(x>=RESX/16*3&&x<=(RESX/16*4-1)){
      setColor(COLOR_DGREEN);
    }else if(x>=RESX/16*4&&x<=(RESX/16*5-1)){
      setColor(COLOR_DRED);
    }else if(x>=RESX/16*5&&x<=(RESX/16*6-1)){
      setColor(COLOR_VIOLET);
    }else if(x>=RESX/16*6&&x<=(RESX/16*7-1)){
      setColor(COLOR_YELLOW);
    }else if(x>=RESX/16*7&&x<=(RESX/16*8-1)){
      setColor(COLOR_TURQOISE);
    }else if(x>=RESX/16*8&&x<=(RESX/16*9-1)){
      setColor(COLOR_BLUE);
    }else if(x>=RESX/16*9&&x<=(RESX/16*10-1)){
      setColor(COLOR_GREEN);
    }else if(x>=RESX/16*10&&x<=(RESX/16*11-1)){
      setColor(COLOR_RED);
    }else if(x>=RESX/16*11&&x<=(RESX/16*12-1)){
      setColor(COLOR_BROWN);
    }else if(x>=RESX/16*12&&x<=(RESX/16*13-1)){
      setColor(COLOR_ORANGE);
    }else if(x>=RESX/16*13&&x<=(RESX/16*14-1)){
      setColor(COLOR_PINK);
    }else if(x>=RESX/16*14&&x<=(RESX/16*15-1)){
      setColor(COLOR_GRAY);
    }else if(x>=RESX/16*15){
      setColor(COLOR_WHITE);
    }
  }
}



void mousefunc(int button,int state,int x,int y){
  if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN){
    left_down_x=x;
    left_down_y=(int)(RESY-y);
    checkGUIButtons(left_down_x,left_down_y);
    mousecurr_x=left_down_x;
    mousecurr_y=left_down_y;
    printf("Caught click at %d,%d!\n",left_down_x,left_down_y);
    mouseleft_down=(state==GLUT_DOWN)?1:0;
    switch(tool_type){
    case PENCIL:
      setPixel(left_down_x,left_down_y);
      glutSwapBuffers();
      break;
    case LINE:
    case CIRCLE:
    case ELLIPSE:
    case POLYGON:break;
    case CURVE:
      if(curvestate==1){
	curve[0]=left_down_x;
	curve[1]=left_down_y;
      }
      break;
    case ERASER:
      setFSquare(left_down_x-squaresize/2,left_down_y-squaresize/2);
      glutSwapBuffers();
      break;
    case SPRAY:
      drawSpray(left_down_x-squaresize/2,left_down_y-squaresize/2);
      break;
    case FILL:
      glReadPixels(left_down_x,left_down_y,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixelcolor);
      drawFill(left_down_x,left_down_y,pixelcolor);
      break;
    case DRAG:
      if(dragstate==2){
	if(left_down_x>=dragsquarev[0]&&left_down_x<=dragsquarev[2]&&
	   left_down_y>=dragsquarev[1]&&left_down_y<=dragsquarev[3]){
	  gdata=(GLubyte *)malloc(sizeof(GLubyte)*3*(dragsquarev[2]-dragsquarev[0]+1)*(dragsquarev[3]-dragsquarev[1]+1));
	  glReadPixels(dragsquarev[0],dragsquarev[1],dragsquarev[2]-dragsquarev[0],dragsquarev[3]-dragsquarev[1],GL_RGB,GL_UNSIGNED_BYTE,gdata);
	  glutSwapBuffers();
	  dragstate=3;
	}else{
	  dragstate=1;
	}
      }
      break;
    }
  }
  if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP){
    mouseleft_down=0;
    switch(tool_type){
    case PENCIL:
      break;
    case LINE:
      glDrawBuffer(GL_BACK);
      printf("Released at %d,%d\n",x,(int)(RESY-y));
      drawLine(left_down_x,left_down_y,mousecurr_x,mousecurr_y);
      glutSwapBuffers();
      break;
    case CIRCLE:
      glDrawBuffer(GL_BACK);
      printf("Released at %d,%d\n",x,(int)(RESY-y));
      drawCircle(left_down_x,left_down_y,(sqrt(abs(mousecurr_x-left_down_x)*abs(mousecurr_x-left_down_x)+abs(mousecurr_y-left_down_y)*abs(mousecurr_y-left_down_y))));
      glutSwapBuffers();
      break;
    case ELLIPSE:
      glDrawBuffer(GL_BACK);
      printf("Released at %d,%d\n",x,(int)(RESY-y));
      drawEllipse(left_down_x,left_down_y,abs(mousecurr_x-left_down_x),abs(mousecurr_y-left_down_y));
      glutSwapBuffers();
      break;
    case POLYGON:
      glDrawBuffer(GL_BACK);
      printf("Released at %d,%d\n",x,(int)(RESY-y));
      drawPolygon(left_down_x,left_down_y,mousecurr_x,mousecurr_y,polysides);
      glutSwapBuffers();
      break;
    case CURVE:
      if(curvestate==1){
	glDrawBuffer(GL_FRONT);
	printf("Released at %d,%d\n",x,(int)(RESY-y));
	curve[6]=mousecurr_x;
	curve[7]=mousecurr_y;
	drawLine(left_down_x,left_down_y,mousecurr_x,mousecurr_y);
	curvestate++;
	glFlush();
      }else if(curvestate==2){
	glDrawBuffer(GL_FRONT);
	printf("Released at %d,%d\n",x,(int)(RESY-y));
	curve[2]=mousecurr_x;
	curve[3]=mousecurr_y;
	drawCurve(curve[0],curve[1],curve[2],curve[3],curve[2],curve[3],curve[6],curve[7]);
	curvestate++;
	glFlush();
      }else{
	glDrawBuffer(GL_BACK);
	printf("Released at %d,%d\n",x,(int)(RESY-y));
	curve[4]=mousecurr_x;
	curve[5]=mousecurr_y;
	drawCurve(curve[0],curve[1],curve[2],curve[3],curve[4],curve[5],curve[6],curve[7]);
	curvestate=1;
	glutSwapBuffers();
      }
      break;
    case ERASER:
      break;
    case SPRAY:
      break;
    case DRAG:
      if(dragstate==1){
	if(left_down_x<=mousecurr_x){
	  dragsquarev[0]=left_down_x;
	  dragsquarev[2]=mousecurr_x;
	}else{
	  dragsquarev[0]=mousecurr_x;
	  dragsquarev[2]=left_down_x;
	}
	if(left_down_y<=mousecurr_y){
	  dragsquarev[1]=left_down_y;
	  dragsquarev[3]=mousecurr_y;
	}else{
	  dragsquarev[1]=mousecurr_y;
	  dragsquarev[3]=left_down_y;
	}
	dragstate=2;
      }
      if(dragstate==3){
	dragstate=1;
      }
      if(dragstate==4){
	glDrawBuffer(GL_BACK);
	glWindowPos2i(mousecurr_x-((dragsquarev[2]-dragsquarev[0])/2),mousecurr_y-((dragsquarev[3]-dragsquarev[1])/2));
	glDrawPixels(dragsquarev[2]-dragsquarev[0],dragsquarev[3]-dragsquarev[1],GL_RGB,GL_UNSIGNED_BYTE,gdata);
	glutSwapBuffers();
	dragstate=1;
	dragcut=0;
	free(gdata);
      }      
      break;
    }
  }
}

    

void mousemotion(int x,int y){
  mousecurr_x=x;
  mousecurr_y=(int)(RESY-y);
  if(mouseleft_down){
    switch(tool_type){
    case PENCIL:
      drawLine(left_down_x,left_down_y,mousecurr_x,mousecurr_y);
      left_down_x=mousecurr_x;
      left_down_y=mousecurr_y;
      glutSwapBuffers();
      break;
    case LINE:
      glDrawBuffer(GL_FRONT);
      drawLine(left_down_x,left_down_y,mousecurr_x,mousecurr_y);
      glutSwapBuffers();
      break;
    case CIRCLE:
      glDrawBuffer(GL_FRONT);
      drawCircle(left_down_x,left_down_y,(sqrt(abs(mousecurr_x-left_down_x)*abs(mousecurr_x-left_down_x)+abs(mousecurr_y-left_down_y)*abs(mousecurr_y-left_down_y))));
      glutSwapBuffers();
      break;
    case ELLIPSE:
      glDrawBuffer(GL_FRONT);
      drawEllipse(left_down_x,left_down_y,abs(mousecurr_x-left_down_x),abs(mousecurr_y-left_down_y));
      glutSwapBuffers();
      break;
    case POLYGON:
      glDrawBuffer(GL_FRONT);
      drawPolygon(left_down_x,left_down_y,mousecurr_x,mousecurr_y,polysides);
      glutSwapBuffers();
      break;
    case CURVE:
      if(curvestate==1){
	glDrawBuffer(GL_FRONT);
	drawLine(curve[0],curve[1],mousecurr_x,mousecurr_y);
	glutSwapBuffers();
      }else if(curvestate==2){
	glDrawBuffer(GL_FRONT);
	drawCurve(curve[0],curve[1],mousecurr_x,mousecurr_y,mousecurr_x,mousecurr_y,curve[6],curve[7]);
	glutSwapBuffers();
      }else{
	glDrawBuffer(GL_FRONT);
	drawCurve(curve[0],curve[1],curve[2],curve[3],mousecurr_x,mousecurr_y,curve[6],curve[7]);
	glutSwapBuffers();
      }
      break;
    case ERASER:
      drawEraser(left_down_x-squaresize/2,left_down_y-squaresize/2,mousecurr_x,mousecurr_y);
      left_down_x=mousecurr_x;
      left_down_y=mousecurr_y;
      glutSwapBuffers();
      break;
    case SPRAY:
      drawSpray(mousecurr_x,mousecurr_y);
      break;
    case FILL:break;
    case DRAG:
      if(dragstate==1){
	glDrawBuffer(GL_FRONT);
	drawLine(left_down_x,left_down_y,mousecurr_x,left_down_y);
	drawLine(mousecurr_x,left_down_y,mousecurr_x,mousecurr_y);
	drawLine(mousecurr_x,mousecurr_y,left_down_x,mousecurr_y);
	drawLine(left_down_x,mousecurr_y,left_down_x,left_down_y);
	glutSwapBuffers();
      }
      if(dragstate==3||dragstate==4){
	if(!dragcut){
	  glDrawBuffer(GL_BACK);
	  glBegin(GL_QUADS);
	  glColor3f(1.0,1.0,1.0);
	  glVertex2f(xvalueto1(dragsquarev[0]),yvalueto1(dragsquarev[1]));
	  glVertex2f(xvalueto1(dragsquarev[2]),yvalueto1(dragsquarev[1]));
	  glVertex2f(xvalueto1(dragsquarev[2]),yvalueto1(dragsquarev[3]));
	  glVertex2f(xvalueto1(dragsquarev[0]),yvalueto1(dragsquarev[3]));
	  glEnd();
	  glutSwapBuffers();
	  glDrawBuffer(GL_FRONT);
	  glColor3ubv(currentcolor);
	  dragcut=1;
	}
	glWindowPos2i(mousecurr_x-((dragsquarev[2]-dragsquarev[0])/2),mousecurr_y-((dragsquarev[3]-dragsquarev[1])/2));
	glDrawPixels(dragsquarev[2]-dragsquarev[0],dragsquarev[3]-dragsquarev[1],GL_RGB,GL_UNSIGNED_BYTE,gdata);
	glutSwapBuffers();
	dragstate=4;
      }
      break;
    }
  }
}

void keypressed(unsigned char key,int x,int y){
  switch(key){
  case '0':
    printf("#Point selected.\n");
    tool_type=PENCIL;
    break;
  case '1':
    printf("#Line selected.\n");
    tool_type=LINE;
    break;
  case '2':
    printf("#Circle selected.\n");
    tool_type=CIRCLE;
    break;
  case '3':
    printf("#Ellipse selected.\n");
    tool_type=ELLIPSE;
    break;
  case '4':
    printf("#Polygon selected.\n");
    tool_type=POLYGON;
    break;
  case '5':
    printf("#Curve selected.\n");
    tool_type=CURVE;
    curvestate=1;
    break;
  case '6':
    glColor3f(1.0,1.0,1.0);
    printf("#Eraser selected.\n");
    tool_type=ERASER;
    break;
  case '7':
    printf("#Spray selected.\n");
    tool_type=SPRAY;
    break;
  case '8':
    printf("#Fill selected.\n");
    tool_type=FILL;
    break;
  case '9':
    printf("#Drag selected.\n");
    tool_type=DRAG;
    break;
  case '+':
    switch(tool_type){
    case POLYGON:
      if(polysides<12){
	polysides++;
	drawGUI();
	glutSwapBuffers();
      }
      printf("#Polygon sides: %d.\n",polysides);
      break;
    case ERASER:
    case SPRAY:
      if(squaresize<100){
	squaresize++;
      }
      printf("#Size: %d.\n",squaresize);
      break;
    }
    break;
  case '-':
    switch(tool_type){
    case POLYGON:
      if(polysides>3){
	polysides--;
	drawGUI();
	glutSwapBuffers();
      }
    case ERASER:
    case SPRAY:
      if(squaresize>5){
	squaresize--;
      }
      printf("#Size: %d.\n",squaresize);
      break;
    }
    printf("#Polygon sides: %d.\n",polysides);
    break;
  case 'x':
    glClearColor(1.0,1.0,1.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawGUI();
    glutSwapBuffers();
    break;
  case 's':
    savetoBMP();
    break;
  case 'l':
    loadfromBMP();
    break;
  default:
    printf("%c key pressed! (%d,%d)\n",key,x,(int)(RESY-y));
  }
  if(tool_type!=ERASER){
    glColor3ubv(currentcolor);
  }
}


void slmenu(int mode){
   switch(mode){
  case 's':
    savetoBMP();
    break;
  case 'l':
    loadfromBMP();
    break;
  }
}

void reshape (int width, int height) {
}

int main(int argc,char *argv[]){
  if(argc!=3){
    RESX=800;
    RESY=600;
  }else{
  RESX=atof(argv[1]);
  RESY=atof(argv[2]);
  }
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(RESX,RESY);
  glutCreateWindow("Computer Graphics");
  glClearColor(1.0,1.0,1.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glutDisplayFunc(display);
  glutMouseFunc(mousefunc);
  glutMotionFunc(mousemotion);
  glutKeyboardFunc(keypressed);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  
  glutCreateMenu(slmenu);
  glutAddMenuEntry("Guardar",'s');
  glutAddMenuEntry("Cargar",'l');
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  

  glutMainLoop();
  return 0;
}

