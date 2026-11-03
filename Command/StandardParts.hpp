#ifndef _Kompas3DPrint_StandardParts_hpp_
#define _Kompas3DPrint_StandardParts_hpp_

#include <CtrlLib/CtrlLib.h>
#include "../Resources.h"
#include "../KompasAPI/Include/Kompas3D.h"

// Выбранное изделие: модель, исполнение и значения внешних переменных
struct StandardPartChoice {
	Doc3D doc;           // модель, открытая невидимой
	String path;         // файл модели — из него вставляются компоненты
	String title;        // «DIN912 M3, L=10» — для кнопки на панели
	int embodiment = -1; // индекс исполнения КОМПАС
	VectorMap<String, double> params;
};

// Окно выбора изделия. Исполнение и параметры выбираются здесь, а не на панели процесса:
// элементы, добавленные на панель работающего процесса, КОМПАС не показывает без перезапуска
// процесса и не отдаёт их значения, а смена фантома у процесса его ломает
class StandardPartsSelector : public WithStandardPartsModelsLay<TopWindow>  {
private:
	Doc3D doc;             // модель, выбранная в дереве
	String docPath;
	Vector<String> paramVars; // переменная каждой строки параметров

	// Размер из имени исполнения: «M2.5» → 2.5
	static double EmbodimentSize(const String& name) {
		const char* p = name;
		while (*p && !IsDigit(*p)) ++p;
		return *p ? ScanDouble(p) : Null;
	}

	// КОМПАС хранит исполнения не по порядку (базовое всегда последнее), поэтому сортируем по размеру
	static bool EmbodimentLess(const String& a, const String& b) {
		double sa = EmbodimentSize(a);
		double sb = EmbodimentSize(b);
		if (!IsNull(sa) && !IsNull(sb) && sa != sb) return sa < sb;
		return a < b;
	}

	void CloseModel() {
		if (doc) doc.Close();
		doc = Doc3D();
		docPath = Null;
		embodiment.Clear();
		params.Clear();
		paramVars.Clear();
	}

	bool CanAccept() const {
		return doc && !IsNull(~embodiment);
	}

	void OpenModel() {
		String path = GetSelectedPath();
		if (path == docPath) return;
		CloseModel();
		if (!IsNull(path)) {
			try {
				doc = std::move(Kompas3D::Open3D(path.ToStd(), false));
			} catch (const Kompas3DException& e) {
				Kompas3D::Error(e.what());
			}
			docPath = path;
			if (doc) {
				Vector<String> names;
				for (int i = 0; i < doc.GetEmbodimentsCount(); ++i) {
					names.Add(doc.GetEmbodimentName(i).c_str());
				}
				for (int i : GetSortOrder(names, &StandardPartsSelector::EmbodimentLess)) {
					embodiment.Add(i, names[i]);
				}
				if (embodiment.GetCount()) embodiment.SetIndex(0);
				LoadParams();
			}
		}
		bOk.Enable(CanAccept());
	}

	// Внешние переменные исполнения. Комментарий «Длина:4,5,6» задаёт название и список значений
	void LoadParams() {
		params.Clear();
		paramVars.Clear();
		if (!CanAccept()) return;
		doc.SetCurrentEmbodiment(~embodiment);
		doc.Rebuild();
		Part top = doc.GetTopPart();
		if (!top) return;
		for (const Part::Variable& v : top.GetVariables(true)) {
			String name = v.name;
			Vector<String> values;
			Vector<String> nameVal = Split(v.comment.c_str(), ':');
			if (nameVal.GetCount() == 2) {
				name = nameVal[0] + "(" + name + ")";
				values = Split(nameVal[1], ',');
			} else {
				values = Split(v.comment.c_str(), ',');
				if (values.GetCount() == 1) {
					values.Clear();
					name = String(v.comment) + "(" + name + ")";
				}
			}
			int row = params.GetCount();
			params.Add(name, v.value);
			paramVars.Add(v.name);
			if (values.GetCount()) {
				DropList& list = params.CreateCtrl<DropList>(row, 1);
				for (const String& s : values) {
					double d = StrDbl(TrimBoth(s));
					if (!IsNull(d)) list.Add(d, FormatDouble(d));
				}
				if (list.FindKey(v.value) < 0) list.Add(v.value, FormatDouble(v.value));
				list <<= v.value;
			} else {
				params.CreateCtrl<EditDouble>(row, 1) <<= v.value;
			}
		}
	}

