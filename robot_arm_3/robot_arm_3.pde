float segLength1 = 150;
float segLength2 = 100;
float x, y, x2, y2;
float nemoX, nemoY;
float angle1, angle2;
import processing.serial.*;
Serial port;
int mouse_X = 170, mouse_Y = 170;
float an1,an2;
int i,j;

int mod=1;
int[][] nemo = {{150,200},
                {150,150},
                {200,150},
                {200,200}
                };
short[][] angle_arr = new short[180][2];
void setup() {
  size(640, 360);
  strokeWeight(20.0);
  stroke(255, 155);
  //port = new Serial(this, Serial.list()[0], 115200);
  printArray(Serial.list());
  x = width/2;
  y = height/2;
  x2 = x;
  y2 = y;
  
  //for(int p=0;p<180;p++)
  //{
  //  if(mod == 0)
  //  {
  //    nemoX = width/2 + i -75;
  //    nemoY = height-40;
  //    j++;
  //    if(j>100) i++;
  //    else i--;
  //    if(j>200) j=0;
  //  }
  //  if(mod == 1)
  //  {
  //   nemoX = 150 + (30*cos((p*2)*PI/180));
  //   nemoY = 200 + (30*sin((p*2)*PI/180));
  //  }
  //    float dx = 150 - x;
  //    float dy = 200 - y;
  //    angle1 = atan2(dy, dx);  
      
  //    float tx = 150 - cos(angle1) * segLength1;
  //    float ty = 200 - sin(angle1) * segLength1;
  //    dx = tx - x2;
  //    dy = ty - y2;
  //    angle2 = atan2(dy, dx);  
  //    x = x2 + cos(angle2) * segLength2;
  //    y = y2 + sin(angle2) * segLength2;
    
  //    an2 = (int)(180+angle2*(180/PI));
  //    an1 = abs((int)(180+angle1*(180/PI))-270);
  //    if(an1>90) an1=abs(360-an1);
  //    an1=an1+(90-an2);
      


      
      
  //    //if(p>4){
  //    //println(p);
  //    //println("angle1:" +(short)an1 + "angle2:" +(short)an2);
  //    //port.write((short)an1);
  //    //delay(5);
  //    //port.write((short)an2);
  //    //delay(5);
  //    //line(x2,y2+50,x2,y2);
  //    //line(0,y2+50,width,y2+50);
  //    //segment(x, y, angle1, segLength1); 
  //    //segment(x2, y2, angle2, segLength2);
  //    //}
  //}
  
  
}



void draw() {
  background(0);
  //if(mod == 0) {
  //  nemoX = width/2 + i -75;
  //  nemoY = height-40;
  //  j++;
  //  if(j>100) i++;
  //  else i--;
  //  if(j>200) j=0;
  //}
  //if(mod == 1) {
  // nemoX = nemo[i][0];
  // nemoY = nemo[i][1];
  // i++;
  // if(i==4) i=0;
  //}
   
   //println(nemoX + ", " + nemoY + ", " + i + "," +cos(i*30) + "," + sin(i*30));
   
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
    println(an1 + " " + an2);
    
    line(0,height,width,height);
    segment(x, y, angle1, segLength1); 
    segment(x2, y2, angle2, segLength2); 
  
}

void segment(float x, float y, float a, float segLength) {
  pushMatrix();
  translate(x, y);
  rotate(a);
  line(0, 0, segLength, 0);
  popMatrix();
}
