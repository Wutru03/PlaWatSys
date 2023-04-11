#define pumpPin 8
#define outputPin 12
#define inputPin 13
#define moisturePin A0

#define pumpTime 1000
#define moistureMin 0
#define moistureMax 505



void setup() {
  pinMode(pumpPin, OUTPUT);
  pinMode(outputPin, OUTPUT);

  Serial.begin(9600);


}

void loop() {

  int inputValue = digitalRead(inputPin);

// check for starting bit
  if(inputValue == LOW) return;

  delay(1);

// get command bit
  inputValue = digitalRead(inputPin);
  Serial.println(inputValue);
  if(inputValue == LOW){

    
    byte data = readMoisture();
    Serial.println("sending data");
    Serial.println(data, DEC);

    delay(1);
    sendData(data);
    Serial.println("sending data complete\n");

  } else {

    digitalWrite(outputPin, LOW);

    pump();

    // ending bit
    digitalWrite(outputPin, HIGH);
    delay(1);
    digitalWrite(outputPin, LOW);
  }
  
}

int readMoisture(){
  int moisture = map(analogRead(moisturePin), moistureMin, moistureMax, 0, 100);
  return moisture;
}

void pump(){
  Serial.println("Turning on pump");
  digitalWrite(pumpPin, HIGH);
  delay(pumpTime);
  Serial.println("Pumping done");
  digitalWrite(pumpPin, LOW);
}

void sendData(byte data){

  delay(10);
  digitalWrite(outputPin, HIGH);
  delay(1);

  for (int i=7; i>=0; i--){
    int bit = bitRead(data, i);
    digitalWrite(outputPin, bit);
    Serial.print(bit);
    delay(5);

  }

  Serial.println();

  digitalWrite(outputPin, LOW);
}