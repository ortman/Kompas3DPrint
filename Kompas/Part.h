#ifndef _ComTest_Part_h_
#define _ComTest_Part_h_

#pragma once
#include "Node.h"
#include "Node/Plane.h"
#include "Node/Axis.h"
#include <vector>
#include <utility>

class Part {
private:
	IUnknown* pDoc;
	IUnknown* pPart;
	IUnknown* CreateEntity(int type);
	IUnknown* GetDefaultEntity(int type);
	
public:
	Part(IUnknown* pDoc, IUnknown* pPart);
	~Part();
	std::vector<std::unique_ptr<Node>> GetNodes();
	template <typename T, typename... Args>
	std::unique_ptr<T> Create(Args&&... args) {
		static_assert(std::is_base_of<Node, T>::value, "T must be derived from Node");
		IUnknown* entity = CreateEntity(T::TYPE);
		if (!entity) throw Kompas3DException(std::string("Не могу создать объект ") + typeid(T).name());
		return std::make_unique<T>(entity, std::forward<Args>(args)...);
	}
	Part& Remove(std::unique_ptr<Node>& node);
	std::unique_ptr<Plane> GetPlaneXOY();
	std::unique_ptr<Plane> GetPlaneXOZ();
	std::unique_ptr<Plane> GetPlaneYOZ();
	std::unique_ptr<Axis> GetAxisOX();
	std::unique_ptr<Axis> GetAxisOY();
	std::unique_ptr<Axis> GetAxisOZ();
};

#endif
