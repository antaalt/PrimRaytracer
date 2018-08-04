#include "Camera.h"


namespace app {

	namespace tracer {

		Camera::Camera(unsigned int width, unsigned int height) : m_transform(Matrix4::identity()), m_width(width), m_height(height)
		{
		}


		Camera::~Camera()
		{
		}
		void Camera::lookAt(const Point3 & eye, const Point3 & target, const Vector3 &up)
		{
			Vector3 forward = Vector3::normalize(target - eye);
			Vector3 right = Vector3::cross(Vector3::normalize(up), forward);
			Vector3 upCoordinate = Vector3::cross(forward, right);

			m_transform[0] = convert::toVec4(right, 0.f);
			m_transform[1] = convert::toVec4(upCoordinate, 0.f);
			m_transform[2] = convert::toVec4(forward, 0.f);
			m_transform[3] = convert::toVec4(eye, 1.f);
			m_changed = true;
		}
		void Camera::rotate(float angle, const Vector3 & axis)
		{
			Matrix4 m;
			float radians = angle * M_PIf / 180.f;
			// NOTE: Element 0,1 is wrong in Foley and Van Dam, Pg 227!
			float sintheta = sinf(radians);
			float costheta = cosf(radians);
			Vector3 an = Vector3::normalize(axis);
			float ux = an.x;
			float uy = an.y;
			float uz = an.z;
			m[0][0] = ux*ux + costheta*(1 - ux*ux);
			m[0][1] = ux*uy*(1 - costheta) - uz*sintheta;
			m[0][2] = uz*ux*(1 - costheta) + uy*sintheta;
			m[0][3] = 0.f;

			m[1][0] = ux*uy*(1 - costheta) + uz*sintheta;
			m[1][1] = uy*uy + costheta*(1 - uy*uy);
			m[1][2] = uy*uz*(1 - costheta) - ux*sintheta;
			m[1][3] = 0;

			m[2][0] = uz*ux*(1 - costheta) - uy*sintheta;
			m[2][1] = uy*uz*(1 - costheta) + ux*sintheta;
			m[2][2] = uz*uz + costheta*(1 - uz*uz);
			m[2][3] = 0;

			m[3][0] = 0;
			m[3][1] = 0;
			m[3][2] = 0;
			m[3][3] = 1;
			m_transform = m_transform * m;
			m_changed = true;
		}
		void Camera::translate(const Vector3 & translation)
		{
			Matrix4 m = Matrix4::identity();
			m[3].x = translation.x;
			m[3].y = translation.y;
			m[3].z = translation.z;
			m_transform = m_transform * m;
			m_changed = true;
		}
	}
}