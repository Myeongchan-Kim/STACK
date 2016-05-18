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

	//Play�Լ��� Update, Start�Լ��� ������ �ҷ��ִ� �Լ��� SystemClass��ü���� �θ��� �ֽ��ϴ�. ������ �ϴ� �Լ��� �ƴմϴ�.
	void Play(float detaTime, InputClass& input, Camera& camera);


private:
	std::list<ModelClass*> m_modelsToBeRendered;
	bool m_started = false;


	friend class SystemClass;
	friend class Renderer;
};

