#include "World03.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#include <glm/glm/gtc/type_ptr.hpp>
#include <Renderer/GUI.h>
#include "Renderer/VertexBuffer.h"
#include "World02.h"

#define INTERLEAVE

namespace nc
{
    bool World03::Initialize()
    {
        // disable all messages with severity `GL_DEBUG_SEVERITY_NOTIFICATION`
        glDebugMessageControl(
            GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DEBUG_SEVERITY_NOTIFICATION,
            0, NULL,
            GL_FALSE);

        m_material = GET_RESOURCE(Material, "materials/quad.mtrl");

        /*m_program = GET_RESOURCE(Program, "shaders/unlit_texture.prog");
        m_program->Use();

        m_texture = GET_RESOURCE(Texture, "textures/llama.jpg");
        m_texture->Bind();
        m_texture->SetActive(GL_TEXTURE0);*/

        //vertex
        float vertexData[] =
        {
            -0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //left
             0.8f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,//right
             0.8f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,//top
            -0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f//gear 4 
        };


        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        //glGenVertexArrays(1, &m_vao);
        //glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 8 * sizeof(GLfloat));

        // //position
        // glEnableVertexAttribArray(0);
        // glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        // glVertexAttribBinding(0, 0);

        // //color
        // glEnableVertexAttribArray(1);
        // glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        // glVertexAttribBinding(1, 0);

        // //texcoord
        // glEnableVertexAttribArray(2);
        // glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat));
        // glVertexAttribBinding(2, 0);

        //m_transform.position.z = -10.0f;
        m_vertexBuffer = GET_RESOURCE(VertexBuffer, "vb");
        m_vertexBuffer->CreateVertexBuffer(sizeof(vertexData), 4, vertexData);
        m_vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GLfloat), 0);                  // position 
        m_vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(float));  // color 
        m_vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GLfloat), 6 * sizeof(float));  // texcoord

        return true;
    }


    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0]);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
        ImGui::End();

        //m_transform.rotation.z += 180 * dt;
        
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt : 0;

        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * +dt : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : 0;

        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_speed * +dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_speed * -dt : 0;

        m_time += dt;
        m_material->ProcessGui();
        m_material->Bind();

        //model matrix
        m_material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        //view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0});
        m_material->GetProgram()->SetUniform("view", view);

        //uniform = glGetUniformLocation(m_program->m_program, "view");
        //glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(view));

        //projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
        m_material->GetProgram()->SetUniform("projection", projection);

        //uniform = glGetUniformLocation(m_program->m_program, "projection");
        //glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(projection));

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World03::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        m_vertexBuffer->Draw(GL_TRIANGLE_STRIP);
        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
