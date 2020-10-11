#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>

class My_cat {
	char* name;
	int weight;

public:
	My_cat();
	My_cat(const char* name);
	My_cat(const My_cat& cat);
	~My_cat();

	//eat 함수 선언
	My_cat& eat(int food);

	void show_status() const;
};

My_cat::My_cat() :name(NULL), weight(10) { }

My_cat::My_cat(const char* cat_name) : weight(10) {
	name = new char[strlen(cat_name) + 1];
	strcpy(name, cat_name);
}

My_cat::My_cat(const My_cat& cat) : weight(10) { //Copy constructor
	std::cout << "Copy constructor invocation ! " << std::endl;
	name = new char[strlen(cat.name) + 1];
	strcpy(name, cat.name);
}

My_cat::~My_cat() {		//Destructor
	if (name) delete[] name;
}

//eat 함수
My_cat& My_cat::eat(int food) {
	weight += food;
	return *this;
}

void My_cat::show_status() const {
	std::cout << " Weight : " << weight << std::endl;
}
int main() {
	My_cat cat1("Nabi");	//Cat 1 생성
	cat1.show_status();

	cat1.eat(4);
	cat1.show_status();

	cat1.eat(4);
	cat1.show_status();

	cat1.eat(4).eat(5).eat(6);
	cat1.show_status();

	getchar();
	return 0;
}
