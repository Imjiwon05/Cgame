#include <windows.h>
#include <stdio.h>

#define CONSOLE_SIZE_X 100
#define CONSOLE_SIZE_Y 50
#define Y_SPACE 1

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

//캐릭터 다시 그리기
void DrawCharacter(int x, int y) {                                    //변경한 위치에 다시 나타나기
	SetColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(x, y);
	printf("●");
	SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}


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
			Sleep(2000);
			system("cls");
			start_scene(1, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			Sleep(500);
			start_scene(2, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			Sleep(500);
			start_scene(3, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			Sleep(500);
			start_scene(4, FOREGROUND_RED | FOREGROUND_GREEN );
			Sleep(500);
			start_scene(5, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			Sleep(500);
			start_scene(6, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			Sleep(500);
			start_scene(7, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			Sleep(300);
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
			_getch(); // 사용자 입력 대기
			system("cls");
		}
	}
}

//=======================================게임 메인 함수=================================================
void game_main() {
	int gameWidth = 80;                 //게임 배경 가로 세로 벽 크기
	int gameHeight = 38;

	CHARACTER.x = gameWidth / 2;
	CHARACTER.y = gameHeight / 2;
	CHARACTER.Active = 1;

	system("cls");                                               // 게임 시작 시 한 번만 화면을 지운다
	drawWall(gameWidth, gameHeight);											//가로 50, 세로 50 벽 세우기
	DrawCharacter(CHARACTER.x, CHARACTER.y);

	HideCursor_();

	while (1) {
		KeyInput();


		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
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
			printf("◈ 게임 도중 ESC키를 눌러 일시정지가 가능합니다.");
			gotoxy(20, 16);
			printf("◈ 탄막을 피해 오래 버티며 높은 점수를 얻으세요.");
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);             //FOREGROUND_WHITE는 없어서 RGB 섞은 흰색으로
			gotoxy(20, 20);
			printf("아무 키나 누르면 메인 메뉴로 돌아갑니다...");
			_getch();                        //사용자가 키를 누를 때까지 프로그램 일시 중지
			break;
		case 3:
			return 0;                       //break하면 메인메뉴로 가져서 return으로 프로그램 종료
		default:
			system("cls");
			gotoxy(20, 22);
			printf("잘못된 입력입니다. 아무 키나 누르면 메인 메뉴로 돌아갑니다...");
			_getch();
			break;
		}

		// 게임 로직 구현
	}
	return 0;
}


