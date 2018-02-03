#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <memory>

template <class modelType, class solverType, class propsType>
class Scene
{
public:
	typedef modelType Model;
	typedef solverType Method;
	typedef propsType Properties;
protected:
	std::shared_ptr<Model> model;
	std::shared_ptr<Method> method;
public:
	Scene() {};
	~Scene() {};

	void load(const propsType& props)
	{
		model = std::make_shared<Model>();
		model->load(props);
		model->setSnapshotter(model.get());
		method = std::make_shared<Method>(model.get());
	}
	void start()
	{
		method->start();
	}
};

#endif /* SCENE_HPP_ */