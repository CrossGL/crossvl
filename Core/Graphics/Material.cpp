#include "Material.h"

namespace CGL::Graphics
{
	CGL_DEFINE_LOG_CATEGORY(Material);

	Material::Material()
		: m_id(0)
	{
		m_vs = std::make_unique<ShaderProgram<VertexShader>>();
		m_ps = std::make_unique<ShaderProgram<PixelShader>>();

		m_vs->State = ShaderState::Invalid;
		m_ps->State = ShaderState::Invalid;
	}

	Material::~Material()
	{
		m_vs.reset();
		m_ps.reset();
	}

	void Material::AddSource(const ShaderSource& source)
	{
		switch (source.Type)
		{
		case ShaderType::Vertex:
		{
			assert(m_vs);
			m_vs->State  = ShaderState::CompilePending;
			m_vs->Source = source;
			break;
		}

		case ShaderType::Pixel:
		{
			assert(m_ps);
			m_ps->State = ShaderState::CompilePending;
			m_ps->Source = source;
			break;
		}

		default:
			CGL_LOG(Material, Error, "Unable to set shader source for given shader source! Error: Undefined shader type!");
		}
	}
}