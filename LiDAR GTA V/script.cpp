#include "script.h"
#include "keyboard.h"

#include <string>
#include <ctime>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>

#pragma warning(disable : 4244 4305) // double <-> float conversions

std::string statusText;
DWORD statusTextDrawTicksMax;
bool statusTextGxtEntry;

void update_status_text()
{
	if (GetTickCount() < statusTextDrawTicksMax)
	{
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
		if (statusTextGxtEntry)
		{
			UI::_SET_TEXT_ENTRY((char *)statusText.c_str());
		} else
		{
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((char *)statusText.c_str());
		}
		UI::_DRAW_TEXT(0.5, 0.5);
	}
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false)
{
	statusText = str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}

struct ray {
	int rayResult;
	bool hit;
	Vector3 endCoord;
	Vector3 surfaceNormal;
	int hitEntityHandle;
	std::string entityTypeText;
};

ray raycast(Vector3 source, Vector3 direction, float maxDistance, int intersectFlags) {
	ray result;
	float targetX = source.x + (direction.x * maxDistance);
	float targetY = source.y + (direction.y * maxDistance);
	float targetZ = source.z + (direction.z * maxDistance);
	int rayHandle = WORLDPROBE::_CAST_RAY_POINT_TO_POINT(source.x, source.y, source.z, targetX, targetY, targetZ, intersectFlags, 0, 7);
	int hit = 0;
	int hitEntityHandle = -1;
	Vector3 endCoord;
	endCoord.x = 0;
	endCoord.y = 0;
	endCoord.z = 0;
	Vector3 surfaceNormal;
	surfaceNormal.x = 0;
	surfaceNormal.y = 0;
	surfaceNormal.z = 0;
	int rayResult = WORLDPROBE::_GET_RAYCAST_RESULT(rayHandle, &hit, &endCoord, &surfaceNormal, &hitEntityHandle);
	result.rayResult = rayResult;
	result.hit = hit;
	result.endCoord = endCoord;
	result.surfaceNormal = surfaceNormal;
	result.hitEntityHandle = hitEntityHandle;
	std::string entityTypeText = "Unknown";
	if (ENTITY::DOES_ENTITY_EXIST(hitEntityHandle)) {
		int entityType = ENTITY::GET_ENTITY_TYPE(hitEntityHandle);
		if (entityType == 1) {
			entityTypeText = "GTA.Ped";
		}
		else if (entityType == 2) {
			entityTypeText = "GTA.Vehicle";
		}
		else if (entityType == 3) {
			entityTypeText = "GTA.Prop";
		}
	}
	result.entityTypeText = entityTypeText;
	return result;
}

ray angleOffsetRaycast(double angleOffsetX, double angleOffsetZ,int range){
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	double rotX = (rot.x + angleOffsetX) * (M_PI / 180.0);
	double rotZ = (rot.z + angleOffsetZ) * (M_PI / 180.0);
	double absX = abs(cos(rotX));
	Vector3 direction;
	direction.x = sin(rotZ) * absX * -1;
	direction.y = cos(rotZ) * absX;
	direction.z = sin(rotX);
	ray result = raycast(CAM::GET_GAMEPLAY_CAM_COORD(), direction, range, -1);
	return result;
}

void lidar(double horiFovMin, double horiFovMax, double vertFovMin, double vertFovMax, double horiStep, double vertStep,int range)
{
	double vertexCount = (horiFovMax - horiFovMin) * (1 / horiStep) * (vertFovMax - vertFovMin) * (1 / vertStep);
	std::ofstream fileOutput;
	fileOutput.open("scripts/dataset/LiDAR_CPP.ply");
	fileOutput << "ply\nformat ascii 1.0\nelement vertex " + std::to_string((int)vertexCount) + "\nproperty float x\nproperty float y\nproperty float z\nproperty uchar red\nproperty uchar green\nproperty uchar blue\nend_header\n";
	for (double z = horiFovMin; z < horiFovMax; z += horiStep)
	{
		std::string vertexData = "";
		for (double x = vertFovMin; x < vertFovMax; x += vertStep)
		{
			std::string entityName3 = "None";
			int entityHash = 0;
			unsigned char r = 0; unsigned char g = 0; unsigned char b = 0;
			ray result = angleOffsetRaycast(x, z, range);
			if (result.hit)
			{
				r = 255; g = 255; b = 255;
			}
			if (result.hitEntityHandle != -1)
			{
				entityName3 = result.entityTypeText;
				if (entityName3 == "GTA.Vehicle")
				{
					r = 255; g = 0; b = 0;
				}
				else if (entityName3 == "GTA.Ped")
				{
					r = 0; g = 255; b = 0;
				}
				else if (entityName3 == "GTA.Prop")
				{
					r = 0; g = 0; b = 255;
				}
			}
			vertexData += std::to_string(result.endCoord.x) + " " + std::to_string(result.endCoord.y) + " " + std::to_string(result.endCoord.z) + " " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + "\n";
		}
		fileOutput << vertexData;
	}
	fileOutput.close();
	set_status_text("LiDAR Point Cloud written to file.");
}

void ScriptMain()
{
	srand(GetTickCount());
	while (true)
	{
		if (IsKeyJustUp(VK_F4))
		{
			GAMEPLAY::SET_GAME_PAUSED(true);
			TIME::PAUSE_CLOCK(true);
			lidar(0, 360, -10, 30,0.25,0.25,75);
			GAMEPLAY::SET_GAME_PAUSED(false);
			TIME::PAUSE_CLOCK(false);
		}
		else if (IsKeyJustUp(VK_F5)) {
			GAMEPLAY::SET_GAME_PAUSED(true);
			TIME::PAUSE_CLOCK(true);
			ray result = angleOffsetRaycast(0, 0, 75);
			GAMEPLAY::SET_GAME_PAUSED(false);
			TIME::PAUSE_CLOCK(false);
			set_status_text(result.entityTypeText+" x: "+std::to_string(result.endCoord.x) + " y: " + std::to_string(result.endCoord.y) + " z: " + std::to_string(result.endCoord.z)+" hit: "+std::to_string(result.hit),1000);
		}
		update_status_text();
		WAIT(0);
	}
}