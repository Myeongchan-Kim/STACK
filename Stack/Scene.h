#pragma once
#include <vector>
#include "Renderer.h"

class ModelClass;
class InputClass;
class Camera;

class Scene
{
public:
	void AddModel(ModelClass* model);
	virtual void Start() = 0;
	virtual void Update(InputClass& input, Camera& camera) = 0;

private:
	void Play(float detaTime, InputClass& input, Camera& camera);


private:
	std::vector<ModelClass*> m_modelsToBeRendered;
	bool m_started = false;


	friend class SystemClass;
	friend class Renderer;
};

