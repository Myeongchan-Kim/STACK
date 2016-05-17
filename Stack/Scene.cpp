#include <windows.h>
#include "Scene.h"
#include "Renderer.h"
#include "InputClass.h"
#include "ModelClass.h"
#include "VanishingBlock.h"
#include "Camera.h"
#include <algorithm>    // std::remove_if


//Only SystemClass calls this function
void Scene::Play(float deltaTime, InputClass& input, Camera& camera)
{
	if (!m_started)
	{
		Start(camera);
		m_started = true;
	}
	Update(deltaTime, input, camera);

	for (auto& model : m_modelsToBeRendered)
	{
		model->Play(deltaTime);
	}
}



void Scene::AddModel(ModelClass* model)
{
	m_modelsToBeRendered.push_back(model);
}


void Scene::RemoveModel(std::function<bool(ModelClass*)> compare)
{
	auto iter = std::remove_if(m_modelsToBeRendered.begin(), m_modelsToBeRendered.end(), compare);
	m_modelsToBeRendered.erase(iter, m_modelsToBeRendered.end());
}