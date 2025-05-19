// RubikPi로부터 시리얼 통신 신호를 받으면 리니어 액추에이터를 최대로 늘리고 2초 후, 다시 최소로 줄이는 테스트코드
// 핀 번호 정의
const int IN1 = 9;
const int IN2 = 10;

enum ActuatorState {
  EXTEND,
  RETRACT,
  STOP
};

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

  // ⚠️ 초기 전원 시에는 아무 동작도 하지 않음
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "RUN") {
      // 기존 동작 순서 실행
      controlActuator(EXTEND);
      delay(15000);
      controlActuator(STOP);

      delay(2000);  // 대기

      controlActuator(RETRACT);
      delay(15000);
      controlActuator(STOP);
    }
  }
}

// // 핀 번호 정의
// const int IN1 = 9;
// const int IN2 = 10;

// // 액추에이터 동작 상태 정의
// enum ActuatorState {
//   EXTEND,  // 늘리기
//   RETRACT, // 줄이기
//   STOP     // 정지
// };

// // 제어 함수 정의
// void controlActuator(ActuatorState state) {
//   switch (state) {
//     case EXTEND:
//       digitalWrite(IN1, HIGH);
//       digitalWrite(IN2, LOW);
//       break;
//     case RETRACT:
//       digitalWrite(IN1, LOW);
//       digitalWrite(IN2, HIGH);
//       break;
//     case STOP:
//       digitalWrite(IN1, LOW);
//       digitalWrite(IN2, LOW);
//       break;
//   }
// }

// void setup() {
//   pinMode(IN1, OUTPUT);
//   pinMode(IN2, OUTPUT);

//   // 1. 최대까지 늘리기 (리미트 스위치까지)
//   controlActuator(EXTEND);
//   delay(15000);  // 충분히 늘어나도록 여유 시간 줌 (리미트 스위치에 의존)
//   controlActuator(STOP);

//   // 2. 대기
//   delay(2000);

//   // 3. 다시 최소까지 줄이기
//   controlActuator(RETRACT);
//   delay(15000);  // 충분히 줄어들도록 여유 시간 줌
//   controlActuator(STOP);
// }

// void loop() {
//   // 아무 것도 안 함
// }
