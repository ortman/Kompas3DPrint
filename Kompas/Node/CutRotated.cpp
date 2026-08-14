/*
 * #include "../Kompas3D.h"
 * #include "CutRotated.h"
 * 
 * CutRotated::CutRotated(IUnknown* pE, IDispatch* pD, Sketch& sketch, const Axis& axis, const std::optional<std::string>& name) : Node(pE, pD) {
 * 	sketch.EndEdit();
 * 	K5::ksEntityPtr entity = pEntity;
 * 	if (name.has_value()) entity->name = Utf8ToCp1251(name.value()).c_str();
 * 	K5::ksCutRotatedDefinitionPtr def = pDefinition;
 * 	if (!def) throw Kompas3DException("Не могу получить CutRotatedDefinition, entityType=" + std::to_string(entity->type));
 * 	K5::ksEntityPtr sketchEntity = sketch.pEntity;
 * 	if (!sketchEntity) throw Kompas3DException("Не могу получить Эскиз");
 * 	def->SetSketch(sketchEntity);
 * 	def->cut = true;
 * 	def->directionType = KConst3D::dtNormal;
 * 	def->toroidShapeType = true;
 * 	def->SetSideParam(true, 360.);
 * 	K5::ksRotatedParamPtr param = def->RotatedParam();
 *   param->direction = KConst3D::dtNormal;
 *   param->angleNormal = 360.;
 *   param->angleReverse = 0.;
 *   param->toroidShape = true;
 * 	
 * 	K7::ICutRotatedPtr iRotated = Kompas3D::ToApi7<K7::ICutRotatedPtr>(pEntity);
 *   iRotated->Axis = Kompas3D::ToApi7<K7::IAxis3DPtr>(axis.pEntity);
 * 	
 * 	entity->Create();
 * }
 * 
 * int CutRotated::TYPE = KConst3D::o3d_cutRotated;
 */
