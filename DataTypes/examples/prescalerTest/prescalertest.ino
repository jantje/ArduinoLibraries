// Do not remove the include below

#define pin 9

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(115200);
	Serial.println("prescaler failure");
	Serial.println("on a mega with prescaler 6 on pin 2 does not generate a pulse");
	pinMode(pin,OUTPUT);
	int myEraser=7;
	int prescaler=6; //Works with 1,2,3,4,5 but not with 6
	TCCR2B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
	TCCR2B |= prescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
  analogWrite(pin,160);
}


void loop()
{
//take your time to look at the scope
}
