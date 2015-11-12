// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Meshes.cpp
// Description	: CMeshes implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "ClothSimulatorStd.h"

// Library Includes

// This Include
#include "Meshes.h"

// Local Includes
#include <Graphics/SlimRenderer.h>

namespace Meshes {

	void CreateSphere(int rings, int segments, std::shared_ptr<AVertexGpuBuffer>& pVertexBuffer, std::shared_ptr<AIndexGpuBuffer>& pIndexBuffer)
	{
		std::vector<TVertex> vertices;
		std::vector<int> indices;

		float fDeltaRingAngle = Math::s_PI / rings;
		float fDeltaSegAngle = 2 * Math::s_PI / segments;

		for (int ring = 0; ring <= rings; ++ring) {
			float r0 = std::sinf(ring * fDeltaRingAngle);
			float y0 = std::cosf(ring * fDeltaRingAngle);

			for (int seg = 0; seg <= segments; ++seg) {
				float x0 = r0 * std::sinf(seg * fDeltaSegAngle);
				float z0 = r0 * std::cosf(seg * fDeltaSegAngle);

				TVertex vertex;

				vertex.position = CVector3(x0, y0, z0);
				vertex.u = static_cast<float>(seg) / static_cast<float>(segments);
				vertex.v = static_cast<float>(ring) / static_cast<float>(rings);
				vertex.normal = CVector3::Normalise(r0 * vertex.position);
				vertex.burntLevel = 0.0f;

				vertices.push_back(vertex);
			}
		}

		for (int ring = 0; ring < rings; ++ring) {
			for (int seg = 0; seg < segments; ++seg) {
				int first = (ring * (segments + 1) + seg);
				int second = first + segments + 1;

				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);

				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}

		pVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
		pIndexBuffer = g_pApp->GetRenderer()->CreateIndexBuffer(indices);
	}

	void CreateCapsule(float radius, float height, size_t rings, size_t segments, size_t verticalSegments, std::shared_ptr<AVertexGpuBuffer>& pVertexBuffer, std::shared_ptr<AIndexGpuBuffer>& pIndexBuffer)
	{
		std::vector<TVertex> vertices;
		std::vector<int> indices;

		auto deltaRingAngle = (Math::s_PI / 2.0f) / static_cast<float>(rings);
		auto deltaSegAngle = (Math::s_PI * 2.0f) / static_cast<float>(segments);

		auto sphereRatio = radius / (2.0f * radius + height);
		auto cylinderRatio = height / (2.0f * radius + height);

		size_t offset = 0;

		for (size_t ring = 0; ring <= rings; ++ring) {
			float r0 = radius * sinf(ring * deltaRingAngle);
			float y0 = radius * cosf(ring * deltaRingAngle);

			// Generate the group of segments for the current ring
			for (size_t seg = 0; seg <= segments; ++seg) {
				float x0 = r0 * cosf(seg * deltaSegAngle);
				float z0 = r0 * sinf(seg * deltaSegAngle);

				// Add one vertex to the strip which makes up the sphere
				TVertex vertex;
				vertex.position = CVector3(x0, 0.5f * height + y0, z0);
				vertex.normal = CVector3::Normalise(CVector3(x0, y0, z0));
				vertex.u = seg / static_cast<float>(segments);
				vertex.v = ring / static_cast<float>(rings) * sphereRatio;
				vertex.burntLevel = 0.0f;

				vertices.push_back(vertex);

				// Each vertex (except the last) has six indices pointing to it
				indices.push_back(offset + segments + 1);
				indices.push_back(offset + segments);
				indices.push_back(offset);
				indices.push_back(offset + segments + 1);
				indices.push_back(offset);
				indices.push_back(offset + 1);

				offset++;
			}
		}

		// Cylinder part
		float deltaAngle = (Math::s_PI * 2.0f) / static_cast<float>(segments);
		float deltaHeight = height / static_cast<float>(verticalSegments);

		for (size_t i = 1; i < verticalSegments; i++) {
			for (size_t j = 0; j <= segments; j++) {
				float x0 = radius * cosf(j * deltaAngle);
				float z0 = radius * sinf(j * deltaAngle);

				// Add one vertex to the strip which makes up the sphere
				TVertex vertex;
				vertex.position = CVector3(x0, 0.5f * height - i * deltaHeight, z0);
				vertex.normal = CVector3::Normalise(CVector3(x0, 0, z0));
				vertex.u = i / static_cast<float>(verticalSegments);
				vertex.v = j / static_cast<float>(segments) * cylinderRatio + sphereRatio;
				vertex.burntLevel = 0.0f;

				vertices.push_back(vertex);

				// Each vertex (except the last) has six indices pointing to it
				indices.push_back(offset + segments + 1);
				indices.push_back(offset + segments);
				indices.push_back(offset);
				indices.push_back(offset + segments + 1);
				indices.push_back(offset);
				indices.push_back(offset + 1);

				offset++;
			}
		}

		// Generate the group of rings for the sphere
		for (size_t ring = 0; ring <= rings; ring++) {
			float r0 = radius * sinf((Math::s_PI / 2.0f) + ring * deltaRingAngle);
			float y0 = radius * cosf((Math::s_PI / 2.0f) + ring * deltaRingAngle);

			// Generate the group of segments for the current ring
			for (size_t seg = 0; seg <= segments; seg++) {
				float x0 = r0 * cosf(seg * deltaSegAngle);
				float z0 = r0 * sinf(seg * deltaSegAngle);

				// Add one vertex to the strip which makes up the sphere
				TVertex vertex;
				vertex.position = CVector3(x0, -0.5f * height + y0, z0);
				vertex.normal = CVector3::Normalise(CVector3(x0, y0, z0));
				vertex.u = seg / static_cast<float>(segments);
				vertex.v = ring / static_cast<float>(rings) * sphereRatio + cylinderRatio + sphereRatio;
				vertex.burntLevel = 0.0f;

				vertices.push_back(vertex);

				// Each vertex (except the last) has six indices pointing to it
				if (ring != rings) {
					indices.push_back(offset + segments + 1);
					indices.push_back(offset + segments);
					indices.push_back(offset);
					indices.push_back(offset + segments + 1);
					indices.push_back(offset);
					indices.push_back(offset + 1);
				}

				offset++;
			}
		}

		pVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(vertices);
		pIndexBuffer = g_pApp->GetRenderer()->CreateIndexBuffer(indices);
	}

