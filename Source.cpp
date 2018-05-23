#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

long double fib(int x)
{
	if (x == 1 || x == 2)
	{
		return 1;
	}
	return (fib(x - 1) + fib(x - 2));
}
											  // Рассматриваемая нами функция
long double f(long double x) 
{
	return sin(x) ;
}


long double random_search(long double(*function) (long double x),
	long double a, long double b, long double k,
	long double e, int & N)
{
	long double x1, x2; // Точки, делящие текущий отрезок в отношении золотого сечения
	x1 = b - (b - a) * k;
	x2 = a + (b - a) * k;

	while (fabs(b - a) > e) {
		N++;
		x1 = b - (b - a) * k;
		x2 = a + (b - a) * k;
		if (function(x1) <= function(x2)) // Условие для поиска максимума
			a = x1;
		else
			b = x2;
	} // Выполняем, пока не достигнем заданной точности

	//cout << "(" << (a + b) / 2 << ", " << function((a + b) / 2) << ")";
	return ((a + b) / 2);
}


long double fibonacci_search(long double(*function) (long double x),
	long double a, long double b, long double n, long double e, int & N)
{
	N = 0;
	long double result = 0;
	long double x1 = a + (b - a) * (fib(n - 2) / fib(n));
	long double x2 = a + (b - a) * (fib(n - 1) / fib(n));

	long double y1 = function(x1);
	long double y2 = function(x2);

	while ((n != 1) && (abs(x1 - x2) > e))
	{
		N++;
		n--;
		if (y1 < y2)
		{
			a = x1;
			x1 = x2;
			x2 = b - (x1 - a);
			y1 = y2;
			y2 = function(x2);
		}
		else
		{
			b = x2;
			x2 = x1;
			x1 = a + (b - x2);
			y2 = y1;
			y1 = function(x1);
		}
	}
	result = (x1 + x2) / 2;
	return result;
}


long double hooke_n_jeeves(long double(*f) (long double x), long double x1, long double h, long double m, long double e, int & N) // e погрешность
{	
	long double x2 = x1 + h;
	long double result = 0;
	long double prev_x1 = x1;
	long double prev_x2 = x2;
	N = 0;
	while (h > e)
	{		
		N++;
		while (f(x2) > f(x1))
		{
			N++;
			prev_x1 = x1;
			prev_x2 = x2;

			//cout << "in loop --- " << f(x1) << "		" << x1 << "		" << x2 << "		" << f(x2) << endl;
			x1 = x2;
			x2 = x1 + h;
			//cout << "chekin in loop --- " << f(x1) << "		" << x1 << "		" << x2 << "		" << f(x2) << endl;

			if (f(x2) < f(x1)) // роверяем значения функции при новых значениях х
			{
				// если значение х2 меньше тогда переменным присваиваются предыдущие значения, и происходит выход из цикла
				x1 = prev_x1;
				x2 = prev_x1 + h;
				break;
			}
			//cout << endl;
		}
		h /= m;	
		//cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		//cout << f(x1) << "		" << x1 << "		" << x2 << "		" << f(x2) << endl;
	}
	

	return (x1 < x2) ? x2 : x1; // возвращаем наибольший элемент
}


