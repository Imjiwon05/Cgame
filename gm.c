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

void ShowCursor_() {                                                        //Ŀ�� ���̰�
	CONSOLE_CURSOR_INFO cursorInfo = { 1, TRUE };
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void HideCursor_() {														//Ŀ�� �Ⱥ��̰�
	CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

//Ŀ�� ��ġ
void gotoxy(int x, int y) {
	COORD coord = { (SHORT)x, (SHORT)y };                                  // �ܼ� ȭ���� ���� �� ��ǥ�� ����
	SetConsoleCursorPosition(hConsole, coord);
}

//�ܼ�â ����
void SetConsole() {
	system("My Game");                                                      // �ܼ�â �̸� ����
	system("mode con:cols=100 lines=50");                                  // �ܼ�â ũ�⼳��

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

//ĳ���� �����
void ClearCharacter(int x, int y) {                                   //�� �ɸ� ������ ���� �����϶� �� ���� �����
	gotoxy(x, y);
	printf(" ");
}

//ĳ���� �ٽ� �׸���
void DrawCharacter(int x, int y) {                                    //������ ��ġ�� �ٽ� ��Ÿ����
	gotoxy(x, y);
	printf("��");
}

//�ؽ�Ʈ ���� ���ϱ�
void SetColor(WORD textColor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, textColor);
}


//����Ű �Է�
void KeyInput() {
	if (CHARACTER.Active) {
		int oldX = CHARACTER.x;
		int oldY = CHARACTER.y;

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

		if (oldX != CHARACTER.x || oldY != CHARACTER.y) {           //ĳ���Ͱ� �������� ���� ȭ�� �ٽ�
			ClearCharacter(oldX, oldY);                             //���� �ִ� �ڸ� clear
			DrawCharacter(CHARACTER.x, CHARACTER.y);                //�ٽ� ĳ���� �׸���
		}
	}
}

//=======================================���� ���� �Լ�=================================================
void game_main() {
	CHARACTER.x = 25;
	CHARACTER.y = 26;
	CHARACTER.Active = 1;

	system("cls");                                               // ���� ���� �� �� ���� ȭ���� �����
	DrawCharacter(CHARACTER.x, CHARACTER.y);

	HideCursor_();

	while (1) {
		KeyInput();


		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			break;  // ESC Ű�� ���� ����
		}
		Sleep(50);       //������ �����ϰ� ��� ���
	}

	ShowCursor_();

}

//===============================================���� �ڵ�=======================================
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
		printf("ź�� ���ϱ�");
		SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		gotoxy(25, 14);
		printf("1. ���� ����");
		gotoxy(25, 16);
		printf("2. ���� ����");
		gotoxy(25, 18);
		printf("3. ����");
		gotoxy(20, 26);
		printf("���ϴ� �޴��� ��ȣ�� �Է��ϼ���: ");

		int choice;
		if (scanf_s("%d", &choice) != 1) {
			// ���ڰ� �ƴ� �Է��� �޾��� ���
			while (getchar() != '\n');           // �Է� ���� ����
			choice = 0;							// �߸��� �Է��� ��Ÿ���� ������ ����
		}

		switch (choice) {
		case 1:
			game_main();
			break;
		case 2:
			system("cls");
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN);
			gotoxy(30, 4);
			printf("����������������������������������������������������������");
			gotoxy(30, 5);
			printf("��         ���� ����         ��");
			gotoxy(30, 6);
			printf("����������������������������������������������������������");
			gotoxy(20, 10);
			printf("�� ĳ���ʹ� ����Ű ��ư���� �̵��� �����մϴ�.");
			gotoxy(20, 13);
			printf("�� ���� ���� ESCŰ�� ���� �Ͻ������� �����մϴ�.");
			gotoxy(20, 16);
			printf("�� ź���� ���� ���� ��Ƽ�� ���� ������ ��������.");
			SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);             //FOREGROUND_WHITE�� ��� RGB ���� �������
			gotoxy(20, 20);
			printf("�ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�...");
			_getch();                        //����ڰ� Ű�� ���� ������ ���α׷� �Ͻ� ����
			break;
		case 3:
			return 0;                       //break�ϸ� ���θ޴��� ������ return���� ���α׷� ����
		default:
			system("cls");
			gotoxy(20, 22);
			printf("�߸��� �Է��Դϴ�. �ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�...");
			_getch();
			break;
		}

		// ���� ���� ����
	}
	return 0;
}


