#define RELAY_SIGNAL 0
#define CH4 2

unsigned long pulseWidth;
int sig;

void setup() {
  pinMode(CH4, INPUT);
  pinMode(RELAY_SIGNAL, OUTPUT);
  pulseWidth = 0;
  sig = HIGH;
}

void loop() {
  
  // Read ON/OFF signal from Remote Controller's switch
  pulseWidth = (CH4, HIGH, 5000);

  // If CH4 reads ON, close relay circuit
  // else, open relay circuit
  sig = (pulseWidth > 1500) ? LOW: HIGH;
  digitalWrite(RELAY_SIGNAL, sig);
  
  // Delay code execution to conserve battery life
  delay(250);
  
}
