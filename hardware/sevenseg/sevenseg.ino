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

// converts decimal number to 4-bit binary representation
int *bin_4bit(int n){
  int *bin = malloc(sizeof(int) * 4);
  for(int i = 0; i < 4; i++) bin[i] = 0;
  int index = 0;

  while(n > 0){
    bin[index] = n%2;
    n /= 2;
    index++;
  }

  return bin;
}

// the setup function runs once when you press reset or power the board
void setup(){
  Serial.begin(9600);
  for(int i = 8; i >= 2; i--) pinMode(i, OUTPUT);
}

// the loop function runs over and over again forever
void loop(){
  Serial.println(0);

  for(int i = 0; i <= 9; i ++){
    int* bin = bin_4bit(i);
    delay(600);
  }
}