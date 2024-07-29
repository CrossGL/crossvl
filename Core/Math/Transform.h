#pragma
#include <Core/Common.h>
#include <Core/Math/Internal/MathInternal.h>

namespace CGL::Math
{
	class OrthogonalTransform;
	class ScaleAndTranslationTransform;
	class AffineTransform;
	class UniformTransform;

	// Transform for rotation and translation
	//class OrthogonalTransform
	//{
	//public:
	//	inline OrthogonalTransform() : m_rotation(Quaternion::Identity), m_translation(Vector3::Zero) {}
	//	inline OrthogonalTransform(const Quaternion& rotation) : m_rotation(rotation), m_translation(Vector3::Zero) {}
	//	inline OrthogonalTransform(const Vector3& translation) : m_rotation(Quaternion::Identity), m_translation(translation) {}
	//	inline OrthogonalTransform(const Quaternion& rotation, Vector3 translation) : m_rotation(rotation), m_translation(translation) {}
	//	inline OrthogonalTransform(const Matrix& rotationMatrix) : m_rotation(rotationMatrix), m_translation(Vector3::Zero) {}
	//	inline OrthogonalTransform(const Matrix& rotationMatrix, Vector3 translation) : m_rotation(rotationMatrix), m_translation(translation) {}

	//	inline void SetRotation(const Quaternion& rotation) { m_rotation = rotation; }
	//	inline void SetTranslation(const Vector3& translation) { m_translation = translation; }
	//	inline Quaternion GetRotation() const { return m_rotation; }
	//	inline Vector3 GetTranslation() const { return m_translation; }

	//private:
	//	Quaternion m_rotation;
	//	Vector3 m_translation;
	//};

	//// Transform without rotation and uniform scale
	//class ScaleAndTranslationTransform
	//{
	//public:
	//	inline ScaleAndTranslationTransform() : m_scaleAndTranslation(Vector4::One) {}
	//	inline ScaleAndTranslationTransform(f32 translateX, f32 translateY, f32 translateZ, f32 scale) : m_scaleAndTranslation(translateX, translateY, translateZ, scale) {}
	//	inline ScaleAndTranslationTransform(const Vector4& scaleAndTranslation) : m_scaleAndTranslation(scaleAndTranslation) {}
	//	inline ScaleAndTranslationTransform(const Vector3& translate, f32 scale) : m_scaleAndTranslation(translate.x, translate.y, translate.z, scale) {}

	//	inline void SetScale(f32 scale) { m_scaleAndTranslation.w = scale; }
	//	inline void SetTranslation(const Vector3& translate) { m_scaleAndTranslation.x = translate.x; m_scaleAndTranslation.y = translate.y; m_scaleAndTranslation.z = translate.z; }
	//	inline f32 GetScale() const { return m_scaleAndTranslation.w; }
	//	inline Vector3 GetTranslation() const { return Vector3(m_scaleAndTranslation.x, m_scaleAndTranslation.y, m_scaleAndTranslation.z); }

	//private:
	//	Vector4 m_scaleAndTranslation;  // X, Y, Z for translation, W for scale
	//};
}
