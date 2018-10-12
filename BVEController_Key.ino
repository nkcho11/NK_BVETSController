
#include <Keyboard.h>
#include <Keypad.h>

int i;
int dsd2 = 1;
int enable = 0;
int tmp1, tmp2, tmp3;
int power, brake, forward;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  
  Serial.begin(9600);
  Keyboard.begin();
  
  digitalWrite(13,HIGH);
  power = brake = forward = 0;
  
  analogWrite(9,255);
  delay(500);
  analogWrite(9,0);
}

void loop() {
  int a = map(analogRead(A1), 0, 1023, 255, 0);
  int b = map(analogRead(A2), 0, 1023, 0, 255);
  int c = map(analogRead(A0), 0, 1023, 90, -90);
  char key = keypad.getKey();
  int dsd = digitalRead(7);

  
  // 현재 brake, power, forward위치 산출
  
  brake = brk(a,brake);
  power = pwr(b,power);
  forward = fwd(c);

  // 키패드 작동
  keyfunc(key, &dsd2, &enable);

  // 키보드 작동 (DSD2도 포함)
  // Enable이 1일때만 작동하게 함
  
  if(enable == 1)
    keycontrol(brake,power,forward,tmp1,tmp2,tmp3,dsd,dsd2);
  
  // 현재 brake, power, forward값을 tmp에 저장
  
  tmp1 = brake;
  tmp2 = power;
  tmp3 = forward;
}

void keycontrol(int b,int p,int f,int t1,int t2,int t3,int d,int d2)
{
  Serial.println(f);
  Serial.println(b);
  Serial.println(p);
  //reverser
  if (t3 > f)
  {
    Keyboard.press(KEY_UP_ARROW);
    Keyboard.release(KEY_UP_ARROW);
  }
  else if (t3 < f)
  {
    Keyboard.press(KEY_DOWN_ARROW);
    Keyboard.release(KEY_DOWN_ARROW);
  }
  
  //brake
  if (t1 > b)
  {
    Keyboard.press('<');
    Keyboard.release('<');
  }
  else if (t1 < b)
  {
    Keyboard.press('>');
    Keyboard.release('>');
  }
  
  //power
  if (t2 > p)
  {
    Keyboard.press(81);
    Keyboard.release(81);
  }
  else if (t2 < p)
  {
    Keyboard.press(90);
    Keyboard.release(90);
  }
  
  // DSD
  if(d == 0)
    Keyboard.press(KEY_TAB);
  else      
    Keyboard.release(KEY_TAB);
    
  // DSD매크로
  if(d2 == 0)
    Keyboard.press(KEY_TAB);
  else
    Keyboard.release(KEY_TAB);

  return;
}

int brk(int a,int br) {
  int brake;
  if (a >= 0 && a < 10)       brake = 0;
  else if (a >= 2 && a < 10)  brake = 1;
  else if (a >= 12 && a < 20) brake = 2;
  else if (a >= 22 && a < 30) brake = 3;
  else if (a >= 32 && a < 40) brake = 4;
  else if (a >= 42 && a < 50) brake = 5;
  else if (a >= 52 && a < 60) brake = 6;
  else if (a >= 62 && a < 70) brake = 7;
  else if (a >= 72 && a < 80) brake = 8;
  else                        brake = br;
  return brake;  
}

int pwr(int b,int ac) {
  int power;
  if (b >= 0 && b < 15)         power = 0;
  else if (b >= 17 && b < 30)   power = 1;
  else if (b >= 32 && b < 45)   power = 2;
  else if (b >= 47 && b < 60)   power = 3;
  else if (b >= 62 && b < 75)   power = 4;  
  else                          power = ac;
  return power;
}

int fwd(int c) {
  int forward;
  if (c >= 30)        forward = 1;
  else if (c <= -30)  forward = -1;
  else                forward = 0;
  return forward;
}

void keyfunc(char key, int *dsd2, int *enable)
{
  switch(key)
  {
    case '1' : *dsd2 = 0; break;
    case '2' : *dsd2 = 1; break;
    case '3' : Keyboard.press(KEY_RETURN);Keyboard.release(KEY_RETURN); break; 
    case '4' : break;
    case '5' : break;
    case '6' : break;
    case '7' : Keyboard.press('p');Keyboard.release('p'); break;
    case '8' : Keyboard.press(KEY_F3);Keyboard.release(KEY_F3); break;
    case '9' : break;
    case '*' : {
      *enable = *enable + 1;
      *enable = *enable % 2; 
      for(i=0;i<*enable+1;i++)
      {
        analogWrite(9,255);
        delay(50);
        analogWrite(9,0);
        delay(50); 
      }
      break;
    }
    case '0' : break;
    case '#' : break;    
  }  
  return;
}

