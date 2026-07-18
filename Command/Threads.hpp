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
    // Базовая плоскость спирали
    ParallelPlane plt = part.Create<ParallelPlane>(planarFace, beginEdge.GetBeginVertex());
    // Создаём спираль
    CylindricSpiral helix = part.Create<CylindricSpiral>();
    //helix = topPart.NewEntity(const3d.o3d_cylindricSpiral);
    //helix.name = "Спираль_{0}".format(opNum)
    helix.SetDiam(thread.GetDiameter())
         .SetStep(thread.GetPitch())
         .SetHeight(thread.GetLength())
         .TurnDir(thread.IsForwardDir())
         .BuildDir(false)
         .BuildMode(CylindricSpiral::ModeStepHeight)
         .SetPlane(plt)
         .SetLocation(0, 0)
         .Create();
    //surfPl = plt.GetSurface().GetSurfaceParam().GetPlacement();
    //_, loc_x, loc_y = surfPl.PointProjection(cx, cy, cz);
    //_, x0, y0 = surfPl.PointProjection(0, 0, 0);
    //helix.SetPlane(plt);
    //helix.SetLocation(loc_x - x0, loc_y - y0);
    
    // Ось резьбы
    ConeAxis axis = part.Create<ConeAxis>(cylinderFace);
    
    // Плоскость эскиза
    Vertex startVertex = helix.GetBeginVertex();
    EdgePointPlane sketchPlane = part.Create<EdgePointPlane>(axis, startVertex);
    
    // TODO: Профиль резьбы
    
    // TODO: Фаска резьбы
    
    macro.Add(plt)
         .Add(helix)
         .Add(axis)
         .Add(sketchPlane);
    
    return true;
	}
};

#endif
