//////////////////////////////////////////////////////////////////////////
// This file is part of the "LibRenderer" 3D graphics library           //
//                                                                      //
// Copyright (C) 2014 - Iftode Bogdan-Marius <iftode.bogdan@gmail.com>  //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not, see <http://www.gnu.org/licenses/>. //
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <Importer.hpp> // C++ importer interface
#include <scene.h> // Output data structure
#include <postprocess.h> // Post processing flags

#include "Renderer.h"
#include "VertexFormat.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ResourceManager.h"
#include "ModelLoader.h"
using namespace LibRendererDll;

const unsigned int ModelLoader::LoadModelToVertexBuffer(const char* path, VertexFormat*& vfOut, VertexBuffer*& vbOut, IndexBuffer*& ibOut)
{
	ResourceManager* const resMan = Renderer::GetInstance()->GetResourceManager();

	// Create an instance of the Importer class
	Assimp::Importer importer;
	const aiScene* scene = nullptr;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// propably to request more postprocessing than we do in this example.
	unsigned int ppFlags = aiProcessPreset_TargetRealtime_MaxQuality & ~aiProcess_FindDegenerates;
	if (Renderer::GetAPI() == API_DX9)
		ppFlags |= aiProcess_ConvertToLeftHanded;
	scene = importer.ReadFile(path, ppFlags);

	std::vector<VertexAttributeUsage> arrVAU;
	unsigned int maxUVComponents = 0;
	unsigned int maxUVChannels = 1;		// In case we don't have any UVs or vertex colors
	unsigned int maxColorChannels = 1;	// arrVAU.size() + maxUVChannels + maxColorChannels - 2
	unsigned int totalIndexCount = 0;	// will still be correct
	unsigned int totalVertexCount = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		totalIndexCount += scene->mMeshes[i]->mNumFaces * 3;
		totalVertexCount += scene->mMeshes[i]->mNumVertices;

		if (scene->mMeshes[i]->HasPositions())
			if (std::find(arrVAU.begin(), arrVAU.end(), VAU_POSITION) == arrVAU.end())
				arrVAU.push_back(VAU_POSITION);

		if (scene->mMeshes[i]->HasNormals())
			if (std::find(arrVAU.begin(), arrVAU.end(), VAU_NORMAL) == arrVAU.end())
				arrVAU.push_back(VAU_NORMAL);

		bool hasTexCoords = false;
		for (unsigned int j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; j++)
			if (scene->mMeshes[i]->HasTextureCoords(j))
			{
				hasTexCoords = true;
				maxUVChannels = j + 1;
			}
		if (hasTexCoords)
		{
			for (unsigned int j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; j++)
				if (scene->mMeshes[i]->mNumUVComponents[j] > maxUVComponents)
					maxUVComponents = scene->mMeshes[i]->mNumUVComponents[j];
			if (std::find(arrVAU.begin(), arrVAU.end(), VAU_TEXCOORD) == arrVAU.end())
				arrVAU.push_back(VAU_TEXCOORD);
		}

		bool hasVertexColors = false;
		for (unsigned int j = 0; j < AI_MAX_NUMBER_OF_COLOR_SETS; j++)
			if (scene->mMeshes[i]->HasVertexColors(j))
			{
				hasVertexColors = true;
				maxColorChannels = j + 1;
			}
		if (hasVertexColors)
			if (std::find(arrVAU.begin(), arrVAU.end(), VAU_COLOR) == arrVAU.end())
				arrVAU.push_back(VAU_COLOR);

		if (scene->mMeshes[i]->HasTangentsAndBitangents())
			if (std::find(arrVAU.begin(), arrVAU.end(), VAU_TANGENT) == arrVAU.end())
			{
				arrVAU.push_back(VAU_TANGENT);
				arrVAU.push_back(VAU_BINORMAL);
			}
	}
	
	// Create the vertex format
	const unsigned int vfIdx = resMan->CreateVertexFormat((unsigned int)arrVAU.size() + maxUVChannels + maxColorChannels - 2);
	vfOut = resMan->GetVertexFormat(vfIdx);
	for (unsigned int i = 0, attribIdx = 0, offset = 0; i < arrVAU.size(); i++)
	{
		VertexAttributeType type = VAT_NONE;
		switch (arrVAU[i])
		{
		case VAU_POSITION:
		case VAU_NORMAL:
		case VAU_TANGENT:
		case VAU_BINORMAL:
			type = VAT_FLOAT3;
			break;
		case VAU_TEXCOORD:
			switch (maxUVComponents)
			{
			case 1:
				type = VAT_FLOAT1;
				break;
			case 2:
				type = VAT_FLOAT2;
				break;
			case 3:
				type = VAT_FLOAT3;
				break;
			default:
				assert(false);
			}
			break;
		case VAU_COLOR:
			type = VAT_UBYTE4;
			break;
		default:
			assert(false);
		}

		if (arrVAU[i] == VAU_TEXCOORD)
			for (unsigned int j = 0; j < maxUVChannels; j++)
			{
				vfOut->SetAttribute(attribIdx++, offset, arrVAU[i], type, j);
				offset += VertexFormat::GetAttributeTypeSize(type);
			}
		else
			if (arrVAU[i] == VAU_COLOR)
				for (unsigned int j = 0; j < maxColorChannels; j++)
				{
					vfOut->SetAttribute(attribIdx++, offset, arrVAU[i], type, j);
					offset += VertexFormat::GetAttributeTypeSize(type);
				}
			else
			{
				vfOut->SetAttribute(attribIdx++, offset, arrVAU[i], type, 0);
				offset += VertexFormat::GetAttributeTypeSize(type);
			}
	}
	vfOut->SetStride(vfOut->CalculateStride());
	vfOut->Update();

	const unsigned int ibIdx = resMan->CreateIndexBuffer(totalIndexCount, totalIndexCount > 65535 ? IBF_INDEX32 : IBF_INDEX16);
	ibOut = resMan->GetIndexBuffer(ibIdx);
	const unsigned int vbIdx = resMan->CreateVertexBuffer(vfOut, totalVertexCount, ibOut);
	vbOut = resMan->GetVertexBuffer(vbIdx);

	ibOut->Lock(BL_WRITE_ONLY);
	vbOut->Lock(BL_WRITE_ONLY);

	unsigned int iterIndices = 0, iterVertices = 0, indexOffset = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		// Populate IB
		for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
		{
			for (unsigned int k = 0; k < 3; k++)
			{
				assert(scene->mMeshes[i]->mFaces[j].mNumIndices == 3);
				ibOut->SetIndex(iterIndices++, scene->mMeshes[i]->mFaces[j].mIndices[k] + indexOffset);
			}
		}
		indexOffset += scene->mMeshes[i]->mNumVertices;

		// Populate VB
		for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{
			if (vbOut->HasPosition())
				vbOut->Position<Vec3f>(iterVertices) = Vec3f(
					scene->mMeshes[i]->mVertices[j].x,
					scene->mMeshes[i]->mVertices[j].y,
					scene->mMeshes[i]->mVertices[j].z);

			if (vbOut->HasNormal())
				vbOut->Normal<Vec3f>(iterVertices) = Vec3f(
					scene->mMeshes[i]->mNormals[j].x,
					scene->mMeshes[i]->mNormals[j].y,
					scene->mMeshes[i]->mNormals[j].z);

			if (vbOut->HasTangent())
				vbOut->Tangent<Vec3f>(iterVertices) = Vec3f(
					scene->mMeshes[i]->mTangents[j].x,
					scene->mMeshes[i]->mTangents[j].y,
					scene->mMeshes[i]->mTangents[j].z);

			if (vbOut->HasBinormal())
				vbOut->Binormal<Vec3f>(iterVertices) = Vec3f(
					scene->mMeshes[i]->mBitangents[j].x,
					scene->mMeshes[i]->mBitangents[j].y,
					scene->mMeshes[i]->mBitangents[j].z);

			for (unsigned int k = 0; k < AI_MAX_NUMBER_OF_TEXTURECOORDS; k++)
				if (vbOut->HasTexCoord(k) && scene->mMeshes[i]->HasTextureCoords(k))
					switch (scene->mMeshes[i]->mNumUVComponents[k])
					{
					case 1:
						vbOut->TexCoord<float>(iterVertices, k) = scene->mMeshes[i]->mTextureCoords[k][j].x;
						break;
					case 2:
						vbOut->TexCoord<Vec2f>(iterVertices, k) = Vec2f(
							scene->mMeshes[i]->mTextureCoords[k][j].x,
							scene->mMeshes[i]->mTextureCoords[k][j].y);
						break;
					case 3:
						vbOut->TexCoord<Vec3f>(iterVertices, k) = Vec3f(
							scene->mMeshes[i]->mTextureCoords[k][j].x,
							scene->mMeshes[i]->mTextureCoords[k][j].y,
							scene->mMeshes[i]->mTextureCoords[k][j].z);
						break;
					default:
						assert(false);
					}

			for (unsigned int k = 0; k < AI_MAX_NUMBER_OF_COLOR_SETS; k++)
				if (vbOut->HasColor(k) && scene->mMeshes[i]->HasVertexColors(k))
					vbOut->Color<DWORD>(iterVertices, k) =
						((((DWORD)(scene->mMeshes[i]->mColors[k][j].a * 255.f)) & 0xff) << 24) |
						((((DWORD)(scene->mMeshes[i]->mColors[k][j].r * 255.f)) & 0xff) << 16) |
						((((DWORD)(scene->mMeshes[i]->mColors[k][j].g * 255.f)) & 0xff) <<  8) |
						((((DWORD)(scene->mMeshes[i]->mColors[k][j].b * 255.f)) & 0xff));

			iterVertices++;
		}
	}
	assert(iterIndices == totalIndexCount && iterVertices == totalVertexCount);

	ibOut->Update();
	ibOut->Unlock();
	vbOut->Update();
	vbOut->Unlock();

	return vbIdx;
}