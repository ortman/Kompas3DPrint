#ifndef _Kompas3DPrint_Process3D_h_
#define _Kompas3DPrint_Process3D_h_

#include "Node.h"
#include "Part.h"
#include "Panel.h"

enum MateType : int {
	MateCoincidence   = 0,  // совпадение объектов
	MateParallel      = 1,  // параллельность
	MatePerpendicular = 2,  // перпендикулярность
	MateTangency      = 3,  // касательность
	MateConcentric    = 4,  // концентричность
	MateDistance      = 5,  // постоянное расстояние между объектами
	MateAngle         = 6,  // постоянный угол между объектами
	MateInPlace       = 7,  // создание компонента "на месте"
	MateTransmission  = 9,  // Механическая передача
	MateCamGear       = 10, // Кулачковый механизм. Кулачек-толкатель
	MateSymmetric     = 11, // Симметрия
	MateDependent     = 14  // Зависимое положение
};

enum MateDir : int {
	MateDirUndefined  = 0,  // направление не учитывается
	MateDirSame       = 1,  // объекты однонаправленные
	MateDirOpposite   = -1  // объекты разнонаправленные
};

enum MateFixed : int {
	MateFixedNone     = 0,  // детали не фиксируются
	MateFixedFirst    = 1,  // фиксируется первая деталь
	MateFixedSecond   = 2   // фиксируется вторая деталь
};

class MateConstraint {
private:
	IUnknown* mate;
	MateType type;
	MateDir dir;
	MateFixed fixed;
	Node first;
	Node second;
	double value;
public:
	MateConstraint() : mate(nullptr), first(nullptr), second(nullptr) {}
	MateConstraint(IUnknown* mate, MateType type, MateDir dir, MateFixed fixed, const Node& first, const Node& second, double value);
	MateConstraint(const MateConstraint& m);
	~MateConstraint();
	MateConstraint& SetType(MateType type);
	MateConstraint& SetDir(MateDir dir);
	MateConstraint& SetFixed(MateFixed fixed);
	MateConstraint& SetFirst(const Node& node);
	MateConstraint& SetSecond(const Node& node);
	Node& GetFirst() { return first; }
	Node& GetSecond() { return second; }
	operator bool() const { return mate; }
	friend class Doc3D;
};

class Doc3D;
class Process3DNotifyLoc;
class KProcess3D : public Panel {
protected:
	Doc3D* doc = nullptr;
	IUnknown* pProc3D = nullptr;
	Process3DNotifyLoc* procEvent = nullptr;

	void Init(Doc3D* doc);
	
  bool hasPlacementChangeMethod = false;
  bool hasFilterObjectMethod = false;
  
	virtual bool OnPlacementChange(const Node& node) { return false; }
	virtual bool OnFilterObject(const Node& node) { return false; }

public:
	KProcess3D() : Panel("") {}
	KProcess3D(const KProcess3D& proc) = delete; // Конструктор копирования
  KProcess3D& operator=(const KProcess3D& proc) = delete; // Оператор копирующего присваивания
	KProcess3D(KProcess3D&& proc) noexcept = delete; // Конструктор перемещения
	KProcess3D& operator=(KProcess3D&& proc) noexcept = delete; // Оператор перемещающего присваивания
	virtual ~KProcess3D();
	bool Run(bool prop, bool cmd);
	bool Stop();
	void SetPhantom(const Part& part);
	Part GetPhantom();
	MateConstraint AddMateConstraint(MateType type, const Node& object1, const Node& object2, MateDir direction, MateFixed fixed = MateFixedNone, double value = 0.0);
	void SetCaption(const std::string& caption);
	bool Create() { return false; }
	void Update();
	void Show(bool isShow = true) {}
	void Hide() { Show(false); }
	
	friend class Doc3D;
	friend class Process3DNotifyLoc;
};

template <typename T>
concept CheckOnPlacementChange = requires(T a, const Node& node) { { a.OnPlacementChange(node) } -> std::same_as<bool>; };
template <typename T>
concept CheckOnFilterObject    = requires(T a, const Node& node) { { a.OnFilterObject(node)    } -> std::same_as<bool>; };

template <typename ProcClass>
class Process3D : public KProcess3D {
public:
	Process3D() {
		if constexpr (CheckOnPlacementChange<ProcClass>) hasPlacementChangeMethod = true;
		if constexpr (CheckOnFilterObject<ProcClass>) hasFilterObjectMethod = true;
	}
};

#endif
