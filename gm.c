#include <windows.h>
#include <stdio.h>

#define CONSOLE_SIZE_X 100
#define CONSOLE_SIZE_Y 50
#define Y_SPACE 1

//�ؽ�Ʈ ���� ���ϱ�
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
	system("mode con:cols=82 lines=47");                                  // �ܼ�â ũ�⼳��

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

//ĳ���� �����
void ClearCharacter(int x, int y) {                                   //�� �ɸ� ������ ���� �����϶� �� ���� �����
	gotoxy(x, y);
	printf(" ");
}

//ĳ���� �ٽ� �׸���
void DrawCharacter(int x, int y) {                                    //������ ��ġ�� �ٽ� ��Ÿ����
	SetColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(x, y);
	printf("��");
	SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}


//����ȭ�� �� �����
void drawWall(int width, int height) {
	int i, j;

	// ��� ��
	for (i = 0; i < width; i++) {
		gotoxy(i, 0);
		printf("��");
	}

	// �ϴ� ��
	for (i = 0; i < width; i++) {
		gotoxy(i, height - 1);
		printf("��");
	}

	// �¿� ��
	for (i = 1; i < height - 1; i++) {
		gotoxy(0, i);
		printf("��");
		gotoxy(width - 1, i);
		printf("��");
	}
}

//����Ű �Է�
void KeyInput() {
	if (CHARACTER.Active) {
		int oldX = CHARACTER.x;
		int oldY = CHARACTER.y;

		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {            //Ư��Ű ���� Ȯ��(����ȭ��ǥ) & 0x8000 ���� ���ȴ��� Ȯ���ϴ� ��Ʈ ����
			CHARACTER.x--;
			if (CHARACTER.x < 1)                            //ȭ�� ����� �ʰ�
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

		if (oldX != CHARACTER.x || oldY != CHARACTER.y) {           //ĳ���Ͱ� �������� ���� ȭ�� �ٽ�
			ClearCharacter(oldX, oldY);                             //���� �ִ� �ڸ� clear
			DrawCharacter(CHARACTER.x, CHARACTER.y);                //�ٽ� ĳ���� �׸���
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
		printf("����������������������������������������������������������������������������������������");
		gotoxy(29, 26);
		printf("��\t\t�� �� �� ź�� ���ϱ�� �� ��\t\t��");
		gotoxy(29, 27);
		printf("����������������������������������������������������������������������������������������");
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
			printf("�÷��� ����!");
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
			break; // ����� ũ�Ⱑ �Ǹ� ���� ����
		}
		else {
			system("cls");
			SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			gotoxy(30, 11);
			printf("����üȭ�� �����");
			gotoxy(20, 15);
			printf("�ܼ� â�� �ʹ� �۽��ϴ�. �ּ� 100x40 ũ�Ⱑ �ʿ��մϴ�.");
			gotoxy(20, 17);
			printf("���� ũ��: %dx%d", width, height);
			gotoxy(20, 19);
			printf("�ܼ� â ũ�⸦ ������ �� �ƹ� Ű�� ��������...");
			_getch(); // ����� �Է� ���
			system("cls");
		}
	}
}

//=======================================���� ���� �Լ�=================================================
void game_main() {
	int gameWidth = 80;                 //���� ��� ���� ���� �� ũ��
	int gameHeight = 38;

	CHARACTER.x = gameWidth / 2;
	CHARACTER.y = gameHeight / 2;
	CHARACTER.Active = 1;

	system("cls");                                               // ���� ���� �� �� ���� ȭ���� �����
	drawWall(gameWidth, gameHeight);											//���� 50, ���� 50 �� �����
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


