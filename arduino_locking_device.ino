#include <Adafruit_Fingerprint.h>  // 지문인식 센서 라이브러리
#include <LiquidCrystal_I2C.h>     // lcd 라이브러리
#include <Keypad.h>                // 키패드 라이브러리
#include <Servo.h>                 // 서보모터 라이브러리

SoftwareSerial mySerial(12, 13);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

Servo locker;
LiquidCrystal_I2C lcd(0x27, 16, 2);
char secretCode[4] = {'1', '2', '3', '4'};  // 키패드 비밀번호 설정
char input[4];
char key;
int position = 0;
int wrong = 0;
int i = 0;
int j = 0;
int pos = 0;
int a = 0;
int b = 0;
const byte rows = 4;
const byte cols = 4;
boolean c = false;

char keys[rows][cols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[rows] = {8, 7, 6, 5};  // 행
byte colPins[cols] = {4, 3, 2, 1};  // 열

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

void input_key();      // 비밀번호 입력을 나타내는 함수
void password();       // 비밀번호 1차확인을 나타내는 함수
void password_check(); // 비밀번호 2차확인을 나타내는 함수
void fingerscan();     // 지문인식 확인을 나타내는 함수

void setup()
{
  Serial.begin(9600);
  finger.begin(57600);
  lcd.init();
  lcd.backlight();
  locker.attach(10);      // 10번 pin으로 아두이노와 연결
  locker.write(90);
  pinMode(10, INPUT);
}

void loop()
{
  while (a == 0)
  {
    input_key();            // 비밀번호 입력
    password();             // 비밀번호 1차확인
    password_check();       // 비밀번호 2차확인
  }
  while (b == 0)
  {
    fingerscan();           // 지문인식 확인
  }
  int sum = a + b;          
  if (sum == 2)             // 비밀번호 + 지문인식이 맞다면
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    pinMode(10, OUTPUT);

    locker.write(180);      // 서보모터 회전(열림)
    lcd.print("    Lock OFF");
    delay(2000);
    pinMode(10, INPUT);
    key = keypad.waitForKey();
    if (key == '*')
    {
      a = 0;                // 초기화
      b = 0;                // 초기화
      lcd.clear();
      lcd.setCursor(0, 0);
      pinMode(10, OUTPUT);
      locker.write(90);     // 서보모터 회전(잠김)
      lcd.print("    Lock ON");
      delay(2000);         // 2초 대기
      pinMode(10, INPUT);
    }
  }
}

void input_key() // 비밀번호 입력을 나타내는 함수
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Password");
  for (i = 0; i < 4; i++)
  {
    key = keypad.waitForKey();
    input[i] = key;
    lcd.setCursor(i + 6, 1);  // lcd 화면 출력위치 설정
    lcd.print("*");
  }
}

void password() // 비밀번호 1차확인을 나타내는 함수
{
  wrong = 0;
  for (j = 0; j < 4; j++)
  {
    if (secretCode[j] == input[j])
    {
    }
    else if (secretCode[j] != input[j]) // 비밀번호가 틀리면
    {
      wrong += 1;  // wrong은 1이 된다.
    }
  }
}

void password_check() // 비밀번호 2차확인을 나타내는 함수
{
  if (wrong == 0)
  {
    Serial.println("correct");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("      PASS");
    delay(2000);
    a = 1;
    lcd.clear();
  }
  else if (wrong != 0)  // wrong이 0이 아니라면
  {
    Serial.println("wrong");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     wrong");
    delay(2000);
    a = 0;
    lcd.clear();
    wrong = 0;
  }
}

int getFingerprintIDez() {              // 변수 선언
  uint8_t p = finger.getImage();   // 지문인식센서에 지문이 인식되었는지 체크
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();           // 지정된 슬롯에 센서에서 인식된 이미지를 특성데이터로 변환
  if (p != FINGERPRINT_OK)  return -2;

  p = finger.fingerFastSearch();   // 슬롯에 저장된 지문과 동인한 지문이 있는지 체크
  if (p != FINGERPRINT_OK)  return -3;

  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  return finger.fingerID;
}

void fingerscan() // 지문인식 확인을 나타내는 함수
{
  pinMode(10, INPUT); //test
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("press finger");
  int data;
  data = getFingerprintIDez();
  if (data == 10) {
    b = 1;
  }
  else if (data == 11) {
    b = 1;
  }
  else if (data == 12) {
    b = 1;
  }
  else if (data == -3) {
    lcd.setCursor(0, 0);
    lcd.print(" Unknown User   ");
  }
  delay(1000);
}
