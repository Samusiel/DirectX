#include "stdafx.h"
#include "Camera.h"

/**
*	Finds the point on the near plane in camera space,
*	given x and y in clip space.
*/
Vector3 Camera::nearPlanePoint(float xClip, float yClip) const
{
	const float yLength = m_nearPlane * tanf(m_fov * 0.5f);
	const float xLength = yLength * m_aspectRatio;

	return Vector3(xLength * xClip, yLength * yClip, m_nearPlane);
}

Vector3 Camera::farPlanePoint(float xClip, float yClip) const
{
	const float yLength = m_farPlane * tanf(m_fov * 0.5f);
	const float xLength = yLength * m_aspectRatio;

	return Vector3(xLength * xClip, yLength * yClip, m_farPlane);
}

bool Camera::isZoomedView() const
{
	return fov() <= ZOOM_FOV_THRESHOLD;
}

const float Camera::DEFAULT_NEAR_PLANE = 0.25f;
const float Camera::ZOOM_FOV_THRESHOLD = 0.7f; // binoculars enabled on 120 degree fov