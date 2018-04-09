

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <cmath>

#include <wiz/big_int.h>
//#include <wiz/big_fraction.h>

namespace wiz {
	class big_float {
	public:
		class BigFloat
		{
		public:
			static const long long BIGFLOAT_BASE = (long long)1000000000; /// chk!!  changable?
			static const long long BIGFLOAT_DIGIT_NUM = 9;
			static long long BIGFLOAT_MANT_NUM; // rename? 소수점 아래 자리수?

			// check..
			static wiz::big_float::BigFloat remove_last_zeros(const wiz::big_float::BigFloat& bi) {
				std::string str = bi.ToString();

				long long i = str.size() - 1;
				while (str.size() >= 1) {
					if (str[i] == '0') {
						str.pop_back();
					}
					else {
						break;
					}
					--i;
				}

				return wiz::big_float::BigFloat(std::move(str));
			}
		private:
			static BigFloat ABS(const BigFloat& bigFloat)
			{
				return BigFloat(bigFloat.value.val, bigFloat.value.sign, bigFloat.point_offset);
			}
			static std::vector<long long> remove_first_zeros(const std::vector<long long>& x) /// remove first all zeros.
			{
				int zero_num = 0;

				for (int i = 0; i < x.size(); i++)
				{
					if (x[i] != 0) { break; }
					zero_num++;
				}
				if (0 == zero_num) { return x; }
				if (x.size() == zero_num) { return big_int_ndef_int; } /// chk...
				std::vector<long long> temp(x.size() - zero_num);
				for (int i = 0; i < temp.size(); i++)
				{
					temp[i] = x[i + zero_num];
				}

				return temp;
			}
			static bool IsSameValues(const std::vector<long long>& arr1, const std::vector<long long>& arr2)
			{
				std::vector<long long> _arr1 = remove_first_zeros(arr1);
				std::vector<long long> _arr2 = remove_first_zeros(arr2);

				if (_arr1.size() != _arr2.size()) { return false; }
				for (int i = 0; i < _arr1.size(); i++)
				{
					if (_arr1[i] != _arr2[i]) { return false; }
				}
				return true;
			}
		private:
			wiz::big_int::BigInt value;
			long long point_offset; /// right->lett?
		public:
			explicit BigFloat()
			{
			}
			virtual ~BigFloat() {

			}
			explicit BigFloat(const std::vector<long long>& val, const bool& sign, const long long point_offset)
				: value(val, sign), point_offset(point_offset)
			{

			}

			BigFloat(const BigFloat& other)
			{
				value = other.value;
				point_offset = other.point_offset;

				// debug
				//std::cout << "BigFloat " << this->ToString() << std::endl;
				//std::cout << this->point_offset << std::endl;
			}
			BigFloat(const long long number)
			{
				this->value = wiz::big_int::BigInt(number);
				this->point_offset = 0;
			}
			BigFloat(std::string str)
			{
				if ('-' == str[0]) {
					str.erase(str.begin());
					this->value.sign = false;
				}
				else {
					this->value.sign = true;
				}

				while (!str.empty() && str[0] == '0') {
					str.erase(str.begin());
				}

				// set point_offset
				std::string::size_type x = str.find('.');

				if (x != std::string::npos) {
					this->point_offset = str.size() - 1 - x;
					str.erase(str.begin() + x);
				}
				else {
					this->point_offset = 0;
				}

				// set value
				this->value.val = wiz::big_int::BigInt(std::move(str)).val;

				// debug
				//std::cout << "BigFloat " << this->ToString() << std::endl;
				//std::cout << this->point_offset << std::endl;
			}

