#ifndef _ComTest_Node_h_
#define _ComTest_Node_h_

#include <string>
#include <exception>
#include <optional>

class Kompas3DException : public std::exception {
protected:
    std::string message;
public:
    Kompas3DException(std::string msg) : message(std::move(msg)) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class Node {
protected:
	class K3D_Node {
	public:
		virtual int GetType() const;
		virtual std::string GetName() const;
		virtual void SetName(const std::string& name);
		virtual void Update();
		virtual operator bool() const;
	};

public:
	K3D_Node p;
	
	static int TYPE;
	Node(const K3D_Node& p) : p(p) {}
	Node(const Node& node) : Node(node.p) {}
	virtual ~Node();
	int GetType() const { return p.GetType(); }
	bool IsType(int type) const { return p.GetType() == type; }
	std::string GetName() const { return p.GetName(); }
	Node& SetName(const std::string& name) { p.SetName(name); return *this; }
	Node& Update() { p.Update(); return *this; }
	static std::string Utf8ToCp1251(const std::string& utf8Str);
	static std::string Cp1251ToUtf8(const char* cp1251Str);
	operator bool() const { return p; }
	Node& operator=(const Node& other);
};

#endif
