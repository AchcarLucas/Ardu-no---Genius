//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// Lucas Campos - Jogo do Genius
// 
// Made by Lucas Campos
// License: CC-BY-SA 3.0
// Downloaded from: https://123d.circuits.io/circuits/979438-lucas-campos-jogo-do-genius

#define WAIT 0
#define GAME 1

#define TONE_RED 2272
#define TONE_GREEN 2028
#define TONE_BLUE 7634
#define TONE_YELLOW 1433

char TEXT_MSG[256];  // TEMP MSG TEXT

// Conector para o Pieze (6V)
const short int buzzer      = 8;

// LEDS
const short int led_red     = 4;
const short int led_green   = 5;
const short int led_blue    = 6;
const short int led_yellow  = 7;

// BUTTONS
const short int button_00    = 12;
const short int button_01    = 11;
const short int button_02    = 2;
const short int button_03    = 3;

// STATUS DO JOGO
short int STATE_GAME = WAIT;
short int LED_GAME_DELAY_DIFICULTY = 1500;

// Sound do Jogo.
int melody_music[] = {
  2272, 300, 
  0,    150, 
  2028, 300,
  0,    150,
  7634, 300,
  0,    150,
  1433, 300,
  0,    150};

// Adiciona um array contendo as notas e os tempos determinados.
void MusicBuzzer(int *music, int _size){
  int total_notes = _size/2;
  for(int i = 0; i < total_notes; i+=2){
     tone(buzzer, music[i]);
     delay(music[i+1]);
     if(music[i] != 0)
      BlinkLed();
  }
  noTone(buzzer);
}

void LightOn(short int led){
  digitalWrite(led, HIGH);
}

void LightOff(short int led){
  digitalWrite(led, LOW);
}

void BlinkLed(){
  static short int count = 0;
  if(count == 0){
    LightOffLed();
    LightOn(led_red);
  }else if(count == 1){
    LightOffLed();
    LightOn(led_green);
  }else if(count == 2){
    LightOffLed();
    LightOn(led_blue);
  }else{
    LightOffLed();
    LightOn(led_yellow);
    count = -1;
  }
  count++;
}

void LightOffLed(){
  LightOff(led_red);
  LightOff(led_green);
  LightOff(led_blue);
  LightOff(led_yellow);
}

int checkButton(short int button){
  return digitalRead(button);
}

/* START STRUCT GAME CLASS */
typedef struct stack_game_color{
  short int color_type;
  stack_game_color *next;
};
/* END STRUCT GAME CLASS */

/* START GAME CLASS */
class Game{
  private:
    int level;
    int cycle;
    bool state;
    stack_game_color *SGC; // Struct Principal
    stack_game_color *nextCycle;  // Struct Next Cycle
  public:
    Game();
    void InitGame();
    void RestartGame();
    void FreeStackGameColor();
    void GetNextColor();
    void LoopGame();
    void ShowSequence();
  protected:
};

Game::Game(){
  level = 0;
  state = WAIT;
}

void Game::InitGame(){
  SGC = (stack_game_color*)malloc(sizeof(stack_game_color));
  SGC->next = NULL;
  RestartGame();
}

void Game::RestartGame(){
  FreeStackGameColor();
  SGC->color_type = random(0, 4);
  level = 1;
  cycle = 0;
  state = WAIT;
  LightOffLed();
}

void Game::FreeStackGameColor(){
  stack_game_color *tmp = SGC;
  stack_game_color *next_tmp;
  Serial.println("Free Stack Game Color");
  while(tmp->next != NULL){
    sprintf(TEXT_MSG, "Free Memory Address: %x", tmp->next);
    Serial.println(TEXT_MSG);
    next_tmp = tmp->next;
    free(tmp->next);
    tmp = next_tmp;
  }
  nextCycle = SGC;
  SGC->next = NULL;
}

