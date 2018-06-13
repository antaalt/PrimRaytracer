#if defined(TYPE_H) && !defined(MATRIX_H)
#define MATRIX_H

#include <ostream>
namespace Type {

	template <typename T, int N>
	class Matrix {
		static_assert(N > 1 && N < 5, "Matrix support only 2, 3, 4 dimensions");
	public:

#if defined(USE_VECTOR)
		Vector<T, N> &operator[](const int col);
		const Vector<T, N> &operator[](const int col) const;
#else

		std::array<T, N> &operator[](const int col);
		const std::array<T, N> &operator[](const int col) const;
#endif

		template <typename T, int N>
		friend std::ostream& operator <<(std::ostream& os, const Matrix<T, N>& mat);

	public:
		inline Matrix<T, N> transpose() const;
		inline Matrix<T, N> inverse() const;

		static Matrix<T, N> identity();

	private:
		union {
#if defined(USE_VECTOR)
			Vector<T, N> m_cols[N];
#else
			std::array<T, N> m_cols[N];
#endif
			T m_data[N * N];
		};
	};
}
#endif // MATRIX_H