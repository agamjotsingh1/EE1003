#define CLOCK 13

#define Q0 8
#define Q1 9
#define Q2 11
#define Q3 10

#define D0 2
#define D1 3
#define D2 4
#define D3 5

int write(int A, int B, int C, int D){
  digitalWrite(D0, A);
  digitalWrite(D1, B);
  digitalWrite(D2, C);
  digitalWrite(D3, D);
}

int A(int W, int X, int Y, int Z){
  return (!W);
}

int B(int W, int X, int Y, int Z){
  return (!W && X) || (W && !X && !Z);
}

int C(int W, int X, int Y, int Z){
  return (!X && Y) || (!W && Y) || (W && X && !Y);
}

int D(int W, int X, int Y, int Z){
  return (!W && Z) || (W && X && Y);
}


void setup() {
  Serial.begin(9600);
  pinMode(CLOCK, OUTPUT);
  
  pinMode(Q0, INPUT);
  pinMode(Q1, INPUT);
  pinMode(Q2, INPUT);
  pinMode(Q3, INPUT);
  
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);

  
  digitalWrite(CLOCK, HIGH);
  write(0, 0, 0, 0);
  delay(1);
  digitalWrite(CLOCK, LOW);
  delay(999);
}

void loop() {
  Serial.print(String(digitalRead(Q0)) + " " + String(digitalRead(Q1)) + " " + String(digitalRead(Q2)) + " " + String(digitalRead(Q3)) + "\n");
  
  write(A(digitalRead(Q0),digitalRead(Q1),digitalRead(Q2),digitalRead(Q3)),
        B(digitalRead(Q0),digitalRead(Q1),digitalRead(Q2),digitalRead(Q3)),
        C(digitalRead(Q0),digitalRead(Q1),digitalRead(Q2),digitalRead(Q3)),
        D(digitalRead(Q0),digitalRead(Q1),digitalRead(Q2),digitalRead(Q3)));
        
  digitalWrite(CLOCK, HIGH);
  delay(1);
  digitalWrite(CLOCK, LOW);
  delay(999);
}
