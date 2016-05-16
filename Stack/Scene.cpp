#include <windows.h>
#include "Scene.h"
#include "Renderer.h"
#include "InputClass.h"
#include "ModelClass.h"
#include "VanishingBlock.h"
#include "Camera.h"


//Only SystemClass calls this function
void Scene::Play(float deltaTime, InputClass& input, Camera& camera)
{
	if (!m_started)
	{
		Start();
		m_started = true;
	}
	Update(input, camera);

	for (auto& model : m_modelsToBeRendered)
	{
		model->Play(deltaTime);
	}
}



void Scene::AddModel(ModelClass* model)
{
	m_modelsToBeRendered.push_back(model);
}