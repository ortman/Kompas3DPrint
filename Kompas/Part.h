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
	IUnknown* CreateEntity(int type);
	IUnknown* GetDefaultEntity(int type);
	
public:
	IUnknown* pPart;
	Part(IUnknown* pDoc, IUnknown* pPart);
	Part() : Part(nullptr, nullptr) {}
	Part(const Part& part);                // Конструктор копирования
	Part& operator=(const Part& part);     // Оператор копирующего присваивания
	Part(Part&& part) noexcept;            // Конструктор перемещения
	Part& operator=(Part&& part) noexcept; // Оператор перемещающего присваивания
	~Part();
	std::vector<Node> GetNodes();
	template <typename T, typename... Args>
	T Create(Args&&... args) {
		static_assert(std::is_base_of<Node, T>::value, "T must be derived from Node");
		IUnknown* entity = CreateEntity(T::TYPE);
		if (!entity) throw Kompas3DException(std::string("Не могу создать объект ") + typeid(T).name());
		return T(entity, NULL, std::forward<Args>(args)...);
	}
	std::string Name();
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
