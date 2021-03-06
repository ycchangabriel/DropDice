#pragma once
#ifndef SCENE
#define SCENE

#include <vector>
#include "Floor.h"
#include "RubiksCore.h"
#include "RayTracer.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>

struct Scene
{
	std::string name;
	std::vector<std::vector<float> > floors;
private:
	friend class cereal::access;
	template<class Archive> void save(Archive & ar) const
	{
		ar(name, floors);
	}
	template<class Archive> void load(Archive & ar)
	{
		ar(name, floors);
	}
};

#include <map>
struct SceneConfigFile
{
	const std::string filename = "scenes.dat";
	std::map<std::string, Scene> scenes;
	void Save();
	void Load();
private:
	friend class cereal::access;
	template<class Archive> void save(Archive & ar) const
	{
		ar(scenes);
	}
	template<class Archive> void load(Archive & ar)
	{
		ar(scenes);
	}
};

class SceneMachine
{
public:
	SceneConfigFile		mConfig;
	Scene				mCurrentScene;
	Floor				mFloor;
	RubiksCore			mRubiksCore;
	Instance*			mActiveInstance;
	enum InstanceType   { NTH, FLOOR, ROD, CUBE };

	SceneMachine();
	~SceneMachine();
	void SetScene();	// modify later to switch scene
	void Render();
	Instance* PickNearestInstance(glm::vec3, glm::vec3);	// Only pick Floor, Rod and Cube
	int ClassifyInstance(Instance*);
	void ClickOnFloor(Instance*);
	void ClickOnCore(Instance*, const int&, const RayTracer::Line&);
	void ReleaseCore();
};

#endif SCENE