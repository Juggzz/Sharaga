#include <iostream>
#include <cmath>

using namespace std;

long double fib(int x)
{
	if (x == 1 || x == 2)
	{
		return 1;
	}
	return (fib(x - 1) + fib(x - 2));
}
											  // –ассматриваема€ нами функци€
long double f(long double x) 
{
	return sin(x) ;
}


long double random_search(long double(*function) (long double x),
	long double a, long double b, long double k,
	long double e, int & N)
{
	long double x1, x2; // “очки, дел€щие текущий отрезок в отношении золотого сечени€

	while (fabs(b - a) > e) {
		N++;
		x1 = b - (b - a) * k;
		x2 = a + (b - a) * k;
		if (function(x1) <= function(x2)) // ”словие дл€ поиска максимума
			a = x1;
		else
			b = x2;
	} // ¬ыполн€ем, пока не достигнем заданной точности

	//cout << "(" << (a + b) / 2 << ", " << function((a + b) / 2) << ")";
	return ((a + b) / 2);
}



long double fibonacci_search(long double(*function) (long double x),
	long double a, long double b, long double n, long double e)
{
	int N = 0;
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



long double hok_n_jeeves(long double(*f) (long double x), long double x1, long double h, long double m, long double e) // e погрешность
{	
	long double x2 = x1 + h;
	long double result = 0;
	long double prev_x1 = x1;
	long double prev_x2 = x2;
	while (h > e)
	{		
		while (f(x2) > f(x1))
		{
			prev_x1 = x1;
			prev_x2 = x2;

			//cout << "in loop --- " << f(x1) << "		" << x1 << "		" << x2 << "		" << f(x2) << endl;
			x1 = x2;
			x2 = x1 + h;
			//cout << "chekin in loop --- " << f(x1) << "		" << x1 << "		" << x2 << "		" << f(x2) << endl;

			if (f(x2) < f(x1)) // ровер€ем значени€ функции при новых значени€х х
			{
				// если значение х2 меньше тогда переменным присваиваютс€ предыдущие значени€, и происходит выход из цикла
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


int main() 
{
	int N = 0;
	const long double goldenRatio = (1 + sqrt(5)) / 2; // "«олотое" число

	long double hnj = hok_n_jeeves(f, 0, 0.1, 10, 0.0000001);

	
	long double rand_s = random_search(f, 0, 5, (1 / goldenRatio), 0.01, N); // поиск по золотому сечению через функию с произвольным параметром
	long double fib_s = fibonacci_search(f, 0, 5, 30, 0.01);
	cout << "\n" << f(rand_s) << "		" << f(fib_s) << "		" << f(hnj) << endl;
	
	return 0;
}