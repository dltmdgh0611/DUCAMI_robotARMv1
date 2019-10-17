#define width 640
#define height 360
#define SERVOB 399
#define SERVOBOT SERVOB+140
#define SERVOTOP SERVOB+580
#define SERVOMID SERVOB+360
#define SERVOZMID SERVOMID + 30

unsigned long per_millis,cur_millis;

int i,j;
float segLength1 = 150;
float segLength2 = 100;
float x, y, z, x2, y2, z2;
float nemoX, nemoY;
float an1, an2, anZ;
float angle1, angle2;

int sum_angle1,sum_angle2,sum_angleZ;

int ovf_count;
int joy_x,joy_y;
int servo_var = SERVOMID;
int serialcount;
bool x_flag, y_flag;
bool movetrigger = false;

int pos3d[3];
int count;


int joyread(int port);

void setup() {
  Serial.begin(115200);
  DDRB = 0x0F;
  TCCR1A = 0x03; // 10-bit Fast PWM
  TCCR1B = 0x0B; // 8pls/ck
  TCCR1C = 0x00; 
  TCNT1 = 399;
  OCR1A = 399+175;
  TIMSK1 = 0x03; //ovf,compa enable
  pinMode(13,OUTPUT);
  digitalWrite(13,0);
  x = width/2;
  y = height/2;
  x2 = x;
  y2 = y;
  z2 = z;
}

void loop() {
  cur_millis = millis();
  if(Serial.available()){
    movetrigger = false;
    pos3d[serialcount] = Serial.parseInt();
    serialcount++;
    if(serialcount == 3){
      Serial.println(pos3d[0]);
      Serial.println(pos3d[1]);
      Serial.println(pos3d[2]);
      serialcount = 0;
      movetrigger = true;
      
    }
  }


  if(cur_millis-per_millis>50)
  {
    per_millis=cur_millis;
    if(movetrigger){  
    float dx = (pos3d[0] - x);
    float dy = (pos3d[1] - y);
    float dz = pos3d[2];
   
     
    float angleZ = atan2(dz, abs(dx));
    dx = (dx/cos(angleZ));

    float angle1 = atan2(dy, dx);
    float tx = (pos3d[0] - cos(angle1) * segLength1);
    float ty = (pos3d[1] - sin(angle1) * segLength1);
    dx = tx - x2;
    dy = ty - y2;
    
    float angle2 = atan2(dy, dx);  
    x = x2 + cos(angle2) * segLength2;
    y = y2 + sin(angle2) * segLength2;
    
    an2 = (int)(180+angle2*(180/PI));
    an1 = abs((int)(180+angle1*(180/PI))-270);
    if(an1>90) an1=abs(360-an1);
    an1=an1+(90-an2);
    anZ = (int)(angleZ*(180/PI));
    Serial.println(an2);
    Serial.println(an1);
    Serial.println(anZ);
    an1 = an1/180*440;
    an2 = an2/180*440;
    anZ = anZ/180*440;
    
    sum_angle1 = (int)an1;
    sum_angle2 = (int)an2;
    sum_angleZ = (int)anZ;
    
    
    }
  }

  joy_x = joyread(0);
  joy_y = joyread(1);
  checkcheck();
  delay(5);

    
}

SIGNAL(TIMER1_OVF_vect){ //2.5ms (* 8 servos)
  TCNT1 = 399;
  ovf_count++;
  if(ovf_count == 8) ovf_count = 0;

  if(ovf_count == 0){
    PORTB |= 0x01;
    OCR1A = SERVOTOP - sum_angle2; //2nd servo ocr setting - 2
  }
  else if(ovf_count == 1){
    PORTB |= 0x02;
    OCR1A = SERVOZMID + sum_angleZ;                    //1st servo ocr setting -1r
  }
  else if(ovf_count == 2){
    PORTB |= 0x04;
    OCR1A = SERVOTOP - sum_angle1; //3nd servo ocr setting - 3
  }
  else if(ovf_count == 3) PORTB |= 0x08;
  else if(ovf_count == 7){
    OCR1A = servo_var;                   //4th servo ocr setting -4
  }
}

SIGNAL(TIMER1_COMPA_vect){ //0.7~2.3ms (* 8 servos)
  if(ovf_count == 0) PORTB &= ~0x01;
  else if(ovf_count == 1) PORTB &= ~0x02;
  else if(ovf_count == 2) PORTB &= ~0x04;
  else if(ovf_count == 3) PORTB &= ~0x08;
}

int joyread(int port) {
  while(ADCSRA&0x40);
  ADMUX &= ~0xff;
  ADMUX = (port|0x40);
  ADCSRA |= 0x87;
  ADCSRA |= 0x40;
  return ADC;
}

void checkcheck()
{
  if(joy_x < 520 && joy_x > 500){
    if(x_flag == 1 || x_flag==-1){
      x_flag = 0;
    }
  }
  else if(joy_x < 200 && joy_x >= 0){
    if(x_flag == 0){
    x_flag = 1;
    }
    if(servo_var>SERVOBOT) servo_var--;
  }
  else if(joy_x < 1030 && joy_x > 800){
    if(x_flag == 0){
    x_flag = -1;
    }
    if(servo_var<SERVOMID+50) servo_var++;
  }
}
