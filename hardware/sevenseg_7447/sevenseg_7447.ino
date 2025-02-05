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

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  for(int i = 5; i >= 2; i--) pinMode(i, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println(0);

  for(int i = 0; i <= 9; i ++){
    int* bin = bin_4bit(i);
    display_7447(bin[3], bin[2], bin[1], bin[0]);
    delay(600);
  }

  //display_7447(0, 1, 0, 0);
}
