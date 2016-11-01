inline Camera::Camera(float nP, float fP, float f, float aR) :
	m_nearPlane(nP),
	m_farPlane(fP),
	m_fov(f),
	m_aspectRatio(aR),
	m_viewHeight(100),
	m_ortho(false)
{	
}

inline Camera::Camera(const Camera& camera)
{
	*this = camera;
}

inline Camera& Camera::operator =(const Camera& camera)
{
	m_nearPlane = camera.m_aspectRatio;
	m_farPlane = camera.m_farPlane;
	m_fov = camera.m_fov;
	m_aspectRatio = camera.m_aspectRatio;
	m_viewHeight = camera.m_viewHeight;
	m_ortho = camera.m_ortho;
	return *this;
}

inline Camera::~Camera()
{
}


inline float Camera::nearPlane() const
{
	assert(m_nearPlane < m_farPlane);
	return m_nearPlane;
}

inline void Camera::nearPlane(float f)
{
	assert(f > 0);
	m_nearPlane = f;
}


inline float Camera::farPlane() const
{
	assert(m_nearPlane < m_farPlane);
	return m_farPlane;
}

inline void Camera::farPlane(float f)
{
	assert(m_farPlane > 0);
	m_farPlane = f;
}


inline float Camera::fov() const
{
	return m_fov;
}

inline void Camera::fov(float f)
{
	m_fov = (f > 0 && f < PI);
	assert(f > 0 && f < PI);
	m_fov = f;
}


inline float Camera::aspectRatio() const
{
	return m_aspectRatio;
}

inline void Camera::aspectRatio(float f)
{
	assert(f != 0);
	m_aspectRatio = f;
}


inline float Camera::viewHeight() const
{
	return m_viewHeight;
}

inline void Camera::viewHeight(float height)
{
	m_viewHeight = height;
}


inline float Camera::viewWidth() const
{
	return m_aspectRatio * m_viewHeight;
}


inline bool Camera::ortho() const
{
	return m_ortho;
}

inline void Camera::ortho(bool b)
{
	m_ortho = b;
}

