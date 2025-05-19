# `Arduino:` LinearActuator-Receiver
`Arduino:` Controlling linear actuators via serial communication with RubikPi

<br>

## 개요

이 시스템은 RubikPi로부터 `"UP"` 또는 `"DOWN"` 명령을 시리얼 통신으로 수신하고, 이에 따라 리니어 액추에이터를 확장 또는 수축하는 역할을 수행한다.

<br>

## 하드웨어 연결

- IN1: 액추에이터 제어 핀 1 (디지털 9번)
- IN2: 액추에이터 제어 핀 2 (디지털 10번)
- 시리얼 통신 속도: 9600 bps

<br>

## 동작 흐름

- `"UP\n"` 명령 수신 → 액추에이터 확장
- `"DOWN\n"` 명령 수신 → 액추에이터 수축
- 모든 동작은 약 15초 동안 지속된 후 자동 정지

<br>

## Arduino 제어코드

```c
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
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "UP") {
      controlActuator(EXTEND);
      delay(15000);
      controlActuator(STOP);
    } else if (command == "DOWN") {
      controlActuator(RETRACT);
      delay(15000);
      controlActuator(STOP);
    }
  }
}
```

<br>

## 고려사항

- 리미트 스위치를 사용하는 경우, delay 기반 동작 대신 상태 기반 제어 로직 구현 가능
- RubikPi에서 전송하는 시리얼 명령어에는 개행 문자(`\n`) 포함 필요
- 액추에이터 동작 중 안정적인 전원 공급 요구됨
