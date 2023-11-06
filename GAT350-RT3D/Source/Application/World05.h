#pragma once
#include "Framework/World.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include "Renderer/VertexBuffer.h"

namespace nc
{
	class World05 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 5;

		float m_refraction{ 0.5 };
		glm::vec3 m_ambientColor{ 0.2f };

	};
}