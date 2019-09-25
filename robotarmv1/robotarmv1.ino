#define SERVOB 399
#define SERVOBOT SERVOB+140
#define SERVOTOP SERVOB+600
#define SERVOMID SERVOB+370
#define FSERVOMID SERVOB+485

int serial_buf[6];
int sum_angle1,sum_angle2,sum_angle3,sum_angle4;
int ovf_count,x,y,servo_var = SERVOBOT;
int ocr_arr[6]={0,};
bool x_flag, y_flag;
unsigned long per_millis,cur_millis;

int joyread(int port);

void setup() {
  Serial.begin(9600);
  DDRB = 0x0F;
  TCCR1A = 0x03; // 10-bit Fast PWM
  TCCR1B = 0x0B; // 8pls/ck
  TCCR1C = 0x00; 
  TCNT1 = 399;
  OCR1A = 399+175;
  TIMSK1 = 0x03; //ovf,compa enable
  pinMode(13,OUTPUT);
  digitalWrite(13,0);
}

int i=0;
void loop() {
  cur_millis = millis();
  if(cur_millis-per_millis>5)
  {
    per_millis=cur_millis;
    x = joyread(0);
    y = joyread(1);
    checkcheck();
  }
  if(Serial.available()){
    serial_buf[i] = Serial.read();
    digitalWrite(13,0);
    i++;
    if(i==6){
        if(serial_buf[1]<180&&serial_buf[1]>0){
          sum_angle1 = (float)serial_buf[1]/180*460;
          Serial.println("S1" + (String)serial_buf[1]);
        }
        if(serial_buf[2]<180&&serial_buf[2]>0){
          sum_angle2 = (float)serial_buf[2]/180*460;
          Serial.println("S2" + (String)serial_buf[2]);
        }
        if(serial_buf[3]<180&&serial_buf[3]>0){
          sum_angle3 = (float)serial_buf[3]/180*460;
          Serial.println("S3" + (String)serial_buf[3]);
          
        }
        if(serial_buf[4]<180&&serial_buf[4]>0){
          sum_angle4 = (float)serial_buf[4]/180*460;
          Serial.println("S4" + (String)serial_buf[4]);
        }
      i=0;
      digitalWrite(13,1);
      }
      
  }
}


SIGNAL(TIMER1_OVF_vect){ //2.5ms (* 8 servos)
  TCNT1 = 399;
  ovf_count++;
  if(ovf_count == 8) ovf_count = 0;

  if(ovf_count == 0){
    PORTB |= 0x01;
    OCR1A = SERVOTOP-sum_angle2; //2nd servo ocr setting -2
  }
  else if(ovf_count == 1){
    PORTB |= 0x02;
    OCR1A = FSERVOMID; //1st servo ocr setting -1r
  }
  else if(ovf_count == 2){
    PORTB |= 0x04;
    OCR1A = SERVOTOP-sum_angle1; //3nd servo ocr setting -3
  }
  else if(ovf_count == 3) PORTB |= 0x08;
  else if(ovf_count == 7){
    OCR1A = servo_var; //4th servo ocr setting -4
  }
}

SIGNAL(TIMER1_COMPA_vect){ //0.7~2.3ms (* 8 servos)
  if(ovf_count == 0) PORTB &= ~0x01;
  else if(ovf_count == 1) PORTB &= ~0x02;
  else if(ovf_count == 2) PORTB &= ~0x04;
  else if(ovf_count == 3) PORTB &= ~0x08;
}

int joyread(int port)
{
  while(ADCSRA&0x40);
  ADMUX &= ~0xff;
  ADMUX = (port|0x40);
  ADCSRA |= 0x87;
  ADCSRA |= 0x40;
  return ADC;
}

void checkcheck()
{
  if(x < 520 && x > 500){
    if(x_flag == 1 || x_flag==-1){
      x_flag = 0;
    }
  }
  else if(x < 200 && x >= 0){
    if(x_flag == 0){
    x_flag = 1;
    }
    if(servo_var>SERVOBOT) servo_var--;
  }
  else if(x < 1030 && x > 800){
    if(x_flag == 0){
    x_flag = -1;
    }
    if(servo_var<SERVOMID+50) servo_var++;
  }
}