	void LoadTree(int parent, const String& folder) {
		FindFile ff;
		if (ff.Search(AppendFileName(folder, "*"))) {
			int count = 0;
			do {
				if (ff.IsFile()) {
					if (ToLower(GetFileExt(ff.GetName())) == ".m3d") {
						String name = ff.GetName();
						name.TrimLast(4);
						models.Add(parent, CtrlImg::File(), ff.GetPath(), name);
						++count;
					}
				} else if (ff.IsFolder()) {
					if (ff.GetName() != "." && ff.GetName() != "..") {
						int p = models.Add(parent, CtrlImg::Dir(), Null, ff.GetName());
						LoadTree(p, ff.GetPath());
						++count;
					}
				}
			} while (ff.Next());
			models.Open(parent, count <= 1);
		}
	}

	String GetSelectedPath() {
		Vector<int> sel = models.GetSel();
		if (sel.GetCount() == 1 && sel[0]) {
			Value e = models.Get(sel[0]);
			if (!e.IsNull()) return e;
		}
		return Null;
	}

public:
	StandardPartsSelector() {
		CtrlLayout(*this, t_("Select model"));
		Zoomable().Sizeable();
		params.AddColumn(t_("Параметр"));
		params.AddColumn(t_("Значение"));
		bOk.Ok();
		Acceptor(bOk, IDOK);
		bCancel.Cancel();
		Rejector(bCancel, IDCANCEL);
		models.WhenSel = [=]() { OpenModel(); };
		models.WhenLeftDouble = [=]() {
			if (CanAccept()) AcceptBreak(IDOK);
		};
		embodiment.WhenAction = [=]() { LoadParams(); };
	}

	~StandardPartsSelector() {
		CloseModel();
	}

	// Модель передаётся в choice открытой; закрывает её вызывающий
	bool Choose(StandardPartChoice& choice) {
		CloseModel();
		models.Clear();
		models.NoRoot();
		LoadTree(0, AppendFileName(GetDocumentsFolder(), "Стандартные изделия"));
		bOk.Disable();
		if (Execute() != IDOK || !CanAccept()) {
			CloseModel();
			return false;
		}
		choice.embodiment = ~embodiment;
		choice.title = GetFileTitle(docPath) + " " + AsString(embodiment.GetValue());
		for (int i = 0; i < params.GetCount() && i < paramVars.GetCount(); ++i) {
			double value = params.Get(i, 1);
			if (IsNull(value)) continue;
			choice.params.Add(paramVars[i], value);
			choice.title << ", " << paramVars[i] << "=" << FormatDouble(value);
		}
		choice.path = docPath;
		choice.doc = std::move(doc);
		doc = Doc3D();
		CloseModel();
		return true;
	}
};

class InsertModelProc : public Process3D<InsertModelProc> {
public:
	struct : Panel::Tab {
		PropertyButton model {"Выберите модель"};
	} mainTab {"Основные"};

	MateConstraint planeMate;
	MateConstraint axisMate;

	bool OnPlacementChange(Node&& node) override {
		if (node.GetType() != Face::TYPE) return false;
		Face face(std::move(node));
		if (face.IsPlanar()) {
			if (!planeMate) {
				Part phantom = GetPhantom();
				planeMate = AddMateConstraint(MateCoincidence,
					phantom ? Node(phantom.GetPlaneXOY()) : Node(nullptr), Node(nullptr), MateDirSame);
			} else if (!planeMate.GetFirst() && GetPhantom()) {
				planeMate.SetFirst(GetPhantom().GetPlaneXOY());
			}
			planeMate.SetSecond(std::move(face));
			return true;
		}
		if (face.IsCylinder()) {
			if (!axisMate) {
				Part phantom = GetPhantom();
				axisMate = AddMateConstraint(MateConcentric,
					phantom ? Node(phantom.GetAxisOZ()) : Node(nullptr), Node(nullptr), MateDirUndefined);
			} else if (!axisMate.GetFirst() && GetPhantom()) {
				axisMate.SetFirst(GetPhantom().GetAxisOZ());
			}
			axisMate.SetSecond(std::move(face));
			return true;
		}
		return false;
	}

	bool OnFilterObject(Node&& node) override {
		if (node.GetType() != Face::TYPE) return false;
		Face face(std::move(node));
		return face.IsCylinder() || face.IsPlanar();
	}
};

