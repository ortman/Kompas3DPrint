#include "ComKompas.h"
#include "Kompas3D.h"
#include "Part.h"

Part::Part(IUnknown* d, IUnknown* p) : pDoc(d), pPart(p) {
	if (pDoc) pDoc->AddRef();
	if (pPart) pPart->AddRef();
}

Part::~Part() {
	if (pPart) pPart->Release();
	if (pDoc) pDoc->Release();
}

std::string Part::Name() {
	K5::ksPartPtr part = pPart;
	if (!part) throw Kompas3DException("Не могу получить объект Part");
	return Node::Cp1251ToUtf8(part->name);
}

std::vector<Node> Part::GetNodes() {
	std::vector<Node> nodes;
	K5::ksPartPtr part = pPart;
	if (!part) return nodes;
	K5::ksFeaturePtr topFeature = part->GetFeature();
	if (!topFeature) return nodes;
	K5::ksFeatureCollectionPtr subFeatures = topFeature->SubFeatureCollection(true, true);
	if (!subFeatures) return nodes;
	int count = subFeatures->GetCount();
	for (int i = 0; i < count; ++i) {
		K5::ksFeaturePtr feature = subFeatures->GetByIndex(i);
		K5::ksEntityPtr entity = feature->GetObject();
		if (entity) {
			//entity.AddRef();
			nodes.push_back(Node(entity.GetInterfacePtr()));
		}
	}
	return nodes;
}

IUnknown* Part::CreateEntity(int type) {
	K5::ksPartPtr part = pPart;
	if (!part) return nullptr;
	K5::ksEntityPtr entity = part->NewEntity(type);
	if (!entity) return nullptr;
	entity->AddRef();
	return entity;
}

Part& Part::Remove(Node node) {
	K5::ksDocument3DPtr doc = pDoc;
	if (doc) {
		K5::ksEntityPtr entity = node.pEntity;
		doc->DeleteObject(entity);
	}
	return *this;
}

IUnknown* Part::GetDefaultEntity(int type) {
	K5::ksPartPtr part = pPart;
	if (!part) return nullptr;
	K5::ksEntityPtr entity = part->GetDefaultEntity(type);
	entity->AddRef();
	return entity;
}

Plane Part::GetPlaneXOY() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_planeXOY);
	if (!entity) return nullptr;
	return Plane(entity);
}

Plane Part::GetPlaneXOZ() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_planeXOZ);
	if (!entity) return nullptr;
	return Plane(entity);
}

Plane Part::GetPlaneYOZ() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_planeYOZ);
	if (!entity) return nullptr;
	return Plane(entity);
}

Axis Part::GetAxisOX() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_axisOX);
	if (!entity) return nullptr;
	return Axis(entity);
}

Axis Part::GetAxisOY() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_axisOY);
	if (!entity) return nullptr;
	return Axis(entity);
}

Axis Part::GetAxisOZ() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_axisOZ);
	if (!entity) return nullptr;
	return Axis(entity);
}

std::vector<Part::Variable> Part::GetVariables(bool isExternal) {
	std::vector<Part::Variable> variables;
	K5::ksPartPtr part = pPart;
	K5::ksVariableCollectionPtr vs = part->VariableCollection();
	int cnt = vs->GetCount();
	for (int i = 0; i < cnt; ++i) {
		K5::ksVariablePtr v = vs->GetByIndex(i);
		variables.push_back({
			(bool)v->external,
			v->value,
			Node::Cp1251ToUtf8(v->name),
			Node::Cp1251ToUtf8(v->note)
		});
	}
	return variables;
}