#include "console.h"
#include <iostream>
#include <ctime>
#include <string>

#define BOARD_SIZE 15
#define MOVE_DELAY 15
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

bool gameOver;
int x, y, appleX, appleY, score;
int tailX[BOARD_SIZE-2], tailY[BOARD_SIZE-2];
console::Key dir = console::K_LEFT;
int nTail;
int delay = 250 / MOVE_DELAY;
long count = 0;

//기본값으로 세팅
void Setup() {
	gameOver = false;
	x = BOARD_SIZE / 2;
	y = BOARD_SIZE / 2;
  nTail = 0;
	appleX = rand() % (BOARD_SIZE-3)+1;
	appleY = rand() % (BOARD_SIZE-3)+1;
	score = 0;
  count = 0;
}
//랜덤으로 만들어낸 사과가 꼬리랑 겹치는 위치인지 체크하고 다시 생성.
void isAppleOnTail(int appleX, int appleY, int tailX[], int tailY[], int nTail) {
    for (int k = 0; k < nTail; k++) {
        if (appleX == tailX[k] && appleY == tailY[k]) {
          	appleX = rand() % (BOARD_SIZE-3)+1;
          	appleY = rand() % (BOARD_SIZE-3)+1;
            isAppleOnTail(appleX, appleY, tailX, tailY, nTail);
        }
        else 
          break;
    }
}

void draw() {
  //첫번째 벽 출력
  console::draw(0,0,WALL_LEFT_TOP_STRING);
	for (int i = 1; i <= BOARD_SIZE-1; i++)
		console::draw(i,0,WALL_HORIZONTAL_STRING);
	console::draw(BOARD_SIZE,0,WALL_RIGHT_TOP_STRING);

  //중간 출력
  //apple과 snake, 벽을 같이 출력 시킨다.
	for (int i = 1; i < BOARD_SIZE; i++) {
		for (int j = 0; j <= BOARD_SIZE; j++) {
			if (j == 0) console::draw(0,i, WALL_VERTICAL_STRING);
			  if (j == BOARD_SIZE) console::draw(BOARD_SIZE,i, WALL_VERTICAL_STRING);
			    if (j == x && i == y)
				    console::draw(j,i, SNAKE_STRING);
			    else if (j == appleX && i == appleY){
            //apple위치와 꼬리의 위치가 겹치는지 확인
            isAppleOnTail(appleX, appleY, tailX, tailY, nTail);
            console::draw(j,i,APPLE_STRING);
            }
			    else {
				    bool print = false;
				    for (int k = 0; k < nTail; k++) {
					    if (tailX[k] == j && tailY[k] == i) {
						    console::draw(j,i, SNAKE_BODY_STRING);
						    print = true;
				  	  }
				    }
				  if (!print && i>1 && i<BOARD_SIZE-1 && j>0 && j<BOARD_SIZE)
				  	console::draw(j,i," ");
		  	  }
		}
		std::cout << "\n";
	}
  //마지막 벽 출력
	console::draw(0,BOARD_SIZE,WALL_LEFT_BOTTOM_STRING);
	for (int i = 1; i <= BOARD_SIZE-1; i++)
		console::draw(i,BOARD_SIZE,WALL_HORIZONTAL_STRING);
	console::draw(BOARD_SIZE,BOARD_SIZE,WALL_RIGHT_BOTTOM_STRING);
  //점수 출력
  std::string scoreS = "Score : " + std::to_string(score);
	console::draw(BOARD_SIZE/2-scoreS.length()/2,BOARD_SIZE+1,scoreS);
  //벽에 부딪혔을때
  if (x > BOARD_SIZE-1 || x < 1 || y > BOARD_SIZE-1 || y < 1){
      console::draw(BOARD_SIZE/2-4, BOARD_SIZE/2, "You lose!");
      console::draw(BOARD_SIZE/2-10, BOARD_SIZE/2+1, "Press Enter to Restart.");
			gameOver = true;
  }
  //머리가 꼬리에 닿았을때
	for (int i = 0; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y){
      console::draw(BOARD_SIZE/2-4, BOARD_SIZE/2, "You lose!");
      console::draw(BOARD_SIZE/2-10, BOARD_SIZE/2+1, "Press Enter to Restart.");
			gameOver = true;
    }
}

//누른 키가 무엇인지 파악하는 함수
void handleInput() {
  if (console::key(console::K_LEFT)&&dir != console::K_RIGHT) {
    dir = console::K_LEFT;
  }
  if (console::key(console::K_RIGHT)&&dir!=console::K_LEFT) {
    dir = console::K_RIGHT;
  }
  if (console::key(console::K_UP)&&dir!=console::K_DOWN) {
    dir = console::K_UP;
  }
  if (console::key(console::K_DOWN)&&dir!=console::K_UP) {
    dir = console::K_DOWN;
  }
  if (console::key(console::K_ESC)) {
    gameOver=true; 
  }
}

void restrictInScreen() {
  // x, y 위치를 화면의 최대 크기에서 벗어나지 않게 한다.
  if (x < 0)
    x = 0;
  if (x >= console::SCREEN_WIDTH)
    x = console::SCREEN_WIDTH - 1;
  if (y < 0)
    y = 0;
  if (y >= console::SCREEN_HEIGHT)
    y = console::SCREEN_HEIGHT - 1;
}

//뱀의 움직임을 파악
void Logic() {
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++) {
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	if (dir == console::K_LEFT) {
    x--;
  }
  else if (dir == console::K_RIGHT) {
    x++;
  }
  else if (dir == console::K_UP) {
    y--;
  }
  else if (dir == console::K_DOWN) {
    y++;
  }
	
	if (x == appleX && y == appleY) {
		score += 10;
	  appleX = rand() % (BOARD_SIZE-3)+1;
	  appleY = rand() % (BOARD_SIZE-3)+1;
		nTail++;
	}
}
//게임을 이겼는지
bool gameWin(){
  if(nTail==(BOARD_SIZE-2)*(BOARD_SIZE-2))
    return true;
  else return false;
}

void game() {

  // 콘솔 라이브러리를 초기화한다.
  console::init();
  Setup();

  while (!gameOver) {
    // 화면을 지운다.
    console::clear();

    handleInput();
    restrictInScreen();
    if (count % delay == 0) {
      Logic();
    }
    count++;
    draw();
    
    if(gameWin()){
      console::draw(BOARD_SIZE/2-4, BOARD_SIZE/2, "You Win!");
      return;
    }

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    console::wait();
  }
  
}
int main(){
    game();
    while(!console::key(console::K_ESC)){
      if (console::key(console::K_ENTER)){
        game();
      }
    }
    
}