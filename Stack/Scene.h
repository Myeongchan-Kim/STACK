#pragma once
#include <vector>

class ModelClass;
class InputClass;
class Camera;
class Renderer;

class Scene
{
public:
	void Play(float detaTime, InputClass& input, std::vector<ModelClass*>& modelsToBeRendered, Renderer& renderer);
	Scene();
	~Scene();

private:
	ModelClass* m_backGround = nullptr;

private:
	Camera* m_camera;

};

