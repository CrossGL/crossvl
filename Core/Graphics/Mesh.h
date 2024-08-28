#pragma once
#include <Core/Common.h>
#include <Core/Logging/Log.h>
#include <Core/Math/Math.h>
#include <Core/Graphics/Buffer.h>

namespace CGL::Graphics
{
	CGL_DECLARE_LOG_CATEGORY(Mesh);

	struct MeshData
	{
		VertexBuffer VBuffer;
		IndexBuffer IBuffer;
	};


	class SubMesh
	{
		friend class Mesh;
	public:
		SubMesh() = default;
		~SubMesh() = default;

		inline const std::vector<SM::Vector3>& GetVertices() const { return m_vertices; }
		inline const std::vector<SM::Vector3>& GetNormals() const { return m_normals; }
		inline const std::vector<SM::Vector2>& GetUVs() const { return m_uvs; }
		inline const std::vector<u32>& GetIndices() const { return m_indices; }
		inline MeshData& GetMeshData() { return m_meshData; }

	private:
		MeshData                 m_meshData;
		std::vector<u32>         m_indices;
		std::vector<SM::Vector3> m_vertices;
		std::vector<SM::Vector3> m_normals;
		std::vector<SM::Vector2> m_uvs;
	};


	class Mesh
	{
	public:
		Mesh() = default;
		~Mesh() = default;

		bool LoadOBJ(const std::string& file);

	public:
		std::vector<SubMesh> SubMeshes;
	};
}