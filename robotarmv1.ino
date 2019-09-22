#define SERVOB 399
#define SERVOBOT SERVOB+140
#define SERVOTOP SERVOB+600
#define SERVOMID SERVOB+370
#define FSERVOMID SERVOB+485
double read_angle;
int serial_buf[5];
bool serial_flag;
int sum_angle1,sum_angle2;
int ovf_count;
int ocr_arr[6]={0,};
int dir=1,dir2=1;
unsigned long cur_millis, per_millis,per_millis2;


void setup() {
  Serial.begin(9600);
  DDRB = 0x0F;
  TCCR1A = 0x03; // 10-bit Fast PWM
  TCCR1B = 0x0B; // 8pls/ck
  TCCR1C = 0x00; 
  TCNT1 = 399;
  OCR1A = 399+175;
  TIMSK1 = 0x03; //ovf,compa enable
}

int i=0;
void loop() {

  if(Serial.available()){
    read_angle = Serial.read();
    serial_buf[i] = read_angle;
    i++;
    if(i==4){
        if(serial_buf[1]<90&&serial_buf[1]>45){
          sum_angle1 = (float)serial_buf[1]/180*460;
        }
        if(serial_buf[2]<90&&serial_buf[2]>45){
          sum_angle2 = (float)serial_buf[2]/180*460;
        }
      i=0;
      }
  }
  
//   cur_millis = millis(); //가변
//   if(cur_millis-per_millis>10){
//     per_millis=cur_millis;
//     if(dir==0){
//      ocr_arr[0]--;
//     }
//     else if(dir==1){
//       ocr_arr[0]++;
//     }
//     if(ocr_arr[0]>SERVOTOP-50){
//       dir=0;
//     }
//     else if(ocr_arr[0]<SERVOMID){
//       dir=1;
//     }
//   }


SIGNAL(TIMER1_OVF_vect){ //2.5ms (* 8 servos)
  TCNT1 = 399;
  ovf_count++;
  if(ovf_count == 8) ovf_count = 0;

  if(ovf_count == 0){
    PORTB |= 0x01;
    OCR1A = SERVOTOP+sum_angle2;//+sum_angle1; //2nd servo ocr setting -2
  }
  else if(ovf_count == 1){
    PORTB |= 0x02;
    OCR1A = FSERVOMID; //3nd servo ocr setting -1r
  }
  else if(ovf_count == 2){
    PORTB |= 0x04;
    OCR1A = SERVOTOP-sum_angle1;//-sum_angle2; //3nd servo ocr setting -3
  }
  else if(ovf_count == 3) PORTB |= 0x08;
  else if(ovf_count == 7){
    OCR1A = SERVOMID; //1st servo ocr setting -4
  }
}


SIGNAL(TIMER1_COMPA_vect){ //0.7~2.3ms (* 8 servos)
  if(ovf_count == 0) PORTB &= ~0x01;
  else if(ovf_count == 1) PORTB &= ~0x02;
  else if(ovf_count == 2) PORTB &= ~0x04;
  else if(ovf_count == 3) PORTB &= ~0x08;
}
