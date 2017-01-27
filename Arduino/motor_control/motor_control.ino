// Pins for motor control
const int rDir   = 7; 
const int rBrake = 2;
const int rPWM   = 5;

const int lDir   = 8;
const int lBrake = 4;
const int lPWM   = 6;


void setup() {
  // put your setup code here, to run once:
  pinMode(rDir, OUTPUT);
  pinMode(rBrake, OUTPUT);
  pinMode(rPWM, OUTPUT);
  
  pinMode(lDir, OUTPUT);
  pinMode(lBrake, OUTPUT);
  pinMode(lPWM, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite( rDir, 1 );
  digitalWrite( rBrake, 0 );

  digitalWrite( lDir, 0 );
  digitalWrite( lBrake, 0 );

  analogWrite( rPWM, 175 );
  analogWrite( lPWM, 175 );

  delay(5000);
  analogWrite( rPWM, 0 );
  analogWrite( lPWM, 0 );

  delay(5000);
   

}
