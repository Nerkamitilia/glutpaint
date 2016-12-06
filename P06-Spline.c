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

void mousemotion(int x,int y){
  mousecurr_x=x;
  mousecurr_y=(int)(RESY-y);
  if(mouseleft_down){
    switch(tool_type){
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
    case CURVE:
      if(curvestate==1){
	curve[0]=left_down_x;
	curve[1]=left_down_y;
      }
      break;
    }
  }
  if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP){
  mouseleft_down=0;
  switch(tool_type){
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
  }
}








