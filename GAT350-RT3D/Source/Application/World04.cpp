#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#include <glm/glm/gtc/type_ptr.hpp>

#define INTERLEAVE

namespace nc
{
    bool World04::Initialize()
    {
        auto material = GET_RESOURCE(Material, "materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
        //m_model->Load("models/buddha.obj", glm::vec3{0}, glm::vec3{90, 0, 0});

        m_light.type = light_t::eType::Point;
        m_light.position = glm::vec3{0, 5, 0};
        m_light.direction = glm::vec3{0, -1, 0};
        m_light.color = glm::vec3{ 1, 1, 1 };
        m_light.innerAngle = 10.0f;
        m_light.outerAngle = 30.0f;

        return true;
    }
    
    void World04::Shutdown()
    {
        //
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Scene");
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_ambientColor));
        ImGui::Separator();

        for (int i = 0; i < 3; i++)
        {
            std::string name = "light" + std::to_string(i);
            if (ImGui::Selectable(name.c_str(), m_selected == i)) m_selected = i;
        }
        ImGui::End();

        ImGui::Begin("Light");
        const char* types[] = { "Point", "Directional", "Spot" };
        ImGui::Combo("Type", (int*)(& m_light.type), types, 3);

        if (m_light.type != light_t::Directional) ImGui::DragFloat3("Position", glm::value_ptr(m_light.position), 0.1f);
        if (m_light.type != light_t::Point) ImGui::DragFloat3("Direction", glm::value_ptr(m_light.direction), 0.1f);
        if (m_light.type != light_t::Spot)
        {
            ImGui::DragFloat("Inner Angle", &m_light.innerAngle, 1, 0, m_light.outerAngle);
            ImGui::DragFloat("Outer Angle", &m_light.outerAngle, 1, m_light.innerAngle, 90);
        }
        ImGui::ColorEdit3("Color", glm::value_ptr(m_light.color));

        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_ambientColor));
        ImGui::End();
        
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt : 0;

        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * +dt : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : 0;

        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_speed * +dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_speed * -dt : 0;

        m_time += dt;

        auto material = m_model->GetMaterial();

        material->ProcessGui();
        material->Bind();

        material->GetProgram()->SetUniform("light.type", m_light.type);
        material->GetProgram()->SetUniform("light.position", m_light.position);
        material->GetProgram()->SetUniform("light.direction", glm::normalize(m_light.direction));
        material->GetProgram()->SetUniform("light.color", m_light.color);
        material->GetProgram()->SetUniform("light.innerAngle", glm::radians(m_light.innerAngle));
        material->GetProgram()->SetUniform("light.outerAngle", glm::radians(m_light.outerAngle));

        material->GetProgram()->SetUniform("ambientLight", m_ambientColor);

        //model matrix
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        //view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0});
        material->GetProgram()->SetUniform("view", view);

        //projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // renderf
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        m_model->Draw(GL_TRIANGLES); 

        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
