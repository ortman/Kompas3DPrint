#ifndef _Kompas3DPrint_RealThreads_hpp_
#define _Kompas3DPrint_RealThreads_hpp_
#include "../Kompas/Kompas3D.h"

class Threads {
public:
	void Start() {
		
		auto doc = Kompas3D::GetActiveDocument3D();
		if (!doc) return;
		Part topPart = doc->GetTopPart();
		
		// Удаляем макрос
		for (Node& node : topPart.GetNodes()) {
			if (node.IsType(NodeMacro::TYPE) && node.GetName() == "Реальные резьбы") {
				topPart.Remove(node);
			}
		}
		
		NodeMacro macro = topPart.Create<NodeMacro>(false, "Реальные резьбы");
		for (Node& node : topPart.GetNodes()) {
			if (node.IsType(ThreadDesignation::TYPE)) {
				CreateThread(topPart, macro, ThreadDesignation(node));
			}
		}
		macro.Update();
	}

private:
	void CreateThread(Part& part, NodeMacro& macro, const ThreadDesignation& thread) {
		Kompas3D::Error("Резьба " + std::to_string(thread.GetDiameter()) + (thread.IsForwardDir() ? " правая" : " левая"));
		
		Edge beginEdge = thread.GetBeginEdge();

    //Face> planarFace = beginEdge->RightFace();
    //Face> cylinderFace = beginEdge->LeftFace();
    //if (!planarFace->IsPlanar()) {
    //    planarFace = beginEdge->LeftFace();
    //    cylinderFace = beginEdge->RightFace();
    //}
	}
};

#endif