			BigFloat& operator=(const BigFloat& longInt)
			{
				this->value = longInt.value;
				this->point_offset = longInt.point_offset;

				return *this;
			}
			BigFloat& operator=(BigFloat&& other)
			{
				this->value = std::move(other.value);
				this->point_offset = other.point_offset;

				return *this;
			}
		public:
			friend bool operator!=(const BigFloat& num1, const BigFloat& num2)
			{
				return !IsSameValues((num1 - num2).value.val, big_int_zero_int);
			}
			friend bool operator<(const BigFloat& num1, const BigFloat& num2)
			{
				return (num1 - num2).value.sign == false;
			}
			friend bool operator>(const BigFloat& num1, const BigFloat& num2)
			{
				return num2 < num1;
			}
			friend bool operator==(const BigFloat& num1, const BigFloat& num2)
			{
				return IsSameValues((num1 - num2).value.val, big_int_zero_int);
			}
			friend bool operator<=(const BigFloat& num1, const BigFloat& num2)
			{
				return num1 < num2 || num1 == num2;
			}
			friend bool operator>=(const BigFloat& num1, const BigFloat& num2)
			{
				return num1 > num2 || num1 == num2;
			}
			friend BigFloat operator+(const BigFloat& num1, const BigFloat& num2)
			{
				BigFloat x, y;
				BigFloat temp;
				long long length = std::max(num1.point_offset, num2.point_offset);

				if (num1.point_offset >= length) {
					x = num1.ToString();
					y = num2.ToString();

					std::string str = num2.value.ToString();
					for (long long i = 0; i < length - num2.point_offset; ++i) {
						str.push_back('0');
					}
					y = BigFloat(std::move(str));
				}
				else if (num2.point_offset >= length) {
					x = num1.ToString();
					y = num2.ToString();

					std::string str = num1.value.ToString();
					for (long long i = 0; i < length - num1.point_offset; ++i) {
						str.push_back('0');
					}
					x = BigFloat(std::move(str));
				}

				temp.value = x.value + y.value;
				temp.point_offset = length;

				return temp;
			}
			friend BigFloat operator-(const BigFloat& num1, const BigFloat& num2)
			{
				BigFloat x, y;
				BigFloat temp;
				long long length = std::max(num1.point_offset, num2.point_offset);

				if (num1.point_offset >= length) {
					x = num1.ToString();
					y = num2.ToString();

					std::string str = num2.value.ToString();
					for (long long i = 0; i < length - num2.point_offset; ++i) {
						str.push_back('0');
					}
					y = BigFloat(std::move(str));
				}
				else if (num2.point_offset >= length) {
					x = num1.ToString();
					y = num2.ToString();

					std::string str = num1.value.ToString();
					for (long long i = 0; i < length - num1.point_offset; ++i) {
						str.push_back('0');
					}
					x = BigFloat(std::move(str));
				}

				temp.value = x.value - y.value;
				temp.point_offset = length;

				return temp;
			}
			friend BigFloat operator*(const BigFloat& num1, const BigFloat& num2)
			{
				BigFloat temp;
				long long length = num1.point_offset + num2.point_offset;

				temp.value = num1.value * num2.value;
				temp.point_offset = length;

				return temp;
			}


			// using BIGFLOAT_MANT_NUM
			friend BigFloat operator/(BigFloat num1, BigFloat num2)
			{
				// todo - num2가 0인 경우 에러체크!
				
				BigFloat a = num1;
				num1 = BigFloat(1);

				BigFloat result;
				const BigFloat ten(10);
				BigFloat tens(1);

				long long size1, size2;

				size1 = num1.point_offset;
				size2 = num2.point_offset;
				
				{
					const long long x = std::max(size1, size2);

					for (long long i = size1; i < x; ++i) {
						num1 = num1 * BigFloat(10);
					}
					for (long long i = size2; i < x; ++i) {
						num2 = num2 * BigFloat(10);
					}
				}

				num1.point_offset = 0;
				num2.point_offset = 0;

				{
					for (long long i = 0; i < BIGFLOAT_MANT_NUM; ++i) {
						tens = tens * ten;
					}

					auto x = num1 * tens;
					auto q = x.value / num2.value;

					result.value = q;
					result.point_offset = BIGFLOAT_MANT_NUM;
				}

				return a * result;
			}

		public:
			// todo..
			bool Cut(long long offset) { // 소수점 아래 자리수
				// todo - check error for offset.

				//
				std::string str = this->ToString();
				std::string::size_type dot_pos = str.find('.');
				const std::string::size_type str_size = str.size();

				if (std::string::npos == dot_pos) { return false; }

				for (int i = 0; i < str_size - dot_pos - 1 - offset; ++i) {
					str.pop_back();
				}
				this->point_offset = offset; // chk!
				
				(*this) = BigFloat(std::move(str));

				return true;
			}

		public:
			std::string ToString() const
			{
				std::string temp;

				if (this->value.sign == false) {
					temp = "-";
				}
				for (int i = 0; i < this->value.val.size(); ++i) {
					temp = temp + wiz::toStr2(this->value.val[i], 9);
				}

				// todo - insert "point"!
					// error..
				if (temp.size() <= this->point_offset) {
					std::cout << "chk " << temp << std::endl;
					std::cout << temp.size() << " // " << this->point_offset << std::endl;
					exit(-2);
				}
				else {
					temp.insert(temp.begin() + temp.size() - this->point_offset, '.');
				}

				return temp;
			}

			friend
				ostream& operator<<(ostream& stream, const BigFloat& bigFloat)
			{
				stream << bigFloat.ToString();
				return stream;
			}
		};
	};
}

long long wiz::big_float::BigFloat::BIGFLOAT_MANT_NUM = 10;


int main(void)
{
	// BigFloat Test
		// 3.140 -> 3140
		// 0.1       100
		//   
	wiz::big_float::BigFloat x("0.001"), y("3.001"); // removal of zeor in 0.33 !!

	std::cout << x << " " << y << "\n";

	{
		wiz::big_float::BigFloat z(x + y);

		std::cout << z << "\n";
		std::cout << "\n";
	}
	{
		wiz::big_float::BigFloat z(x - y);

		std::cout << z << "\n";
		std::cout << "\n";
	}
	{
		wiz::big_float::BigFloat z(x * y);

		std::cout << z << "\n";
		std::cout << "\n";
	}
	{
		wiz::big_float::BigFloat z(x / y);
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

	return 0;
}
