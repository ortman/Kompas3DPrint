#ifndef _ComTest_Part_h_
#define _ComTest_Part_h_

#include "Node.h"
#include "Node/Plane.h"
#include "Node/Axis.h"
#include <vector>
#include <utility>

class Part {
public:
	struct Variable {
		bool isExternal;
		double value;
		std::string name;
		std::string comment;
	};

private:
	IUnknown* pDoc;
	IUnknown* pPart;
	IUnknown* CreateEntity(int type);
	IUnknown* GetDefaultEntity(int type);
	
public:
	Part(IUnknown* pDoc, IUnknown* pPart);
	~Part();
	std::vector<Node> GetNodes();
	template <typename T, typename... Args>
	T Create(Args&&... args) {
		static_assert(std::is_base_of<Node, T>::value, "T must be derived from Node");
		IUnknown* entity = CreateEntity(T::TYPE);
		if (!entity) throw Kompas3DException(std::string("Не могу создать объект ") + typeid(T).name());
		return T(entity, std::forward<Args>(args)...);
	}
	Part& Remove(Node node);
	Plane GetPlaneXOY();
	Plane GetPlaneXOZ();
	Plane GetPlaneYOZ();
	Axis GetAxisOX();
	Axis GetAxisOY();
	Axis GetAxisOZ();
	std::vector<Variable> GetVariables(bool isExternal = false);
	operator bool() const { return pPart; }
};

#endif
