#include "../ComKompas.h"
#include "../Kompas3D.h"
#include "ThreadDesignation.h"

double ThreadDesignation::GetLength() const {
	K5::ksThreadDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition");
	return def->length;
}

double ThreadDesignation::GetPitch() const {
	K5::ksThreadDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition");
	return def->p;
}

double ThreadDesignation::GetDiameter() const {
	K5::ksThreadDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition");
	return def->dr;
}

bool ThreadDesignation::IsForwardDir() const {
	K5::ksThreadDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition");
	K7::IThreadPtr iThread = Kompas3D::ToApi7<K7::IThreadPtr>(pEntity);
	return iThread ? !iThread->LeftThread : true;
}

bool ThreadDesignation::IsOutside() const {
	K5::ksThreadDefinitionPtr def = pDefinition;
	if (!def) throw Kompas3DException("Не могу получить ksMeshCopyDefinition");
	return def->outside;
}

Edge ThreadDesignation::GetBeginEdge() const {
	K5::ksThreadDefinitionPtr def = pDefinition;
	K5::ksEntityPtr face = def->GetFaceBegin();
	//face.AddRef();
	return Edge(face);
}

Edge ThreadDesignation::GetEndEdge() const {
	K5::ksThreadDefinitionPtr def = pDefinition;
	K5::ksEntityPtr face = def->GetFaceEnd();
	//face.AddRef();
	return Edge(face);
}

int ThreadDesignation::TYPE = KConst3D::o3d_thread;
