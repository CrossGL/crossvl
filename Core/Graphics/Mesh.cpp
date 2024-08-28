#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#define NULL nullptr
#include <tiny_obj_loader.h>
#undef NULL

namespace CGL::Graphics
{
	CGL_DEFINE_LOG_CATEGORY(Mesh);
	
	bool Mesh::LoadOBJ(const std::string& file)
	{
		namespace obj = tinyobj;

		obj::ObjReader reader;

		if (!reader.ParseFromFile(file))
		{
			if (!reader.Error().empty())
			{
				CGL_LOG(Mesh, Error, "Failed to parse OBJ file: {}", reader.Error());
			}
			return false;
		}

		if (!reader.Warning().empty())
		{
			CGL_LOG(Mesh, Warn, "Failed to parse OBJ file: {}", reader.Warning());
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();

		// Clear any existing SubMeshes
		SubMeshes.clear();

		// Each shape corresponds to a mesh
		for (const auto& shape : shapes)
		{
			SubMesh subMesh;

			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
			{
				size_t fv = shape.mesh.num_face_vertices[f];

				// Loop over vertices in the face
				for (size_t v = 0; v < fv; v++)
				{
					// Access to vertex index
					tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

					// Vertex coordinates
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					subMesh.m_vertices.push_back(SM::Vector3(vx, vy, vz));

					// Normals (if present)
					if (idx.normal_index >= 0)
					{
						tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
						tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
						tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
						subMesh.m_normals.push_back(SM::Vector3(nx, ny, nz));
					}

					// Texture coordinates (if present)
					if (idx.texcoord_index >= 0)
					{
						tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
						tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
						subMesh.m_uvs.push_back(SM::Vector2(tx, ty));
					}

					// Save index (used in drawing the elements)
					subMesh.m_indices.push_back(static_cast<u32>(index_offset + v));
				}
				index_offset += fv;
			}

			// Add the processed submesh to the mesh
			SubMeshes.push_back(subMesh);
		}

		CGL_LOG(Mesh, Info, "Successfully loaded OBJ file: {}", file);
		return true;
	}
}