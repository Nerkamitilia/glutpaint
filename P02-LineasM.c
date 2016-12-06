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

void mousemotion(int x,int y){
  mousecurr_x=x;
  mousecurr_y=(int)(RESY-y);
  if(mouseleft_down){
    switch(tool_type){
      case LINE:
        glDrawBuffer(GL_FRONT);
        drawLine(left_down_x,left_down_y,mousecurr_x,mousecurr_y);
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
    case LINE:break;
    }
  }
  if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP){
  mouseleft_down=0;
  switch(tool_type){
  case LINE:
      glDrawBuffer(GL_BACK);
      printf("Released at %d,%d\n",x,(int)(RESY-y));
      drawLine(left_down_x,left_down_y,mousecurr_x,mousecurr_y);
      glutSwapBuffers();
      break;
  }
}








