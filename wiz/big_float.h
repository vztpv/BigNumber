
#ifndef BIG_FLOAT_H
#define BIG_FLOAT_H

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
			static BigFloat remove_last_zeros(const BigFloat& bi) {
				std::string str = bi.ToString();
				long long offset = bi.exponent;

				long long i = str.size() - 1;
				while (str.size() >= 1) {
					if (str[i] == '0') {
						str.pop_back();
						offset--;
					}
					else {
						break;
					}
					--i;
				}

				BigFloat result = BigFloat(std::move(str));
				result.exponent = offset;

				return result;
			}
		private:
			static BigFloat ABS(const BigFloat& bigFloat)
			{
				return BigFloat(bigFloat.value.val, bigFloat.value.sign, bigFloat.exponent);
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
			long long exponent; /// right->lett?
		public:
			explicit BigFloat()
			{
			}
			virtual ~BigFloat() {

			}
			explicit BigFloat(const std::vector<long long>& val, const bool& sign, const long long exponent)
				: value(val, sign), exponent(exponent)
			{

			}

			BigFloat(const BigFloat& other)
			{
				value = other.value;
				exponent = other.exponent;

				// debug
				//std::cout << "BigFloat " << this->ToString() << std::endl;
				//std::cout << this->exponent << std::endl;
			}
			BigFloat(const long long number)
			{
				this->value = wiz::big_int::BigInt(number);
				this->exponent = 0;
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

				if (str.size() > 0 && str[0] == '.') {
					str.insert(str.begin(), '0');
				}
				else if (str.empty()) {
					str = "0";
				}

				// set exponent
				std::string::size_type x = str.find('.');

				if (x != std::string::npos) {
					this->exponent = (str.size() - 1 - x);
					str.erase(str.begin() + x);
				}
				else {
					this->exponent = 0;
				}

				// set value
				this->value.val = wiz::big_int::BigInt(std::move(str)).val;

				// debug
				//std::cout << "BigFloat " << this->ToString() << std::endl;
				//std::cout << this->exponent << std::endl;
			}

			BigFloat& operator=(const BigFloat& longInt)
			{
				this->value = longInt.value;
				this->exponent = longInt.exponent;

				return *this;
			}
			BigFloat& operator=(BigFloat&& other)
			{
				this->value = std::move(other.value);
				this->exponent = other.exponent;

				return *this;
			}
		public:
			int CompABS(const BigFloat& x, const BigFloat& y)
			{
				//bool a_minus = false, b_minus = false;
				std::string a = x.ToString(), b = y.ToString();
				big_int::BigInt x_int, y_int;

				//if (!a.empty() && a[0] == '-') {
				//	a.erase(a.begin());
				//	a_minus = true;
				//}
				//if (!b.empty() && b[0] == '-') {
				//	b.erase(b.begin());
				//	b_minus = true;
				//}
				//if (a_minus && !b_minus) {
				//	return -1; // <
				//}
				//else if (!a_minus && b_minus) {
				//	return 1; // >
				//}

				std::string::size_type a_size = a.find('.'), b_size = b.find('.');
				x_int = big_int::BigInt(a.substr(0, a_size));
				y_int = big_int::BigInt(b.substr(0, b_size));

				if (x_int > y_int) { return 1; }
				else if (x_int < y_int) { return -1; }
				else {
					int i = a_size + 1, j = b_size + 1;

					for (; i < a.size() && j < b.size(); ++i, ++j) {
						if (a[i] > b[i]) {
							return 1;
						}
						else if (a[i] < b[i]) {
							return -1;
						}
					}
					return 0;
				}
			}
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
				// !=  :  bool xor
				BigFloat x, y;
				BigFloat temp;
				long long length = std::max(num1.exponent, num2.exponent);

				if (num1.exponent >= length) {
					x = num1.ToString();
					y = num2.ToString();

					std::string str = num2.value.ToString();
					for (long long i = 0; i < length - num2.exponent; ++i) {
						str.push_back('0');
					}

					y = BigFloat(std::move(str));
				}
				else if (num2.exponent >= length) {
					x = num1.ToString();
					y = num2.ToString();

					std::string str = num1.value.ToString();
					for (long long i = 0; i < length - num1.exponent; ++i) {
						str.push_back('0');
					}

					x = BigFloat(std::move(str));
				}

				temp.value = x.value + y.value;
				temp.exponent = length;

				return temp;
			}
			friend BigFloat operator-(const BigFloat& num1, const BigFloat& num2)
			{
				BigFloat x, y;
				BigFloat temp;
				long long length = std::max(num1.exponent, num2.exponent);

				if (num1.exponent >= length) {
					x = num1.ToString();
					y = num2.ToString();

					std::string str = num2.value.ToString();
					for (long long i = 0; i < length - num2.exponent; ++i) {
						str.push_back('0');
					}

					y = BigFloat(std::move(str));


				}
				else if (num2.exponent >= length) {
					x = num1.ToString();
					y = num2.ToString();

					std::string str = num1.value.ToString();
					for (long long i = 0; i < length - num1.exponent; ++i) {
						str.push_back('0');
					}

					x = BigFloat(std::move(str));
				}

				temp.value = x.value - y.value;
				temp.exponent = length;

				return temp;
			}
			friend BigFloat operator*(const BigFloat& num1, const BigFloat& num2)
			{
				// num1 == zero or num2 == zero -> then return 0. ?
				BigFloat temp;
				long long length = num1.exponent + num2.exponent;

				temp.value = num1.value * num2.value;
				temp.exponent = length;

				return temp;
			}


			// using BIGFLOAT_MANT_NUM
			friend BigFloat operator/(BigFloat num1, BigFloat num2)
			{
				// todo - num2가 0인 경우 에러체크!
				if (num2 == BigFloat(0)) {
					std::cout << "divide by zero Error" << std::endl;
					throw "divide by zero Error";
				}

				BigFloat a = num1;
				num1 = BigFloat(1);

				BigFloat result;
				const BigFloat ten(10);
				BigFloat tens(1);

				long long size1, size2;

				size1 = num1.exponent;
				size2 = num2.exponent;

				{
					const long long x = std::max(size1, size2);

					for (long long i = size1; i < x; ++i) {
						num1 = num1 * BigFloat(10);
					}
					for (long long i = size2; i < x; ++i) {
						num2 = num2 * BigFloat(10);
					}
				}

				num1.exponent = 0;
				num2.exponent = 0;

				{
					for (long long i = 0; i < BIGFLOAT_MANT_NUM; ++i) {
						tens = tens * ten;
					}

					auto x = num1 * tens;
					auto q = x.value / num2.value;

					result.value = q;
					result.exponent = BIGFLOAT_MANT_NUM;
				}

				return a * result;
			}

		public:
			bool Cut()
			{
				BigFloat temp(*this);

				temp = remove_last_zeros(temp);

				*this = std::move(temp);
				return true;
			}
			// todo..
			bool Cut(long long offset) { // 소수점 아래 자리수
										 // todo - check error for offset.
				if (this->exponent < offset) {
					return false;
				}
				//
				std::string str = this->ToString();
				std::string::size_type dot_pos = str.find('.');
				const std::string::size_type str_size = str.size();

				if (std::string::npos == dot_pos) { return false; }

				for (int i = 0; i < str_size - dot_pos - 1 - offset; ++i) {
					str.pop_back();
				}
				this->exponent = offset; // chk!

				(*this) = BigFloat(std::move(str));

				return true;
			}

		public:
			std::string ToString() const
			{
				std::string temp;

				if (0 < this->value.val.size()) {
					temp += wiz::toStr(this->value.val[0]);
				}
				for (int i = 1; i < this->value.val.size(); ++i) {
					temp = temp + wiz::toStr2(this->value.val[i], 9);
				}

				// insert point to temp.
				while (temp.size() <= this->exponent)
				{
					temp.insert(temp.begin(), '0');
				}

				temp.insert(temp.begin() + temp.size() - this->exponent, '.');

				if (this->value.sign == false) {
					temp.insert(temp.begin(), '-');
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

long long wiz::big_float::BigFloat::BIGFLOAT_MANT_NUM = 100;


#endif