// Изделие выбирается в окне до запуска процесса, фантом ставится до запуска. Процесс живёт
// до отмены: после вставки можно выбрать следующее отверстие, другое изделие меняет фантом
class StandardParts {
	StandardPartsSelector selector;
	InsertModelProc* proc = nullptr;
	Doc3D doc;
	// Выбранные изделия. Фантом и сопряжения ссылаются на их модели, поэтому закрываем при выходе из команды
	Array<StandardPartChoice> choices;
	StandardPartChoice* choice = nullptr;
	Part part; // фантом: деталь выбранного исполнения
	// После proc->Stop() КОМПАС присылает событие кнопки 2 — его нельзя принять за выход из команды
	int pendingStops = 0;

public:
	void Start() {
		try {
			StartProc();
		} catch (const Kompas3DException& e) {
			Kompas3D::Error(e.what());
		}
	}

private:
	void CloseDocs() {
		for (StandardPartChoice& c : choices) {
			if (c.doc) c.doc.Close();
		}
		choices.Clear();
		choice = nullptr;
	}

	// Объект живёт между вызовами команды — сбрасываем состояние прошлого вызова
	void Reset() {
		part = Part();
		CloseDocs();
		pendingStops = 0;
	}

	bool Choose() {
		StandardPartChoice& c = choices.Add();
		if (!selector.Choose(c)) {
			choices.Drop();
			return false;
		}
		choice = &c;
		// Значения параметров записываем в модель до того, как взять деталь исполнения для фантома
		Doc3D& model = c.doc;
		model.SetCurrentEmbodiment(c.embodiment);
		Part top = model.GetTopPart();
		for (int i = 0; i < c.params.GetCount(); ++i) {
			bool set = top && top.SetVariable(c.params.GetKey(i).ToStd(), c.params[i]);
			RLOG("StandardParts: set " << c.params.GetKey(i) << " = " << c.params[i] << ": " << set);
		}
		bool rebuilt = model.Rebuild();
		part = model.GetEmbodiment(c.embodiment);
		RLOG("StandardParts: " << c.title << ", rebuild " << rebuilt << ", part " << (bool)part);
		for (const Part::Variable& v : part.GetVariables(true)) {
			RLOG("StandardParts: phantom " << v.name << " = " << v.value);
		}
		return true;
	}

	void NewProcess() {
		proc =&doc.CreatePorcess<InsertModelProc>(); // прежний процесс удаляется здесь
		proc->SetCaption("Стандартные изделия");
		if (choice) proc->mainTab.model.SetName(choice->title.ToStd());
		SetupHandlers();
		if (part) proc->SetPhantom(part);
		bool run = proc->Run(false, false);
		RLOG("StandardParts: new process, phantom " << (bool)part << ", run " << run);
	}

	// Перезапуск процесса с текущим фантомом: при смене изделия и после вставки — без него
	// процесс перестаёт реагировать на выбор граней. Работает только из обработчика события
	// КОМПАС; новый процесс, созданный не из команды, получает «мёртвый» фантом
	void RestartWithPhantom() {
		++pendingStops;
		proc->Stop();
		proc->SetPhantom(part);
		bool run = proc->Run(false, true);
		// Сопряжения процесса ссылаются на плоскость и ось прежнего фантома
		if (proc->planeMate) proc->planeMate.SetFirst(part.GetPlaneXOY());
		if (proc->axisMate) proc->axisMate.SetFirst(part.GetAxisOZ());
		RLOG("StandardParts: restart with phantom, run " << run);
	}

