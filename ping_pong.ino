#include <TVout.h>
#include <fontALL.h>

TVout TV;

int ys, yd, xc, yc, PS = 0, PD = 0, frame = 0;
int pxc, pyc;
int vxc, vyc;
bool tr = false, tt = false;
char val;

void fin_control(){
  if (PS >= 8) {
    TV.clear_screen();
    TV.select_font(font8x8);
    TV.print(0, TV.vres()/2 - 10, "SINISTRA VINCE!!!");
    tt = true;
  }
  if (PD >= 8) {
    TV.clear_screen();
    TV.select_font(font8x8);
    TV.print(0, TV.vres()/2 - 15, "DESTRA VINCE!!!");
    tt = true;
  }
  if (tt) {
    TV.select_font(font6x8);
    TV.print(10, TV.vres()/2 + 15, "Premere C");
    TV.print(10, TV.vres()/2 + 30, "per continuare");
    val = Serial.read();
    if (val == 'C' || val == 'c') {
      PS = 0;
      PD = 0;
      xc = TV.hres()/2;
      yc = TV.vres()/2;
      tt = false;
    }
  }
}

void input_control(){
  val = Serial.read();
  if (val == 'w') ys -= 5;
  if (val == 's') ys +=5;
  if (val == 'i') yd -=5;
  if (val == 'k') yd +=5;
}

void graf(){
  TV.clear_screen();
  TV.draw_line(1, 8, 128, 8, WHITE);                           //bordo alto
  TV.draw_line(1, 95, 128, 95, WHITE);                         //bordo basso
  TV.draw_line(1, 0, 1, TV.vres(), WHITE);                     //bordo sinistro
  TV.draw_line(TV.hres() - 1, 0, TV.hres() - 1, TV.vres(), WHITE);     //bordo destro
  TV.draw_rect(1, ys + 8, 10, 17, WHITE);                      //rettangolo di sinistra
  TV.draw_rect(TV.hres() - 11, yd + 8, 10, 17, WHITE);         //rettangolo di destra
  TV.set_pixel(xc, yc, WHITE);                                 //pallina
  TV.select_font(font4x6);                                     //selezione font
  TV.print(2, 2, PS);                                          //punteggio di sinistra
  TV.print(120, 2, PD);                                        //punteggio di destra
  TV.print(22, 1, "--- ARDUINO PONG ---");                     //name
}

void BallCalc(){
  if(xc <= 10){
    if(yc <= ys + 8 || yc >= ys + 25){
      PD++;
      tr = true;
    }
    else {
      vxc++;
      vxc = -vxc;
    }
  }
  if(xc >= TV.hres() - 10){
    if(yc <= yd + 8 || yc >= yd + 25){
      PS++;
      tr = true;
    }
    else {
      vxc++;
      vxc = -vxc;
    }
  }
  if(yc <= 9 || yc >= TV.vres() - 1) vyc = -vyc;
  if (vxc > 6) vxc = random(0, 4);
  if (vxc < -6) vxc = random(-4, 0);
  if(!tr){
  xc = pxc + vxc;
  yc = pyc + vyc;
  }
  else{
    xc = TV.hres()/2;
    yc = TV.vres()/2;
    do{
      vxc = random(-4, 4);
    }while(vxc == 0);
    vyc = random(-4, 4);
  }
  pxc = xc;
  pyc = yc;
  tr = false;
}

void setup(){
  Serial.begin(9600);
  TV.begin(PAL, 128, 96);
  ys = 0;
  yd = 0;
  do{
    vxc = random(-4, 4);
  }while(vxc == 0);
  vyc = random(-4, 4);
  xc = TV.hres()/2;
  yc = TV.vres()/2;
  pxc = xc;
  pyc = yc;
}

void loop() {
  fin_control();
  if (!tt) {
    input_control();
    if(frame%3 == 0) BallCalc();
    graf();
    /*TV.draw_rect(1, ys, 10, 17, WHITE);    //rettangolo di sinistra
    TV.draw_rect(TV.hres() - 11, yd, 10, 17, WHITE);  //rettangolo di destra*/
    //TV.draw_circle(xc, yc, 1, WHITE, WHITE);      //pallina (sfera)
  }
  TV.delay_frame(1);
  frame++;
  if(frame == 60) frame = 0;                  //contatore dei frame
}
