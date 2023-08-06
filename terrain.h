#include <GL/glut.h>

#define TERRAIN_SIZE 100
#define TERRAIN_XYAMP 10.0f
#define TERRAIN_ZAMP 100.0f
#define TERRAIN_THRESHOLD 0.8f

float color[TERRAIN_SIZE*TERRAIN_SIZE*3];

float heights[TERRAIN_SIZE * TERRAIN_SIZE];
float landType[TERRAIN_SIZE * TERRAIN_SIZE];

float noiseSeed01[TERRAIN_SIZE * TERRAIN_SIZE];
float noiseSeed02[TERRAIN_SIZE * TERRAIN_SIZE];

float maxHeight = 1.0f;
float maxHeightLandType = 1.0f;

float groundVerts[TERRAIN_SIZE*TERRAIN_SIZE*3];
std::vector<GLuint> indexes;
float triangleNormals[TERRAIN_SIZE*TERRAIN_SIZE*2*3];
float normals[TERRAIN_SIZE*TERRAIN_SIZE*3];

int numOfTriangles = 0;

int spawnX = 0;
int spawnY = 0;

bool isThereATriangle(int x, int y, bool rev)
{
	return true;
	if(!rev)
		return landType[x*TERRAIN_SIZE+y] > TERRAIN_THRESHOLD || landType[x*TERRAIN_SIZE+y + 1] > TERRAIN_THRESHOLD || landType[x*TERRAIN_SIZE+y + TERRAIN_SIZE] > TERRAIN_THRESHOLD;
	else
		return landType[x*TERRAIN_SIZE+y + 1] > TERRAIN_THRESHOLD || landType[x*TERRAIN_SIZE+y + TERRAIN_SIZE + 1] > TERRAIN_THRESHOLD || landType[x*TERRAIN_SIZE+y + TERRAIN_SIZE] > TERRAIN_THRESHOLD;
}

