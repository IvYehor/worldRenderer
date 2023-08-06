#define PLAYERSPEED 0.3f
#define PLAYERSPEED2 0.6f
#define LERPCOEFFICIENT 0.08f
#define PLAYERHEIGHT 150.0f
#define SENSITIVITY 0.1f
#define WANDSENSITIVITY 0.0001f
#define MAXROTVELOCITY 0.5f

WorldPos camPos = {1.0f, 1.0f, 1.0f, 0.0f, 0.0f};

float rotVelX = 0;
float rotVelY = 0;

void MoveCamera(bool keyW, bool keyA, bool keyS, bool keyD, bool keyShift)
{
	float speed = keyShift ? PLAYERSPEED2 : PLAYERSPEED;
	if (keyW)
	{
		camPos.x -= cos(camPos.zRot * PI / 180.0f - PI/2.0f) * speed;
		camPos.y -= sin(camPos.zRot * PI / 180.0f - PI/2.0f) * speed;
	}
	else if (keyS)
	{
		camPos.x += cos(camPos.zRot * PI / 180.0f - PI/2.0f) * speed;
		camPos.y += sin(camPos.zRot * PI / 180.0f - PI/2.0f) * speed;
	}

	if (keyA)
	{
		camPos.x += sin(camPos.zRot * PI / 180.0f - PI/2.0f) * speed;
		camPos.y -= cos(camPos.zRot * PI / 180.0f - PI/2.0f) * speed;
	}
	else if (keyD)
	{
		camPos.x -= sin(camPos.zRot * PI / 180.0f - PI/2.0f) * speed;
		camPos.y += cos(camPos.zRot * PI / 180.0f - PI/2.0f) * speed;
	}

	if(camPos.x < 1.0f)
		camPos.x = 1.0f;
	else if(camPos.x > TERRAIN_SIZE * TERRAIN_XYAMP - 1.0f)
		camPos.x = TERRAIN_SIZE * TERRAIN_XYAMP - 1.0f;
	
	if(camPos.y < 1.0f)
		camPos.y = 1.0f;
	else if(camPos.y > TERRAIN_SIZE * TERRAIN_XYAMP - 1.0f)
		camPos.y = TERRAIN_SIZE * TERRAIN_XYAMP - 1.0f;

	int playerXInt = static_cast<int>(camPos.x / (float)TERRAIN_XYAMP);
	int playerYInt = static_cast<int>(camPos.y / (float)TERRAIN_XYAMP);

	camPos.z = lerp(camPos.z, heights[playerXInt * TERRAIN_SIZE + playerYInt] * TERRAIN_ZAMP + PLAYERHEIGHT, LERPCOEFFICIENT);
}

void RotateCamera(float diffX, float diffY)
{
	rotVelX = 0;
	rotVelY = 0;

	camPos.xRot -= diffY * SENSITIVITY;
	camPos.zRot -= diffX * SENSITIVITY;

	/*if(camPos.xRot < 0.0f)
		camPos.xRot = 0.0f;
	if(camPos.xRot > 180.0f)
		camPos.xRot = 180.0f;*/
}
