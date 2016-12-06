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

void mousemotion(int x,int y){
  mousecurr_x=x;
  mousecurr_y=(int)(RESY-y);
  if(mouseleft_down){
    switch(tool_type){
      case ERASER:
        drawEraser(left_down_x-squaresize/2,left_down_y-squaresize/2,mousecurr_x,mousecurr_y);
        left_down_x=mousecurr_x;
        left_down_y=mousecurr_y;
        glutSwapBuffers();
        break;
      case SPRAY:
        drawSpray(mousecurr_x,mousecurr_y);
        break;
      case PENCIL:
        drawLine(left_down_x,left_down_y,mousecurr_x,mousecurr_y);
        left_down_x=mousecurr_x;
        left_down_y=mousecurr_y;
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
    case ERASER:
      setFSquare(left_down_x-squaresize/2,left_down_y-squaresize/2);
      glutSwapBuffers();
      break;
    case SPRAY:
      drawSpray(left_down_x-squaresize/2,left_down_y-squaresize/2);
      break;
    case PENCIL:
      setPixel(left_down_x,left_down_y);
      glutSwapBuffers();
      break;
    }
  }
  if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP){
  mouseleft_down=0;
  switch(tool_type){
  case ERASER:
      break;
  case SPRAY:
      break;
  case PENCIL:
      break;
  }
}








