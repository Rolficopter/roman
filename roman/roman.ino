typedef enum {
  motor1,
  motor2
} motor_t;
typedef enum {
  forward,
  backward
} direction_t;

// Motor 1 (right)
const int motor1Dir1 = 22;
const int motor1Dir2 = 23;
const int motor1Speed = 4;
const int motor1SpeedMes = 2;
volatile int motor1Counter = 0;
// Motor 2 (left)
const int motor2Dir1 = 24;
const int motor2Dir2 = 25;
const int motor2Speed = 5;
const int motor2SpeedMes = 3;
volatile int motor2Counter = 0;
// BT Control
const int command_length = 8; // bytes
char current_angle = 0;
int current_speed = 0;
int current_motor1Speed = 0;
int current_motor2Speed = 0;
direction_t current_direction = forward;

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

void setup_bluetooth() {
  Serial1.begin(9600);
}

void drive(int speed, char angle) {
  current_angle = angle;
  current_speed = speed;

  if ( angle == 0 ) {
    current_motor1Speed = speed;
    current_motor2Speed = speed;
  } else if ( angle < 0 ) {
    if ( current_direction == forward ) {
      current_motor1Speed = speed;
      current_motor2Speed = map(angle, 0, -90, speed, 0);
    } else {
      current_motor1Speed = map(angle, 0, -90, speed, 0);
      current_motor2Speed = speed;
    }
  } else if ( angle > 0 ) {
    if ( current_direction == forward ) {
      current_motor1Speed = map(angle, 0, 90, speed, 0);
      current_motor2Speed = speed; 
    } else {
      current_motor1Speed = speed;
      current_motor2Speed = map(angle, 0, 90, speed, 0);
    }
  } else {
    debug("invalid angle");
  }

  set_motor_speed(motor1, current_motor1Speed);
  set_motor_speed(motor2, current_motor2Speed);
}
void drive_speed(int speed) {
  drive(speed, current_angle);
}
void drive_angle(int angle) {
  drive(current_speed, angle);
}
void drive_direction(direction_t dir) {
  current_direction = dir;
  set_motor_direction(motor1, dir);
  set_motor_direction(motor2, dir);
}

void debug(const String &msg) {
  Serial.println(msg);
  Serial1.println(msg);
}
void debug(char c) {
  debug(String(c));
}
void debug(int i) {
  debug(String(i));
}
void debug(byte b) {
  debug(String(b));
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setup_motors();
  setup_motor_speed_counters();
  setup_bluetooth();

  debug("roman ready.");

  set_motor_direction(motor1, forward);
  set_motor_direction(motor2, forward);
  set_motor_speed(motor1, 0);
  set_motor_speed(motor2, 0);
}

unsigned long lastCommandTime;
void loop() {
  // put your main code here, to run repeatedly:

  // bluetooth
  if ( Serial1.available() <= 0 ) {

    if ( lastCommandTime+1000 < millis() ) {
      debug("stopping. No command for one second.");
      drive_speed(0);
    }
    return;
  }
  lastCommandTime = millis();

  String input = Serial1.readStringUntil('\0');
  debug("BT REC: " + input);

  int value = input.substring(1).toInt();
  if ( input.startsWith("s") ) {
    if ( value >= 0 ) {
      value = map(value, 0, 100, 0, 255);
      drive_direction(forward);
    } else {
      value = map(value, 0, -100, 0, 255);
      drive_direction(backward);
    }
    drive_speed(value);
  } else if ( input.startsWith("a") ) {
    drive_angle(value);
  } else {
    debug("unknown command: " + input);
  }
}

