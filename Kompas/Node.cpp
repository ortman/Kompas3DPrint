#include "ComKompas.h"
#include "Node.h"

Node::Node(IUnknown* pE, IDispatch* pD) : pEntity(pE), pDefinition(pD) {
	if (pEntity) pEntity->AddRef();
	if (pDefinition) pDefinition->AddRef();
	if (pEntity && !pDefinition) {
		K5::ksEntityPtr entity = pEntity;
		IDispatchPtr def = entity->GetDefinition();
		def.AddRef();
		pDefinition = def.GetInterfacePtr();
	}
}

Node::~Node() {
	if (pDefinition) pDefinition->Release();
	if (pEntity) pEntity->Release();
}

int Node::GetType() const {
	K5::ksEntityPtr entity = pEntity;
	return entity ? entity->type : 0;
}

std::string Node::GetName() const {
	K5::ksEntityPtr entity = pEntity;
	if (!entity) return std::string();
	return Cp1251ToUtf8(entity->name);
}

Node& Node::SetName(const std::string& name) {
	K5::ksEntityPtr entity = pEntity;
	if (entity) {
		entity->name = Utf8ToCp1251(name).c_str();
	}
	return *this;
}

Node& Node::Update() {
	K5::ksEntityPtr entity = pEntity;
	if (entity) entity->Update();
	return *this;
}

std::string Node::Cp1251ToUtf8(const char* cp1251Str) {
    if (!cp1251Str || strlen(cp1251Str) == 0) return "";

    // 1. Из CP1251 в UTF-16
    int wsize = MultiByteToWideChar(1251, 0, cp1251Str, -1, NULL, 0);
    std::wstring wstr(wsize, 0);
    // Использовали wstr.data() вместо &wstr
    MultiByteToWideChar(1251, 0, cp1251Str, -1, wstr.data(), wsize);

    // 2. Из UTF-16 в UTF-8
    int csize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string utf8Str(csize, 0);
    // Использовали utf8Str.data() вместо &utf8Str
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, utf8Str.data(), csize, NULL, NULL);

    if (!utf8Str.empty() && utf8Str.back() == '\0') {
        utf8Str.pop_back();
    }
    return utf8Str;
}

std::string Node::Utf8ToCp1251(const std::string& utf8Str) {
    if (utf8Str.empty()) return "";
    
    // 1. Из UTF-8 в UTF-16
    int wsize = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
    std::wstring wstr(wsize, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], wsize);
    
    // 2. Из UTF-16 в CP1251
    int csize = WideCharToMultiByte(1251, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string cp1251Str(csize, 0);
    WideCharToMultiByte(1251, 0, wstr.c_str(), -1, &cp1251Str[0], csize, NULL, NULL);
    
    // Убираем лишний нуль-терминатор из размера std::string
    if (!cp1251Str.empty() && cp1251Str.back() == '\0') {
        cp1251Str.pop_back();
    }
    return cp1251Str;
}

int Node::TYPE = KConst3D::o3d_unknown;