// 핀 번호 정의
const int IN1 = 9;
const int IN2 = 10;

// 액추에이터 동작 상태 열거형
enum ActuatorState {
  EXTEND,  // 최대 길이로 늘림
  RETRACT, // 최소 길이로 줄임
  STOP     // 정지
};

// 상태별 제어 함수
void controlActuator(ActuatorState state) {
  switch (state) {
    case EXTEND:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      break;
    case RETRACT:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      break;
    case STOP:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      break;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  // 초기 전원 시에는 아무 동작도 하지 않는다.
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\\n');
    cmd.trim();

    if (cmd == "UP") {
      // 최대 길이로 늘리고 정지
      controlActuator(EXTEND);
      delay(15000);
      controlActuator(STOP);
    }
    else if (cmd == "DOWN") {
      // 최소 길이로 줄이고 정지
      controlActuator(RETRACT);
      delay(15000);
      controlActuator(STOP);
    }
  }
}
