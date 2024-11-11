#include <windows.h>
#include <stdio.h>

#define CONSOLE_SIZE_X 100
#define CONSOLE_SIZE_Y 50
#define Y_SPACE 1

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
	system("mode con:cols=100 lines=50");                                  // 콘솔창 크기설정

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

//캐릭터 지우기
void ClearCharacter(int x, int y) {                                   //렉 걸림 방지를 위해 움직일때 그 곳만 지우기
	gotoxy(x, y);
	printf(" ");
}

//캐릭터 다시 그리기
void DrawCharacter(int x, int y) {                                    //변경한 위치에 다시 나타나기
	gotoxy(x, y);
	printf("■");
}

//텍스트 색상 정하기
void SetColor(WORD textColor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, textColor);
}


//방향키 입력
void KeyInput() {
	if (CHARACTER.Active) {
		int oldX = CHARACTER.x;
		int oldY = CHARACTER.y;

		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {            //특정키 상태 확인(왼쪽화살표) & 0x8000 현재 눌렸는지 확인하는 비트 연산
			CHARACTER.x--;
			if (CHARACTER.x < 0)                            //화면 벗어나지 않게
				CHARACTER.x = 0;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			CHARACTER.x++;
			if (CHARACTER.x > CONSOLE_SIZE_X - 2)
				CHARACTER.x = CONSOLE_SIZE_X - 2;
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			CHARACTER.y--;
			if (CHARACTER.y < 0)
				CHARACTER.y = 0;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			CHARACTER.y++;
			if (CHARACTER.y > CONSOLE_SIZE_Y - Y_SPACE)
				CHARACTER.y = CONSOLE_SIZE_Y - Y_SPACE;
		}

		if (oldX != CHARACTER.x || oldY != CHARACTER.y) {           //캐릭터가 움직였을 때만 화면 다시
			ClearCharacter(oldX, oldY);                             //원래 있던 자리 clear
			DrawCharacter(CHARACTER.x, CHARACTER.y);                //다시 캐릭터 그리기
		}
	}
}

//=======================================게임 메인 함수=================================================
void game_main() {
	CHARACTER.x = 25;
	CHARACTER.y = 26;
	CHARACTER.Active = 1;

	system("cls");                                               // 게임 시작 시 한 번만 화면을 지운다
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


