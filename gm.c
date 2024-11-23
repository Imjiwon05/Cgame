#include <windows.h>
#include <stdio.h>
#include <stdbool.h>  //충돌 감지하기 위해
#include <conio.h>    //_kbhit()로 키 입력 확인하기 위해
#include <mmsystem.h>  //배경음악
#pragma comment(lib, "winmm.lib") //Windows 멀티미디어 라이브러리를 프로젝트에 연결

#define MAX_BULLETS 20   //처음 탄막 갯수

void gameover();
int score = 0;
int highscore = 0;

//배경음악 재생
void playBackgroundMusic(song) {
	if (song == 1) {
		PlaySound(L"BG.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (song == 2) {
		PlaySound(L"crash.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (song == 3) {
		PlaySound(L"main.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
}

//배경음악 정지
void stopBackgroundMusic() {
	PlaySound(NULL, NULL, 0);
}

//1. 탄막 구조체
struct Bullet {
	int x, y;
	int direction; // 0: 왼쪽에서 오른쪽, 1: 오른쪽에서 왼쪽, 2: 위에서 아래, 3: 아래에서 위
	int active;
};
struct Bullet* bullets = NULL;

//탄막 난이도 조절
int NextMaxBullets = MAX_BULLETS;
void adjustDifficulty() {
	int newMaxBullets = NextMaxBullets;

	if (score >= 4000 && NextMaxBullets < 80) {
		newMaxBullets = 80;
	}
	else if (score >= 3000 && NextMaxBullets < 50) {
		newMaxBullets = 50;
	}
	else if (score >= 2000 && NextMaxBullets < 40) {
		newMaxBullets = 40;
	}
	else if (score >= 1000 && NextMaxBullets < 30) {
		newMaxBullets = 30;
	}
	else if (score >= 5000 && NextMaxBullets < 150) {
		newMaxBullets = 150;
	}

	if (newMaxBullets > NextMaxBullets) {
		struct Bullet* newBullets = (struct Bullet*)realloc(bullets, sizeof(struct Bullet) * newMaxBullets);
		if (newBullets != NULL) {
			bullets = newBullets;
			for (int i = NextMaxBullets; i < newMaxBullets; i++) {
				bullets[i].active = 0;
			}
			NextMaxBullets = newMaxBullets;
		}
		else {
			printf("메모리 재할당 실패\n");
		}
	}
}

//텍스트 색상 정하기
void SetColor(WORD textColor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, textColor);
}

struct Character {
	int x;
	int y;
	int Active;
};
struct Character CHARACTER;

HANDLE hConsole;


void ShowCursor_() {                                                        //커서 보이게
	CONSOLE_CURSOR_INFO cursorInfo = { 1, TRUE };
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void HideCursor_() {														//커서 안보이게
	CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

//커서 위치
void gotoxy(int x, int y) {
	COORD coord = { (SHORT)x, (SHORT)y };                                  // 콘솔 화면의 문자 셀 좌표를 정의
	SetConsoleCursorPosition(hConsole, coord);
}

//콘솔창 띄우기
void SetConsole() {
	system("My Game");                                                      // 콘솔창 이름 설정
	system("mode con:cols=82 lines=47");                                  // 콘솔창 크기설정

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

//캐릭터 지우기
void ClearCharacter(int x, int y) {                                   //렉 걸림 방지를 위해 움직일때 그 곳만 지우기
	gotoxy(x, y);
	printf(" ");
}

bool shieldActive = false; // 방어막 상태

//캐릭터 다시 그리기
void DrawCharacter(int x, int y) {                                    //변경한 위치에 다시 나타나기
	if (shieldActive) {
		SetColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	else {
		SetColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	gotoxy(x, y);
	printf("●");
	SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ ITEM ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//아이템 구조체
struct Item {
	int x, y;
	int type;     // 0: 점수 추가, 1: 방어막, 2: 탄막 제거
	bool active;
};
struct Item currentItem = { 0, 0, -1, false };

ULONGLONG itemEffectStartTime = 0;               //아이템 효과가 뭔지 나타내는데 시간 측정
bool showItemEffect = false;
int itemEffectType = -1;

//bool shieldActive = false;      // 방어막 상태 <- 충돌 감지 위에 있어야 해서 위에 코딩

//아이템 생성
void createItem(int gameWidth, int gameHeight) {
	if (score % 1000 == 0 && score > 0 && !currentItem.active) {      //점수가 1000의 배수일 때 아이템 그리기
		currentItem.x = rand() % (gameWidth - 2) + 1;
		currentItem.y = rand() % (gameHeight - 2) + 1;
		currentItem.type = rand() % 3;
		currentItem.active = true;
	}
}

//아이템 그리기
void drawItem() {
	if (currentItem.active) {
		gotoxy(currentItem.x, currentItem.y);
		SetColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		printf("▣");
		SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	}
}

// 아이템을 먹었을 때 무슨 능력인지
void activateItemEffect(int type) {
	itemEffectType = type;
	showItemEffect = true;
	itemEffectStartTime = GetTickCount64();
}

//아이템 보여주기
void updateItemEffect() {
	ULONGLONG currentTime = GetTickCount64();

	if (showItemEffect) {
		if (currentTime - itemEffectStartTime < 2000) { // 3초 동안 표시
			gotoxy(83, 10);
			SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("┏━━━━━━━━━━━━━━━━━━┓");
			gotoxy(83, 11);
			switch (itemEffectType) {
			case 0:
				printf("┃    추가 점수+    ┃");
				break;
			case 1:
				printf("┃   방어막 활성화  ┃");
				break;
			case 2:
				printf("┃    탄막 BOOM     ┃");
				break;
			}
			gotoxy(83, 12);
			printf("┗━━━━━━━━━━━━━━━━━━┛");
		}
		else {
			showItemEffect = false;
			gotoxy(83, 10);
			printf("           ");
			gotoxy(83, 11);
			printf("           ");
			gotoxy(83, 12);
			printf("           ");
		}
	}
}

//아이템 효과
void applyItemEffect() {
	if (CHARACTER.x == currentItem.x && CHARACTER.y == currentItem.y) {
		switch (currentItem.type) {
			case 0:
				score += 400;
				activateItemEffect(0);
				break;
			case 1:
				shieldActive = true;
				activateItemEffect(1);
				break;
			case 2:
				for (int i = 0; i < NextMaxBullets; i++) {
					bullets[i].active = 0;
				}
				activateItemEffect(2);
				break;
			}
		currentItem.active = false;
	}
}
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ ITEM ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 탄막 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
//2.탄막 생성
void createBullet(int gameWidth, int gameHeight) {
	int bulletsToCreate = NextMaxBullets / 10;     // 현재 최대 탄막 수의 10%만큼 생성
	int created = 0;
	for (int i = 0; i < NextMaxBullets && created < bulletsToCreate; i++) {
		if (!bullets[i].active) {
			bullets[i].active = 1;
			bullets[i].direction = rand() % 4;
			created++;

			switch (bullets[i].direction) {
			case 0: // 왼쪽에서 오른쪽
				bullets[i].x = 1;
				bullets[i].y = rand() % (gameHeight - 2) + 1;
				break;
			case 1: // 오른쪽에서 왼쪽
				bullets[i].x = gameWidth - 2;
				bullets[i].y = rand() % (gameHeight - 2) + 1;
				break;
			case 2: // 위에서 아래
				bullets[i].x = rand() % (gameWidth - 2) + 1;
				bullets[i].y = 1;
				break;
			case 3: // 아래에서 위
				bullets[i].x = rand() % (gameWidth - 2) + 1;
				bullets[i].y = gameHeight - 2;
				break;
			}
			break;
		}
	}
}

//3. 탄막 이동
void moveBullets(int gameWidth, int gameHeight) {
	for (int i = 0; i < NextMaxBullets; i++) {
		if (bullets[i].active) {
			switch (bullets[i].direction) {
			case 0: bullets[i].x++; break;
			case 1: bullets[i].x--; break;
			case 2: bullets[i].y++; break;
			case 3: bullets[i].y--; break;
			}

			// 화면 밖으로 나가면 비활성화
			if (bullets[i].x <= 0 || bullets[i].x >= gameWidth - 1 ||
				bullets[i].y <= 0 || bullets[i].y >= gameHeight - 1) {
				bullets[i].active = 0;
			}
		}
	}
}

//4, 탄막 그리기
void drawBullets() {
	for (int i = 0; i < NextMaxBullets; i++) {
		if (bullets[i].active) {
			gotoxy(bullets[i].x, bullets[i].y);
			printf("*");
		}
	}
}
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 탄막 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

//게임화면 벽 만들기
void drawWall(int width, int height) {
	int i, j;

	// 상단 벽
	for (i = 0; i < width; i++) {
		gotoxy(i, 0);
		printf("■");
	}

	// 하단 벽
	for (i = 0; i < width; i++) {
		gotoxy(i, height - 1);
		printf("■");
	}

	// 좌우 벽
	for (i = 1; i < height - 1; i++) {
		gotoxy(0, i);
		printf("■");
		gotoxy(width - 1, i);
		printf("■");
	}
	gotoxy(83, 2);
	printf("ESC : 메인화면");
}

//점수
void displayScore(score_x, score_y) {
	gotoxy(score_x, score_y);
	SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("Score: %d", score);
	gotoxy(score_x, score_y - 1);
	printf("최고 점수 %d", highscore);
}

// 충돌 감지 함수
bool checkCollision() {
	for (int i = 0; i < NextMaxBullets; i++) {
		if (bullets[i].active) {
			if (bullets[i].x == CHARACTER.x && bullets[i].y == CHARACTER.y) {
				if (shieldActive) {      //// 방어막이 있으면 충돌 무시
					shieldActive = false;
					return false;
				}
				return true; // 방어막 없으면 충돌 발생
			}
		}
	}
	return false; // 충돌 없음
}

//방향키 입력
void KeyInput() {
	if (CHARACTER.Active) {
		int oldX = CHARACTER.x;
		int oldY = CHARACTER.y;

		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {            //특정키 상태 확인(왼쪽화살표) & 0x8000 현재 눌렸는지 확인하는 비트 연산
			CHARACTER.x--;
			if (CHARACTER.x < 1)                            //화면 벗어나지 않게
				CHARACTER.x = 1;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			CHARACTER.x++;
			if (CHARACTER.x > 78)
				CHARACTER.x = 78;
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			CHARACTER.y--;
			if (CHARACTER.y < 1)
				CHARACTER.y = 1;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			CHARACTER.y++;
			if (CHARACTER.y > 36)
				CHARACTER.y = 36;
		}

		if (oldX != CHARACTER.x || oldY != CHARACTER.y) {           //캐릭터가 움직였을 때만 화면 다시
			ClearCharacter(oldX, oldY);                             //원래 있던 자리 clear
			DrawCharacter(CHARACTER.x, CHARACTER.y);                //다시 캐릭터 그리기
		}
	}
}

void start_scene(int c, char color) {
	SetColor(color);
	system("cls");
	if (c == 1 || c == 2 || c == 3 || c == 4 || c == 5 || c == 6 || c == 7) {
		gotoxy(15, 3);
		printf("             ________________________________________________");
		gotoxy(15, 4);
		printf("            /                                                \\");
		gotoxy(15, 5);
		printf("           |    _________________________________________     |");
		gotoxy(15, 6);
		printf("           |   |                                         |    |");
		gotoxy(15, 7);
		printf("           |   |       ___  __ __   ___   __ ____        |    |");
		gotoxy(15, 8);
		printf("           |   |      // \\\\ || ||  // \\\\  || || \\\\       |    |");
		gotoxy(15, 9);
		printf("           |   |      ||=|| \\\\ // ((   )) || ||  ))      |    |");
	}
	if (c == 2 || c == 3 || c == 4 || c == 5 || c == 6 || c == 7) {
		gotoxy(15, 10);
		printf("           |   |      || ||  \\V/   \\\\_//  || ||_//       |    |");
		gotoxy(15, 11);
		printf("           |   |                                         |    |");
		gotoxy(15, 12);
		printf("           |   |                                         |    |");
		gotoxy(15, 13);
		printf("           |   |        ___   ___  ___  ___  ____        |    |");
		gotoxy(15, 14);
		printf("           |   |       // \\\\ // \\\\ ||\\\\//|| ||           |    |");
		gotoxy(15, 15);
		printf("           |   |      (( ___ ||=|| || \\/ || ||==         |    |");
	}
	if (c == 3 || c == 4 || c == 5 || c == 6 || c == 7) {
		gotoxy(15, 16);
		printf("           |   |       \\\\_|| || || ||    || ||___        |    |");
		gotoxy(15, 17);
		printf("           |   |                                         |    |");
		gotoxy(15, 18);
		printf("           |   |_________________________________________|    |");
		gotoxy(15, 19);
		printf("           |                                                  |");
		gotoxy(15, 20);
		printf("            \\_________________________________________________/");
		gotoxy(15, 21);
		printf("                   \\___________________________________/");
	}
	if (c == 4 || c == 5 || c == 6 || c == 7) {
		gotoxy(29, 25);
		printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
		gotoxy(29, 26);
		printf("┃\t\t★ ★ ★ 탄막 피하기★ ★ ★\t\t┃");
		gotoxy(29, 27);
		printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
	}
}

void start() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int width, height;

	HideCursor_();
	while (1) {
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		if (width >= 100 && height >= 40) {
			system("cls");
			SetColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);         //rgb  ->  rb, rg, gb
			gotoxy(35, 15);
			printf("플레이 가능!");
			Sleep(800);
			system("cls");
			start_scene(1, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			Sleep(400);
			start_scene(2, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			Sleep(400);
			start_scene(3, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			Sleep(400);
			start_scene(4, FOREGROUND_RED | FOREGROUND_GREEN );
			Sleep(300);
			start_scene(5, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			Sleep(300);
			start_scene(6, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			Sleep(300);
			start_scene(7, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			Sleep(200);
			ShowCursor_();
			break; // 충분한 크기가 되면 루프 종료
		}
		else {
			system("cls");
			SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			gotoxy(30, 11);
			printf("※전체화면 권장※");
			gotoxy(20, 15);
			printf("콘솔 창이 너무 작습니다. 최소 100x40 크기가 필요합니다.");
			gotoxy(20, 17);
			printf("현재 크기: %dx%d", width, height);
			gotoxy(20, 19);
			printf("콘솔 창 크기를 조정한 후 아무 키나 누르세요...");
			(void)_getch(); // 사용자 입력 대기
			system("cls");
		}
	}
}

//=======================================게임 메인 함수=================================================
void game_main() {
	int gameWidth = 80;                 //게임 배경 가로 세로 벽 크기
	int gameHeight = 38;
	score = 0;

	ULONGLONG  lastScoreUpdate = GetTickCount64();  // 마지막 점수 업데이트 시간
	srand(time(NULL)); // 난수 생성기 초기화

	ULONGLONG lastBulletTime = GetTickCount64();
	ULONGLONG lastMoveTime = GetTickCount64();

	CHARACTER.x = gameWidth / 2;
	CHARACTER.y = gameHeight / 2;
	CHARACTER.Active = 1;

	bullets = (struct Bullet*)malloc(sizeof(struct Bullet) * NextMaxBullets);        //메모리 할당
	if (bullets == NULL) {
		printf("메모리 할당 실패\n");
		exit(1);
	}
	for (int i = 0; i < NextMaxBullets; i++) {
		bullets[i].active = 0;
	}

	// 아이템 상태 초기화 (아이템을 못 먹고 죽었을 때 그 자리에 있던 아이템을 없애기 위해)
	currentItem.active = false;
	itemEffectType = -1;
	showItemEffect = false;

	system("cls");                                               // 게임 시작 시 한 번만 화면을 지운다
	drawWall(gameWidth, gameHeight);											//가로 80, 세로 38 벽 세우기
	DrawCharacter(CHARACTER.x, CHARACTER.y);
	displayScore(83, 37);					// 초기 점수 표시
	playBackgroundMusic(1); //배경음 틀기


	HideCursor_();

	while (1) {
		ULONGLONG currentTime = GetTickCount64();
		KeyInput();

		adjustDifficulty();

		// 0.1초마다 새 탄막 생성
		if (currentTime - lastBulletTime >= 100) {
			createBullet(gameWidth, gameHeight);
			lastBulletTime = currentTime;
		}

		// 0.1초마다 탄막 이동
		if (currentTime - lastMoveTime >= 100) {
			system("cls"); // 화면 지우기
			drawWall(gameWidth, gameHeight);
			moveBullets(gameWidth, gameHeight);
			drawBullets();
			DrawCharacter(CHARACTER.x, CHARACTER.y);
			displayScore(83, 37);
			lastMoveTime = currentTime;
		}

		// 0.1초마다 점수 업데이트
		if (currentTime - lastScoreUpdate >= 100) {  // 100ms = 0.1초
			score += 10;
			displayScore(83, 37);
			lastScoreUpdate = currentTime;
		}

		//아이템 함수들
		createItem(gameWidth, gameHeight);
		drawItem();
		applyItemEffect();
		updateItemEffect();

		if (checkCollision()) {
			stopBackgroundMusic();
			playBackgroundMusic(2);
			Sleep(1000);
			stopBackgroundMusic();
			free(bullets);
			bullets = NULL;
			gameover();
			break;
			// 여기서 필요하다면 탄막 비활성화 또는 게임 종료 등의 추가 로직을 구현할 수 있습니다.
		}

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			stopBackgroundMusic();  //음악 멈추기
			break;  // ESC 키로 게임 종료
		}
		Sleep(50);       //프레임 조절하게 잠시 대기
	}

	ShowCursor_();

}

//===============================================메인 코딩=======================================
int main() {
	SetConsole();
	start();


	while (1) {
		ShowCursor_();
		system("cls");
		playBackgroundMusic(3);

		SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		gotoxy(20, 0);
		printf("  ___                 _      _ \n");
		gotoxy(20, 1);
		printf(" / _ \\               (_)    | |\n");
		gotoxy(20, 2);
		printf("/ /_\\ \\__   __  ___   _   __| |\n");
		gotoxy(20, 3);
		printf("|  _  |\\ \\ / / / _ \\ | | / _` |\n");
		gotoxy(20, 4);
		printf("| | | | \\ V / | (_) || || (_| |\n");
		gotoxy(20, 5);
		printf("\\_| |_/  \\_/   \\___/ |_| \\__,_|\n");

		SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		gotoxy(30, 9);
		printf("탄막 피하기");
		SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		gotoxy(25, 14);
		printf("1. 게임 시작");
		gotoxy(25, 16);
		printf("2. 게임 설명");
		gotoxy(25, 18);
		printf("3. 종료");
		gotoxy(20, 26);
		printf("원하는 메뉴의 번호를 입력하세요: ");

		int choice;
		if (scanf_s("%d", &choice) != 1) {
			// 숫자가 아닌 입력을 받았을 경우
			while (getchar() != '\n');           // 입력 버퍼 비우기
			choice = 0;							// 잘못된 입력을 나타내는 값으로 설정
		}

		switch (choice) {
		case 1:
			stopBackgroundMusic();
			game_main();
			break;
		case 2:
			system("cls");
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
			gotoxy(30, 4);
			printf("┌───────────────────────────┐");
			gotoxy(30, 5);
			printf("│         게임 설명         │");
			gotoxy(30, 6);
			printf("└───────────────────────────┘");
			gotoxy(20, 10);
			printf("◈ 캐릭터는 방향키 버튼으로 이동이 가능합니다.");
			gotoxy(20, 13);
			printf("◈ 게임 도중 ESC키를 눌러 메인 화면으로 올 수 있습니다.");
			gotoxy(20, 16);
			printf("◈ 시간이 갈 수록 날아오는 탄막이 많아집니다.");
			gotoxy(20, 19);
			printf("◈ 탄막을 피해 오래 버티며 높은 점수를 얻으세요.");
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);             //FOREGROUND_WHITE는 없어서 RGB 섞은 흰색으로
			gotoxy(20, 24);
			printf("아무 키나 누르면 아이템 설명으로 넘어갑니다...");
			(void)_getch();                        //사용자가 키를 누를 때까지 프로그램 일시 중지
			system("cls");
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
			gotoxy(30, 4);
			printf("┌────────────────────────────┐");
			gotoxy(30, 5);
			printf("│         아이템 설명        │");
			gotoxy(30, 6);
			printf("└────────────────────────────┘");
			SetColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			gotoxy(40, 10);
			printf("▣ <- 아이템");
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
			gotoxy(20, 12);
			printf("◈ 아이템은 점수가 1000의 배수일때마다 랜덤 아이템이 나옵니다.");
			gotoxy(20, 14);
			printf("◈ 아이템은 한번에 하나밖에 나오지 않습니다.");
			gotoxy(20, 16);
			printf("◈ 어쩔때는 무리하게 먹는 것보다 피하는게 우선일 수도 있습니다.");
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);             //FOREGROUND_WHITE는 없어서 RGB 섞은 흰색으로
			gotoxy(20, 24);
			printf("아무 키나 누르면 아이템 종류 설명으로 돌아갑니다...");
			(void)_getch();                        //사용자가 키를 누를 때까지 프로그램 일시 중지
			system("cls");
			SetColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			gotoxy(20, 9);
			printf("1. 추가 점수+");
			gotoxy(20, 10);
			printf("◈ 추가 점수를 얻을 수 있는 아이템입니다.");
			gotoxy(20, 11);
			printf("◈ 갑자기 점수가 올라 난이도가 확 올라갈 수 있습니다.");
			gotoxy(20, 14);
			printf("2. 방어막");
			gotoxy(20, 15);
			printf("◈ 탄막을 한번 막아주는 방어막이 생깁니다.");
			gotoxy(20, 16);
			printf("◈ 방어막이 있는 상태라면 플레이어의 색이 파란색으로 변합니다.");
			gotoxy(20, 19);
			printf("3. 탄막 BOOM");
			gotoxy(20, 20);
			printf("◈ 날아오는 모든 탄막을 없앱니다.");
			gotoxy(20, 21);
			printf("◈ 후반에 탄막이 많이 나올 때 얻으면 좋은 아이템 입니다.");
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);             //FOREGROUND_WHITE는 없어서 RGB 섞은 흰색으로
			gotoxy(20, 24);
			printf("아무 키나 누르면 메인 화면으로 돌아갑니다...");
			(void)_getch();
			break;
		case 3:
			return 0;                       //break하면 메인메뉴로 가져서 return으로 프로그램 종료
		default:
			system("cls");
			gotoxy(20, 22);
			printf("잘못된 입력입니다. 아무 키나 누르면 메인 메뉴로 돌아갑니다...");
			(void)_getch();
			break;
		}

		// 게임 로직 구현
	}
	return 0;
}


void gameover() {
	system("cls");
	free(bullets);
	bullets = NULL;
	NextMaxBullets = MAX_BULLETS;
	if (score > highscore) {
		highscore = score;    // 새로운 최고 점수 기록
	}

	gotoxy(15, 3);
	printf(" _____   ___  ___  ___ _____ ");
	gotoxy(15, 4);
	printf("|  __ \\ / _ \\ |  \\/  ||  ___|");
	gotoxy(15, 5);
	printf("| |  \\// /_\\ \\| .  . || |__  ");
	gotoxy(15, 6);
	printf("| | __ |  _  || |\/| ||  __|");
	gotoxy(15, 7);
	printf("| |_\\ \\| | | || |  | || |___ ");
	gotoxy(15, 8);
	printf(" \\____/\\_| |_/\\_|  |_/\\____/ ");
	gotoxy(15, 11);
	printf(" _____  _   _ ___________    ");
	gotoxy(15, 12);
	printf("|  _  || | | |  ___| ___ \\   ");
	gotoxy(15, 13);
	printf("| | | || | | | |__ | |_/ /   ");
	gotoxy(15, 14);
	printf("| | | || | | |  __||    /    ");
	gotoxy(15, 15);
	printf("\\ \\_/ /\\ \\_/ / |___| |\\ \\    ");
	gotoxy(15, 16);
	printf(" \\___/  \\___/\\____/\\_| \\_|   ");
	displayScore(20, 20);
	gotoxy(15, 24);
	printf("Y키를 누르면 메인 메뉴로 돌아갑니다...");
	while (1) {
		if (_kbhit()) {
			char key = _getch();
			if (key == 'y' || key == 'Y') {
				break;
			}
		}
	}
}