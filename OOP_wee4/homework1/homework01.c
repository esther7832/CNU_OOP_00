#include <stdio.h>

int main(void) {
	char s[101];
	printf("���ڸ� �Է��ϼ���(100�� �̳�).\n");
	scanf_s("%s", s, 101);
	printf("�Էµ� ���ڴ� %s�Դϴ�.\n", s);

	system("pause"); // keep terminal open
	return 0;
}
