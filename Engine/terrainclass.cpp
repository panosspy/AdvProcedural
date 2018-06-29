////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "terrainclass.h"
#include <cmath>

TerrainClass::TerrainClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_heightMap = 0;
	m_terrainGeneratedToggle = false;

	m_GrassTexture = 0;
	m_SlopeTexture = 0;
	m_RockTexture = 0;

	m_vertices = 0;
}


TerrainClass::TerrainClass(const TerrainClass& other)
{
}


TerrainClass::~TerrainClass()
{
}

bool TerrainClass::InitializeTerrain(ID3D11Device* device, int terrainWidth, int terrainHeight, WCHAR* grassTextureFilename, WCHAR* slopeTextureFilename,
	WCHAR* rockTextureFilename)
{
	int index;
	float height = 0.0;
	bool result;

	// Save the dimensions of the terrain.
	m_terrainWidth = terrainWidth;
	m_terrainHeight = terrainHeight;

	// Create the structure to hold the terrain data.
	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if (!m_heightMap)
	{
		return false;
	}

	// Initialise the data in the height map (flat).
	for (int j = 0; j<m_terrainHeight; j++)
	{
		for (int i = 0; i<m_terrainWidth; i++)
		{
			index = (m_terrainHeight * j) + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = (float)height;
			m_heightMap[index].z = (float)j;

		}
	}
	//buulding the maze
	//starting with the outer wall
	int j = 0, i = 0;
	do {
		index = (m_terrainHeight * j) + i;
		m_heightMap[index].y = 10.0;
		j++;
	} while (j<m_terrainHeight);

	i = m_terrainWidth - 3;
	j = 0;
	do {
		index = (m_terrainHeight * j) + i;
		m_heightMap[index].y = 10.0;
		j++;
	} while (j<m_terrainHeight);
	j = 0;
	i = 0;
	do {
		index = (m_terrainHeight * j) + i;
		m_heightMap[index].y = 10.0;
		i++;
	} while (i<m_terrainWidth);
	j = m_terrainHeight - 2;
	i = 0;
	do {
		index = (m_terrainHeight * j) + i;
		m_heightMap[index].y = 10.0;
		i++;
	} while (i<m_terrainWidth);
	//building the inside
	//1 column
	i = 0;
	j = 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (i>192 && i<224) {
			m_heightMap[index].y = 10.0;
		}
		i++;
	} while (i<m_terrainWidth);
	//2 column
	i = 0;
	j += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (i>160 && i<192) {
			m_heightMap[index].y = 10.0;
		}

		i++;
	} while (i<m_terrainWidth);
	//3 column
	i = 0;
	j += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (i>32 && i<64) {
			m_heightMap[index].y = 10.0;
		}
		if (i>96 && i<128) {
			m_heightMap[index].y = 10.0;
		}
		i++;
	} while (i<m_terrainWidth);
	//4 column
	i = 0;
	j += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (i>32 && i<160) {
			m_heightMap[index].y = 10.0;
		}
		i++;
	} while (i<m_terrainWidth);
	//5 column
	i = 0;
	j += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (i>0 && i<32) {
			m_heightMap[index].y = 10.0;
		}
		if (i>64 && i<160) {
			m_heightMap[index].y = 10.0;
		}
		if (i>224 && i<256) {
			m_heightMap[index].y = 10.0;
		}
		i++;
	} while (i<m_terrainWidth);
	//6 column
	i = 0;
	j += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (i>32 && i<64) {
			m_heightMap[index].y = 10.0;
		}
		i++;
	} while (i<m_terrainWidth);
	//7 column
	i = 0;
	j += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (i>32 && i<96) {
			m_heightMap[index].y = 10.0;
		}
		if (i>160 && i<192) {
			m_heightMap[index].y = 10.0;
		}
		i++;
	} while (i<m_terrainWidth);
	//1 raw
	j = 0;
	i = 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (j>0 && j<96) {
			m_heightMap[index].y = 10.0;
		}
		if (j>128 && j<160) {
			m_heightMap[index].y = 10.0;
		}
		if (j>192 && j<224) {
			m_heightMap[index].y = 10.0;
		}
		j++;
	} while (j<m_terrainHeight);
	//2 raw
	j = 0;
	i += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (j>160 && j<192) {
			m_heightMap[index].y = 10.0;
		}
		j++;
	} while (j<m_terrainHeight);
	//3 raw
	j = 0;
	i += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (j>192 && j<224) {
			m_heightMap[index].y = 10.0;
		}
		j++;
	} while (j<m_terrainHeight);
	//4 raw
	j = 0;
	i += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (j>0 && j<96) {
			m_heightMap[index].y = 10.0;
		}
		if (j>192 && j<256) {
			m_heightMap[index].y = 10.0;
		}
		j++;
	} while (j<m_terrainHeight);
	//5 raw
	j = 0;
	i += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (j>32 && j<160) {
			m_heightMap[index].y = 10.0;
		}
		if (j>192 && j<224) {
			m_heightMap[index].y = 10.0;
		}
		j++;
	} while (j<m_terrainHeight);
	//6 raw
	j = 0;
	i += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (j>64 && j<224) {
			m_heightMap[index].y = 10.0;
		}
		j++;
	} while (j<m_terrainHeight);
	//7 raw
	j = 0;
	i += 32;
	do {
		index = (m_terrainHeight * j) + i;
		if (j>32 && j<128) {
			m_heightMap[index].y = 10.0;
		}
		if (j>160 && j<256) {
			m_heightMap[index].y = 10.0;
		}
		j++;
	} while (j<m_terrainHeight);


	//even though we are generating a flat terrain, we still need to normalise it. 
	// Calculate the normals for the terrain data.
	result = CalculateNormals();
	if (!result)
	{
		return false;
	}

	// Calculate the texture coordinates.
	CalculateTextureCoordinates();

	// Load the textures.
	result = LoadTextures(device, grassTextureFilename, slopeTextureFilename, rockTextureFilename);
	if (!result) 
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}
bool TerrainClass::Initialize(ID3D11Device* device, char* heightMapFilename)
{
	bool result;


	// Load in the height map for the terrain.
	result = LoadHeightMap(heightMapFilename);
	if (!result)
	{
		return false;
	}

	// Normalize the height of the height map.
	NormalizeHeightMap();

	// Calculate the normals for the terrain data.
	result = CalculateNormals();
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}


