#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>

// A minimal JSON value wrapper
struct JsonValue {
    enum Type { NULL_VAL, OBJECT, ARRAY, STRING, NUMBER, BOOL };
    Type type = NULL_VAL;
    
    std::map<std::string, JsonValue> object_val;
    std::vector<JsonValue> array_val;
    std::string string_val;
    double number_val = 0;
    bool bool_val = false;

    JsonValue() {}
    JsonValue(double d) : type(NUMBER), number_val(d) {}
    JsonValue(int i) : type(NUMBER), number_val(i) {}
    JsonValue(bool b) : type(BOOL), bool_val(b) {}
    JsonValue(std::string s) : type(STRING), string_val(s) {}
    JsonValue(const char* s) : type(STRING), string_val(s) {}

    static JsonValue object() { JsonValue v; v.type = OBJECT; return v; }
    static JsonValue array() { JsonValue v; v.type = ARRAY; return v; }

    JsonValue& operator[](const std::string& key) {
        if (type == NULL_VAL) type = OBJECT;
        return object_val[key];
    }
    
    JsonValue& operator[](size_t index) {
        if (type == NULL_VAL) type = ARRAY;
         if (index >= array_val.size()) array_val.resize(index + 1);
        return array_val[index];
    }

    void push_back(const JsonValue& v) {
        if (type == NULL_VAL) type = ARRAY;
        array_val.push_back(v);
    }
    
    std::string dump(int indent = 0) const {
        std::string s;
        std::string pad(indent, ' ');
        switch(type) {
            case NULL_VAL: return "null";
            case NUMBER: {
                std::string num = std::to_string(number_val);
                // Remove trailing zeros
                num.erase ( num.find_last_not_of('0') + 1, std::string::npos );
                if (num.back() == '.') num.pop_back();
                return num;
            }
            case BOOL: return bool_val ? "true" : "false";
            case STRING: return "\"" + string_val + "\"";
            case ARRAY:
                s = "[\n";
                for (size_t i = 0; i < array_val.size(); i++) {
                    s += pad + "  " + array_val[i].dump(indent + 2);
                    if (i < array_val.size() - 1) s += ",";
                    s += "\n";
                }
                s += pad + "]";
                return s;
            case OBJECT:
                s = "{\n";
                int i = 0;
                for (auto& kv : object_val) {
                    s += pad + "  \"" + kv.first + "\": " + kv.second.dump(indent + 2);
                    if (i < object_val.size() - 1) s += ",";
                    s += "\n";
                    i++;
                }
                s += pad + "}";
                return s;
        }
        return "";
    }
};

class JsonParser {
    std::string str;
    size_t pos;

    void skipWhitespace() {
        while (pos < str.length() && isspace(str[pos])) pos++;
    }

    std::string parseString() {
        std::string val;
        pos++; // skip "
        while (pos < str.length() && str[pos] != '"') {
            val += str[pos++];
        }
        pos++; // skip closing "
        return val;
    }

    double parseNumber() {
        size_t start = pos;
        while (pos < str.length() && (isdigit(str[pos]) || str[pos] == '.' || str[pos] == '-')) pos++;
        return std::stod(str.substr(start, pos - start));
    }

    bool parseBool() {
        if (str.substr(pos, 4) == "true") { pos += 4; return true; }
        if (str.substr(pos, 5) == "false") { pos += 5; return false; }
        return false;
    }

    JsonValue parseValue() {
        skipWhitespace();
        if (pos >= str.length()) return JsonValue();

        char c = str[pos];
        if (c == '{') return parseObject();
        if (c == '[') return parseArray();
        if (c == '"') return JsonValue(parseString());
        if (isdigit(c) || c == '-') return JsonValue(parseNumber());
        if (c == 't' || c == 'f') return JsonValue(parseBool());
        
        pos++; // skip unknown
        return JsonValue();
    }

    JsonValue parseObject() {
        JsonValue obj = JsonValue::object();
        pos++; // skip {
        while (true) {
            skipWhitespace();
            if (str[pos] == '}') { pos++; break; }
            
            std::string key = parseString();
            skipWhitespace();
            if (str[pos] == ':') pos++;
            
            obj.object_val[key] = parseValue();
            
            skipWhitespace();
            if (str[pos] == ',') pos++;
        }
        return obj;
    }

    JsonValue parseArray() {
        JsonValue arr = JsonValue::array();
        pos++; // skip [
        while (true) {
            skipWhitespace();
            if (str[pos] == ']') { pos++; break; }
            
            arr.push_back(parseValue());
            
            skipWhitespace();
            if (str[pos] == ',') pos++;
        }
        return arr;
    }

public:
    JsonValue parse(const std::string& input) {
        str = input;
        pos = 0;
        return parseValue();
    }
    
    static JsonValue parseFile(const std::string& filename) {
        std::ifstream f(filename);
        std::stringstream buffer;
        buffer << f.rdbuf();
        JsonParser parser;
        return parser.parse(buffer.str());
    }
};

#endif
