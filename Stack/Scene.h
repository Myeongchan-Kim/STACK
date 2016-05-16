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

	//Play함수는 Update, Start함수를 적절히 불러주는 함수로 SystemClass객체만이 부를수 있습니다. 재정의 하는 함수가 아닙니다.
	void Play(float detaTime, InputClass& input, Camera& camera);


private:
	std::vector<ModelClass*> m_modelsToBeRendered;
	bool m_started = false;


	friend class SystemClass;
	friend class Renderer;
};

