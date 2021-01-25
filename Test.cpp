#include <iostream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

class Сalculator {

	vector<string> expression; //массив для обратной польской нотации
	stack<char> operand; //стэк для операндов и скобок

	bool expression_correct(string& str, string& wrong) { //функция для проверки корректности ввода

		string dop;
		int left = 0, right = 0;
		bool doubleOperand = false;

		for (size_t i = 0; i < str.length(); i++) {

			if (str[i] == 32) continue;

			if (str[i] < 40 || str[i] > 57) { 
				dop.push_back(str[i]); 
			}		
			
			if (str[i] == '(') left++; // проверка ПСП
			else if (str[i] == ')' && left >= right) right++;
			else if (str[i] == ')' && left < right) {
				wrong = "неправильная расстановка скобок";
				return false;
			}

			if (str[i] > 41 && str[i] < 48 && doubleOperand == true) { //проверка разделителей и операндов на задвоение
				wrong = "ошибка, задвоение операда или разделителя";
				return false;
			}
			else if (str[i] > 41 && str[i] < 48) {
				doubleOperand = true;
			}
			else {
				doubleOperand = false;
			}
		}


		if (left != right) {
			wrong = "неправильная расстановка скобок";
			return false;
		}
		if (dop.length() > 0) {
			wrong = "некорректный воод, строка содержит недопустимое выражение " + dop;
			return false;
		}
		return true;
	}

	void readNumber(string& num, char a) { //запись значений и проверка разделителя
		if (a == 44) num.push_back('.');
		else num.push_back(a);
	}

	void readBracket(char& a) {//функция обработки скобок

		if (a == '(') {
			operand.push(a);
		} else {//если закрывающая скобка, то идет выталкивание операндов до открывающей скобки

			while (operand.size() > 0) {

				if (operand.top() == '(') {
					operand.pop();
					break;
				}
				else { 
					string dop; dop.push_back(operand.top());
					expression.push_back(dop);
					operand.pop();
				}
			}
		}
	}

	void readSum(char& a) {

		if (!operand.empty()) {
			while (operand.size() > 0) {
				if (operand.top() != '(') { //выталкивание предыдущих операндов до скобок если есть.
					string dop; dop.push_back(operand.top());
					expression.push_back(dop);
					operand.pop();
				}
				else {
					break;
				}
			}
		}
		operand.push(a);	
	}

	void readDivision(char& a) {

		if (!operand.empty()) {
			if (operand.top() == '*' || operand.top() == '/') {
				string dop; dop.push_back(operand.top());
				expression.push_back(dop);
				operand.pop();
			}
		}
		operand.push(a);
	}

	string find_answer() {//функция для расчета обратной польской нотации

		vector<double> v;
		v.push_back(stod(expression[0]));

		for (int i = 1; i < expression.size(); i++) {

			int one = v.size() - 2, two = v.size() - 1;

			if (expression[i] == "*") {
				v[one] = v[one] * v[two];
				v.pop_back();
			}
			else if (expression[i] == "/") {
				if (v[two] == 0) return "error";
				v[one] = v[one] / v[two];
				v.pop_back();
			}
			else if (expression[i] == "+") {
				v[one] = v[one] + v[two];
				v.pop_back();
			}
			else if (expression[i] == "-") {
				v[one] = v[one] - v[two];
				v.pop_back();
			} 
			else {
				v.push_back(stod(expression[i]));
			}
		}

		return to_string(v[0]);
	}

public:

	string read(string& str) {

		if (str == "") return "Не введено выражение";

		string wrong;
		string num;
		int index = 0;
		char backStep = '`';
		
		if (expression_correct(str, wrong)) { //первоначальная проверка на корректность

			if (str[0] == '-') {
				num.push_back('-');
				index = 1;
			}

			for (size_t i = index; i < str.length(); i++) {

				if (str[i] == ' ') continue;

				if (str[i] < 58 && str[i] > 47 || str[i] == 44 || str[i] == 46) { //блок работы с символами
					readNumber(num, str[i]);
				} 
				else if (str[i] == '-' && backStep == '(') { //проверка наличия знака перед открывающей скобкой
					num.push_back(str[i]);
					continue;
				}
				else if (!num.empty()) { //запись значения в массив
					expression.push_back(num);
					num.clear();
				}


				if (str[i] == '(' || str[i] == ')') readBracket(str[i]); //определение операндов и скобок
				else if (str[i] == '-' || str[i] == '+') readSum(str[i]);
				else if (str[i] == '*' || str[i] == '/') readDivision(str[i]);

				backStep = str[i];
			}

			if (!num.empty()) expression.push_back(num);

			while (operand.size() > 0) { //перевод всех операндов в массив
				string dop; dop.push_back(operand.top());
				expression.push_back(dop);
				operand.pop();
			}

			return find_answer(); //вызов функции расчета
		}

		return wrong;
	}

};

int main() {

	string str; getline(cin, str);

	Сalculator one;

	string answer = one.read(str);

	if (answer[0] > 47 && answer[0] < 58 || answer[0] == '-') {
		cout.precision(2);
		cout << fixed << stod(answer) << endl;
	}
	else {
		cout << answer << endl;
	}

	return 0;
}