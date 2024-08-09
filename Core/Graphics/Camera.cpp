#include "Camera.h"

namespace CGL::Graphics
{
    CGL_DEFINE_LOG_CATEGORY(Camera);

    Camera::Camera()
        : m_position(0.0f, 0.0f, -1.0f)
        , m_target(0.0f, 0.0f, 0.0f)
        , m_angle(0.0f)
        , m_width(0.0f)
        , m_height(0.0f)
        , m_nearPlane(0.1f)
        , m_farPlane(1000.0f)
        , m_view(SM::Matrix::Identity)
        , m_proj(SM::Matrix::Identity)
        , m_ortho(SM::Matrix::Identity)
    {
        m_up = m_position + SM::Vector3::Up;

        InitViewMatrix();
    }

    void Camera::InitAsPerspective(const f32 angle, const f32 width, const f32 height, const f32 nearPlane,
                                   const f32 farPlane)
    {
        m_angle     = angle;
        m_width     = width;
        m_height    = height;
        m_nearPlane = nearPlane;
        m_farPlane  = farPlane;

        m_proj = XMMatrixPerspectiveFovLH(m_angle, (m_width / m_height), m_nearPlane, m_farPlane);
    }

    void Camera::InitAsOrthographic(const f32 width, const f32 height, const f32 nearPlane, const f32 farPlane)
    {
        m_width     = width;
        m_height    = height;
        m_nearPlane = nearPlane;
        m_farPlane  = farPlane;

        m_ortho = XMMatrixOrthographicLH(m_width, m_height, m_nearPlane, m_farPlane);
    }

    void Camera::OnResize(u32 width, u32 height)
    {
        m_width  = f32(width);
        m_height = f32(height);

        InitAsPerspective(m_angle, m_width, m_height, m_nearPlane, m_farPlane);
        InitAsOrthographic(m_width, m_height, m_nearPlane, m_farPlane);
    }

    void Camera::Translate(SM::Vector3 direction)
    {
        SM::Matrix translation = SM::Matrix::CreateTranslation(direction);

        m_up       = SM::Vector3::Transform(m_up, translation);
        m_target   = SM::Vector3::Transform(m_target, translation);
        m_position = SM::Vector3::Transform(m_position, translation);

        InitViewMatrix();
    }

    void Camera::Rotate(SM::Vector3 axis, f32 angle)
    {
        if (axis == SM::Vector3::Zero || angle == 0.0f)
        {
            CGL_LOG(Camera, Warn, "Failed to rotate camera! Either axis or angle was 0");
            return;
        }

        SM::Matrix rotation      = SM::Matrix::CreateFromAxisAngle(axis, XMConvertToRadians(angle));
        SM::Vector3 lookAtTarget = m_target - m_position;
        SM::Vector3 lookAtUp     = m_up - m_position;

        lookAtTarget = SM::Vector3::Transform(lookAtTarget, rotation);
        lookAtUp     = SM::Vector3::Transform(lookAtUp, rotation);

        // Restore end points for m_target and m_up from new rotated vectors
        m_target = m_position + lookAtTarget;
        m_up     = m_position + lookAtUp;

        InitViewMatrix();
    }

    void Camera::LookAt(SM::Vector3 target)
    {
        if (target == m_position || target == m_target)
        {
            CGL_LOG(Camera, Warn,
                    "Failed to set camera LookAt! Either targets are same or the "
                    "target is same as the position.");
            return;
        }

        SM::Vector3 oldLookAtTarget = m_target - m_position;
        SM::Vector3 newLookAtTarget = target - m_position;

        SM::Vector3 oldLookAtTargetNormalized = oldLookAtTarget;
        SM::Vector3 newLookAtTargetNormalized = newLookAtTarget;
        oldLookAtTargetNormalized.Normalize();
        newLookAtTargetNormalized.Normalize();

        SM::Vector3 angleVec = XMVector3AngleBetweenNormals(oldLookAtTargetNormalized, newLookAtTargetNormalized);
        f32 angle            = XMConvertToDegrees(angleVec.x);

        if (angle != 0.0f && angle != 360.0f && angle != 180.0f)
        {
            SM::Vector3 axis = oldLookAtTarget.Cross(newLookAtTarget);
            Rotate(axis, angle);
        }

        m_target = target;
        InitViewMatrix();
    }

    const SM::Vector3 Camera::GetLookAtTarget() const
    {
        return m_target - m_position;
    }

    void Camera::SetPosition(const SM::Vector3& position)
    {
        SM::Vector3 moveVector = position - m_position;
        SM::Vector3 target     = m_target;
        Translate(moveVector);
        LookAt(target);
    }

    void Camera::SetAngle(const f32 angle)
    {
        m_angle = angle;
        InitAsPerspective(m_angle, m_width, m_height, m_nearPlane, m_farPlane);
    }

    void Camera::SetNearPlane(const f32 nearPlane)
    {
        m_nearPlane = nearPlane;
        OnResize(u32(m_width), u32(m_height));
    }

    void Camera::SetFarPlane(const f32 farPlane)
    {
        m_farPlane = farPlane;
        OnResize(u32(m_width), u32(m_height));
    }

    void Camera::InitViewMatrix()
    {
        m_view = XMMatrixLookAtLH(m_position, m_target, m_up);
    }
}  // namespace CGL::Graphics
