#include "../ComKompas.h"
#include "NodeMacro.h"
#include "../Kompas3D.h"

NodeMacro::NodeMacro(IUnknown* pE, IDispatch* pD, bool show, const std::optional<std::string>& name) : Node(pE, pD) {
	if (!pEntity) return;
	K5::ksEntityPtr entity = pEntity;
	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
	K5::ksMacro3DDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить Macro3DDefinition, entityType=" + std::to_string(entity->type));
	def->StaffVisible = show;
	entity->Create();
}

NodeMacro& NodeMacro::Add(Node node) {
	K5::ksMacro3DDefinitionPtr def = pDefinition;
	K5::ksEntityPtr e = node.pEntity;
	if (def && e) def->Add(e);
	return *this;
}

std::vector<Node> NodeMacro::GetNodes() {
	std::vector<Node> nodes;
	K5::ksMacro3DDefinitionPtr def = pDefinition;
	if (!def) return nodes;
	K5::ksFeatureCollectionPtr subFeatures = def->FeatureCollection();
	if (!subFeatures) return nodes;
	int count = subFeatures->GetCount();
	for (int i = 0; i < count; ++i) {
		K5::ksFeaturePtr feature = subFeatures->GetByIndex(i);
		if (K5::ksEntityPtr e = feature->GetObject()) {
			nodes.push_back(Node(e));
		}
	}
	return nodes;
}

NodeMacro& NodeMacro::Show(bool show) {
	K5::ksMacro3DDefinitionPtr def = pDefinition;
	if (def) def->StaffVisible = show;
	return *this;
}

bool NodeMacro::SetUserParam(void* param, size_t size, int cmd) {
	K5::ksUserParamPtr pParam = Kompas3D::GetParamStruct<K5::ksUserParamPtr>(KConst::ko_UserParam);
	if (pParam) {
		pParam->Init();
		if (K5::ksMacro3DDefinitionPtr def = pDefinition) {
			pParam->libName  = "Kompas 3D Print";
	        pParam->fileName = "Kompas3DPrint.rtw";
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

    if (K5::ksUserParamPtr pParam = Kompas3D::GetParamStruct<K5::ksUserParamPtr>(KConst::ko_UserParam)) {
        pParam->Init();
        if (K5::ksMacro3DDefinitionPtr def = pDefinition) {
            // Инициализируем структуру теми же библиотечными данными для поиска
            pParam->libName  = "Kompas 3D Print";
            pParam->fileName = "Kompas3DPrint.rtw";

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