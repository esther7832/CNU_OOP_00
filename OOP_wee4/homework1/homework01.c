#include <stdio.h>

int main(void) {
	char s[101];
	printf("문자를 입력하세요(100자 이내).\n");
	scanf_s("%s", s, 101);
	printf("입력된 문자는 %s입니다.\n", s);

	system("pause"); // keep terminal open
	return 0;
}
