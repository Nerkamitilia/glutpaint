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
