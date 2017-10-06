// Motor 1
const int motor1Dir1 = 22;
const int motor1Dir2 = 23;
const int motor1Speed = 4;
const int motor1SpeedMes = 2;
volatile int motor1Counter = 0;
// Motor 2
const int motor2Dir1 = 24;
const int motor2Dir2 = 25;
const int motor2Speed = 5;
const int motor2SpeedMes = 3;
volatile int motor2Counter = 0;

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

void intr_count_motor1_speed() {
  motor1Counter++;
}
void intr_count_motor2_speed() {
  motor2Counter++;
}

void setup_motor_speed_counters() {
  attachInterrupt(digitalPinToInterrupt(motor1SpeedMes), intr_count_motor1_speed, RISING);
  attachInterrupt(digitalPinToInterrupt(motor2SpeedMes), intr_count_motor2_speed, RISING);
}
void reset_motor_counters() {
  motor1Counter = 0;
  motor2Counter = 0;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setup_motors();
  setup_motor_speed_counters();

  Serial.println("roman ready.");

  set_motor_direction(motor1, forward);
  set_motor_direction(motor2, forward);
  set_motor_speed(motor1, 255);
  set_motor_speed(motor2, 255);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("Motor speeds:");
  Serial.println(motor1Counter);
  Serial.println(motor2Counter);
  Serial.println(" ");
  reset_motor_counters();

  delay(1000);
}

