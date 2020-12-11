#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <iomanip>

using namespace std;

static vector<vector<string>> textVector;
static wstring consoleMessage;
static string userMessage;
static int pagePosition = 0;
static bool programEnd = false;

void printText() {
	int lineNumber = 1;
	cout << setw(2) << lineNumber << "| ";
	for (int i = pagePosition; i < pagePosition + 20; i++) {
		for (int j = 0; j < textVector[i].size(); j++) {
			cout << textVector[i][j] << " ";
		}
		cout << endl;
		lineNumber += 1;
		if (lineNumber == 21) {
			break;
		}
		if (i == textVector.size() - 1) { break; }
		cout << setw(2) << lineNumber << "| ";
	}
}

void printDivisionLine() {
	for (int i = 0; i < 75; i++) {
		cout << "-";
	}
	cout << endl;
}

void printMenu() {
	wcout << L"n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
}

void printConsoleMessage(wstring& message) {
	wcout << message << endl;
}

void scanUserMessage(string& userMessage) {
	wcout << L"�Է�: ";
	cin >> userMessage;
}

void adjustTextVectorWhenAdd(){
	vector<string> tempVector;
	vector<string>::iterator lineVectorItr;
	int ByteofLine = 0;
	for (int i = 0; i < textVector.size(); i++) {
		lineVectorItr = textVector[i].begin();
		for (int j = 0; j < tempVector.size(); j++) {
			lineVectorItr = textVector[i].begin();
			textVector[i].insert(lineVectorItr + j, tempVector[j]);
		}
		tempVector.clear();
		ByteofLine = 0;
		for (int j = 0; j < textVector[i].size(); j++) {
			ByteofLine += textVector[i][j].size() + 1;
			if (ByteofLine > 71) {
				tempVector.push_back(textVector[i][j]);
			}
		}
		textVector[i].erase(textVector[i].end() - tempVector.size(), textVector[i].end());
	}
	if (!tempVector.empty()) {
		textVector.push_back(tempVector);
	}
}

void adjustTextVectorWhenSub() {
	vector<string> tempVector;
	vector<string>::iterator lineVectorItr;
	int ByteofLine = 0;
	for (int i = 0; i < textVector.size(); i++) {
		lineVectorItr = textVector[i + 1].begin();
		for (int j = 0; j < textVector[i].size(); j++) {
			ByteofLine += textVector[i][j].size() + 1;
		}
		int k = 0;
		do {
			ByteofLine += textVector[i + 1][k].size() + 1;
			if (ByteofLine >= 71) {
				break;
			}
			lineVectorItr = textVector[i + 1].begin();
			tempVector.push_back(textVector[i + 1][k]);
			k++;
		} while (ByteofLine <= 71);
		textVector[i + 1].erase(textVector[i + 1].begin(), textVector[i + 1].begin() + tempVector.size());
		for (int j = 0; j < tempVector.size(); j++) {
			textVector[i].push_back(tempVector[j]);
		}
		if (i == textVector.size() - 2) {
			break;
		}
		tempVector.clear();
		ByteofLine = 0;
	}
}

class Job {
public:
	virtual void performJob() = 0;
};


class NextPage : public Job {
public:
	void performJob() {
		if (pagePosition == textVector.size() - 20) {
			throw wstring(L"������ �������Դϴ�. �ٽ� �Է����ּ���.");
		}
		pagePosition += 20;
		if (pagePosition + 20 > textVector.size()) {
			pagePosition = textVector.size() - 20;
		}
		consoleMessage = L"(�ָܼ޽���)";
		printText();
	}
};

class PreviousPage : public Job {
public:
	void performJob() {
		if (pagePosition == 0) {
			throw wstring(L"ù �������Դϴ�. �ٽ� �Է����ּ���.");
		}
		pagePosition -= 20;
		if (pagePosition < 0) {
			pagePosition = 0;
		}
		consoleMessage = L"(�ָܼ޽���)";
		printText();
	}
};

class InsertWord : public Job {
public:
	int first;
	int second;
	string third;
	
	InsertWord(int newFirst, int newSecond, string newThird) {
		first = newFirst;
		second = newSecond;
		third = newThird;
	}

	void performJob() {
		if (first < 1 || 20 < first) {
			throw wstring(L"���� ��� â�� �����ϴ� ������ �Է����ּ���.");
		}
		if (second < 1 || textVector[pagePosition + first - 1].size() < second) {
			throw wstring(L"�Է��� ������ �ܾ �����ϴ�. �ٽ� �Է����ּ���.");
		}
		if (third.size() >= 75) {
			throw wstring(L"75����Ʈ �̻� ����� �� �����ϴ�. �ٽ� �Է����ּ���.");
		}
		vector<string>::iterator lineVectorItr = textVector[pagePosition + first - 1].begin();
		textVector[pagePosition + first - 1].insert(lineVectorItr + second, third);
		adjustTextVectorWhenAdd();
		consoleMessage = L"(�ָܼ޽���)";
		printText();
	}
};

class DeleteWord : public Job {
public:
	int first;
	int second;

	DeleteWord(int newFirst, int newSecond) {
		first = newFirst;
		second = newSecond;
	}
	void performJob() {
		if (first < 1 || 20 < first) {
			throw wstring(L"���� ��� â�� �����ϴ� ������ �Է����ּ���.");
		}
		if (second < 1 || textVector[pagePosition + first - 1].size() < second) {
			throw wstring(L"���� ��� â�� �����ϴ� �ܾ �Է����ּ���.");
		}
		vector<string>::iterator lineVectorItr = textVector[pagePosition + first - 1].begin();
		textVector[pagePosition + first - 1].erase(lineVectorItr + second - 1);	
		adjustTextVectorWhenSub();
		consoleMessage = L"(�ָܼ޽���)";
		printText();
	}	
};