void Game::LoopGame(){
  short int id_button = -1;
  state = GAME;
  stack_game_color *oldSGC;
  do{
    ShowSequence();
    LightOffLed();
    cycle = 0;
    nextCycle = SGC;
    /*sprintf(TEXT_MSG, "TESTE [%d %d %d %d %d %d NEXT [%d %d]]", nextCycle, &SGC, nextCycle->color_type, SGC->color_type, &nextCycle->next, &SGC->next, nextCycle->next, SGC->next);
    Serial.println(TEXT_MSG);*/
     
    do{
      id_button = -1;
      if(checkButton(button_00) == HIGH){
        id_button = 0;
        if(nextCycle->color_type == id_button)
        {
         LightOn(led_red);
         tone(buzzer, TONE_RED);
         delay(LED_GAME_DELAY_DIFICULTY/2);
         LightOff(led_red);
        }
      LightOff(led_red);
      }else if(checkButton(button_01) == HIGH){
        id_button = 1;
        if(nextCycle->color_type == id_button)
        {
         LightOn(led_green);
         tone(buzzer, TONE_GREEN);
         delay(LED_GAME_DELAY_DIFICULTY/2);
         LightOff(led_green);
        }
      }else if(checkButton(button_02) == HIGH){
        id_button = 2;
        if(nextCycle->color_type == id_button)
        {
         LightOn(led_blue);
         tone(buzzer, TONE_BLUE);
         delay(LED_GAME_DELAY_DIFICULTY/2);
         LightOff(led_blue);
        }
      }else if(checkButton(button_03) == HIGH){
        id_button = 3;
        if(nextCycle->color_type == id_button)
        {
         LightOn(led_yellow);
         tone(buzzer, TONE_YELLOW);
         delay(LED_GAME_DELAY_DIFICULTY/2);
         LightOff(led_yellow);
        }
      }
      
      if(id_button != -1){
       sprintf(TEXT_MSG, "Color: %d Button: %d", nextCycle->color_type, id_button);
       Serial.println(TEXT_MSG);
       if(nextCycle->color_type == id_button){
        oldSGC = nextCycle;
        nextCycle = nextCycle->next;
        cycle++;
        
        sprintf(TEXT_MSG, "Cycle: %d", cycle);
        Serial.println(TEXT_MSG);
        // Check Cycle ...
        delay(200);
       }else{
        // Game Over
        Serial.println("GAME OVER");
        tone(buzzer, 1000);
        delay(1000);
        state = WAIT;
        break;
       }
      }
      delay(100);
    }while(nextCycle != NULL);

    if(state != WAIT){
     nextCycle = (stack_game_color*)malloc(sizeof(stack_game_color));
     nextCycle->color_type = random(0, 4);
     nextCycle->next = NULL;
     oldSGC->next = nextCycle;
     level++;
     sprintf(TEXT_MSG, "Level: %d", level);
     Serial.println(TEXT_MSG);
    }
  }while(state == GAME);
  FreeStackGameColor();
}

void Game::ShowSequence(){
  Serial.println("ShowSequence Enter");
  stack_game_color *tmp = SGC;
  do{
    Serial.println(tmp->color_type);
    if(tmp->color_type == 0){
      LightOn(led_red);
      tone(buzzer, TONE_RED);
      delay(LED_GAME_DELAY_DIFICULTY/2);
      LightOff(led_red);
      delay(LED_GAME_DELAY_DIFICULTY/2);
    }else if(tmp->color_type == 1){
      LightOn(led_green);
      tone(buzzer, TONE_GREEN);
      delay(LED_GAME_DELAY_DIFICULTY/2);
      LightOff(led_green);
      delay(LED_GAME_DELAY_DIFICULTY/2);
    }else if(tmp->color_type == 2){
      LightOn(led_blue);
      tone(buzzer, TONE_BLUE);
      delay(LED_GAME_DELAY_DIFICULTY/2);
      LightOff(led_blue);
      delay(LED_GAME_DELAY_DIFICULTY/2);
    }else if(tmp->color_type == 3){
      LightOn(led_yellow);
      tone(buzzer, TONE_YELLOW);
      delay(LED_GAME_DELAY_DIFICULTY/2);
      LightOff(led_yellow);
      delay(LED_GAME_DELAY_DIFICULTY/2);
    }
    tmp = tmp->next;
  }while(tmp != NULL);

  noTone(buzzer);
  Serial.println("ShowSequence Exit");
}

/* END GAME CLASS */

Game *game;

// Setup Arduino.
void setup() {
  Serial.begin(9600);
  pinMode(buzzer,     OUTPUT);
  pinMode(led_red,    OUTPUT);
  pinMode(led_green,  OUTPUT);
  pinMode(led_blue,   OUTPUT);
  pinMode(led_yellow, OUTPUT);
  pinMode(button_00,  INPUT);
  pinMode(button_01,  INPUT);
  pinMode(button_02,  INPUT);
  pinMode(button_03,  INPUT);
  randomSeed(analogRead(0));
  
  game = new Game();
  game->InitGame();
}

// Loop Arduino.
void loop() {
  /*Serial.println(STATE_GAME);*/
  do{
   /*sprintf(TEXT_MSG, "Button00 [%d]  - Button01 [%d] - Button02 [%d] - Button03 [%d]", checkButton(button_00), checkButton(button_01), checkButton(button_02), checkButton(button_03));
   Serial.println(TEXT_MSG);*/
   MusicBuzzer(melody_music, sizeof(melody_music));
   if(checkButton(button_00) == HIGH || checkButton(button_01) == HIGH || checkButton(button_02) == HIGH || checkButton(button_03) == HIGH){
    Serial.println("GAME START");
    STATE_GAME = GAME;
   }
  }while(STATE_GAME == WAIT);
  game->RestartGame();
  game->LoopGame();
  STATE_GAME = WAIT;
}
