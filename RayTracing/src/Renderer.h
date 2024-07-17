#pragma once

#include "Walnut/Image.h"

#include "Walnut/Random.h"

#include <memory>

class Renderer
{
public:

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	glm::vec4 PerPixel(glm::vec2 coord);

	const std::shared_ptr<Walnut::Image> GetFinalImage() { return m_FinalImage; }
	glm::vec3& GetSphereColor() { return m_SphereColor; }
	glm::vec3& GetLightDirection() { return m_LightDirection; }
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	float m_AspectRatio = 0.0f;
	glm::vec3 m_SphereColor = { 1, 0, 1 };
	glm::vec3 m_LightDirection = { -1, -1, -1 };
};