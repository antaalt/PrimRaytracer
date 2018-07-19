#pragma once

namespace Application {


	namespace Math {

		template <typename T>
		struct Matrix4 {
			union {
				T data[4 * 4];
				struct {
					T	xx, yx, zx, wx,
						xy, yy, zy, wy,
						xz, yz, zz, wz,
						xw, yw, zw, ww;
				};
			};
			Matrix4() {}
		};
	}
}