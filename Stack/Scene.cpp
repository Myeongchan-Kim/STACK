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

	auto isAlphaZero = [](ModelClass*& m) {
		return m->GetColor().w < 0.01f;
	};
	m_modelsToBeRendered.remove_if(isAlphaZero);
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
	for (auto model : m_UImodel)
	{
		delete model;
		model = nullptr;
	}
	//Set UI string
	m_UImodel.clear();
}

XMFLOAT4 Scene::MakeCircularRGB(int seed)
{
	const float seed_max = 100.0f;
	float r = sinf(float(seed % 100) / seed_max * 3.141592 * 2.0f) * 0.5f + 0.5f;
	float g = sinf(float((seed + 33) % 100) / seed_max * 3.141592 * 2.0f) * 0.5f + 0.5f;
	float b = sinf(float((seed + 66) % 100) / seed_max * 3.141592 * 2.0f) * 0.5f + 0.5f;
	return XMFLOAT4({ r, g, b, 1.0f });
}