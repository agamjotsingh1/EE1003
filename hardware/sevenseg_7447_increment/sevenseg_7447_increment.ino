void display_7447(int D, int C, int B, int A){
  // Binary format: D C B A
  // D -> MSB, A -> LSB
  digitalWrite(2, A);
  digitalWrite(3, B); 
  digitalWrite(4, C); 
  digitalWrite(5, D);
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

int *bin_inc_wrap(int Z, int Y, int X, int W){
  int *bin = malloc(sizeof(int) * 4);

  // increment logic
  // ZYXW + 1 = ABCD
  int A = (!W);
  int B = (!W && X && !Z) || (W && !X && !Z);
  int C = (!W && Y && !Z) || (!X && Y && !Z) || (W && X && !Y && !Z);
  int D = (W && X && Y && !Z) || (!W && !X && !Y && Z);

  bin[0] = A;
  bin[1] = B;
  bin[2] = C;
  bin[3] = D;
  return bin;
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  for(int i = 5; i >= 2; i--) pinMode(i, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println(0);

  for(int i = 0; i <= 9; i ++){
    int *bin = bin_4bit(i);
    int *bin_inc = bin_inc_wrap(bin[3], bin[2], bin[1], bin[0]);
    display_7447(bin_inc[3], bin_inc[2], bin_inc[1], bin_inc[0]);
    free(bin);
    free(bin_inc);
    delay(600);
  }
}
