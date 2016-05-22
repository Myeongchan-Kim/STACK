#pragma once
#include <list>
#include "ConstVars.h"
#include "Renderer.h"

class UIModel;
class ModelClass;
class InputClass;
class Camera;

class Scene
{
public:
	virtual ~Scene();
	void AddModel(ModelClass* model);
	void AddUIModel(UIModel* model);
	void RemoveModel(std::function<bool(ModelClass*)> compare);
	virtual void Start(Camera& camera) = 0;
	virtual bool Update(float dt, InputClass& input, Camera& camera) = 0;
	virtual void UpdateUIString(Camera& camera);
	
	XMFLOAT4 MakeCircularRGB(int seed);
protected:
	std::list<ModelClass*> m_modelsToBeRendered;
	std::list<UIModel*> m_UImodel;

private:

	//Play�Լ��� Update, Start�Լ��� ������ �ҷ��ִ� �Լ��� SystemClass��ü���� �θ��� �ֽ��ϴ�. ������ �ϴ� �Լ��� �ƴմϴ�.
	bool Play(float detaTime, InputClass& input, Camera& camera);


	bool m_started = false;

	friend class SystemClass;
	friend class Renderer;
};

