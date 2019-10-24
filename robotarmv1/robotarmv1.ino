#define width 640
#define height 360
#define SERVOB 399
#define SERVOBOT SERVOB+140
#define SERVOTOP SERVOB+580
#define SERVOMID SERVOB+360
#define SERVOZMID SERVOMID + 30
#define _X 0
#define _Y 1
#define _Z 2
unsigned long per_millis, cur_millis;

int addX, addY; //증감
int counter = 0;

float segLength1 = 150;
float segLength2 = 100;
float x, y, z, x2, y2, z2, dz;
float an1, an2, anZ;
float angle1, angle2, angleZ;

int sum_angle1, sum_angle2, sum_angleZ;


int ovf_count;
int joy_x, joy_y;
int servo_var = SERVOMID;
int serialcount;
bool x_flag, y_flag;
bool movetrigger = false, Posflag;

int pos3d[3], prev3d[3], distance[3], discounter[3], moving[3];


int joyread(int port);
void Bresenham(int x1, int y1, int x2, int y2);
void setup() {
  Serial.begin(115200);
  DDRB = 0x0F;
  TCCR1A = 0x03; // 10-bit Fast PWM
  TCCR1B = 0x0B; // 8pls/ck
  TCCR1C = 0x00;
  TCNT1 = 399;
  OCR1A = 399 + 175;
  TIMSK1 = 0x03; //ovf,compa enable
  pinMode(13, OUTPUT);
  digitalWrite(13, 0);
  x = 320;
  y = 180;
  x2 = x;
  y2 = y;
  z2 = z;
  prev3d[_X] = 200;
  prev3d[_Y] = 250;
  prev3d[_Z] = 0;
  moving[_X] = prev3d[_X];
  moving[_Y] = prev3d[_Y];
}

void loop() {
  cur_millis = millis();
  if (Serial.available()) {
    movetrigger = false;
    pos3d[serialcount] = Serial.parseInt();
    serialcount++;
    if (serialcount == 3) {
      serialcount = 0;
      movetrigger = true;
      angleZ = atan2(dz, abs(prev3d[_X] - x2));
      prev3d[_X] = ((prev3d[_X] * cos(angleZ)));

      for(int i=0;i<3;i++){
        discounter[i] = 0;
      }

      distance[_X] = pos3d[_X] - prev3d[_X];
      distance[_Y] = pos3d[_Y] - prev3d[_Y];
      //X이동거리가 음수
      if (distance[_X] < 0) {
        addX = -1;
        distance[_X] = -distance[_X];
      }
      else addX = 1;

      //Y이동거리가 음수
      if (distance[_Y] < 0) {
        addY = -1;
        distance[_Y] = -distance[_Y];
      }
      else addY = 1;
    }
  }


  if (cur_millis - per_millis > 10)
  {
    per_millis = cur_millis;
    if (movetrigger) {
      //dx가 더 클경우( 기울기 < 1) y의 조건이 만족하면 y를 1증가시킨다
      if (distance[_X] >= distance[_Y]) {
        if (discounter[_X] < distance[_X]) {
          //x를  1증가 시키고 dy만큼 Count 증가
          moving[_X] += addX;
          //x의 증가값만큼 Count 증가
          counter += distance[_Y];
          //증가하는 y값 보다 크다면
          if (counter >= distance[_X]) {
            // y를 1증가 시키고 Count를 초기화한다
            moving[_Y] += addY;
            counter -= distance[_X];
          }
          Serial.print("x : ");
          Serial.println(moving[_X]);
          Serial.print("y : ");
          Serial.println(moving[_Y]);
          discounter[_X]++;
        }
        else if(discounter[_Y] == distance[_Y]){
          Posflag = true;
          discounter[_X]++;
          Serial.println("XF");
        }
        else ;
      }
      //dy가 더 클 경우( 기울기 > 1) x의 조건이 만족하면 y를 1증가시킨다
      else {
        if (discounter[_Y] < distance[_Y]) {
          //y를  1증가 시키고 dx만큼 Count 증가
          moving[_Y] += addY;
          counter += distance[_X];

          //증가하는 y값 보다 크다면
          if (counter >= distance[_Y]) {
            // x를 1증가 시키고 Count를 초기화한다
            moving[_X] += addX;
            counter -= distance[_Y];
          }
          Serial.print("x : ");
          Serial.println(moving[_X]);
          Serial.print("y : ");
          Serial.println(moving[_Y]);
          discounter[_Y]++;
        }
        else if(discounter[_Y] == distance[_Y]){
          Posflag = true;
          discounter[_Y]++;
          Serial.println("YF");
        }
        else ;
      }


    {
      Serial.println("------------------");
      Serial.print("prex : ");
      Serial.println(x);
      Serial.print("prey : ");
      Serial.println(y);
      float dx = (moving[_X] - x);
      float dy = (moving[_Y] - y);
      dz = moving[_Z];
      Serial.print("dx : ");
      Serial.println(dx);
      Serial.print("dy : ");
      Serial.println(dy);
      float angle1 = atan2(dy, dx);
      float tx = (moving[_X] - cos(angle1) * segLength1);
      float ty = (moving[_Y] - sin(angle1) * segLength1);
      dx = tx - x2;
      dy = ty - y2;
      Serial.print("dx : ");
      Serial.println(dx);
      Serial.print("dy : ");
      Serial.println(dy);
      float angle2 = atan2(dy, dx);
      x = x2 + cos(angle2) * segLength2;
      y = y2 + sin(angle2) * segLength2;
      Serial.print("x : ");
      Serial.println(x);
      Serial.print("y : ");
      Serial.println(y);
      an2 = (int)(180 + angle2 * (180 / PI));
      an1 = abs((int)(180 + angle1 * (180 / PI)) - 270);
      if (an1 > 90) an1 = abs(360 - an1);
      an1 = an1 + (90 - an2);
      anZ = (int)(angleZ * (180 / PI));
      Serial.println(an1);
      Serial.println(an2);
      Serial.println(anZ);
      an1 = an1 / 180 * 440;
      an2 = an2 / 180 * 440;
      anZ = anZ / 180 * 440;
      
      sum_angle1 = (int)an1;
      sum_angle2 = (int)an2;
      sum_angleZ = (int)anZ;
      Serial.println("------------------");
      Serial.println();
    }

      if (Posflag) { //Finish event
        for(int j=0;j<3;j++){
          Posflag = false;
          prev3d[j] = moving[j];
        }
        movetrigger = false;

      }

    }
  }

  joy_x = joyread(0);
  joy_y = joyread(1);
  checkcheck();
  delay(5);


}

