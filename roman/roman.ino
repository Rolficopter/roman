// Motor 1
const int motor1Dir1 = 22;
const int motor1Dir2 = 23;
const int motor1Speed = 2;
// Motor 2
const int motor2Dir1 = 24;
const int motor2Dir2 = 25;
const int motor2Speed = 3;

typedef enum {
  motor1,
  motor2
} motor_t;
typedef enum {
  forward,
  backward
} direction_t;

void setup_motors() {
  pinMode(motor1Dir1, OUTPUT);
  pinMode(motor1Dir2, OUTPUT);
  pinMode(motor1Speed, OUTPUT);
  pinMode(motor2Dir1, OUTPUT);
  pinMode(motor2Dir2, OUTPUT);
  pinMode(motor2Speed, OUTPUT);
}

void set_motor_direction(motor_t motor, direction_t dir) {
  int pin1 = -1;
  int pin2 = -1;
  switch ( motor ) {
    case motor1:
      pin1 = motor1Dir1;
      pin2 = motor1Dir2;
      break;
    case motor2:
      pin1 = motor2Dir1;
      pin2 = motor2Dir2;
      break;
    default:
      return;
  }

  int pin1Voltage = -1;
  int pin2Voltage = -1;
  switch ( dir ) {
    case forward:
      pin1Voltage = LOW;
      pin2Voltage = HIGH;
      break;
    case backward:
      pin1Voltage = HIGH;
      pin2Voltage = LOW;
      break;
    default:
      return;
  }

  digitalWrite(pin1, pin1Voltage);
  digitalWrite(pin2, pin2Voltage);
}

void set_motor_speed(motor_t motor, int value) {
  int pin = -1;
  switch ( motor ) {
    case motor1:
      pin = motor1Speed;
      break;
    case motor2:
      pin = motor2Speed;
      break;
    default:
      return;
  }

  analogWrite(pin, value);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setup_motors();

  Serial.println("roman ready.");

  set_motor_direction(motor1, forward);
  set_motor_direction(motor2, forward);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("Speed 255");
  set_motor_speed(motor1, 255);
  set_motor_speed(motor2, 255);

  delay(3000);

  Serial.println("Speed 0");
  set_motor_speed(motor1, 0);
  set_motor_speed(motor2, 0);

  delay(3000);
}

