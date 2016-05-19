#include <windows.h>
#include "Scene.h"
#include "Renderer.h"
#include "InputClass.h"
#include "ModelClass.h"
#include "UIModel.h"
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

	for (auto& model : m_UImodel)
	{
		model->SetUIPosition(camera);
	}
}



Scene::~Scene()
{
	for (auto& model : m_modelsToBeRendered)
	{
		delete model;
		model = nullptr;
	}
	m_modelsToBeRendered.clear();
	for (auto& model : m_UImodel)
	{
		delete model;
		model = nullptr;
	}
	m_UImodel.clear();
}

void Scene::AddModel(ModelClass* model)
{
	m_modelsToBeRendered.push_back(model);
}

void Scene::AddUIModel(UIModel* model)
{
	m_UImodel.push_back(model);
}

void Scene::RemoveModel(std::function<bool(ModelClass*)> compare)
{
	auto iter = std::remove_if(m_modelsToBeRendered.begin(), m_modelsToBeRendered.end(), compare);
	m_modelsToBeRendered.erase(iter, m_modelsToBeRendered.end());
}


void Scene::UpdateUI(Camera& camera)
{
	for (auto& model : m_UImodel)
	{
		delete model;
		model = nullptr;
	}
	//Set UI string
	m_UImodel.clear();
}
