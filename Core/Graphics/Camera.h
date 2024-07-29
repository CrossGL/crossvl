#pragma once
#include <Core/Common.h>
#include <Core/Math/Transform.h>

namespace CGL::Core
{
	class CameraBase
	{
	public:
		void Update();

	protected:
		CameraBase();

	protected:
		SM::Matrix m_basis;
		SM::Matrix m_viewMatrix;      // World to view matrix
		SM::Matrix m_projMatrix;      // View to projection matrix
		SM::Matrix m_viewProjMatrix;  // World to projection matrix
		SM::Matrix m_prevViewProjMatrix;
		SM::Matrix m_repojectMatrix;

		// TODO: Add frustum class
	};
}
