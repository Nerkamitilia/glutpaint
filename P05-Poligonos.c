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

void mousemotion(int x,int y){
  mousecurr_x=x;
  mousecurr_y=(int)(RESY-y);
  if(mouseleft_down){
    switch(tool_type){
      case POLYGON:
      glDrawBuffer(GL_FRONT);
      drawPolygon(left_down_x,left_down_y,mousecurr_x,mousecurr_y,polysides);
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
    case POLYGON:break
    }
  }
  if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP){
  mouseleft_down=0;
  switch(tool_type){
  case POLYGON:
      glDrawBuffer(GL_BACK);
      printf("Released at %d,%d\n",x,(int)(RESY-y));
      drawPolygon(left_down_x,left_down_y,mousecurr_x,mousecurr_y,polysides);
      glutSwapBuffers();
      break;
  }
}








