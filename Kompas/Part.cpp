#include "ComKompas.h"
#include "Part.h"

Part::Part(IUnknown* d, IUnknown* p) : pDoc(d), pPart(p) {
	if (pDoc) pDoc->AddRef();
	if (pPart) pPart->AddRef();
}

Part::~Part() {
    if (pDoc) pDoc->Release();
    if (pPart) pPart->Release();
}

std::vector<std::unique_ptr<Node>> Part::GetNodes() {
	std::vector<std::unique_ptr<Node>> nodes;
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
			nodes.push_back(std::make_unique<Node>(entity));
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

Part& Part::Remove(std::unique_ptr<Node>& node) {
	K5::ksDocument3DPtr doc = pDoc;
	if (doc) {
		K5::ksEntityPtr entity = node->GetEntity();
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

std::unique_ptr<Plane> Part::GetPlaneXOY() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_planeXOY);
	if (!entity) return nullptr;
	return std::make_unique<Plane>(entity);
}

std::unique_ptr<Plane> Part::GetPlaneXOZ() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_planeXOZ);
	if (!entity) return nullptr;
	return std::make_unique<Plane>(entity);
}

std::unique_ptr<Plane> Part::GetPlaneYOZ() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_planeYOZ);
	if (!entity) return nullptr;
	return std::make_unique<Plane>(entity);
}

std::unique_ptr<Axis> Part::GetAxisOX() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_axisOX);
	if (!entity) return nullptr;
	return std::make_unique<Axis>(entity);
}

std::unique_ptr<Axis> Part::GetAxisOY() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_axisOY);
	if (!entity) return nullptr;
	return std::make_unique<Axis>(entity);
}

std::unique_ptr<Axis> Part::GetAxisOZ() {
	IUnknown* entity = GetDefaultEntity(KConst3D::o3d_axisOZ);
	if (!entity) return nullptr;
	return std::make_unique<Axis>(entity);
}
