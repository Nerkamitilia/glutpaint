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
}

void mousemotion(int x,int y){
  mousecurr_x=x;
  mousecurr_y=(int)(RESY-y);
  if(mouseleft_down){
    switch(tool_type){
      case CIRCLE:
      glDrawBuffer(GL_FRONT);
      drawCircle(left_down_x,left_down_y,(sqrt(abs(mousecurr_x-left_down_x)*abs(mousecurr_x-left_down_x)+abs(mousecurr_y-left_down_y)*abs(mousecurr_y-left_down_y))));
      glutSwapBuffers();
      break;
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
    case CIRCLE:break
    }
  }
  if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP){
  mouseleft_down=0;
  switch(tool_type){
  case CIRCLE:
      glDrawBuffer(GL_BACK);
      printf("Released at %d,%d\n",x,(int)(RESY-y));
      drawCircle(left_down_x,left_down_y,(sqrt(abs(mousecurr_x-left_down_x)*abs(mousecurr_x-left_down_x)+abs(mousecurr_y-left_down_y)*abs(mousecurr_y-left_down_y))));
      glutSwapBuffers();
      break;
  }
}








