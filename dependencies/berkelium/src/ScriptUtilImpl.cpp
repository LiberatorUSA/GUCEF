#include "ScriptUtilImpl.hpp"

#include "base/utf_string_conversions.h"
#include "base/scoped_ptr.h"

namespace Berkelium {
namespace Script {

Value *toValue(const Variant &var) {
	switch (var.type()) {
	case Variant::JSSTRING:
	  {
		std::wstring str;
		var.toString().get(str);
		return Value::CreateStringValue(WideToUTF8(str));
	  }
	case Variant::JSBOOLEAN:
		return Value::CreateBooleanValue(var.toBoolean());
	case Variant::JSDOUBLE:
		return Value::CreateDoubleValue(var.toDouble());
	case Variant::JSNULL:
		return Value::CreateNullValue();
	default:
		return Value::CreateNullValue();
	}
}

bool toJSON(const Variant &var, std::string *outString) {
	std::string synchronous = "'asyncCall'";
	switch (var.type()) {
	case Variant::JSNULL:
	case Variant::JSDOUBLE:
	case Variant::JSSTRING:
	case Variant::JSBOOLEAN:
	{
		scoped_ptr<Value> toSerialize (toValue(var));
		JSONStringValueSerializer ser(outString);
		return ser.Serialize(*toSerialize);
	}
	case Variant::JSBINDSYNCFUNC:
		synchronous = "'syncCall'";
		// fall through;
	case Variant::JSBINDFUNC:
	{
		std::string serializedName;
		toJSON(Variant(var.toFunctionName()), &serializedName);
		*outString = "function(){return window.Berkelium(";
		*outString += synchronous + "," + serializedName;
		*outString += ", arguments);}";
		return true;
	}
	case Variant::JSEMPTYOBJECT:
		*outString = "{}";
		return true;
	case Variant::JSEMPTYARRAY:
		*outString = "[]";
		return true;
	default:
		return false;
	}
}

WideString BERKELIUM_EXPORT toJSON(const Variant &var) {
	std::string outString;
	if (!toJSON(var, &outString)) {
		return WideString::empty();
	}
	std::wstring wideString = UTF8ToWide(outString);
	wchar_t *buffer = new wchar_t[wideString.length() + 1];
	memcpy(buffer, wideString.data(), wideString.length() * sizeof(wchar_t));
	buffer[wideString.length()] = 0;

	return WideString::point_to(buffer, wideString.length());
}

void BERKELIUM_EXPORT toJSON_free(WideString returnedValue) {
	delete[] returnedValue.data();
}

Value *fromJSON(const std::string &utf8) {
	JSONStringValueSerializer deser(utf8);
	return deser.Deserialize(NULL, NULL);
}

bool BERKELIUM_EXPORT fromJSON(WideString str, Variant &out) {
	Value *value = fromJSON(WideToUTF8(str.get<std::wstring>()));
	if (!value) {
		out = Variant();
		return false;
	}
	return valueToVariant(value, out);
}

bool valueToVariant(Value *value, Variant &out) {
	switch (value->GetType()) {
	case Value::TYPE_BOOLEAN:
	{
		bool val = false;
		value->GetAsBoolean(&val);
		out = Variant(val);
		break;
	}
	case Value::TYPE_INTEGER:
	{
		int val = 0;
		value->GetAsInteger(&val);
		out = Variant(val);
		break;
	}
	case Value::TYPE_DOUBLE:
	{
		double val = 0.0;
		value->GetAsDouble(&val);
		out = Variant(val);
		break;
	}
	case Value::TYPE_STRING:
	{
		std::string val;
		value->GetAsString(&val);
		out = Variant(val.c_str());
		break;
	}
	case Value::TYPE_NULL:
		out = Variant();
		break;
	case Value::TYPE_DICTIONARY:
		out = Variant::emptyObject();
		return false;
	case Value::TYPE_LIST:
		out = Variant::emptyArray();
		return false;
	default:
		out = Variant();
		return false;
	}
	return true;
}

}
}

