#define BUTTON_NORTH 8
#define BUTTON_EAST 2
#define BUTTON_WALK 4

#define LED_N_R 9
#define LED_N_Y 10
#define LED_N_G 11

#define LED_E_R 5
#define LED_E_Y 6
#define LED_E_G 7

#define LED_W_R 13
#define LED_W_G 12

#define goN 0
#define waitN 1
#define goE 2
#define waitE 3
#define goW 4
#define off1 5
#define on1 6
#define off2 7
#define on2 8
#define off3 9

struct State {
  unsigned long ST_Out; // 6-bit pattern to street output
  unsigned long Time; // delay in ms units
  unsigned long NextST[8];
}; // next state for inputs 
typedef const struct State SType;
SType FSM[10]= {
  {B00110010, 3000, {goN, waitN, goN, waitN, waitN, waitN, waitN, waitN}},//goN
  {B01010010, 500, {goE, goW, goE, goW, goE, goE, goE, goE}},//waitN
  {B10000110, 3000, {goE, waitE, waitE, waitE, goE, waitE, waitE, waitE}},//goE
  {B10001010, 500, {goW, goW, goN, goW, goW, goW, goN, goW}},//waitE
  {B10010001, 3000, {goW, goW,off1, off1,off1, off1,off1, off1}},//goW
  {B10010000, 300, {on1, on1,on1, on1,on1, on1,on1, on1}},//off1
  {B10010001, 300, {off2, off2,off2, off2,off2, off2,off2, off2}},//on1
  {B10010000, 300, {on2, on2,on2, on2,on2, on2,on2, on2}},//off2
  {B10010001, 300, {off3, off3,off3, off3,off3, off3,off3, off3}},//on2,
  {B10010001, 300, {goN, goE, goN, goN, goE, goN, goN, goN}}
  };

unsigned long ST=0; // index to the current state

void setup() {
  Serial.begin(9600);
  pinMode(LED_N_R, OUTPUT);
  pinMode(LED_N_Y, OUTPUT);
  pinMode(LED_N_G, OUTPUT);
  pinMode(BUTTON_NORTH, INPUT);
  pinMode(LED_E_R, OUTPUT);
  pinMode(LED_E_Y, OUTPUT);
  pinMode(LED_E_G, OUTPUT);
  pinMode(BUTTON_EAST, INPUT);
  pinMode(LED_W_R, OUTPUT);
  pinMode(LED_W_G, OUTPUT);
}
int input,input1, input2, input3;
void loop() {
  digitalWrite(LED_N_R, !(FSM[ST].ST_Out & B10000000));
  digitalWrite(LED_N_Y, !(FSM[ST].ST_Out & B01000000));
  digitalWrite(LED_N_G, !(FSM[ST].ST_Out & B00100000));
  digitalWrite(LED_E_R, !(FSM[ST].ST_Out & B00010000));
  digitalWrite(LED_E_Y, !(FSM[ST].ST_Out & B00001000));
  digitalWrite(LED_E_G, !(FSM[ST].ST_Out & B00000100));
  digitalWrite(LED_W_R, !(FSM[ST].ST_Out & B00000010));
  digitalWrite(LED_W_G, !(FSM[ST].ST_Out & B00000001));
  delay(FSM[ST].Time);
  
  input1 = digitalRead(BUTTON_NORTH);
  Serial.println(input1);
  input2 = digitalRead(BUTTON_EAST);
  input3 = digitalRead(BUTTON_WALK);
  input = input1*2+input2*4+input3;
  ST = FSM[ST].NextST[input];
}