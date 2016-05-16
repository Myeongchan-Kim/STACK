#pragma once
#include <vector>

class ModelClass;
class InputClass;
class Camera;
class Renderer;

class Scene
{
public:
	void AddModel(ModelClass* model);
	virtual void Start() = 0;
	virtual void Update(InputClass& input, Camera& camera) = 0;

private:
	void Play(float detaTime, InputClass& input, std::vector<ModelClass*>& modelsToBeRendered, Camera& camera);


private:
	std::vector<ModelClass*> m_modelsToBeRendered;
	bool m_started = false;


	friend class SystemClass;
};

