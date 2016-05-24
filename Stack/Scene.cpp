#include <windows.h>
#include "Scene.h"
#include "VanishingBlock.h"
#include <algorithm> 
#include "ConstVars.h"


//Only SystemClass calls this function
bool Scene::Play(float deltaTime, InputClass& input, Camera& camera)
{
	bool isOnPlay;
	if (!m_started)
	{
		Start(camera);
		m_started = true;
	}
	isOnPlay = Update(deltaTime, input, camera);
	if (!isOnPlay)
	{
		return false;
	}

	for (auto& model : m_modelsToBeRendered)
	{
		model->Play(deltaTime);
	}

	for (auto& model : m_UImodel)
	{
		model->SetUIPosition(camera);
	}

	return true;
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


void Scene::UpdateUIString(Camera& camera)
{
	for (auto model : m_UImodel)
	{
		delete model;
		model = nullptr;
	}
	//Set UI string
	m_UImodel.clear();
}

void Scene::ShutDownAll()
{
	for (auto model : m_modelsToBeRendered)
		delete model;
	m_modelsToBeRendered.clear();

	for (auto model : m_UImodel)
		delete model;
	m_UImodel.clear();

	//사용자 정의 ShutDown
	ShutDown();
}


XMFLOAT4 Scene::MakeCircularRGB(int seed)
{
	const float seed_max = 100.0f;
	float r = sinf(float(seed % 100) / seed_max * 3.141592 * 2.0f) * 0.4f + 1.0f;
	float g = sinf(float((seed + 33) % 100) / seed_max * 3.141592 * 2.0f) * 0.4f + 1.0f;
	float b = sinf(float((seed + 66) % 100) / seed_max * 3.141592 * 2.0f) * 0.4f + 1.0f;
	return XMFLOAT4({ r, g, b, 1.0f });
}