#pragma once
struct Setting{
	int windowWidth = 1024;
	int windowHeight = 1024;
	int renderWidth = 1024;
	int renderHeight = 1024;
	bool isCam = true;
	int camDeviceId = 0;
	int cameraWidth = 640;
	int cameraHeight = 480;
	string filepath;
	unsigned int subdivide = 1;
};