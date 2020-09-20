#include <iostream>

class SetValue {

private:
	int x, y;

public:
	void setX(int newX) {
		x = newX;
	}
	void setY(int newY) {
		y = newY;
	}
	void printXY() {
		std::cout << "X= " << x << " , Y= " << y << std::endl;
	}

};

int main() {

	SetValue obj;

	obj.setX(33);
	obj.setY(44);

	obj.printXY();

	system("pause");
	return 0;
}