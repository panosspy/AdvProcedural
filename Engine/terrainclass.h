////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

/////////////
// GLOBALS //
/////////////
//const int TEXTURE_REPEAT = 32;
const int TEXTURE_REPEAT = 8;


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>
#include <time.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	    D3DXVECTOR3 normal;
	};

	struct HeightMapType 
	{ 
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType 
	{ 
		float x, y, z;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, char*);
	bool InitializeTerrain(ID3D11Device*, int terrainWidth, int terrainHeight, WCHAR*, WCHAR*, WCHAR*);


	void Shutdown();
	void Render(ID3D11DeviceContext*);
	bool GenerateHeightMap(ID3D11Device* device, bool keydown);
	int  GetIndexCount();
	bool midpoint(int, int, int, int);
	bool Smooth();
	bool Average(int, int);
	bool InBounds(int, int);

	ID3D11ShaderResourceView* GetGrassTexture();
	ID3D11ShaderResourceView* GetSlopeTexture();
	ID3D11ShaderResourceView* GetRockTexture();

	int GetVertexCount();
	void CopyVertexArray(void*);

private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ShutdownHeightMap();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	void CalculateTextureCoordinates();

	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	void ReleaseTextures();
	
private:
	bool m_terrainGeneratedToggle;
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;

	TextureClass *m_GrassTexture, *m_SlopeTexture, *m_RockTexture;

	VertexType* m_vertices;
};

#endif