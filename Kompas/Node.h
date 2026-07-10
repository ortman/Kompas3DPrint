#ifndef _ComTest_Node_h_
#define _ComTest_Node_h_

#include <unknwn.h>
#include <string>
#include <exception>
#include <optional>
//#include <memory>

class Kompas3DException : public std::exception {
private:
    std::string message;
public:
    Kompas3DException(std::string msg) : message(std::move(msg)) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class Node {
protected:
	IUnknown* pEntity;

public:
	static int TYPE;
	Node(IUnknown* pEntity);
	virtual ~Node();
	IUnknown* GetEntity() const { return pEntity; }
	int GetType() const;
	bool IsType(int type) const { return GetType() == type; }
	std::string GetName() const;
	Node& SetName(const std::string& name);
	Node& Update();
	static std::string Utf8ToCp1251(const std::string& utf8Str);
	static std::string Cp1251ToUtf8(const char* cp1251Str);
	operator bool() const { return pEntity; }
};
#endif
