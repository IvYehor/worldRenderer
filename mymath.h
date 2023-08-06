#define PERLINSIZE 128
#define PI 3.14159265f

struct WorldPos
{
	float x, y, z;
	float xRot, zRot;
};

struct timeRet
{
	int hours, minutes;
};

float lerp(float a, float b, float t)
{
	return (b-a) * t + a;
}

float rand01()
{
	return (float)rand()/(float)RAND_MAX;
}

int randInt(int from, int to)
{
	return rand() % (to + 1 - from) + from;
}

void swapFloats(float *a, float *b)
{
	float c = *a;
	*a = *b;
	*b = c;
}

timeRet getTime(float angle)
{
	angle /= 2.0f*PI;
	int hours = (static_cast<int>(angle * 24.0f) + 6) % 24;
	int minutes = static_cast<int>(((angle * 24.0f) - round(angle * 24.0f)) * 60.0f);
	timeRet value = {hours, minutes};
	return value;
}