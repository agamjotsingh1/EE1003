// writes to the seven segment display with inputs a, b, ..., g
void sevenseg(int a,int b,int c,int d,int e,int f,int g){
  digitalWrite(2, a); 
  digitalWrite(3, b); 
  digitalWrite(4, c); 
  digitalWrite(5, d); 
  digitalWrite(6, e); 
  digitalWrite(7, f);     
  digitalWrite(8, g); 
}

// the setup function runs once when you press reset or power the board
void setup(){
  Serial.begin(9600);
  for(int i = 8; i >= 2; i--) pinMode(i, OUTPUT);
}

// the loop function runs over and over again forever
void loop(){
  Serial.println(0);

  // 0
  sevenseg(0,0,0,0,0,0,1);
  delay(500);

  // 1
  sevenseg(1,0,0,1,1,1,1);
  delay(500);

  //2
  sevenseg(0,0,1,0,0,1,0);
  delay(500);

  //3
  sevenseg(0,0,0,0,1,1,0);
  delay(500);

  //4
  sevenseg(1,0,0,1,1,0,0);
  delay(500);

  //5
  sevenseg(0,1,0,0,1,0,0);
  delay(500);

  //6
  sevenseg(0,1,0,0,0,0,0);
  delay(500);

  //7
  sevenseg(0,0,0,1,1,1,1); 
  delay(500);

  //8
  sevenseg(0,0,0,0,0,0,0); 
  delay(500);

  //9
  sevenseg(0,0,0,0,1,0,0);
  delay(500);
}
