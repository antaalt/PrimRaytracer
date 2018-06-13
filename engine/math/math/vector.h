#if defined(TYPE_H) && !defined(VECTOR_H)
#define VECTOR_H
#include <ostream>
namespace Type {

		constexpr bool isDisplayable(int N, int limit)
		{
			return N > limit;
		}
	

	template <typename T, int N>
	class Vector {
		static_assert(N > 1 && N < 5, "Vector support only 2, 3, 4 components");
	public:
		Vector();
		Vector(const T a1, const T a2);
		Vector(const T a1, const T a2, const T a3);
		Vector(const T a1, const T a2, const T a3, const T a4);
		Vector(const Vector<T, N> &vec);
		~Vector();

		T &operator[](const int component);
		const T &operator[](const int component) const;

		inline bool operator<(const Vector<T, N> &v) const;
		inline bool operator>(const Vector<T, N> &v) const;

		inline bool operator==(const Vector<T, N> &v) const;
		inline bool operator!=(const Vector<T, N> &v) const;

		template <typename T, int N, typename C>
		friend Vector<T, N> operator*(const Vector<T, N> &vec, const C scalar);

		template <typename T, int N, typename C>
		friend Vector<T, N> operator/(const Vector<T, N> &vec, const C scalar);

		inline Vector<T, N> operator+(const Vector<T, N> &i) const;
		inline Vector<T, N> operator+(const T &val) const;
		inline Vector<T, N> operator-(const Vector<T, N> &i) const;
		inline Vector<T, N> operator-(const T &val) const;

		template <typename T, int N>
		friend std::ostream& operator <<(std::ostream& os, const Vector<T, N>& vec);

	public:
		float length() const;
		Vector<T, N> reverse() const;

	public:
		union {
			struct {
				T x;
				T y;
				typename std::enable_if<isDisplayable(N, 2), T>::type z;
				typename std::enable_if<isDisplayable(N, 3), T>::type w;
			};
			T data[N];
		};
	};

	// Declarations

	template<typename T, int N>
	inline Vector<T, N>::Vector() {}
	template<typename T, int N>
	inline Vector<T, N>::Vector(const T a1, const T a2) : x(a1), y(a2) {}
	template<typename T, int N>
	inline Vector<T, N>::Vector(const T a1, const T a2, const T a3) : x(a1), y(a2), z(a3) {/* static_assert(N > 2);*/ }
	template<typename T, int N>
	inline Vector<T, N>::Vector(const T a1, const T a2, const T a3, const T a4) : x(a1), y(a2), z(a3), w(a4) { static_assert(N > 3); }
	template<typename T, int N>
	inline Vector<T, N>::Vector(const Vector<T, N>& vec)
	{
		for (int i = 0; i < N; i++)
			data[i] = vec.data[i];
	}
	template<typename T, int N>
	inline Vector<T, N>::~Vector() {}
	// OPERATORS
	template<typename T, int N>
	inline T & Vector<T, N>::operator[](const int component)
	{
		ASSERT(component < N, "index out of range");
		return data[component];
	}
	template<typename T, int N>
	inline const T & Vector<T, N>::operator[](const int component) const
	{
		ASSERT(component < N, "index out of range");
		return data[component];
	}
	template<typename T, int N>
	inline bool Vector<T, N>::operator<(const Vector<T, N>& v) const
	{
		for (int i = 0; i < N; i++)
		{
			if (data[i] < v.data[i])
				return true;
			if (data[i] > v.data[i])
				return false;
		}
		return false;
	}
	template<typename T, int N>
	inline bool Vector<T, N>::operator>(const Vector<T, N>& v) const
	{
		for (int i = 0; i < N; i++)
		{
			if (data[i] > v.data[i])
				return true;
			if (data[i] < v.data[i])
				return false;
		}
		return false;
	}
	template<typename T, int N>
	inline bool Vector<T, N>::operator==(const Vector<T, N>& v) const
	{
		for (int i = 0; i < N; i++)
			if (data[i] != v.data[i])
				return false;
		return true;
	}
	template<typename T, int N>
	inline bool Vector<T, N>::operator!=(const Vector<T, N>& v) const
	{
		for (int i = 0; i < N; i++)
			if (data[i] != v.data[i])
				return true;
		return false;
	}

	template<typename T, int N>
	constexpr float _length(const Vector<T, N> &vec)
	{
		switch (N)
		{
		case 2:
			return x*x + y*y;
			break;
		case 3:
			return x*x + y*y + z*z;
			break;
		case 4:
			return x*x + y*y + z*z + w*w;
			break;
		default:
			break;
		}
	}
	template<typename T, int N>
	inline float Vector<T, N>::length() const
	{
		return _length(*this);
	}
	template<typename T, int N>
	std::ostream & operator<<(std::ostream & os, const Vector<T, N>& vec)
	{
		// TODO: insert return statement here
	}
}
#endif