class ChangeWord : public Job {
public:
	string first;
	string second;

	ChangeWord(string newFirst, string newSecond) {
		first = newFirst;
		second = newSecond;
	}
	void performJob() {
		for (int i = 0; i < textVector.size(); i++) {
			for (int j = 0; j < textVector[i].size(); j++) {
				if (textVector[i][j].find(first) != string::npos) {
					textVector[i][j].replace(textVector[i][j].find(first), first.size(), second);
				}
			}
		}
		if (first.size() < second.size()) {
			adjustTextVectorWhenAdd();
		}
		else if (first.size() > second.size()) {
			adjustTextVectorWhenSub();
		}
		consoleMessage = L"(�ָܼ޽���)";
		printText();
	}
};

class SearchWord : public Job {
public:
	string first;
	SearchWord(string newFirst) {
		first = newFirst;
	}
	void performJob() {
		int searchWordLine = pagePosition;
		bool isSearch = false;
		for (int i = 0; i < textVector.size(); i++) {
			for (int j = 0; j < textVector[i].size(); j++) {
				if (textVector[i][j] == first) {
					searchWordLine = i;
					isSearch = true;
					break;
				}
			}
			if (isSearch) {
				break;
			}
		}
		if (!isSearch) {
			throw wstring(L"ã�� �ܾ �������� �ʽ��ϴ�. �ٽ� �Է����ּ���.");
		}
		consoleMessage = L"(�ָܼ޽���)";
		pagePosition = searchWordLine;
		printText();
	}
};

class Terminate : public Job {
public:
	void performJob() {
		ofstream out("test.txt", ios::out);
		string s;
		if (out.is_open()) {
			for (int i = 0; i < textVector.size(); i++) {
				for (int j = 0; j < textVector[i].size(); j++) {
					s += textVector[i][j];
					s += " ";
				}
				out.write(s.c_str(), s.size());
				s = "";
			}
		}
		out.close();
		programEnd = true;
	}
};

class JobFactory {
public:
	static Job* produceJob(string userMessage);
};

Job* JobFactory::produceJob(string userMessage) {
	char job = userMessage.front();
	char userMessageByChar[256];
	strcpy_s(userMessageByChar, 256, userMessage.c_str());
	char* buffer;
	char* splitUserMessage = strtok_s(userMessageByChar, "(,)", &buffer);
	string UserMessageStringArray[4];
	int i = 0;
	while (splitUserMessage != NULL) {
		UserMessageStringArray[i] = splitUserMessage;
		splitUserMessage = strtok_s(NULL, "(,)", &buffer);
		i++;
	}

	if (job == 'n') {
		return new NextPage;
	}
	else if (job == 'p') {
		return new PreviousPage;
	}
	else if (job == 'i') {
		int first = stoi(UserMessageStringArray[1]);
		int second = stoi(UserMessageStringArray[2]);
		string third = UserMessageStringArray[3];
		return new InsertWord(first, second, third);
	}
	else if (job == 'd') {
		int first = stoi(UserMessageStringArray[1]);
		int second = stoi(UserMessageStringArray[2]);
		return new DeleteWord(first, second);
	}
	else if (job == 'c') {
		string first = UserMessageStringArray[1];
		string second = UserMessageStringArray[2];
		return new ChangeWord(first, second);
	}
	else if (job == 's') {
		string first = UserMessageStringArray[1];
		return new SearchWord(first);
	}
	else if (job == 't') {
		return new Terminate;
	}
	else {
		throw wstring(L"�޴��� �ִ� �͸� �Է����ּ���.");
		return NULL;
	}
}


int main() {
	locale::global(locale("ko_KR.UTF-8"));
	ifstream in("test.txt");
	vector<string> lineVector;
	char word[128];
	consoleMessage = L"(�ָܼ޽���)";

	if (!in.is_open()) {
		cout << "There is no file." << endl;
	}
	int ByteofLine = 0;
	while (in >> word) {
		ByteofLine += strlen(word) + 1;
		if (ByteofLine > 71) {
			textVector.push_back(lineVector);
			lineVector.clear();
			ByteofLine = 0;
			ByteofLine += strlen(word) + 1;
		}
		lineVector.push_back(word);
	}
	textVector.push_back(lineVector);
	in.close();

	printText();
	while (true) {	
		printDivisionLine();
		printMenu();
		printDivisionLine();
		printConsoleMessage(consoleMessage);
		printDivisionLine();
		scanUserMessage(userMessage);
		printDivisionLine();
		
		try {
			JobFactory::produceJob(userMessage)->performJob();
		}
		catch (wstring& errorMessage) {
			consoleMessage = errorMessage;
			printText();
		}
		catch (invalid_argument& e) {
			consoleMessage = L"��ȿ���� ���� ���� �ԷµǾ����ϴ�. ���Ŀ� ���缭 �ٽ� �Է����ּ���.";
			printText();
		}
		catch (...) {
			consoleMessage = L"������ �߻��߽��ϴ�. �ٽ� �Է����ּ���.3";
			printText();
		}
		if (programEnd) {
			break;
		}
	}
	return 0;
}