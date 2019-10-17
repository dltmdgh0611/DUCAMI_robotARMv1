float segLength1 = 100;
float segLength2 = 150;
float x, y, x2, y2, mX, mY;
int count = 0;
float angle1, angle2;
int an1,an2,an3=10,an4=10;
import processing.serial.*;
Serial port;
void setup() {
  
  size(640, 360);
  strokeWeight(20.0);
  stroke(255, 100);
  //port = new Serial(this, Serial.list()[0], 9600);
  printArray(Serial.list());
  x = width/2;
  y = height/2;
  x2 = x;
  y2 = y;

  mX = x;
  mY = y;
  
}



void draw() {
  background(0);
  
//[1]///////////////////////////

   //ellipse(x,y,10,10);

   //segment(x, y, PI); 
  //  //segment(x, y, PI/3);

//[2]///////////////////////////

   //ellipse(x,y,10,10);

   //float dx = mouseX - x;
   //float dy = mouseY - y;
   //float angle1 = atan2(dy, dx);  

   //segment(x, y, angle1); 

   //println("angle1:"+ (180+angle1*(180/PI)) +"도");
   //println("");
   //i = (int)(180+angle1*(180/PI));
   //port.write(i);
   //delay(10);
//[3]////////////////////////////////////


  //ellipse(x2,y2,10,10);
  //ellipse(x,y,5,5);
  //// float dx = mouseX - x;
  //// float dy = mouseY - y;
  //float dx = mX - x;
  //float dy = mY - y;
  //float angle1 = atan2(dy, dx);  

  //float tx = mX - cos(angle1) * segLength;
  //float ty = mY - sin(angle1) * segLength;
  //dx = tx - x2;
  //dy = ty - y2;
  //float angle2 = atan2(dy, dx);  
  //x = x2 + cos(angle2) * segLength;
  //y = y2 + sin(angle2) * segLength;

  //ellipse(x,y,3,3);
  //segment(x, y, angle1); 
  ////segment(x2, y2, angle2); 
  //ellipse(mX,mY,1,1);

  //delay(500);

  //count++;
  //if(count > 6){
  //  count = 0;
  //  mX +=10;
  //  mY +=10;
  //}


//[4]////////////////////////////////////


  // ellipse(x2,y2,10,10);

  // float dx = mouseX - x;
  // float dy = mouseY - y;
  // float angle1 = atan2(dy, dx);  

  // float tx = mouseX - cos(angle1) * segLength;
  // float ty = mouseY - sin(angle1) * segLength;
  // dx = tx - x2;
  // dy = ty - y2;
  // float angle2 = atan2(dy, dx);  
  // x = x2 + cos(angle2) * segLength;
  // y = y2 + sin(angle2) * segLength;

  //   segment(x2, y2, angle2); 

//[5]/////////////////////////////////////

  // ellipse(x2,y2,10,10);

  // float dx = mouseX - x;
  // float dy = mouseY - y;
  // float angle1 = atan2(dy, dx);  

  // float tx = mouseX - cos(angle1) * segLength;
  // float ty = mouseY - sin(angle1) * segLength;
  // dx = tx - x2;
  // dy = ty - y2;
  // float angle2 = atan2(dy, dx);  
  // x = x2 + cos(angle2) * segLength;
  // y = y2 + sin(angle2) * segLength;

  // segment(x, y, angle1); 

  // delay(1000);


//[5]/////////////////////////////////////


//[6]/////////////////////////////////////

  float dx = mouseX - x;
    float dy = mouseY - y;
    angle1 = atan2(dy, dx);  
    
    float tx = mouseX - cos(angle1) * segLength1;
    float ty = mouseY - sin(angle1) * segLength1;
    dx = tx - x2;
    dy = ty - y2;
    angle2 = atan2(dy, dx);  
    x = x2 + cos(angle2) * segLength2;
    y = y2 + sin(angle2) * segLength2;
  
    an2 = (int)(180+angle2*(180/PI));
    an1 = abs((int)(180+angle1*(180/PI))-270);
    if(an1>90) an1=abs(360-an1);
    an1=an1+(90-an2);
    
   
    line(x2,y2+50,x2,y2);
    line(0,y2+50,width,y2+50);
    segment(x, y, angle1, segLength1); 
    segment(x2, y2, angle2, segLength2); 
    delay(10); //<>//

}

void segment(float x, float y, float a, float segLength) {
  pushMatrix();
  translate(x, y);
  rotate(a);
  line(0, 0, segLength, 0);
  popMatrix();
}
