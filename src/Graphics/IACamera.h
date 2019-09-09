//# pragma once
#ifndef IACAMERA_H
#define IACAMERA_H

#include <glm/glm.hpp>

namespace IAC {

//#define M_PI 3.14156265
#define PI_OVER_TWO 1.5707963267948966192313216916397514420985

// Camera struct, used to store interactive camera data, copied to the GPU and used by OpenCL for each frame
struct Camera {
	glm::vec3 position;		// 16 bytes
	glm::vec3 view;			// 16
	glm::vec3 up;			// 16
	glm::vec2 resolution;	// 8
	glm::vec2 fov;			// 8
	float apertureRadius;	// 4
	float focalDistance;	// 4
	//float dummy1;		// 4
	//float dummy2;		// 4
};

// class for interactive camera object, updated on the CPU for each frame and copied into Camera struct
class InteractiveCamera
{
private:

	glm::vec3 centerPosition;
	glm::vec3 viewDirection;
	float yaw;
	float pitch;
	float radius;
	float apertureRadius;
	float focalDistance;

	void fixYaw();
	void fixPitch();
	void fixRadius();
	void fixApertureRadius();
	void fixFocalDistance();

public:
	InteractiveCamera();
	virtual ~InteractiveCamera();
	void changeYaw(float m);
	void changePitch(float m);
	void changeRadius(float m);
	void changeAltitude(float m);
	void changeFocalDistance(float m);
	void strafe(float m);
	void goForward(float m);
	void rotateRight(float m);
	void changeApertureDiameter(float m);
	void setResolution(float x, float y);
	void setFOVX(float fovx);

	void buildRenderCamera(Camera* renderCamera);

	glm::vec2 resolution;
	glm::vec2 fov;
};
};  // namespace
#endif