	void CreateQuad(float width, float height, std::shared_ptr<AVertexGpuBuffer>& pVertexBuffer)
	{
		float halfWidth = width / 2.0f;
		float halfHeight = height / 2.0f;

		std::vector<TVertex> groundVertices = {
			{ CVector3(-halfWidth, 0.0f, -halfHeight), CVector3::s_UP, 0.0f, 0.0f, 0.0f, CColourValue::s_GREEN },
			{ CVector3(-halfWidth, 0.0f, halfHeight), CVector3::s_UP, 0.0f, 1.0f, 0.0f, CColourValue::s_GREEN },
			{ CVector3(halfWidth, 0.0f, -halfHeight), CVector3::s_UP, 1.0f, 0.0f, 0.0f, CColourValue::s_GREEN },
			{ CVector3(halfWidth, 0.0f, halfHeight), CVector3::s_UP, 1.0f, 1.0f, 0.0f, CColourValue::s_GREEN }
		};

		pVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(groundVertices);
	}

	void CreatePyramid(std::shared_ptr<AVertexGpuBuffer>& pVertexBuffer)
	{
		std::vector<TVertex> pyramidVertices = {
			{ CVector3(0.0f, 0.5f, 0.0f), CVector3(-1.0f, 1.0f, 0.0f), 1.0f, 1.0f,		0.0f, CColourValue::s_GREEN }, // Left side.
			{ CVector3(-0.5f, -0.5f, 0.5f), CVector3(-1.0f, 1.0f, 0.0f), 0.0f, 0.0f,	0.0f, CColourValue::s_GREEN }, // Left side.
			{ CVector3(-0.5f, -0.5f, -0.5f), CVector3(-1.0f, 1.0f, 0.0f), 0.0f, 0.0f,	0.0f, CColourValue::s_GREEN }, // Left side.
																			
			{ CVector3(0.0f, 0.5f, 0.0f), CVector3(1.0f, 1.0f, 0.0f), 1.0f, 1.0f,		0.0f, CColourValue::s_GREEN }, // Right side.
			{ CVector3(0.5f, -0.5f, -0.5f), CVector3(1.0f, 1.0f, 0.0f), 0.0f, 0.0f,		0.0f, CColourValue::s_GREEN }, // Right side.
			{ CVector3(0.5f, -0.5f, 0.5f), CVector3(1.0f, 1.0f, 0.0f), 0.0f, 0.0f,		0.0f, CColourValue::s_GREEN }, // Right side.
																				
			{ CVector3(0.0f, 0.5f, 0.0f), CVector3(0.0f, 1.0f, 1.0f), 1.0f, 1.0f,		0.0f, CColourValue::s_GREEN }, // Front side.
			{ CVector3(0.5f, -0.5f, 0.5f), CVector3(0.0f, 1.0f, 1.0f), 0.0f, 0.0f,		0.0f, CColourValue::s_GREEN }, // Front side.
			{ CVector3(-0.5f, -0.5f, 0.5f), CVector3(0.0f, 1.0f, 1.0f), 0.0f, 0.0f,		0.0f, CColourValue::s_GREEN }, // Front side.
																					
			{ CVector3(0.0f, 0.5f, 0.0f), CVector3(0.0f, 1.0f, -1.0f), 1.0f, 1.0f,		0.0f, CColourValue::s_GREEN }, // Back side.
			{ CVector3(-0.5f, -0.5f, -0.5f), CVector3(0.0f, 1.0f, -1.0f), 0.0f, 0.0f,	0.0f, CColourValue::s_GREEN }, // Back side.
			{ CVector3(0.5f, -0.5f, -0.5f), CVector3(0.0f, 1.0f, -1.0f), 0.0f, 0.0f,	0.0f, CColourValue::s_GREEN }, // Back side.
																					
			{ CVector3(-0.5f, -0.5f, 0.5f), CVector3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f,	0.0f, CColourValue::s_GREEN }, // Bottom.
			{ CVector3(0.5f, -0.5f, 0.5f), CVector3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f,		0.0f, CColourValue::s_GREEN }, // Bottom.
			{ CVector3(-0.5f, -0.5f, -0.5f), CVector3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f,	0.0f, CColourValue::s_GREEN }, // Bottom.
																					
			{ CVector3(-0.5f, -0.5f, -0.5f), CVector3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f,	0.0f, CColourValue::s_GREEN }, // Bottom 2.
			{ CVector3(0.5f, -0.5f, 0.5f), CVector3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f,		0.0f, CColourValue::s_GREEN }, // Bottom 2.
			{ CVector3(0.5f, -0.5f, -0.5f), CVector3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f,	0.0f, CColourValue::s_GREEN }, // Bottom 2.
		};

		pVertexBuffer = g_pApp->GetRenderer()->CreateVertexBuffer(pyramidVertices);
	}

}

