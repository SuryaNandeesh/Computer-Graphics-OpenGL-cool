#include "ModelComponent.h"
#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"

namespace nc
{
	CLASS_DEFINITION(ModelComponent)

	bool ModelComponent::Initialize()
	{
		if (!modelName.empty())
		{
			m_model = std::make_shared<Model>();
			m_model->Load(modelName);
			//ADD_RESOURCE(modelName, m_model);
		}
		if (m_model && !materialName.empty())
		{
			m_model->SetMaterial(GET_RESOURCE(Material, materialName));
		}

		return true;
	}

	void ModelComponent::Update(float dt)
	{
	}

	void ModelComponent::Draw(Renderer& renderer)
	{
		auto material = m_model->GetMaterial();
		material->Bind();
		material->GetProgram()->SetUniform("model", m_owner->transform.GetMatrix());
		glDepthMask(enableDepth);
		glCullFace(cullFace);
		m_model->Draw();
	}

	void ModelComponent::Read(const json_t& value)
	{
		READ_DATA(value, modelName);
		READ_DATA(value, materialName);

		READ_DATA(value, enableDepth);
		std::string cullFaceName;
		if (READ_NAME_DATA(value, "cullface", cullFaceName))
		{
			if (StringUtils::IsEqualIgnoreCase(cullFaceName, "front")) cullFace = GL_FRONT;
			if (StringUtils::IsEqualIgnoreCase(cullFaceName, "back")) cullFace = GL_BACK;
		}
		
	}
}