void TerrainClass::Shutdown()
{
	// Release the textures.
	ReleaseTextures();

	// Release the vertex and index buffer.
	ShutdownBuffers();

	// Release the height map data.
	ShutdownHeightMap();

	return;
}


void TerrainClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int TerrainClass::GetIndexCount()
{
	return m_indexCount;
}

bool TerrainClass::GenerateHeightMap(ID3D11Device* device, bool keydown)
{
	bool result;
	int index = 0;
	srand(time(0));
 
	if (keydown && (!m_terrainGeneratedToggle))
	{
		// Initialise the data in the height map to zero
		for (int j = 0; j < m_terrainHeight; j++)
		{
			for (int i = 0; i < m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;

				m_heightMap[index].x = (float)i;
				m_heightMap[index].y = 0.0;
				m_heightMap[index].z = (float)j;

			}
		}


		//start by giving random values to the edges of the terrain
		for (int j = 0; j < m_terrainHeight; j++)
		{
			for (int i = 0; i < m_terrainWidth; i++)
			{
				index = (m_terrainHeight * j) + i;

				if ((j == 0 && i == 0) || (j == 1 && i == m_terrainWidth - 1) || (j == m_terrainHeight - 1 && i == 0) || (j == m_terrainHeight - 1 && i == m_terrainWidth - 1)) {
					m_heightMap[index].x = (float)i;
					m_heightMap[index].y = -1 + 2 * ((float)rand() / (float)RAND_MAX);//i wanted values between -1 to 1
					m_heightMap[index].z = (float)j;
				}
			}
		}

		//some values i am going to use to iterate the midpoint displacement algorithm
		int a = m_terrainHeight - 1;
		int b = m_terrainWidth - 1;
		int startb, starta, movea, moveb;

		//the iteration. working it row by row. 9 iterations are the maximum for the specific size of our terrain i left it at 8 because it had a better visual result and not big difference of heights
		for (int k = 0; k < 8; k++) {
			startb = 0;
			movea = a;
			moveb = b;
			do {
				starta = 0;
				do {
					//calling each time the function for midpoint displacement
					midpoint(movea, moveb, starta, startb);
					starta = starta + a;
					movea = movea + a;
				} while (starta < m_terrainHeight - 1);
				startb = startb + b;
				moveb = moveb + b;
				movea = a;
			} while (startb < m_terrainWidth - 1);
			a = a / 2;
			b = b / 2;
		}

		//run smooth function 4 times for better resutls
		for (int k = 0; k < 5; k++) {
			Smooth();
		}

		//calculate normals
		result = CalculateNormals();
		if (!result)
		{
			return false;
		}

		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		result = InitializeBuffers(device);
		if (!result)
		{
			return false;
		}

		m_terrainGeneratedToggle = true;

	}
	else
	{
		m_terrainGeneratedToggle = false;
	}
	return true;
}


