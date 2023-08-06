#define WANDLENGTH 100

float wandMovement[WANDLENGTH][2];

float wandVerts[3 * WANDLENGTH*3];
std::vector<int> indexes011;

float mouseChangeX = 0.0f;
float mouseChangeY = 0.0f;
int mouseChangeNum = 0;

bool isWandUsed = false;

void initWand()
{
	float prevx = 0.0f;
	float prevy = 0.0f;
	for(int i = 0; i < WANDLENGTH; i++)
	{
		wandMovement[i][0] = rand01()*2.0f-1.0f;
		wandMovement[i][1] = rand01()*2.0f-1.0f;
		prevx += 0.01f;
		prevy += 0.01f;
	}
}

void showWand2(short keyE)
{
	if(keyE >= 0)
		return;
	
	indexes011.clear();

	for (int i = 0; i < WANDLENGTH; i++)
	{
		float s = (float)(WANDLENGTH-i)/(float)(WANDLENGTH);

		wandVerts[i*9] = wandMovement[i][0];
		wandVerts[i*9+1] = wandMovement[i][1];
		wandVerts[i*9+2] = -2.0f;
		wandVerts[i*9+3] = wandMovement[i][0]-(rand01()*2.0f-1.0f)*0.03f*s;
		wandVerts[i*9+4] = wandMovement[i][1]-(rand01()*2.0f-1.0f)*0.03f*s;
		wandVerts[i*9+5] = -2.0f;
		wandVerts[i*9+6] = wandMovement[i][0]+(rand01()*2.0f-1.0f)*0.03f*s;
		wandVerts[i*9+7] = wandMovement[i][1]+(rand01()*2.0f-1.0f)*0.03f*s;
		wandVerts[i*9+8] = -2.0f;

		indexes011.push_back(i*3);
		indexes011.push_back(i*3+1);
		indexes011.push_back((i+1)*3);
		indexes011.push_back(i*3+1);
		indexes011.push_back((i+1)*3+1);
		indexes011.push_back((i+1)*3);
		indexes011.push_back(i*3);
		indexes011.push_back(i*3+2);
		indexes011.push_back((i+1)*3);
		indexes011.push_back(i*3+2);
		indexes011.push_back((i+1)*3+2);
		indexes011.push_back((i+1)*3);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &wandVerts);
		glColor3f(1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, indexes011.size(), GL_UNSIGNED_INT, &indexes011[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void waveWand(float diffX, float diffY)
{
	float cX = wandMovement[0][0];
	float cY = wandMovement[0][1];

	wandMovement[0][0] += diffX * 0.5f;
	wandMovement[0][1] += diffY * 0.5f;
	wandMovement[0][0] *= 0.97f;
	wandMovement[0][1] *= 0.97f;

	if(wandMovement[0][0] > 0.5f)
		wandMovement[0][0] = 0.5f;
	if(wandMovement[0][0] < -0.5f)
		wandMovement[0][0] = -0.5f;

	if(wandMovement[0][1] > 0.5f)
		wandMovement[0][1] = 0.5f;
	if(wandMovement[0][1] < -0.5f)
		wandMovement[0][1] = -0.5f;

	for(int i = 1; i < WANDLENGTH; i++)
	{
		swapFloats(&wandMovement[i][0], &cX);
		swapFloats(&wandMovement[i][1], &cY);
	}
}