	void SetupHandlers() {
		proc->mainTab.model.WhenClick = [=]() {
			try {
				if (!Choose()) return;
				if (choice) proc->mainTab.model.SetName(choice->title.ToStd());
				RestartWithPhantom();
			} catch (const Kompas3DException& e) {
				Kompas3D::Error(e.what());
			}
		};

		proc->WhenButtonClick = [=](int buttonId) {
			RLOG("StandardParts: button " << buttonId << ", plane " << (bool)proc->planeMate.GetSecond()
			     << ", axis " << (bool)proc->axisMate.GetSecond());
			if (buttonId == 1) {
				try {
					if (!part || !choice || !proc->planeMate.GetSecond() || !proc->axisMate.GetSecond()) {
						return false; // грани не выбраны — вставлять некуда
					}
					Node plane = std::move(proc->planeMate.GetSecond());
					Node axis = std::move(proc->axisMate.GetSecond());
					// Плоскость оставляем, а отверстие сбрасываем: фантом следует за указателем
					// по той же грани до выбора следующего отверстия. Убрать одно сопряжение API
					// не позволяет — удаляем все и создаём сопряжение с плоскостью заново.
					// Удаляем до вставки: DeleteTemporaryMateConstraints удаляет и сопряжения сборки,
					// созданные во время процесса
					bool cleared = proc->ClearMateConstraints();
					proc->planeMate = MateConstraint();
					proc->axisMate = MateConstraint();
					// Компонент из файла каталога штатной вставкой API7. Вставка детали другого документа
					// (AddPart → SetPartFromFile) работает, но сопряжения такого компонента появляются
					// в дереве только после переоткрытия сборки
					std::optional<Part::Placement> at = part.GetPlacement();
					if (Part newPart = doc.AddComponent(choice->path.ToStd(), choice->embodiment)) {
						for (int i = 0; i < choice->params.GetCount(); ++i) {
							newPart.SetVariable(choice->params.GetKey(i).ToStd(), choice->params[i]);
						}
						if (choice->params.GetCount()) newPart.Rebuild();
						// Компонент встаёт в начало координат — ставим его туда, где стоял фантом
						bool placed = at && newPart.SetPlacement(*at);
						bool planeOk = doc.AddMateConstraint(MateCoincidence, plane,
								newPart.GetPlaneXOY(), MateDirSame, MateFixedNone);
						bool axisOk = doc.AddMateConstraint(MateConcentric, axis,
								newPart.GetAxisOZ(), MateDirUndefined, MateFixedNone);
						RLOG("StandardParts: inserted, mates cleared " << cleared << ", placed " << placed
						     << ", plane mate " << planeOk << ", axis mate " << axisOk);
					} else {
						RLOG("StandardParts: AddComponent failed " << choice->path);
					}
					// Деталь, переданная в AddPart, фантомом больше не показывается — берём её заново
					part = choice->doc.GetEmbodiment(choice->embodiment);
					RestartWithPhantom();
					// Процесс запускается отложенно (Run с PostMessage): сопряжение, созданное до запуска,
					// фантом не держит, пока не выбрано отверстие. Создаём его после запуска и пересчитываем фантом
					if (plane) {
						auto face = std::make_shared<Node>(std::move(plane));
						InsertModelProc* p = proc;
						PostCallback([=]() {
							if (p != proc || proc->planeMate) return;
							try {
								Part phantom = proc->GetPhantom();
								if (!phantom) return;
								proc->planeMate = proc->AddMateConstraint(MateCoincidence,
									Node(phantom.GetPlaneXOY()), std::move(*face), MateDirSame);
								proc->Update();
								RLOG("StandardParts: plane restored " << (bool)proc->planeMate.GetSecond());
							} catch (const Kompas3DException& e) {
								Kompas3D::Error(e.what());
							}
						});
					}
				} catch (const Kompas3DException& e) {
					Kompas3D::Error(e.what());
				}
				// false — процесс не завершается: можно выбрать следующее отверстие и вставить снова
				return false;
			}
			// Событие от нашей же остановки процесса при смене изделия
			if (pendingStops > 0) {
				--pendingStops;
				return true;
			}
			// Выход из команды. КОМПАС завершает процесс после выхода из обработчика, а фантом —
			// деталь открытой модели: закрытие модели до этого портит сборку, и она падает при сохранении.
			// Поэтому всё освобождаем отложенно, когда процесс уже завершён
			InsertModelProc* p = proc;
			PostCallback([=]() {
				if (p == proc) Release();
			});
			return true;
		};
	}

	// Сначала объект процесса (отписка от событий, фантом), затем модели изделий
	void Release() {
		part = Part();
		proc = nullptr;
		doc = Doc3D();
		CloseDocs();
		pendingStops = 0;
		RLOG("StandardParts: released");
	}

	void StartProc() {
		//if (!Kompas3D::Connect()) return;
		doc = std::move(Kompas3D::GetActiveDocument3D()); // прежний процесс удаляется здесь
		proc = nullptr;
		Reset();
		if (!doc) return;
		if (!Choose()) return;
		NewProcess();
	}
};

#endif
