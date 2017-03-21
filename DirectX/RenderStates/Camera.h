#pragma once

class Camera
{
public:
	Camera(float nearPlane, float farPlane, float fov, float aspectRatio);
	Camera(const Camera& camera);
	~Camera();

	Camera& operator =(const Camera& camera);

	float	nearPlane() const;
	void	nearPlane(float f);

	float	farPlane() const;
	void	farPlane(float f);

	float	fov() const;
	void	fov(float f);

	float	aspectRatio() const;
	void	aspectRatio(float f);

	float	viewHeight() const;
	void	viewHeight(float height);

	float	viewWidth() const;

	bool	ortho() const;
	void	ortho(bool b);

	Vector3	nearPlanePoint(float xClip, float yClip) const;
	Vector3 farPlanePoint(float xClip, float yClip) const;

	bool    isZoomedView() const;

	static const float DEFAULT_NEAR_PLANE;
	static const float ZOOM_FOV_THRESHOLD;
private:
	float	m_nearPlane;
	float	m_farPlane;
	float	m_fov;
	float	m_aspectRatio;
	float	m_viewHeight;
	bool	m_ortho;
};

#include "Camera.ipp"

