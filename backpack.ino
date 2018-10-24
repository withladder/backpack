const int buttonPin = 2;
const int buzzer = 9;
int buttonState = 0;
bool isOpened = true;
bool isEnableSecurity = false;
int status;
unsigned long beeStartTime=millis();
unsigned long beeNowTime=millis();

void setup() {
  // serial
  Serial.begin(9600);

  // button input
  pinMode(buttonPin, INPUT);
  // buzzer output
  pinMode(buzzer, OUTPUT);

  // 一開始鋒明器無聲
  digitalWrite(buzzer, HIGH);

  // delay 0.5 秒先開始
  delay(500);

  // 先讀取有無磁石係前面
  status = analogRead(0);
  if (status < 200) {
    isOpened = false;
  } else {
    isOpened = true;
  }

  // 一插電個下點都係安全的, 唔洗啟用保安裝置
  isEnableSecurity = false;
}

void loop() {
  // 不斷檢測有無磁石反應
  status = analogRead(0);
  if (status < 200) {
    // 書包閉左
    isOpened = false;
  } else {
    // 書包開左
    isOpened = true;
  }

  // 讀取按鈕狀態
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    // 放下按鈕時, 唔做野
  } else {
    // 按下按鈕時, 一定係 safe, 唔洗啟動保安裝置
    isEnableSecurity = false;
    beeStartTime = millis();
    beeNowTime = millis();
    digitalWrite(buzzer, HIGH);
    delay(3000);
    return;
  }

  // print
  Serial.print("status:");Serial.print(status);Serial.print(", ");
  Serial.print("buttonState:");Serial.print(buttonState);Serial.print(", ");
  Serial.print("isEnableSecurity:");Serial.print(isEnableSecurity);Serial.print(", ");
  Serial.print("isOpened:");Serial.print(isOpened);Serial.print(", ");
  Serial.print("mod:");Serial.print((beeNowTime - beeStartTime) % 1000);Serial.print(", ");
  Serial.println("");

  // 如果書包開左
  if (isOpened) {
    if (isEnableSecurity) {
      // 書包開左, 但又啟用左保安裝置, 就 bee
      beeNowTime = millis();
      if ((beeNowTime - beeStartTime) % 1000 <= 100) {
        digitalWrite(buzzer, LOW);
      } else {
        digitalWrite(buzzer, HIGH);
      }
    } else {
      // 唔洗 bee
      beeStartTime = millis();
      beeNowTime = millis();
      digitalWrite(buzzer, HIGH);
    }
  } else {
    // 如果書包關左

    // 唔會 bee
    beeStartTime = millis();
    beeNowTime = millis();
    digitalWrite(buzzer, HIGH);

    // 同埋會啟用保安裝置
    isEnableSecurity = true;
  }
}
