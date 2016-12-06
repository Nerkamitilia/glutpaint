void mousemotion(int x,int y){
  mousecurr_x=x;
  mousecurr_y=(int)(RESY-y);
  if(mouseleft_down){
    switch(tool_type){
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








