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

void display(){
  drawLine(10,10,190,10);
  drawLine(190,10,190,140);
  drawLine(190,140,10,140);
  drawLine(10,140,10,10);
  drawLine(20,20,100,130);
  drawLine(100,130,180,20);
  drawLine(180,20,15,100);
  drawLine(15,100,185,100);
  drawLine(185,100,20,20);
}
