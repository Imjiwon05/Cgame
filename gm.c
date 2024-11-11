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

void gotoxy(int x, int y) {
	COORD coord;                                                            // 콘솔 화면의 문자 셀 좌표를 정의
	coord.X = x;
	coord.Y = y;                                                            //함수에 전달된 x, y 값을 COORD 구조체의 X, Y 멤버에 각각 할당
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetConsole() {
	system("My Game");                                                      // 콘솔창 이름 설정
	system("mode con:cols=100 lines=50");                                  // 콘솔창 크기설정
}

void KeyInput() {
	if (CHARACTER.Active) {
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
	}
}

void game_main() {
	CHARACTER.x = 25;
	CHARACTER.y = 26;
	CHARACTER.Active = 1;


	CONSOLE_CURSOR_INFO cursorInfo = { 0, };  //커서 숨기기
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	while (1) {
		KeyInput();

		system("cls");
		gotoxy(CHARACTER.x, CHARACTER.y);
		printf("■");

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			break;  // ESC 키로 게임 종료
		}
		// 잠시 대기 (CPU 사용률 감소)
		Sleep(100);
	}



}

int main() {
	SetConsole();


	while (1) {
		system("cls");

		gotoxy(30, 5);
		printf("탄막 피하기");
		gotoxy(25, 10);
		printf("1. 게임 시작");
		gotoxy(25, 12);
		printf("2. 게임 설명");
		gotoxy(25, 14);
		printf("3. 종료");
		gotoxy(20, 20);
		printf("원하는 메뉴의 번호를 입력하세요: ");

		int choice;
		scanf_s("%d", &choice);

		if (choice == 1) {
			game_main();
		}

		if (choice == 2) {
			system("cls");
			gotoxy(40, 5);
			printf("게임 설명");
			gotoxy(25, 12);
			printf("랜덤으로 날아오는 탄막들을 피하는 게임입니다");
			gotoxy(25, 13);
			printf("피하는 시간에 비례하여 점수가 올라갑니다");
			gotoxy(25, 14);
			printf("점수를 최대한 얻어보세요.");
			gotoxy(25, 16);
			printf("아무 키나 누르면 메인 메뉴로 돌아갑니다...");
			_getch();                                                           //사용자가 키를 누를 때까지 프로그램의 실행을 일시 중지
		}

		else if (choice == 3) {
			break;
		}

		// 게임 로직 구현
	}
	return 0;


}