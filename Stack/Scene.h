#pragma once
#include <list>
#include "Renderer.h"

class ModelClass;
class InputClass;
class Camera;

class Scene
{
public:
	~Scene();
	void AddModel(ModelClass* model);
	void RemoveModel(std::function<bool(ModelClass*)> compare);
	virtual void Start(Camera& camera) = 0;
	virtual void Update(float dt, InputClass& input, Camera& camera) = 0;

private:

	//Play함수는 Update, Start함수를 적절히 불러주는 함수로 SystemClass객체만이 부를수 있습니다. 재정의 하는 함수가 아닙니다.
	void Play(float detaTime, InputClass& input, Camera& camera);


private:
	std::list<ModelClass*> m_modelsToBeRendered;
	bool m_started = false;


	friend class SystemClass;
	friend class Renderer;
};

