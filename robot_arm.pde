float segLength1 = 80;
float segLength2 = 120;
float x, y, x2, y2;

void setup() {
  size(640, 360);
  strokeWeight(20.0);
  stroke(255, 100);
  
  x = width/2;
  y = height/2;
  x2 = x;
  y2 = y;
}

void draw() {
  background(0);
  
  float dx = mouseX - x;
  float dy = mouseY - y;
  float angle1 = atan2(dy, dx);  
  
  float tx = mouseX - cos(angle1) * segLength1;
  float ty = mouseY - sin(angle1) * segLength1;
  dx = tx - x2;
  dy = ty - y2;
  float angle2 = atan2(dy, dx);  
  x = x2 + cos(angle2) * segLength2;
  y = y2 + sin(angle2) * segLength2;
  
       println("angle1:" +angle1);
       println("angle2:" +angle2);
       println("");
       
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
