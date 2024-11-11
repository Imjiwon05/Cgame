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
	COORD coord;                                                            // �ܼ� ȭ���� ���� �� ��ǥ�� ����
	coord.X = x;
	coord.Y = y;                                                            //�Լ��� ���޵� x, y ���� COORD ����ü�� X, Y ����� ���� �Ҵ�
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetConsole() {
	system("My Game");                                                      // �ܼ�â �̸� ����
	system("mode con:cols=100 lines=50");                                  // �ܼ�â ũ�⼳��
}

void KeyInput() {
	if (CHARACTER.Active) {
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {            //Ư��Ű ���� Ȯ��(����ȭ��ǥ) & 0x8000 ���� ���ȴ��� Ȯ���ϴ� ��Ʈ ����
			CHARACTER.x--;
			if (CHARACTER.x < 0)                            //ȭ�� ����� �ʰ�
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


	CONSOLE_CURSOR_INFO cursorInfo = { 0, };  //Ŀ�� �����
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	while (1) {
		KeyInput();

		system("cls");
		gotoxy(CHARACTER.x, CHARACTER.y);
		printf("��");

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			break;  // ESC Ű�� ���� ����
		}
		// ��� ��� (CPU ���� ����)
		Sleep(100);
	}



}

int main() {
	SetConsole();


	while (1) {
		system("cls");

		gotoxy(30, 5);
		printf("ź�� ���ϱ�");
		gotoxy(25, 10);
		printf("1. ���� ����");
		gotoxy(25, 12);
		printf("2. ���� ����");
		gotoxy(25, 14);
		printf("3. ����");
		gotoxy(20, 20);
		printf("���ϴ� �޴��� ��ȣ�� �Է��ϼ���: ");

		int choice;
		scanf_s("%d", &choice);

		if (choice == 1) {
			game_main();
		}

		if (choice == 2) {
			system("cls");
			gotoxy(40, 5);
			printf("���� ����");
			gotoxy(25, 12);
			printf("�������� ���ƿ��� ź������ ���ϴ� �����Դϴ�");
			gotoxy(25, 13);
			printf("���ϴ� �ð��� ����Ͽ� ������ �ö󰩴ϴ�");
			gotoxy(25, 14);
			printf("������ �ִ��� ������.");
			gotoxy(25, 16);
			printf("�ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�...");
			_getch();                                                           //����ڰ� Ű�� ���� ������ ���α׷��� ������ �Ͻ� ����
		}

		else if (choice == 3) {
			break;
		}

		// ���� ���� ����
	}
	return 0;


}