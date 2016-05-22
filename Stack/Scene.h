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

	//Play함수는 Update, Start함수를 적절히 불러주는 함수로 SystemClass객체만이 부를수 있습니다. 재정의 하는 함수가 아닙니다.
	bool Play(float detaTime, InputClass& input, Camera& camera);


	bool m_started = false;

	friend class SystemClass;
	friend class Renderer;
};

