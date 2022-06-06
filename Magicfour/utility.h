#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>
#include <random>

template <typename T>
struct Vector2d
{
	T x, y;

	Vector2d(std::pair<T, T> val)
		: x(val.first), y(val.second) {}

	constexpr Vector2d(T x, T y)
		: x(x), y(y) {}

	constexpr Vector2d()
		: x(0), y(0) {}


	T square_distance() const
	{
		return x * x + y * y;
	}

	template <typename U>
	decltype(T()*U()) square_distance(const Vector2d<U>& other_point) const
	{
		return (x - other_point.x) * (x - other_point.x) + (y - other_point.y) * (y - other_point.y);
	}

	template <typename U>
	Vector2d<T>& operator+= (const Vector2d<U>& val)
	{
		x += val.x, y += val.y;
		return *this;
	}
	
	template <typename U>
	Vector2d<T>& operator-= (const Vector2d<U>& val)
	{
		x -= val.x, y -= val.y;
		return *this;
	}
	
	template <typename U>
	Vector2d<T>& operator*= (const Vector2d<U>& val)
	{
		x = static_cast<T>(x * val.x), y = static_cast<T>(y * val.y);
		return *this;
	}

	operator SDL_Point()
	{
		return { static_cast<int>(x), static_cast<int>(y) };
	}

	template <typename U>
	operator Vector2d<U>() const
	{
		return { static_cast<U>(x), static_cast<U>(y) };
	}
};

template <typename T>
using Point = Vector2d<T>;