void prog()
{
	string filepath_enter = "enter.txt";
	string filepath_result = "result.txt";

	fstream EnterFile(filepath_enter, fstream::in | fstream::out);
	fstream ResultFile(filepath_result, fstream::in | fstream::out);

	if (!EnterFile.is_open())
	{
		cout << "Unable to read EnterFile" << endl;
	}
	if (!ResultFile.is_open())
	{
		cout << "Unable to read ResultFile" << endl;
	}

	char enter = '#';
	long double e = 0;
	long double a = 0;
	long double b = 0;
	long double k = 0;
	int type = 0;
	char tmp;
	string res;
	res += "Type\textremum\tf(extremum)\ttime\tsteps\n";
	while (!EnterFile.eof())
	{
		int N = 0;
		EnterFile >> tmp;
		while (tmp != enter)
		{
			EnterFile >> tmp;
		}
		// ввод данных
		EnterFile >> e;
		EnterFile >> a;
		EnterFile >> b;
		EnterFile >> k;
		EnterFile >> type;

		if (type == 1)
		{
			/* 
			** параметр f функция
			** параметр a начало отрезка
			** параметр b конец отрезка
			** параметр goldenRatio это "Золотое" число
			** e погрешность
			** N счетчик
			*/
			N = 0;

			const long double goldenRatio = (1 + sqrt(5)) / 2; // "Золотое" число
			clock_t start = clock();

			long double gold_ = random_search(f, a, b, ( 1 / goldenRatio), e, N);
			clock_t finish = clock();

			long double diff = long double(finish - start) / CLOCKS_PER_SEC;

			res += to_string(type) + '\t' + to_string(gold_) + '\t' + to_string(f(gold_)) +  '\t' + to_string(diff) + '\t' + to_string(N) + '\n';

			cout << gold_ << endl;

		}
		if (type == 2)
		{
			/*
			** параметр f функция
			** параметр a начало отрезка
			** параметр b конец отрезка
			** параметр k это номер числа фибоначчи
			** e погрешность
			** N счетчик			
			*/

			N = 0;

			clock_t start = clock();

			long double fibonacci_ = fibonacci_search(f, a, b, k, e, N);
			clock_t finish = clock();

			cout << a << "\t" << b;
			long double diff = long double(finish - start) / CLOCKS_PER_SEC;

			res += to_string(type) + '\t' + to_string(fibonacci_) + '\t' + to_string(f(fibonacci_)) + '\t' + to_string(diff) + '\t' + to_string(N) + '\n';

			cout << fibonacci_<< endl;

		}
		if (type == 3)
		{
			/*
			** параметр f функция
			** параметр a начало отрезка
			** параметр b конец отрезка
			** параметр k это вводимый коэффициент параметра x1 
			** e погрешность
			** N счетчик
			*/
			N = 0;
			clock_t start = clock();

			long double random_ = random_search(f, a, b, k, e, N);
			clock_t finish = clock();
			long double diff = long double(finish - start) / CLOCKS_PER_SEC;

			res += to_string(type) + '\t' + to_string(random_) + '\t' + to_string(f(random_)) + '\t' + to_string(diff) + '\t' + to_string(N) + '\n';

			cout << random_ << endl;

		}
		if (type == 4)
		{
			/*
			** параметр f функция
			** параметр a параметр x1 ( точка отсчёта )
			** параметр b параметр h ( шаг )
			** параметр k параметр m ( во сколько раз, параметр h будет уменьшаться )
			** e погрешность
			** N счетчик
			*/
			N = 0;
			clock_t start = clock();

			long double hknj_ = hooke_n_jeeves(f, a, b, k, e, N);
			clock_t finish = clock();
			long double diff = long double(finish - start) / CLOCKS_PER_SEC;

			res += to_string(type) + '\t' + to_string(hknj_) + '\t' + to_string(f(hknj_)) + '\t' + to_string(diff) + '\t' + to_string(N) + '\n';

			cout << hknj_ << endl;

		}
	}
	ResultFile << res;

	EnterFile.close();
	ResultFile.close();
	cout << res << endl;
}


int main() 
{
	int N = 0;
	const long double goldenRatio = (1 + sqrt(5)) / 2; // "Золотое" число
	
	prog();
	/*
	long double hnj = hooke_n_jeeves(f, 0, 0.1, 10, 0.0000001);

	
	long double rand_s = random_search(f, 0, 5, (1 / goldenRatio), 0.01, N); // поиск по золотому сечению через функию с произвольным параметром
	long double fib_s = fibonacci_search(f, 0, 5, 30, 0.01);
	cout << "\n" << f(rand_s) << "		" << f(fib_s) << "		" << f(hnj) << endl;
	*/
	
	return 0;
}