bool TerrainClass::midpoint(int a, int b, int starta, int startb) {
	int index;
 


	//midpoints
	int topleft, topright, botleft, botright;

	int top, left, right, bottom;

	//finding the outer positions to use them later
	top = (m_terrainHeight * (((a-starta)/2)+starta) + startb);
	left = (m_terrainHeight * starta) + (((b-startb) / 2) + startb);
	right = (m_terrainHeight * a) + (((b - startb) / 2) + startb);
	bottom = (m_terrainHeight * (((a - starta) / 2) + starta) + b);

	topleft = (m_terrainHeight * starta) + startb;
	topright = (m_terrainHeight * a) + startb;
	botleft = (m_terrainHeight * starta) + b;
	botright = (m_terrainHeight * a ) + b;

	for (int j = starta; j<a+1; j++)
	{
		for (int i = startb; i<b+1; i++)
		{
			index = (m_terrainHeight * j) + i;

			if (j == starta && i == (((b - startb) / 2) + startb)) {
				m_heightMap[index].x = (float)i;
				m_heightMap[index].y = (float)((m_heightMap[topleft].y + m_heightMap[botleft].y) / 2) + (-1 + 2 * ((float)rand() / (float)RAND_MAX));
				m_heightMap[index].z = (float)j;
			}
			else if (j == (((a - starta) / 2) + starta) && i == startb) {
				m_heightMap[index].x = (float)i;
				m_heightMap[index].y = (float)((m_heightMap[topleft].y + m_heightMap[topright].y) / 2) + (-1 + 2 * ((float)rand() / (float)RAND_MAX));
				m_heightMap[index].z = (float)j;
			}
			else if (j == a && i == (((b - startb) / 2) + startb)) {
				m_heightMap[index].x = (float)i;
				m_heightMap[index].y = (float)((m_heightMap[topright].y + m_heightMap[botright].y) / 2) + (-1 + 2 * ((float)rand() / (float)RAND_MAX));
				m_heightMap[index].z = (float)j;
			}
			else if (j == (((a - starta) / 2) + starta) && i == b) {
				m_heightMap[index].x = (float)i;
				m_heightMap[index].y = (float)((m_heightMap[botleft].y + m_heightMap[botright].y) / 2) + (-1 + 2 * ((float)rand() / (float)RAND_MAX));
				m_heightMap[index].z = (float)j;
			}
		}
	}

	//giving the center point a value
  	for (int j = starta; j<a+1; j++)
	{
		for (int i = startb; i<b+1; i++)
		{
			index = (m_terrainHeight * j) + i;

			if (j == (((a-starta)/2)+starta) && i == (((b - startb) / 2) + startb)) {
				m_heightMap[index].x = (float)i;
				m_heightMap[index].y = (float)(m_heightMap[top].y + m_heightMap[left].y + m_heightMap[right].y + m_heightMap[bottom].y / 4) + (-1 + 2 * ((float)rand() / (float)RAND_MAX));
				m_heightMap[index].z = (float)j;
			}
		}
	}

	return true;
}

//starting the smooth function
bool TerrainClass::Smooth() {
	for (int i = 0; i < m_terrainHeight; i++) {
		for (int j = 0; j < m_terrainWidth; j++) {
			(Average(i, j));
		}
	}
	return true;
}

// checking not being outside of bounds
bool TerrainClass::InBounds(int row, int col) {
	return row >= 0 && row < (m_terrainHeight - 1) && col >= 0 && col < (m_terrainWidth - 1);
}

