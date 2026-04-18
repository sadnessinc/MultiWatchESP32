#include "screenSnake.h"

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>

#include "core/AppState/AppState.h"
#include "core/Input/Input.h"
#include "additional/AdditionalLib.h"
#include "config/Pins.h"

extern Adafruit_SSD1306 display;
extern InputService input;

const int CELL = 4;
const int FIELD_Y = 16;
const int PADDING = CELL;

const int FIELD_WIDTH = SCREEN_WIDTH - 2*PADDING;
const int FIELD_HEIGHT = SCREEN_HEIGHT - FIELD_Y - PADDING;

const int GRID_W = FIELD_WIDTH / CELL;
const int GRID_H = FIELD_HEIGHT / CELL;

const int FIELD_W_PX = GRID_W * CELL;
const int FIELD_H_PX = GRID_H * CELL;

const int MAX_SNAKE = GRID_W * GRID_H;


enum Dir { UP, DOWN, LEFT, RIGHT };
Dir dir = RIGHT;
Dir nextDir = RIGHT;

struct Point 
{
    int x;
    int y;
};

Point snake[MAX_SNAKE];
int snakeLen = 3;

Point food;
bool gameOver = false;
bool gameWin = false;

unsigned long lastTick = 0;
const unsigned long tickMs = 125;

bool onSnake(int x, int y){
    for(int i = 0; i < snakeLen; i++){
        if(snake[i].x == x && snake[i].y == y) return true;
    }
    return false;
}

void spawnFood() {
    while(onSnake(food.x, food.y)){
        food.x = random(0, GRID_W);
        food.y = random(0, GRID_H);
    }
}

void resetGame(){
    snakeLen = 3; 
    snake[0] = {5,3};
    snake[1] = {4,3};
    snake[2] = {3,3};
    dir = RIGHT;
    nextDir = RIGHT;
    gameOver = false;
    gameWin = false;
    spawnFood();
}

void drawCell(int x, int y, bool fill){
    int px = x * CELL;
    int py = FIELD_Y + y * CELL;
    if (fill){
        display.fillRect(px, py, CELL, CELL, SSD1306_WHITE);
    }else{
        display.drawRect(px, py, CELL, CELL, SSD1306_WHITE);
    }
}

void drawGame(){

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);



    if (gameOver){
        display.clearDisplay();
        printCenter("GAME OVER");
        display.setTextSize(1);

        int x = display.getCursorX();
        int y = display.getCursorY();
        display.setCursor(x+8,y);
        display.print("press OK to restart");
        return;
    }

    if (gameWin){
        display.clearDisplay();
        printCenter("YOU WIN");
        display.setTextSize(1);

        int x = display.getCursorX();
        int y = display.getCursorY();
        display.setCursor(x+8,y);
        display.print("press OK to restart");
        return;
    }

    display.drawRect(
        0,
        FIELD_Y,
        FIELD_WIDTH+1,
        FIELD_HEIGHT+1,
        SSD1306_WHITE
        );

    drawCell(food.x, food.y, true);

    for (int i=0; i < snakeLen; i++){
        drawCell(snake[i].x, snake[i].y, true);
    }


}

void updateGame();

void drawTitle(){
    const char* text = "SNAKE";

    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.print(text);
    }

void screenSnake(){
    display.clearDisplay();

    if (gameOver || gameWin) {
    drawGame();
    display.display();
    return;
    }

    if (millis() - lastTick >= tickMs) {
    lastTick = millis();
    updateGame();
    drawGame();
    drawTime();
    drawWiFiIcon();
    drawTitle();
    display.display();
    } 

}

void updateSnakeInput(){
    if (input.pressed(BTN_BACK)) setScreen(MENU);

    if (input.pressed(BTN_OK) && (gameOver||gameWin))resetGame(); 


    if (input.pressed(BTN_UP) && dir != DOWN)nextDir = UP;
    if (input.pressed(BTN_DOWN) && dir != UP) nextDir = DOWN;
    if (input.pressed(BTN_LEFT) && dir != RIGHT) nextDir = LEFT;
    if (input.pressed(BTN_RIGHT) && dir != LEFT) nextDir = RIGHT;   
}

void updateGame(){
    dir = nextDir;
    Point newHead = snake[0];

    switch (dir)
    {
    case UP:
        newHead.y--;
        break;

    case DOWN:
        newHead.y++;
        break;
    
    case LEFT:
        newHead.x--;
        break;
    
    case RIGHT:
        newHead.x++;
        break;
    }
    
    //wall gameover
    if (newHead.x < 0 || newHead.x >= GRID_W || newHead.y < 0 || newHead.y >= GRID_H){
        gameOver = true;
        return;
    }

    //self gameover
    for (int i = 0; i < snakeLen; i++) {
    if (snake[i].x == newHead.x && snake[i].y == newHead.y) {
      gameOver = true;
      return;
    }
  }

  //body move
  for (int i = snakeLen; i > 0; i--) {
    snake[i] = snake[i - 1];
  }
  snake[0] = newHead;

  //eat
  if (newHead.x == food.x && newHead.y == food.y) {
    snakeLen++;

    //if (snakeLen > MAX_SNAKE) snakeLen = MAX_SNAKE;
    if (snakeLen >= GRID_W * GRID_H) {
    gameWin = true;
    return;
    }
    spawnFood();
  }


}
