#include <Arduino.h> //You dont need this if you are working in the arduino IDE
#include <SPI.h>
#include <Ethernet.h>

int motor1pin1 = 2;
int motor1pin2 = 3;
int motor1ENA = 9;

int motor2pin1 = 4;
int motor2pin2 = 5;
int motor2ENA = 6;

/*void forwardL(int);
void backwardL(int);
void stopL();
void forwardR(int);
void backwardR(int);
void stopR();
void forwardAll(int);
void backwardAll(int);
void stopAll();
*/
void driveL(int);
void driveR(int);
void driveB(int);

struct Command {
  const char* str;
  void (*func)(int);
};

/* Old coamands Array
Command commands[] = {
  { "forwardL", forwardL },
  { "backwardL", backwardL },
  { "stopL", stopL },
  { "forwardR", forwardR },
  { "backwardR", backwardR },
  { "stopR", stopR },
  { "forwardBoth", forwardAll },
  { "backwardBoth", backwardAll },
  { "stopBoth", stopAll }
};*/
Command commands[] = {
  { "driveL" , driveL },
  { "driveR" , driveR },
  { "driveB" , driveB }
};

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Placeholder MAC Address
IPAddress ip(192, 168, 1, 177); // Placeholder IP Address, should be unique in your network

EthernetServer server(80); // Server will run on port 80

void setup() {
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor1ENA, OUTPUT);

  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(motor2ENA, OUTPUT);

  Serial.begin(9600);

  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    String currentLine = ""; 
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();  
        if (c == '\n') {
          if (currentLine.length() == 0) {
            break;
          } else {
            if (currentLine.startsWith("GET")) {
              executeMotorCommand(currentLine.c_str());
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");
              client.stop();
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    delay(1);
    client.stop();
  }
}

void executeMotorCommand(const char* command) {
  if (strncmp(command, "GET /", 5) != 0) {
    return;  // not a GET request
  }

  // Start parsing after "GET /"
  command += 5;

  // Find the end of the command word
  char* endOfCommand = strpbrk(command, " ?");
  if (!endOfCommand) {
    return;  // ill-formed request
  }

  // Find the speed parameter
  int speed = 0; 
  char* speedPtr = strstr(command, "?speed=");
  if (speedPtr) {
    sscanf(speedPtr, "?speed=%d", &speed);
  }

  // Function to parse the client's request and execute the corresponding command
  int commandLength = endOfCommand - command;
  for (unsigned int i = 0; i < sizeof(commands) / sizeof(Command); ++i) {
    if (strncmp(command, commands[i].str, commandLength) == 0 
         && commandLength == strlen(commands[i].str)) { // make sure lengths are equal
      commands[i].func(speed);
      return;
    }
  }
}

void driveL(int speed){
  if(speed == 0){
    analogWrite(motor1ENA, 0);
  } else if(speed < 0){
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(motor1ENA, abs(speed));
  } else{
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      analogWrite(motor1ENA, speed);
  }
}

void driveR(int speed){
  if(speed == 0){
    analogWrite(motor1ENA, 0);
  } else if(speed < 0){
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(motor2ENA, abs(speed));
  } else{
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
      analogWrite(motor2ENA, speed);
  }  
}

void driveB(int speed){
  if(speed == 0){
    analogWrite(motor1ENA, 0);
    analogWrite(motor2ENA, 0);
  } else if(speed < 0){
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    analogWrite(motor1ENA, abs(speed));
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    analogWrite(motor2ENA, abs(speed));
  } else{
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      analogWrite(motor1ENA, abs(speed));
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
      analogWrite(motor2ENA, abs(speed));
  }
}
/* Unused functions from a previous version
void forwardL(int speed) {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  analogWrite(motor1ENA, speed);
}

void backwardL(int speed) {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  analogWrite(motor1ENA, speed);
}

void stopL() {
  analogWrite(motor1ENA, 0);
}

void forwardR(int speed) {
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  analogWrite(motor2ENA, speed);
}

void backwardR(int speed) {
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  analogWrite(motor2ENA, speed);
}

void stopR() {
  analogWrite(motor2ENA, 0);
}

void forwardAll(int speed){
  forwardL(speed);
  forwardR(speed);
}

void backwardAll(int speed){
  backwardL(speed);
  backwardR(speed);
}

void stopAll(){
  stopL();
  stopR();
}*/