//taking the average
bool TerrainClass::Average(int row, int col) {
	int avg = 0.0f;
	int num = 0.0f;
	for (int m = row - 1; m <= row + 1; m++) {
		for (int n = col - 1; n <= col + 1; n++) {
			if (!InBounds(m, n)) continue;

			avg += m_heightMap[m * m_terrainHeight + n].y;
			num++;
		}
	}
	m_heightMap[row * m_terrainHeight + col].y = avg / num;
	return true;
}

ID3D11ShaderResourceView* TerrainClass::GetGrassTexture()
{
	return m_GrassTexture->GetTexture();
}

ID3D11ShaderResourceView* TerrainClass::GetSlopeTexture()
{
	return m_SlopeTexture->GetTexture();
}

ID3D11ShaderResourceView* TerrainClass::GetRockTexture()
{
	return m_RockTexture->GetTexture();
}

bool TerrainClass::LoadHeightMap(char* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;


	// Open the height map file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_terrainWidth * m_terrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Create the structure to hold the height map data.
	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if (!m_heightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (m_terrainHeight * j) + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = (float)height;
			m_heightMap[index].z = (float)j;

			k += 3;
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}


void TerrainClass::NormalizeHeightMap()
{
	int i, j;


	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			m_heightMap[(m_terrainHeight * j) + i].y /= 15.0f;
		}
	}

	return;
}


