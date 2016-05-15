#include <windows.h>
#include "Scene.h"
#include "Renderer.h"
#include "InputClass.h"
#include "ModelClass.h"
#include "VanishingBlock.h"
#include "Camera.h"


//Only SystemClass calls this function
void Scene::Play(float detaTime, InputClass& input, std::vector<ModelClass*>& modelsToBeRendered, Camera& camera)
{
	if (!m_started)
	{
		Start();
		m_started = true;
	}
	Update(input, camera);

	modelsToBeRendered = std::move(m_modelsToBeRendered);
	m_modelsToBeRendered.clear();
}



void Scene::AddModel(ModelClass* model)
{
	m_modelsToBeRendered.push_back(model);
}


