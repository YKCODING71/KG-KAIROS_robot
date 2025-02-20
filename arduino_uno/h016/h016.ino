int ENCODER = 2;

volatile int count = 0;
unsigned long oldTime = 0;
unsigned long newTime = 0;

void ISRencoder(){
  count++;
}

void setup() {
  Serial.begin(115200);
  pinMode(ENCODER,INPUT_PULLUP);
  attachInterrupt(INT0,ISRencoder,FALLING);
}

void loop() {
  newTime = millis();
  if(newTime-oldTime > 1000){
    oldTime = newTime;// \t\errors-when-compiling-esp32-wifi-libraries\10044
    noInterrupts();
    Serial.println(count);
    //count = 0;
    interrupts();
  }
}
