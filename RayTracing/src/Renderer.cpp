#include "Renderer.h"

#include "glm/glm.hpp"

namespace Utils {

	uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = a << 24 | b << 16 | g << 8 | r;
		return result;
	}

}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (!m_FinalImage || width != m_FinalImage->GetWidth() || height != m_FinalImage->GetHeight())
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
		m_AspectRatio = (float)width / (float)height;
		delete[] m_ImageData;
		m_ImageData = new uint32_t[width * height];
	}
}

void Renderer::Render()
{
	for (int y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (int x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f; // -1 -> 1
			coord.x *= m_AspectRatio;
			glm::vec4 sphereColor = PerPixel(coord);
			sphereColor = glm::clamp(sphereColor, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(sphereColor);
		}
	}

	m_FinalImage->SetData(m_ImageData);

}

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
	glm::vec3 rayOrigin = { 0.0f, 0.0f, 2.0f };
	glm::vec3 rayDirection = { coord.x, coord.y, -1.0f };
	glm::vec3 sphereCenter = { 0.0f, 0.0f, 0.0f };
	float radius = 0.5f;

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * (glm::dot(rayOrigin, rayDirection) - glm::dot(rayDirection, sphereCenter));
	float c = glm::dot(rayOrigin, rayOrigin) + glm::dot(sphereCenter, sphereCenter) - 2.0f * glm::dot(rayOrigin, sphereCenter) - radius * radius;

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float t[] = {
		(-b - sqrt(discriminant)) / 2.0f * a,
		(-b + sqrt(discriminant)) / 2.0f * a,
	};

	for (int i = 0; i < 1; i++)
	{
		glm::vec3 hitPosition = rayOrigin + rayDirection * t[i];
		glm::vec3 normal = hitPosition - sphereCenter;
		float light = glm::max(glm::dot(glm::normalize(normal), glm::normalize(-m_LightDirection)), 0.0f);
		return glm::vec4(m_SphereColor * light, 1.0f);
	}
	return glm::vec4(0.0f);
}