SIGNAL(TIMER1_OVF_vect) { //2.5ms (* 8 servos)
  TCNT1 = 399;
  ovf_count++;
  if (ovf_count == 8) ovf_count = 0;

  if (ovf_count == 0) {
    PORTB |= 0x01;
    OCR1A = SERVOTOP - sum_angle2; //2nd servo ocr setting - 2
  }
  else if (ovf_count == 1) {
    PORTB |= 0x02;
    OCR1A = SERVOZMID + sum_angleZ;                    //1st servo ocr setting -1r
  }
  else if (ovf_count == 2) {
    PORTB |= 0x04;
    OCR1A = SERVOTOP - sum_angle1; //3nd servo ocr setting - 3
  }
  else if (ovf_count == 3) PORTB |= 0x08;
  else if (ovf_count == 7) {
    OCR1A = servo_var;                   //4th servo ocr setting -4
  }
}

SIGNAL(TIMER1_COMPA_vect) { //0.7~2.3ms (* 8 servos)
  if (ovf_count == 0) PORTB &= ~0x01;
  else if (ovf_count == 1) PORTB &= ~0x02;
  else if (ovf_count == 2) PORTB &= ~0x04;
  else if (ovf_count == 3) PORTB &= ~0x08;
}

int joyread(int port) {
  while (ADCSRA & 0x40);
  ADMUX &= ~0xff;
  ADMUX = (port | 0x40);
  ADCSRA |= 0x87;
  ADCSRA |= 0x40;
  return ADC;
}

void checkcheck()
{
  if (joy_x < 520 && joy_x > 500) {
    if (x_flag == 1 || x_flag == -1) {
      x_flag = 0;
    }
  }
  else if (joy_x < 200 && joy_x >= 0) {
    if (x_flag == 0) {
      x_flag = 1;
    }
    if (servo_var > SERVOBOT) servo_var--;
  }
  else if (joy_x < 1030 && joy_x > 800) {
    if (x_flag == 0) {
      x_flag = -1;
    }
    if (servo_var < SERVOMID + 50) servo_var++;
  }
}
