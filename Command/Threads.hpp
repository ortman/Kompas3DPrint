#ifndef _Kompas3DPrint_RealThreads_hpp_
#define _Kompas3DPrint_RealThreads_hpp_
#include "../Kompas/Kompas3D.h"

class Threads {
public:
	void Start() {
		
		Doc3D doc = Kompas3D::GetActiveDocument3D();
		if (!doc) return;
		Part topPart = doc.GetTopPart();
		
		// Удаляем макрос
		for (Node& node : topPart.GetNodes()) {
			if (node.IsType(NodeMacro::TYPE) && node.GetName() == "Реальные резьбы") {
				topPart.Remove(node);
			}
		}
		
		NodeMacro macro = topPart.Create<NodeMacro>(true, "Реальные резьбы");
		for (Node& node : topPart.GetNodes()) {
			if (node.IsType(ThreadDesignation::TYPE)) {
				CreateThread(topPart, macro, ThreadDesignation(node));
			}
		}
		macro.Update();
	}

private:
	bool CreateThread(Part& part, NodeMacro& macro, const ThreadDesignation& thread) {
		Edge beginEdge = thread.GetBeginEdge();

    Face planarFace = beginEdge.RightFace();
    Face cylinderFace = beginEdge.LeftFace();
    if (!planarFace.IsPlanar()) {
        planarFace = beginEdge.LeftFace();
        cylinderFace = beginEdge.RightFace();
    }
    if (!planarFace.IsPlanar() || !cylinderFace.IsCylinder()) return false;
    // TODO: Get cx,cy,cz from beginEdge
    ParallelPlane plt = part.Create<ParallelPlane>(planarFace, beginEdge.GetBeginVertex());
    ConeAxis axis = part.Create<ConeAxis>(cylinderFace);
    macro.Add(plt).Add(axis);
    
    return true;
	}
};

#endif
