#include "qdbmp.h"
#include "qdbmp.c"

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
      //glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,data);
      if(x<=(RESX-RESX*.1)&&y<=(RESY-RESY*.1))
	BMP_SetPixelRGB(bmp,x,RESY-y,*(data+z),*(data+z+1),*(data+z+2));
      else
	BMP_SetPixelRGB(bmp,x,RESY-y,255,255,255);
      z+=3;
      //printf("(%d,%d)=%d %d %d\n",x,y,*data,*(data+1),*(data+2));
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
      //glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,data);
      BMP_GetPixelRGB(bmp,x,RESY-y,data+z,data+z+1,data+z+2);
      z+=3;
      //printf("(%d,%d)=%d %d %d\n",x,y,*data,*(data+1),*(data+2));
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
