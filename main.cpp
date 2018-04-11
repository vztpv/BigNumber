

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <cmath>

#include <wiz/big_float.h>

#include <ctime>

#define BIGNUMBER wiz::big_int::BigInt // wiz::big_float::BigFloat // wiz::big_int::BigInt


int main(void)
{
	/*
	// BigFloat Test
		// 3.140 -> 3140
		// 0.1       100
		//   
	wiz::big_float::BigFloat x("-0.0"), y("-3"); // removal of zeor in 0.33 !!

	std::cout << x << " " << y << "\n";

	{
		wiz::big_float::BigFloat z(x + y);
		z.Cut();
		std::cout << z << "\n";
		std::cout << "\n";
	}
	{
		wiz::big_float::BigFloat z(x - y);
		z.Cut();
		std::cout << z << "\n";
		std::cout << "\n";
	}
	{
		wiz::big_float::BigFloat z(x * y);
		z.Cut();
		std::cout << z << "\n";
		std::cout << "\n";
	}
	{
		wiz::big_float::BigFloat z(x / y);	
		z.Cut();
		//z.Cut(0); // 
		std::cout << z << "\n";
		std::cout << "\n";
	}

	// BigInt Test
	{
		wiz::big_int::BigInt x, y;
		x = wiz::big_int::BigInt(std::vector<long long>{ 1 }, true);
		y = wiz::big_int::BigInt(std::vector<long long>{ 1 }, true);
		wiz::big_int::BigInt Max(std::vector<long long>{ 1, 614511616 }, true);

		y = Max / wiz::big_int::BigInt(std::vector<long long>{ 95448 }, true);

		for (auto& val : y.val)
			std::cout << val << std::endl;

		x = wiz::big_int::BigInt(std::vector< long long >{ 999999999 }, true);
		y = x;
		y = y * y;
		std::cout << "-----" << std::endl;

		std::cout << y << std::endl;
	}
	*/
	{ // 100!
		BIGNUMBER bigInt;
		int a = clock();
		for (int i = 0; i < 10; ++i)
		{
			BIGNUMBER total("1");
			BIGNUMBER x("1");

			for (int i = 1; i <= 300; ++i) {
				total = total * x;
				x = x + BIGNUMBER("1");
			}
			//std::cout << total << std::endl;

			bigInt = std::move(total);
		}
		int b = clock();
		int c = clock();
		for (int i = 0; i < 10; ++i) {
			BIGNUMBER total(bigInt);
			BIGNUMBER x("1");

			for (int i = 1; i <= 300; ++i) {
				total = total / x;
				x = x + BIGNUMBER("1");
			}
		}
		int d = clock();
		std::cout << b - a << "ms" << std::endl;
		std::cout << d - c << "ms" << std::endl;
	}
	return 0;
}