bool TerrainClass::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new VectorType[(m_terrainHeight - 1) * (m_terrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j<(m_terrainHeight - 1); j++)
	{
		for (i = 0; i<(m_terrainWidth - 1); i++)
		{
			index1 = (j * m_terrainHeight) + i;
			index2 = (j * m_terrainHeight) + (i + 1);
			index3 = ((j + 1) * m_terrainHeight) + i;

			// Get three vertices from the face.
			vertex1[0] = m_heightMap[index1].x;
			vertex1[1] = m_heightMap[index1].y;
			vertex1[2] = m_heightMap[index1].z;

			vertex2[0] = m_heightMap[index2].x;
			vertex2[1] = m_heightMap[index2].y;
			vertex2[2] = m_heightMap[index2].z;

			vertex3[0] = m_heightMap[index3].x;
			vertex3[1] = m_heightMap[index3].y;
			vertex3[2] = m_heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_terrainHeight - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (m_terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (m_terrainWidth - 1)) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * m_terrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_heightMap[index].nx = (sum[0] / length);
			m_heightMap[index].ny = (sum[1] / length);
			m_heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = 0;

	return true;
}


void TerrainClass::ShutdownHeightMap()
{
	if (m_heightMap)
	{
		delete[] m_heightMap;
		m_heightMap = 0;
	}

	return;
}

void TerrainClass::CalculateTextureCoordinates()
{
	int incrementCount, i, j, tuCount, tvCount;
	float incrementValue, tuCoordinate, tvCoordinate;


	// Calculate how much to increment the texture coordinates by.
	incrementValue = (float)TEXTURE_REPEAT / (float)m_terrainWidth;

	// Calculate how many times to repeat the texture.
	incrementCount = m_terrainWidth / TEXTURE_REPEAT;

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0.0f;
	tvCoordinate = 1.0f;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			// Store the texture coordinate in the height map.
			m_heightMap[(m_terrainHeight * j) + i].tu = tuCoordinate;
			m_heightMap[(m_terrainHeight * j) + i].tv = tvCoordinate;

			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tuCoordinate += incrementValue;
			tuCount++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if (tuCount == incrementCount)
			{
				tuCoordinate = 0.0f;
				tuCount = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate -= incrementValue;
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if (tvCount == incrementCount)
		{
			tvCoordinate = 1.0f;
			tvCount = 0;
		}
	}

	return;
}


bool TerrainClass::LoadTextures(ID3D11Device* device, WCHAR* grassTextureFilename, WCHAR* slopeTextureFilename, WCHAR* rockTextureFilename)
{
	bool result;


	// Create the grass texture object.
	m_GrassTexture = new TextureClass;
	if (!m_GrassTexture)
	{
		return false;
	}

	// Initialize the grass texture object.
	result = m_GrassTexture->Initialize(device, grassTextureFilename);
	if (!result)
	{
		return false;
	}

	// Create the slope texture object.
	m_SlopeTexture = new TextureClass;
	if (!m_SlopeTexture)
	{
		return false;
	}

	// Initialize the slope texture object.
	result = m_SlopeTexture->Initialize(device, slopeTextureFilename);
	if (!result)
	{
		return false;
	}

	// Create the rock texture object.
	m_RockTexture = new TextureClass;
	if (!m_RockTexture)
	{
		return false;
	}

	// Initialize the rock texture object.
	result = m_RockTexture->Initialize(device, rockTextureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}


void TerrainClass::ReleaseTextures()
{
	// Release the texture objects.
	if (m_GrassTexture)
	{
		m_GrassTexture->Shutdown();
		delete m_GrassTexture;
		m_GrassTexture = 0;
	}

	if (m_SlopeTexture)
	{
		m_SlopeTexture->Shutdown();
		delete m_SlopeTexture;
		m_SlopeTexture = 0;
	}

	if (m_RockTexture)
	{
		m_RockTexture->Shutdown();
		delete m_RockTexture;
		m_RockTexture = 0;
	}

	return;
}

bool TerrainClass::InitializeBuffers(ID3D11Device* device)
{
	//VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;
	float tu, tv;

	// Calculate the number of vertices in the terrain mesh.
	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	m_vertices = new VertexType[m_vertexCount];
	if (!m_vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize the index to the vertex buffer.
	index = 0;

	// Load the vertex and index array with the terrain data.
	for (j = 0; j<(m_terrainHeight - 1); j++)
	{
		for (i = 0; i<(m_terrainWidth - 1); i++)
		{
			index1 = (m_terrainHeight * j) + i;          // Bottom left.
			index2 = (m_terrainHeight * j) + (i + 1);      // Bottom right.
			index3 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
			index4 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.

			// Upper left.
			tv = m_heightMap[index3].tv;

			// Modify the texture coordinates to cover the top edge.
			if (tv == 1.0f) { tv = 0.0f; }


			m_vertices[index].position = D3DXVECTOR3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
			m_vertices[index].texture = D3DXVECTOR2(m_heightMap[index3].tu, tv);
			m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
			indices[index] = index;
			index++;

			// Upper right.
			tu = m_heightMap[index4].tu;
			tv = m_heightMap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			m_vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			m_vertices[index].texture = D3DXVECTOR2(tu, tv);
			m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			indices[index] = index;
			index++;

			// Bottom left.
			m_vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			m_vertices[index].texture = D3DXVECTOR2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			indices[index] = index;
			index++;

			// Bottom left.
			m_vertices[index].position = D3DXVECTOR3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
			m_vertices[index].texture = D3DXVECTOR2(m_heightMap[index1].tu, m_heightMap[index1].tv);
			m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
			indices[index] = index;
			index++;

			// Upper right.
			tu = m_heightMap[index4].tu;
			tv = m_heightMap[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if (tu == 0.0f) { tu = 1.0f; }
			if (tv == 1.0f) { tv = 0.0f; }

			m_vertices[index].position = D3DXVECTOR3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
			m_vertices[index].texture = D3DXVECTOR2(tu, tv);
			m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
			indices[index] = index;
			index++;

			// Bottom right.
			tu = m_heightMap[index2].tu;

			// Modify the texture coordinates to cover the right edge.
			if (tu == 0.0f) { tu = 1.0f; }

			m_vertices[index].position = D3DXVECTOR3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
			m_vertices[index].texture = D3DXVECTOR2(tu, m_heightMap[index2].tv);
			m_vertices[index].normal = D3DXVECTOR3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
			indices[index] = index;
			index++;
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	/*delete[] vertices;
	vertices = 0;*/

	delete[] indices;
	indices = 0;

	return true;
}


void TerrainClass::ShutdownBuffers()
{
	// Release the vertex array.
	if (m_vertices)
	{
		delete[] m_vertices;
		m_vertices = 0;
	}

	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void TerrainClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

int TerrainClass::GetVertexCount()
{
	return m_vertexCount;
}

void TerrainClass::CopyVertexArray(void* vertexList)
{
	memcpy(vertexList, m_vertices, sizeof(VertexType) * m_vertexCount);
	return;
}