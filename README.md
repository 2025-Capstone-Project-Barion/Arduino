# `Arduino:` LinearActuator-Receiver
`Arduino:` Controlling linear actuators via serial communication with RubikPi

<br>

## 개요

이 시스템은 RubikPi로부터 `"UP"` 또는 `"DOWN"` 명령을 시리얼 통신으로 수신하고, 이에 따라 리니어 액추에이터를 확장 또는 수축하는 역할을 수행한다.

<br>

<p align="center"><img src="https://github.com/user-attachments/assets/cd126946-7862-4712-a1a4-f9f3487c9770" width="700"/></p>

<br>


## 하드웨어 연결

- IN1: 액추에이터 제어 핀 1 → Arduino 디지털 9번 핀
- IN2: 액추에이터 제어 핀 2 → Arduino 디지털 10번 핀
- 시리얼 통신 속도: 9600 bps
- Arduino와 RubikPi는 USB 케이블을 통해 직접 연결되어, Arduino에 전원을 공급함
- L298N 모터 드라이버 연결 구성:
  - IN1, IN2 핀은 각각 Arduino의 9번, 10번 핀과 연결
  - GND 핀(전면 2번째 슬롯)은 12V 외부 전원의 마이너스(-) 단자 및 Arduino GND와 공통 접지
  - 전면 1번째 슬롯(VCC)에는 12V 외부 전원의 플러스(+) 단자 연결
  - 좌측 출력부는 리니어 액추에이터 연결:
    - 좌측 슬롯 → 액추에이터 + 선
    - 우측 슬롯 → 액추에이터 - 선

<br>

## 동작 흐름

- `"UP\n"` 명령 수신 → 액추에이터 확장
- `"DOWN\n"` 명령 수신 → 액추에이터 수축
- 모든 동작은 약 15초 동안 지속된 후 자동 정지

<br>

## Arduino 제어코드

```c
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
```

<br>

## 고려사항 및 주의사항

- **전원 연결 순서 주의**  
  반드시 RubikPi의 전원을 먼저 인가한 후, L298N 모터 드라이버에 외부 12V 전원을 연결해야 함.  
  잘못된 순서로 전원 공급 시, **역전류로 인한 Arduino 손상(쇼트)** 가능성 있음.

- **정격 전압 고려**  
  Arduino는 5V 정격 전압 기준으로 동작하므로, GND 흐름이 불안정할 경우 오동작 또는 손상 가능성 있음.  
  GND는 RubikPi–Arduino–L298N 간에 항상 공통으로 유지될 것.

- **시리얼 명령어 구성**  
  RubikPi에서 전송되는 명령어는 **개행 문자 `\n`** 포함 필수 (예: `"UP\n"`, `"DOWN\n"`)

- **전원 안정성 확보**  
  리니어 액추에이터는 동작 시 일정한 전압과 전류가 필요하므로, 12V 2~3A 이상의 안정된 어댑터를 사용할 것.

<br>