template <typename T, typename U>
Vector2d<decltype(T()*U())> operator+(const Vector2d<T>& lhs, const Vector2d<U>& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

template <typename T, typename U>
Vector2d<decltype(T()*U())> operator-(const Vector2d<T>& lhs, const Vector2d<U>& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

template <typename T, typename U>
Vector2d<decltype(T()*U())> operator*(const T& lhs, const Vector2d<U>& rhs)
{
	return { lhs * rhs.x, lhs * rhs.y };
}

template <typename T, typename U>
Vector2d<decltype(T()*U())> operator*(const Vector2d<T>& lhs, const U& rhs)
{
	return { lhs.x * rhs, lhs.y * rhs };
}

template <typename T>
std::ostream& operator<<(std::ostream &os, const Vector2d<T>& v)
{
	return os << "(x = " << v.x << ", y = " << v.y << ")";
}

constexpr Vector2d<double> UNIT_VECTOR2D_X = { 1, 0 };
constexpr Vector2d<double> UNIT_VECTOR2D_Y = { 0, 1 };

template <typename T = int>
struct Range
{
	enum { RANGE_CIRCLE, RANGE_RECT } type;
	union
	{
		struct { Point<T> p1; Point<T> p2; };
		struct { Point<T> center; T r; };
	};

	constexpr Range(Point<T> p1, Point<T> p2)
		: type(RANGE_RECT), p1(p1), p2(p2) {};

	constexpr Range(Point<T> center, T r)
		: type(RANGE_CIRCLE), center(center), r(r) {};

	Range<T> flip()
	{
		if (type == RANGE_CIRCLE) return *this;
		else return { {-p2.x, p1.y}, {-p1.x, p2.y} };
	}

	// p3 = 오른쪽 위
	Point<T> get_p3() const
	{
		if (type == RANGE_CIRCLE) throw std::exception("Access to p3 of circle.");
		return { p2.x, p1.y };
	}
	
	// p4 = 왼쪽 아래
	Point<T> get_p4() const
	{
		if (type == RANGE_CIRCLE) throw std::exception("Access to p4 of circle.");
		return { p1.x, p2.y };
	}

	T get_w()
	{
		return p2.x - p1.x;
	}

	T get_h()
	{
		return p2.y - p1.y;
	}

	template <typename U>
	bool is_overlaped(const Point<U> point) const
	{
		switch(type) {
		case RANGE_CIRCLE:
			return center.square_distance(point) <= r * r;
		case RANGE_RECT:
			return p1.x <= point.x && point.x <= p2.x && p1.y <= point.y && point.y <= p2.y;
		default:
			throw std::exception("Unknown range type error");
		}
	}

	template <typename U>
	bool is_overlaped(const Range<U>& other) const
	{
		switch (type) {
		case RANGE_CIRCLE:
			switch (other.type)
			{
			case RANGE_CIRCLE:
				return center.square_distance(other.center) <= (other.r + r) * (other.r + r);
			case RANGE_RECT:
				if (other.is_overlaped(center)) return true;
				else if (other.p1.x <= center.x && center.x <= other.p2.x)
				{
					return std::min(abs(other.p1.y - center.y), abs(other.p2.y - center.y)) <= r;
				}
				else if (other.p1.y <= center.y && center.y <= other.p2.y)
				{
					return std::min(abs(other.p1.x - center.x), abs(other.p2.x - center.x)) <= r;
				}
				else
				{
					return std::min({
						other.p1.square_distance(center), other.p2.square_distance(center),
						other.get_p3().square_distance(center), other.get_p4().square_distance(center)
						}) <= r * r;
				}
			default:
				throw std::exception("Unknown range type error");
			}
		case RANGE_RECT:
			switch (other.type)
			{
			case RANGE_CIRCLE:
				return other.is_overlaped(*this);
			case RANGE_RECT:
				return (!(p2.x < other.p1.x || other.p2.x < p1.x))
					&& (!(p2.y < other.p1.y || other.p2.y < p1.y));
			}
		default:
			throw std::exception("Unknown range type error");
		}
	}

	template <typename U>
	Range<decltype(T()*U())> operator+=(const Vector2d<U>& rhs)
	{
		switch (type)
		{
		case RANGE_CIRCLE:
			center += rhs; break;
		case RANGE_RECT:
			p1 += rhs, p2 += rhs; break;
		default:
			std::exception("Unknown range type error");
		}
		return *this;
	}

	template <typename U>
	Range<decltype(T()*U())> operator-=(const Vector2d<U>& rhs)
	{
		switch (type)
		{
		case RANGE_CIRCLE:
			r -= rhs; break;
		case RANGE_RECT:
			p1 -= rhs, p2 -= rhs; break;
		default:
			std::exception("Unknown range type error");
		}
		return *this;
	}

	template <typename U>
	Range<decltype(T()*U())> operator+(const Vector2d<U>& rhs) const
	{
		return Range<decltype(T()*U())>(*this) += rhs;
	}

	template <typename U>
	Range<decltype(T()*U())> operator-(const Vector2d<U>& rhs) const
	{
		return Range<decltype(T()*U())>(*this) -= rhs;
	}

	operator SDL_Rect() const
	{
		SDL_Rect rect = { p1.x, p1.y, p2.x - p1.x, p2.y - p1.y };
		return rect;
	}

	template <typename U>
	operator Range<U>() const
	{
		switch (type)
		{
		case RANGE_CIRCLE:
			return Range<U>(static_cast<Point<U>>(center), static_cast<U>(r));
		case RANGE_RECT:
			return Range<U>(static_cast<Point<U>>(p1), static_cast<Point<U>>(p2));
		default:
			std::exception("Unknown range type error");
		}
		return Range<U>({ 0, 0 }, { 0, 0 });
	}
};

template <typename T = int>
using Circle = Range<T>;

template <typename T = int>
using Rect = Range<T>;

template <typename T>
struct Unordered_list
{
private:
	std::vector<T> list_vector;
public:
	using iterator = typename std::vector<T>::iterator;

	iterator begin() {
		return list_vector.begin();
	}

	iterator end()
	{
		return list_vector.end();
	}
	
	void insert(const T& _Val)
	{
		list_vector.push_back(_Val);
	}

	void insert(T&& _Val)
	{
		list_vector.push_back(_Val);
	}

	iterator erase(iterator _Where)
	{
		if (_Where == list_vector.end()) return _Where;

		iterator next = _Where;
		++next;

		if (next != list_vector.end())
		{
			std::swap(*_Where, *list_vector.rbegin());
			list_vector.pop_back();
			return _Where;
		}
		else
		{
			list_vector.pop_back();
			return list_vector.end();
		}

	}
};

class RandomGenerator
{
	static std::random_device	rd_;
	static std::mt19937			generator_;

public:
	RandomGenerator() = delete;

	static int rand(int min, int max)
	{
		return std::uniform_int_distribution<>(min, max)(generator_);
	}

	~RandomGenerator() {};
};
