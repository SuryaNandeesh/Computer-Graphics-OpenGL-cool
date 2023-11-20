#include "Scene.h"
#include "Framework/Framework.h"

namespace nc
{
	bool Scene::Initialize()
	{
		for (auto& actor : m_actors) actor->Initialize();

		return true;
	}

	void Scene::Update(float dt)
	{
		m_dt = dt;
		// update and remove destroyed actors
		auto iter = m_actors.begin();
		while (iter != m_actors.end())
		{
			if ((*iter)->active) (*iter)->Update(dt);
			((*iter)->destroyed) ? iter = m_actors.erase(iter) : iter++;
		}
	}

	void Scene::Draw(Renderer& renderer)
	{
		// get light components
		auto lights = GetComponents<LightComponent>();
		/*std::vector<LightComponent*> lights;
		for (auto& actor : m_actors)
		{
			if (!actor->active) continue;

			auto component = actor->GetComponent<LightComponent>();
			if (component){ lights.push_back(component); }
		}*/

		// get camera component
		auto cameras = GetComponents<CameraComponent>();

		CameraComponent* camera = (!cameras.empty()) ? cameras[0] : nullptr;
		/*CameraComponent* camera = nullptr;
		for (auto& actor : m_actors)
		{
			if (!actor->active) continue;

			camera = actor->GetComponent<CameraComponent>();
			if (camera) { break; }
		}*/

		// get all shader programs in the resource system
		auto programs = GET_RESOURCES(Program);
		// set all shader programs camera and lights uniforms
		for (auto& program : programs)
		{
			program->Use();

			// set camera in shader program
			if (camera) camera->SetProgram(program);

			// set lights in shader program
			int index = 0;
			for (auto light : lights)
			{
				std::string name = "lights[" + std::to_string(index++) + "]";

				glm::mat4 view = (camera) ? camera->view : glm::mat4(1);

				light->SetProgram(program, name, view);
			}

			program->SetUniform("numLights", index);
			program->SetUniform("ambientLight", ambientColor);
		}

		for (auto& actor : m_actors)
		{
			if (actor->active) actor->Draw(renderer);
		}
		
	}

	void Scene::Add(std::unique_ptr<Actor> actor)
	{
		actor->m_scene = this;
		actor->m_game = m_game;
		m_actors.push_back(std::move(actor));
	}

	void Scene::RemoveAll(bool force)
	{
		auto iter = m_actors.begin();
		while (iter != m_actors.end())
		{
			(force || !(*iter)->persistent) ? iter = m_actors.erase(iter) : iter++;
		}
	}

	void Scene::Remove(Actor* actor)
	{
		auto iter = m_actors.begin();
		while (iter != m_actors.end())
		{
			if ((*iter).get() == actor) {
				m_actors.erase(iter);
				break;
			}
			iter++;
		}
	}

	bool Scene::Load(const std::string& filename)
	{
		rapidjson::Document document;
		if (!Json::Load(filename, document))
		{
			ERROR_LOG("Could not load scene file: " << filename);
			return false;
		}

		Read(document);

		return true;
	}

	void Scene::Read(const json_t& value)
	{
		if (HAS_DATA(value, actors) && GET_DATA(value, actors).IsArray())
		{
			for (auto& actorValue : GET_DATA(value, actors).GetArray())
			{
				std::string type;
				READ_DATA(actorValue, type);

				auto actor = CREATE_CLASS_BASE(Actor, type);
				actor->Read(actorValue);

				if (actor->prototype)
				{
					std::string name = actor->name;
					Factory::Instance().RegisterPrototype(name, std::move(actor));
				}
				else
				{
					Add(std::move(actor));
				}
			}
		}

	}

	void Scene::ProcessGui()
	{
		float fps = 1 / m_dt;
		float ms = 1000 * m_dt;
		ImVec4 color = (fps < 30) ? ImVec4{ 1, 0, 0, 1 } : ImVec4{ 1, 1, 1, 1 };
		ImGui::TextColored(color, "%.2f FPS (%.2f)", fps, ms);
		ImGui::ColorEdit3("Ambient", glm::value_ptr(ambientColor));

	}

}