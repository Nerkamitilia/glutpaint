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

void mousemotion(int x,int y){
  mousecurr_x=x;
  mousecurr_y=(int)(RESY-y);
  if(mouseleft_down){
    switch(tool_type){
      case ELLIPSE:
      glDrawBuffer(GL_FRONT);
      drawEllipse(left_down_x,left_down_y,abs(mousecurr_x-left_down_x),abs(mousecurr_y-left_down_y));
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
    case ELLIPSE:break
    }
  }
  if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP){
  mouseleft_down=0;
  switch(tool_type){
  case ELLIPSE:
      glDrawBuffer(GL_BACK);
      printf("Released at %d,%d\n",x,(int)(RESY-y));
      drawEllipse(left_down_x,left_down_y,abs(mousecurr_x-left_down_x),abs(mousecurr_y-left_down_y));
      glutSwapBuffers();
      break;
  }
}








