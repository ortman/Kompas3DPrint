#include "../ComKompas.h"
#include "NodeMacro.h"
#include "../Kompas3D.h"

NodeMacro::NodeMacro(IUnknown* p, bool show, const std::optional<std::string>& name) : Node(p) {
	K5::ksEntityPtr entity = pEntity;
	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
	K5::ksMacro3DDefinitionPtr def = entity->GetDefinition();
	if (!def) throw Kompas3DException("Не могу получить Macro3DDefinition, entityType=" + std::to_string(entity->type));
	def->StaffVisible = show;
	entity->Create();
}

NodeMacro& NodeMacro::Add(Node& node) {
	K5::ksEntityPtr entity = pEntity;
	K5::ksMacro3DDefinitionPtr def = entity->GetDefinition();
	K5::ksEntityPtr e = node.GetEntity();
	if (def && e) def->Add(e);
	return *this;
}

std::vector<std::unique_ptr<Node>> NodeMacro::GetNodes() {
	std::vector<std::unique_ptr<Node>> nodes;
	K5::ksEntityPtr entity = pEntity;
	if (!entity) return nodes;
	K5::ksMacro3DDefinitionPtr def = entity->GetDefinition();
	if (!def) return nodes;
	K5::ksFeatureCollectionPtr subFeatures = def->FeatureCollection();
	if (!subFeatures) return nodes;
	int count = subFeatures->GetCount();
	for (int i = 0; i < count; ++i) {
		K5::ksFeaturePtr feature = subFeatures->GetByIndex(i);
		if (K5::ksEntityPtr e = feature->GetObject()) {
			nodes.push_back(std::make_unique<Node>(e));
		}
	}
	return nodes;
}

NodeMacro& NodeMacro::Show(bool show) {
	K5::ksEntityPtr entity = pEntity;
	K5::ksMacro3DDefinitionPtr def = entity->GetDefinition();
	if (def) def->StaffVisible = show;
	return *this;
}

bool NodeMacro::SetUserParam(void* param, size_t size, int cmd) {
	K5::ksUserParamPtr pParam = Kompas3D::GetParamStruct<K5::ksUserParamPtr>(KConst::ko_UserParam);
	K5::ksEntityPtr entity = pEntity;
	if (pParam && entity) {
		pParam->Init();
		if (K5::ksMacro3DDefinitionPtr def = entity->GetDefinition()) {
			pParam->libName  = "Kompas 3D Print";
	        pParam->fileName = "Kompas3DPrint.dll";
	        pParam->number = cmd;
	        
	        SAFEARRAY* pSafeArray = SafeArrayCreateVector(VT_UI1, 0, (ULONG)size);
            if (pSafeArray) {
                // 2. Копируем данные БЕЗ SafeArrayAccessData (для одномерных массивов pvData доступен напрямую)
				if (pSafeArray->pvData) {
                    memcpy(pSafeArray->pvData, param, size);
                }
                VARIANT var;
                var.vt = VT_ARRAY | VT_UI1;
                var.parray = pSafeArray;
				pParam->UserParams = _variant_t(var, false);
				return def->SetUserParam(pParam);
            }
		}
	}
	return false;
}

bool NodeMacro::GetUserParam(void* param, size_t size) {
    if (!param || size == 0) return false;

    K5::ksUserParamPtr pParam = Kompas3D::GetParamStruct<K5::ksUserParamPtr>(KConst::ko_UserParam);
    K5::ksEntityPtr entity = pEntity;

    if (pParam && entity) {
        pParam->Init();
        if (K5::ksMacro3DDefinitionPtr def = entity->GetDefinition()) {
            // Инициализируем структуру теми же библиотечными данными для поиска
            pParam->libName  = "Kompas 3D Print";
            pParam->fileName = "Kompas3DPrint.dll";

            // Запрашиваем параметры у макрообъекта
            if (def->GetUserParam(pParam)) {
                _variant_t var = pParam->UserParams;
                
                // Проверяем, что вернулся массив байт
                if ((var.vt & VT_ARRAY) && (var.vt & VT_UI1) && var.parray) {
                    SAFEARRAY* pSafeArray = var.parray;
                    
                    // Вычисляем размер данных в SafeArray
                    long lBound, uBound;
                    SafeArrayGetLBound(pSafeArray, 1, &lBound);
                    SafeArrayGetUBound(pSafeArray, 1, &uBound);
                    size_t actualSize = (uBound - lBound + 1);

                    // Проверяем, достаточно ли места в целевом буфере
                    if (actualSize <= size && pSafeArray->pvData) {
                        memcpy(param, pSafeArray->pvData, actualSize);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int NodeMacro::TYPE = KConst3D::o3d_MacroObject;