#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "raylib.h"

typedef struct Sprite {
  Texture2D tex;
  Vector2 pos;
} Sprite;

int main(void) {
  const int WIDTH = 480;
  const int HEIGHT = 640;
  
  srand(time(NULL));

  // init
  InitWindow(WIDTH, HEIGHT, "Clang Car Racing");
  SetTargetFPS(30);
  
  // creating the player
  Sprite car;
  car.tex = LoadTexture("res/car.png");
  car.pos.x = WIDTH/2 - car.tex.width/2;
  car.pos.y = HEIGHT*3/5;

  int car_speed = 2;
  int dx = 4;

  // creating trees
  Texture2D ttrees = LoadTexture("res/trees.png");
  Rectangle srect[3];
  for(int i = 0; i < sizeof(srect)/sizeof(srect[0]); i++) {
    srect[i].width = srect[i].height = 48;
    srect[i].x = i*srect[i].width;
    srect[i].y = 0;
  }

  // randomizing positions of trees
  int trees_num = rand()%18 + 10;
  Vector2 trees_pos[trees_num];
  for(int i = 0; i < trees_num; i++) {
    if(i < (int)trees_num/2) {
      trees_pos[i].x = rand()%WIDTH/3 + 1;

      if(trees_pos[i].x + ttrees.width/3 > WIDTH/3) {
        trees_pos[i].x -= ttrees.width/3;
      }
    } else {
      trees_pos[i].x = rand()%WIDTH/3 + 2*WIDTH/3;

      if(trees_pos[i].x + ttrees.width/3 > WIDTH) {
        trees_pos[i].x -= ttrees.width/3;
      }
    }

    trees_pos[i].y = rand()%HEIGHT;
    if(trees_pos[i].y + ttrees.height > HEIGHT) {
      trees_pos[i].y -= ttrees.height;
    }
  }
  
  // creating cars, randomizing positions of cars
  Texture2D tcars = LoadTexture("res/cars.png");
  Rectangle srect_cars[6];
  Vector2 cars_pos[6];
  float cars_speed[6] = { 0 };
  for(int i = 0; i < sizeof(srect_cars)/sizeof(srect_cars[0]); i++) {
    srect_cars[i].width = 16;
    srect_cars[i].height = 24;
    srect_cars[i].x = i*srect_cars[i].width;
    srect_cars[i].y = 0;
    
    int randnum = rand()%3+1;
    if(randnum == 1) {
      cars_pos[i].x = WIDTH/3 + WIDTH/18 - tcars.width/12;
    } else if(randnum == 2) {
      cars_pos[i].x = WIDTH/2 - tcars.width/12;
    } else {
      cars_pos[i].x = WIDTH*2/3 - WIDTH/18 - tcars.width/12;
    }

    cars_pos[i].y = rand()%HEIGHT;
    if(cars_pos[i].y + tcars.width/12 > HEIGHT) {
      cars_pos[i].y -= tcars.width/12;
    }

    cars_speed[i] = rand()%5+6;
  }

  // dark screen
  Rectangle rmuteScreen = { 0, 0, WIDTH, HEIGHT };

  int lives = 9;
  int score = 0;
  double time = 0;
  double vulnerable_time = 0;
  bool gameOver = false;
  bool gameWon = false;
  bool vulnerable = true;
  while(!WindowShouldClose()){
    if(!gameOver) {
      // process events
      if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        car.pos.x -= car_speed;
      } else if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        car.pos.x += car_speed;
      } else if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        car.pos.y -= car_speed;
      } else if(IsKeyDown(KEY_D) || IsKeyDown(KEY_DOWN)) {
        car.pos.y += car_speed;
      }
	
      // update
      for(int i = 0; i < trees_num; i++) {
        trees_pos[i].y += car_speed;

        if(trees_pos[i].y > HEIGHT) {
          trees_pos[i].y = -ttrees.height;
        }
      }

      for(int i = 0; i < sizeof(cars_pos)/sizeof(cars_pos[0]); i++) {
        cars_pos[i].y += cars_speed[i];

      	if(cars_pos[i].y > HEIGHT) {
      	  cars_pos[i].y = -tcars.height;

      	  int randnum = rand()%3+1;
      	  if(randnum == 1) {
      	    cars_pos[i].x = WIDTH/3 + WIDTH/18 - tcars.width/12;
      	  } else if(randnum == 2) {
      	    cars_pos[i].x = WIDTH/2 - tcars.width/12;
      	  } else {
      	    cars_pos[i].x = WIDTH*2/3 - WIDTH/18 - tcars.width/12;
      	  }

      	  cars_speed[i] = rand()%5+6;
        }
	
      Rectangle rec1 = { car.pos.x, car.pos.y, car.tex.width, car.tex.height };
      Rectangle rec2 = { cars_pos[i].x, cars_pos[i].y, tcars.width/6, tcars.height };
      if(CheckCollisionRecs(rec1, rec2)) {
        if(vulnerable) {
    	    lives -= 1;
    	    vulnerable = false;
    	  }
        
        car.pos.y -= car.tex.height-10;
        }
      }

      if(!vulnerable) {
      	vulnerable_time += GetFrameTime();
        if(vulnerable_time > 1) {
          vulnerable = true;
          vulnerable_time = 0;
      	}
      }

      if(lives < 0) {
        gameOver = true;
      }
    	
      if(car.pos.x < WIDTH/3 || car.pos.x + car.tex.width > WIDTH*2/3) {
        car_speed = dx/2;
      } else {
        car_speed = dx;
      }

      time += GetFrameTime();
      if(time > 1) {
        score++;
        time = 0;
      }
    	  
      if(score > 999) {
        gameWon = true;
        gameOver = true;
      }
    }

    // draw
    BeginDrawing();
    ClearBackground(WHITE);

    // draw background
    DrawRectangle(0, 0, WIDTH/3, HEIGHT, GREEN);
    DrawRectangle(WIDTH*2/3, 0, WIDTH/3, HEIGHT, GREEN);
    DrawRectangle(WIDTH/3, 0, WIDTH/3, HEIGHT, GRAY);
    DrawRectangle(WIDTH/3 + WIDTH/9 - 1, 0, 2, HEIGHT, BLACK);
    DrawRectangle(WIDTH/3 + WIDTH*2/9 - 1, 0, 2, HEIGHT, BLACK);
    
    if(!vulnerable) {
      Color col = { 0, 0, 0, 100 };
      DrawTexture(car.tex, car.pos.x, car.pos.y, col);
    } else {
      DrawTexture(car.tex, car.pos.x, car.pos.y, WHITE);
    }

    for(int i = 0; i < sizeof(srect_cars)/sizeof(srect_cars[0]); i++) {
      DrawTextureRec(tcars, srect_cars[i], cars_pos[i], WHITE);
    }

    for(int i = 0; i < trees_num; i++) {
      if(i%2 == 0) {
        DrawTextureRec(ttrees, srect[0], trees_pos[i], WHITE);
      } else if(i%3 == 0) {
        DrawTextureRec(ttrees, srect[1], trees_pos[i], WHITE);
      } else {
        DrawTextureRec(ttrees, srect[2], trees_pos[i], WHITE);
      }
    }

    char scoring[12] = { " " };
    sprintf(scoring, "%d", score);
    DrawText(scoring, 10, 0, 60, WHITE);

    sprintf(scoring, "%d", lives);
    DrawText(scoring, WIDTH-50, 0, 60, WHITE);

    if(gameOver) {
      Color color = { 0, 0, 0, 180 };
      DrawRectangle(rmuteScreen.x, rmuteScreen.y, rmuteScreen.width, rmuteScreen.height, color);
      if(gameWon) {
        DrawText("You Won!", WIDTH/11, HEIGHT/3, 90, WHITE);
      } else {
        DrawText("Game Over!", WIDTH/11, HEIGHT/3, 70, WHITE);
      }
    }

    EndDrawing();
  }
  
  UnloadTexture(tcars);
  UnloadTexture(ttrees);
  UnloadTexture(car.tex);
  CloseWindow();

  return 0;
}
