#include <Arduino.h> //You dont need this if you are working in the arduino IDE
#include <SPI.h>
#include <Ethernet.h>

// Motor Pin numbers on the arduino
//Pin 1 and Pin 2 set the direction 
const int motor1pin1 = 2;
const int motor1pin2 = 3;
const int motor1ENA = 9;

void up();
void down();
void stopMotor();

struct Command {
  const char* str;
  void (*func)();
};

Command commands[] = {
  { "clawOpen", up },
  { "clawClose", down },
  { "stopClaw", stopMotor }
};

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xBE };
IPAddress ip(192, 168, 1, 179); 

EthernetServer server(80); // Server will run on port 80

void setup() {
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor1ENA, OUTPUT);

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
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
            //Ignore if it's not a GET request
            
            if (currentLine.startsWith("GET")) {
              executeMotorCommand(currentLine.c_str());
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("Connection: close");  // the connection will be closed after completion of the response
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
      commands[i].func();
      return;
    }
  }
}

void up() {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  analogWrite(motor1ENA, 255);
}

void down() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  analogWrite(motor1ENA, 255);
}

void stopMotor() {
  analogWrite(motor1ENA, 0); // This stops the motor
}