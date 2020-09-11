
/*

All credit to Khuram Ali

Sourced from: https://alikhuram.wordpress.com/2013/05/20/implementing-fixed-point-numbers-in-c/

*/

template<int E>
struct BasicFixedReal
{
	typedef BasicFixedReal self;
	static const int factor = 1 << (E - 1);
	BasicFixedReal() : m(0) { }
	BasicFixedReal(double d) : m(static_cast(d* factor)) { }
	self& operator+=(const self& x) { m += x.m; return *this; }
	self& operator-=(const self& x) { m -= x.m; return *this; }
	self& operator*=(const self& x) { m *= x.m; m >>= E; return *this; }
	self& operator/=(const self& x) { m /= x.m; m *= factor; return *this; }
	self& operator*=(int x) { m *= x; return *this; }
	self& operator/=(int x) { m /= x; return *this; }
	self operator-() { return self(-m); }
	double toDouble() const { return double(m) / factor; }
	// friend functions
	friend self operator+(self x, const self& y) { return x += y; }
	friend self operator-(self x, const self& y) { return x -= y; }
	friend self operator*(self x, const self& y) { return x *= y; }
	friend self operator/(self x, const self& y) { return x /= y; }
	// comparison operators
	friend bool operator==(const self& x, const self& y) { return x.m == y.m; }
	friend bool operator!=(const self& x, const self& y) { return x.m != y.m; }
	friend bool operator>(const self& x, const self& y) { return x.m > y.m; }
	friend bool operator<(const self& x, const self& y) { return x.m < y.m; }
	friend bool operator>=(const self& x, const self& y) { return x.m >= y.m; }
	friend bool operator<=(const self& x, const self& y) { return x.m <= y.m; }
private:
	int m;
};