void generateTerrain(unsigned int seed)
{
	srand(seed);

	for(int i = 0; i < TERRAIN_SIZE*TERRAIN_SIZE; i++)
		noiseSeed01[i] = rand01();
	for(int i = 0; i < TERRAIN_SIZE*TERRAIN_SIZE; i++)
		noiseSeed02[i] = rand01();


	PerlinNoise2D(TERRAIN_SIZE, TERRAIN_SIZE, noiseSeed01, 6, 2.0f, heights, &maxHeight);
	PerlinNoise2D(TERRAIN_SIZE, TERRAIN_SIZE, noiseSeed02, 6, 2.0f, landType, &maxHeightLandType);

	for(int i = 0; i < TERRAIN_SIZE; i++)
		for(int j = 0; j < TERRAIN_SIZE; j++)
			heights[i * TERRAIN_SIZE + j] /= maxHeight;

	for(int i = 0; i < TERRAIN_SIZE; i++)
	{
		for(int j = 0; j < TERRAIN_SIZE; j++)
		{
			if(i == 0 || j == 0 || i == TERRAIN_SIZE-1 || j == TERRAIN_SIZE-1)
			{
				landType[i * TERRAIN_SIZE + j] = 1.0f;
				continue;
			}
			landType[i * TERRAIN_SIZE + j] /= maxHeightLandType;
		}
	}
	
	while(true)
	{
		spawnX = randInt(1, TERRAIN_SIZE-2);
		spawnY = randInt(1, TERRAIN_SIZE-2);

		if(landType[spawnX*TERRAIN_SIZE+spawnY] > TERRAIN_THRESHOLD)
			break;
	}

	spawnX *= TERRAIN_XYAMP;
	spawnY *= TERRAIN_XYAMP;

	for(int x = 0; x < TERRAIN_SIZE; x++)
	{
		for(int y = 0; y < TERRAIN_SIZE; y++)
		{
			color[(x*TERRAIN_SIZE+y)*3] = (landType[x * TERRAIN_SIZE + y] > TERRAIN_THRESHOLD) ? 0.0f : 1.0f;
			color[(x*TERRAIN_SIZE+y)*3 + 1] = 0.5f + rand01()/4.0f;
			color[(x*TERRAIN_SIZE+y)*3 + 2] = 0.0f;
		}
	}

	for(int x = 0; x < TERRAIN_SIZE; x++)
	{
		for(int y = 0; y < TERRAIN_SIZE; y++)
		{
			groundVerts[(x*TERRAIN_SIZE+y)*3] = x * TERRAIN_XYAMP;
			groundVerts[(x*TERRAIN_SIZE+y)*3 + 1] = y * TERRAIN_XYAMP;
			groundVerts[(x*TERRAIN_SIZE+y)*3 + 2] = heights[x * TERRAIN_SIZE + y] * TERRAIN_ZAMP;
			
			if(x != TERRAIN_SIZE-1 && y != TERRAIN_SIZE-1)
			{
				if(isThereATriangle(x, y, false))
				{
					indexes.push_back(x*TERRAIN_SIZE+y);
					indexes.push_back(x*TERRAIN_SIZE+y + 1);
					indexes.push_back(x*TERRAIN_SIZE+y + TERRAIN_SIZE);
				}

				if(isThereATriangle(x, y, true))
				{
					indexes.push_back(x*TERRAIN_SIZE+y + 1);
					indexes.push_back(x*TERRAIN_SIZE+y + TERRAIN_SIZE + 1);
					indexes.push_back(x*TERRAIN_SIZE+y + TERRAIN_SIZE);
				}
			}
		}
	}

	for(int x = 0; x < TERRAIN_SIZE-1; x++)
	{
		for(int y = 0; y < TERRAIN_SIZE-1; y++)
		{
			float Ax = groundVerts[(x*TERRAIN_SIZE+y + 1)*3] - groundVerts[(x*TERRAIN_SIZE+y)*3];
			float Ay = groundVerts[(x*TERRAIN_SIZE+y + 1)*3 + 1] - groundVerts[(x*TERRAIN_SIZE+y)*3 + 1];
			float Az = groundVerts[(x*TERRAIN_SIZE+y + 1)*3 + 2] - groundVerts[(x*TERRAIN_SIZE+y)*3 + 2];

			float Bx = groundVerts[(x*TERRAIN_SIZE+y + TERRAIN_SIZE)*3] - groundVerts[(x*TERRAIN_SIZE+y)*3];
			float By = groundVerts[(x*TERRAIN_SIZE+y + TERRAIN_SIZE)*3 + 1] - groundVerts[(x*TERRAIN_SIZE+y)*3 + 1];
			float Bz = groundVerts[(x*TERRAIN_SIZE+y + TERRAIN_SIZE)*3 + 2] - groundVerts[(x*TERRAIN_SIZE+y)*3 + 2];

			triangleNormals[(x*TERRAIN_SIZE + y)*6] = Ay * Bz - Az * By;
			triangleNormals[(x*TERRAIN_SIZE + y)*6 + 1] = Az * Bx - Ax * Bz;
			triangleNormals[(x*TERRAIN_SIZE + y)*6 + 2] = Ax * By - Ay * Bx;

			Ax = groundVerts[(x*TERRAIN_SIZE+y + TERRAIN_SIZE + 1)*3] - groundVerts[(x*TERRAIN_SIZE+y + 1)*3];
			Ay = groundVerts[(x*TERRAIN_SIZE+y + TERRAIN_SIZE + 1)*3 + 1] - groundVerts[(x*TERRAIN_SIZE+y + 1)*3 + 1];
			Az = groundVerts[(x*TERRAIN_SIZE+y + TERRAIN_SIZE + 1)*3 + 2] - groundVerts[(x*TERRAIN_SIZE+y + 1)*3 + 2];

			Bx = groundVerts[(x*TERRAIN_SIZE+y + TERRAIN_SIZE)*3] - groundVerts[(x*TERRAIN_SIZE+y + 1)*3];
			By = groundVerts[(x*TERRAIN_SIZE+y + TERRAIN_SIZE)*3 + 1] - groundVerts[(x*TERRAIN_SIZE+y + 1)*3 + 1];
			Bz = groundVerts[(x*TERRAIN_SIZE+y + TERRAIN_SIZE)*3 + 2] - groundVerts[(x*TERRAIN_SIZE+y + 1)*3 + 2];

			triangleNormals[(x*TERRAIN_SIZE + y)*6 + 3] = Ay * Bz - Az * By;
			triangleNormals[(x*TERRAIN_SIZE + y)*6 + 4] = Az * Bx - Ax * Bz;
			triangleNormals[(x*TERRAIN_SIZE + y)*6 + 5] = Ax * By - Ay * Bx;
		}
	}

	for(int x = 0; x < TERRAIN_SIZE; x++)
	{
		for(int y = 0; y < TERRAIN_SIZE; y++)
		{
			if(x == 0 || y == 0 || x == TERRAIN_SIZE-1 || y == TERRAIN_SIZE-1)
			{
				normals[(x*TERRAIN_SIZE+y) * 3] = 0.0f;
				normals[(x*TERRAIN_SIZE+y) * 3 + 1] = 0.0f;
				normals[(x*TERRAIN_SIZE+y) * 3 + 2] = 1.0f;
				continue;
			}
			normals[(x*TERRAIN_SIZE+y) * 3] = -(
				triangleNormals[(x*TERRAIN_SIZE+y)*6] +
				triangleNormals[(x*TERRAIN_SIZE+y - TERRAIN_SIZE)*6] +
				triangleNormals[(x*TERRAIN_SIZE+y - TERRAIN_SIZE)*6+3] +
				triangleNormals[(x*TERRAIN_SIZE+y - 1)*6] +
				triangleNormals[(x*TERRAIN_SIZE+y - 1)*6+3] +
				triangleNormals[(x*TERRAIN_SIZE+y - TERRAIN_SIZE - 1)*6+3]) / 6.0f;
			normals[(x*TERRAIN_SIZE+y) * 3 + 1] = -(
				triangleNormals[(x*TERRAIN_SIZE+y)*6+1] +
				triangleNormals[(x*TERRAIN_SIZE+y - TERRAIN_SIZE)*6+1] +
				triangleNormals[(x*TERRAIN_SIZE+y - TERRAIN_SIZE)*6+3+1] +
				triangleNormals[(x*TERRAIN_SIZE+y - 1)*6+1] +
				triangleNormals[(x*TERRAIN_SIZE+y - 1)*6+3+1] +
				triangleNormals[(x*TERRAIN_SIZE+y - TERRAIN_SIZE - 1)*6+3+1]) / 6.0f;
			normals[(x*TERRAIN_SIZE+y) * 3 + 2] = -(
				triangleNormals[(x*TERRAIN_SIZE+y)*6+2] +
				triangleNormals[(x*TERRAIN_SIZE+y - TERRAIN_SIZE)*6+2] +
				triangleNormals[(x*TERRAIN_SIZE+y - TERRAIN_SIZE)*6+3+2] +
				triangleNormals[(x*TERRAIN_SIZE+y - 1)*6+2] +
				triangleNormals[(x*TERRAIN_SIZE+y - 1)*6+3+2] +
				triangleNormals[(x*TERRAIN_SIZE+y - TERRAIN_SIZE - 1)*6+3+2]) / 6.0f;
		}
	}
}

void showGround()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &groundVerts);
		glColorPointer(3, GL_FLOAT, 0, &color);
		glNormalPointer(GL_FLOAT, 0, &normals);
		glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, &indexes[0]);
		//You can call glDrawElements multiple times it it doesn't halt

		//glDrawArrays(GL_TRIANGLES, 0, 5*5);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
}
