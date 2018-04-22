void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  delay(1000);
  char a = 97;
  String hola = String(a);

  Serial.print(hola);
  // put your main code here, to run repeatedly:
  
}

