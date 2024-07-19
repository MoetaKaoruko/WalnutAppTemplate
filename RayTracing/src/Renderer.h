#pragma once

#include "Walnut/Image.h"

#include "Walnut/Random.h"

#include "Scene.h"
#include "Camera.h"
#include "Ray.h"

#include <memory>

class Renderer
{
public:
	struct Settings
	{
		bool Accumulate = true;
		bool SlowRandom = true;
	};

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera& camera);

	void ResetFrameCount() { m_FrameCount = 1; }

	const std::shared_ptr<Walnut::Image> GetFinalImage() { return m_FinalImage; }
	Settings& GetSettings() { return m_Settings; }
private:
	struct HitPayload
	{
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		int ObjectIndex;
	};

	glm::vec4 PerPixel(uint32_t x, uint32_t y);
	HitPayload TraceRay(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
	HitPayload Miss(const Ray& ray);


private:
	Settings m_Settings;
	std::shared_ptr<Walnut::Image> m_FinalImage;
	std::vector<uint32_t> m_ImageHorizatalIterator;
	std::vector<uint32_t> m_ImageVerticalIterator;

	uint32_t* m_ImageData = nullptr;
	glm::vec4* m_AccumulationData = nullptr;
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;

	float m_AspectRatio = 0.0f;
	uint32_t m_FrameCount = 1;
};