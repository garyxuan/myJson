//
//  myJson.cpp
//  myJson
//
//  Created by garyxuan on 2024/7/16.
//
#include "myJson.hpp"

namespace myJson
{
    // JsonValue模版类
    template <JsonValueType Tag, typename T>
    class Value : public JsonValue
    {
    protected:
        T m_value;

        // 不允许在外部调用
        explicit Value(const T &value)
            : m_value(value){};
        explicit Value(T &&value)
            : m_value(std::move(value)){};

        JsonValueType type() const override
        {
            return Tag;
        }

        bool equals(const JsonValue *other) const override
        {
            return m_value == static_cast<const Value<Tag, T> *>(other)->m_value;
        };

        bool less(const JsonValue *other) const override
        {
            return m_value < static_cast<const Value<Tag, T> *>(other)->m_value;
        };

        double getNumber() const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        bool getBool() const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        const std::string &getString() const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        const array &getArray() const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        const object &getObject() const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        Json &operator[](size_t index) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        Json &operator[](const std::string &key) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        const Json &operator[](size_t index) const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        const Json &operator[](const std::string &key) const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        void setNumber(double value) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        void setBool(bool value) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        void setString(const std::string &value) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        void setArray(const array &vallue) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        void setObject(const object &value) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        void addToArray(const Json &value) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        void addToObject(const std::string &key, const Json &value) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        void removeFromArray(size_t index) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        void removeFromObject(const std::string &key) override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }

        arrayiter arrayBegin() override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }
        const_arrayiter const_arrayBegin() const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }
        arrayiter arrayEnd() override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }
        const_arrayiter const_arrayEnd() const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }
        objectiter objectBegin() override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }
        const_objectiter const_objectBegin() const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }
        objectiter objectEnd() override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }
        const_objectiter const_objectEnd() const override
        {
            THROW_INVALID_TYPE_EXCEPTION(type());
        }
    };

    // 自定义一个NULL的类型
    class NullClass
    {
    public:
        bool operator==(const NullClass &) const { return true; }
        bool operator<(const NullClass &) const { return false; }
    };

    class JsonNull : public Value<JsonValueType::NUL, NullClass>
    {
    public:
        explicit JsonNull()
            : Value(NullClass()){};

    private:
        std::unique_ptr<JsonValue> clone() const override
        {
            return std::make_unique<JsonNull>();
        }

        void dump(std::string &str, size_t depth) const override
        {
            str += "null";
        }
    };
    class JsonNumber : public Value<JsonValueType::NUMBER, double>
    {
    public:
        explicit JsonNumber(double value)
            : Value(value){};

    private:
        double getNumber() const override
        {
            return m_value;
        }
        void setNumber(double value) override
        {
            m_value = value;
        }
        std::unique_ptr<JsonValue> clone() const override
        {
            return std::make_unique<JsonNumber>(m_value);
        }

        void dump(std::string &str, size_t depth) const override
        {
            str += std::to_string(m_value);
        }
    };

    class JsonBool : public Value<JsonValueType::BOOL, bool>
    {
    public:
        explicit JsonBool(bool value)
            : Value(value){};

    private:
        bool getBool() const override
        {
            return m_value;
        }
        void setBool(bool value) override
        {
            m_value = value;
        }
        std::unique_ptr<JsonValue> clone() const override
        {
            return std::make_unique<JsonBool>(m_value);
        }
        void dump(std::string &str, size_t depth) const override
        {
            str += (m_value ? "true" : "false");
        }
    };

    class JsonString : public Value<JsonValueType::STRING, std::string>
    {
    public:
        explicit JsonString(const std::string &value)
            : Value(value)
        {
        }
        explicit JsonString(std::string &&value)
            : Value(std::move(value))
        {
        }

    private:
        const std::string &getString() const override
        {
            return m_value;
        }

        void setString(const std::string &value) override
        {
            m_value = value;
        }
        std::unique_ptr<JsonValue> clone() const override
        {
            return std::make_unique<JsonString>(m_value);
        }
        void dump(std::string &str, size_t depth) const override
        {
            str += "\"" + m_value + "\"";
        }
    };

    class JsonArray : public Value<JsonValueType::ARRAY, array>
    {
    public:
        explicit JsonArray(const array &value)
            : Value(value){};
        explicit JsonArray(array &&value)
            : Value(std::move(value)){};

    private:
        const array &getArray() const override
        {
            return m_value;
        }

        void setArray(const array &value) override
        {
            m_value = value;
        }

        void addToArray(const Json &value) override
        {
            m_value.emplace_back(value);
        }

        void removeFromArray(size_t index) override
        {
            if (index < m_value.size())
            {
                m_value.erase(m_value.begin() + index);
            }
            else
            {
                throw myJsonException(std::string(__func__) + "index out of range", 0);
            }
        }

        Json &operator[](size_t index) override
        {
            if (index < m_value.size())
            {
                return m_value[index];
            }
            else
            {
                throw myJsonException(std::string(__func__) + "index out of range", 0);
            }
        }

        const Json &operator[](size_t index) const override
        {
            if (index < m_value.size())
            {
                return m_value[index];
            }
            else
            {
                throw myJsonException(std::string(__func__) + "index out of range", 0);
            }
        }

        std::unique_ptr<JsonValue> clone() const override
        {
            return std::make_unique<JsonArray>(m_value);
        }

        void dump(std::string &str, size_t depth) const override
        {
            str += "[";
            bool first = true;
            for (const auto &item : m_value)
            {
                if (!first)
                {
                    str += ", ";
                }
                first = false;
                item.dump(str, depth + 1);
            }
            str += "]";
        }

        arrayiter arrayBegin() override { return m_value.begin(); }
        const_arrayiter const_arrayBegin() const override { return m_value.cbegin(); }
        arrayiter arrayEnd() override { return m_value.end(); }
        const_arrayiter const_arrayEnd() const override { return m_value.cend(); }
    };

    class JsonObject : public Value<JsonValueType::OBJECT, object>
    {
    public:
        explicit JsonObject(const object &value)
            : Value(value){};
        explicit JsonObject(object &&value)
            : Value(std::move(value)){};

    private:
        const object &getObject() const override
        {
            return m_value;
        }

        void setObject(const object &value) override
        {
            m_value = value;
        }

        void addToObject(const std::string &key, const Json &value) override
        {
            m_value[key] = value;
        }

        void removeFromObject(const std::string &key) override
        {
            auto iter = m_value.find(key);
            if (iter != m_value.end())
            {
                m_value.erase(iter);
            }
            else
            {
                throw myJsonException(std::string(__func__) + "key[" + key + "] not exists!", 0);
            }
        }

        Json &operator[](const std::string &key) override
        {
            return m_value[key];
        }

        const Json &operator[](const std::string &key) const override
        {
            auto iter = m_value.find(key);
            if (iter != m_value.end())
            {
                return iter->second;
            }
            else
            {
                throw myJsonException(std::string(__func__) + "key[" + key + "] not exists!", 0);
            }
        }

        std::unique_ptr<JsonValue> clone() const override
        {
            return std::make_unique<JsonObject>(m_value);
        }

        void dump(std::string &str, size_t depth) const override
        {
            bool first = true;
            for (const auto &item : m_value)
            {

                if (!first)
                {
                    str += ",\n";
                    for (size_t i = 0; i < depth; i++)
                    {
                        str += '\t';
                    }
                }
                else
                {
                    str += "{\n";
                }
                first = false;
                str += item.first + " : ";
                item.second.dump(str, depth + 1);
            }
            str += "\n}";
        };

        objectiter objectBegin() override { return m_value.begin(); }
        const_objectiter const_objectBegin() const override { return m_value.cbegin(); }
        objectiter objectEnd() override { return m_value.end(); }
        const_objectiter const_objectEnd() const override { return m_value.cend(); }
    };

    ///////////////json//////////////////////
    Json::Json() noexcept
        : m_ptr(std::make_unique<JsonNull>()) {}
    Json::Json(std::nullptr_t) noexcept
        : m_ptr(std::make_unique<JsonNull>()) {}
    Json::Json(int value)
        : m_ptr(std::make_unique<JsonNumber>(double(value))) {}
    Json::Json(double value)
        : m_ptr(std::make_unique<JsonNumber>(value)) {}
    Json::Json(bool value)
        : m_ptr(std::make_unique<JsonBool>(value)) {}
    Json::Json(const std::string &value)
        : m_ptr(std::make_unique<JsonString>(value)) {}
    Json::Json(std::string &&value)
        : m_ptr(std::make_unique<JsonString>(std::move(value))) {}
    Json::Json(const char *value)//这个std::string(value)是个临时对象 是右值
        : m_ptr(std::make_unique<JsonString>(std::string(value))) {}
    Json::Json(const array &value)
        : m_ptr(std::make_unique<JsonArray>(value)) {}
    Json::Json(array &&value)
        : m_ptr(std::make_unique<JsonArray>(std::move(value))) {}
    Json::Json(const object &value)
        : m_ptr(std::make_unique<JsonObject>(value)) {}
    Json::Json(object &&value)
        : m_ptr(std::make_unique<JsonObject>(std::move(value))) {}

    Json::Json(const Json &other)
        : m_ptr(other.m_ptr ? other.m_ptr->clone() : std::make_unique<JsonNull>()) {}
    Json::Json(Json &&other) noexcept
        : m_ptr(std::move(other.m_ptr)) {}

    Json::~Json() noexcept {}

    JsonValueType Json::type() const
    {
        check();
        return m_ptr->type();
    }

    bool Json::is_null() const
    {
        check();
        return m_ptr->type() == JsonValueType::NUL;
    }

    bool Json::is_number() const
    {
        check();
        return m_ptr->type() == JsonValueType::NUMBER;
    }

    bool Json::is_bool() const
    {
        check();
        return m_ptr->type() == JsonValueType::BOOL;
    }

    bool Json::is_string() const
    {
        check();
        return m_ptr->type() == JsonValueType::STRING;
    }

    bool Json::is_array() const
    {
        check();
        return m_ptr->type() == JsonValueType::ARRAY;
    }

    bool Json::is_object() const
    {
        check();
        return m_ptr->type() == JsonValueType::OBJECT;
    }

    double Json::getNumber() const
    {
        check();
        return m_ptr->getNumber();
    }

    bool Json::getBool() const
    {
        check();
        return m_ptr->getBool();
    }

    const std::string &Json::getString() const
    {
        check();
        return m_ptr->getString();
    };

    const array &Json::getArray() const
    {
        check();
        return m_ptr->getArray();
    };

    const object &Json::getObject() const
    {
        check();
        return m_ptr->getObject();
    };

    void Json::setNumber(double value)
    {
        check();
        m_ptr->setNumber(value);
    }

    void Json::setBool(double value)
    {
        check();
        m_ptr->setBool(value);
    }

    void Json::setString(const std::string &value)
    {
        check();
        m_ptr->setString(value);
    }

    void Json::setArray(const array &value)
    {
        check();
        m_ptr->setArray(value);
    }

    void Json::setObject(const object &value)
    {
        check();
        m_ptr->setObject(value);
    }

    void Json::addToArray(const Json &value)
    {
        check();
        m_ptr->addToArray(value);
    }

    void Json::addToObject(const std::string &key, const Json &value)
    {
        check();
        m_ptr->addToObject(key, value);
    }

    void Json::removeFromArray(size_t index)
    {
        check();
        m_ptr->removeFromArray(index);
    }

    void Json::removeFromObject(const std::string &key)
    {
        check();
        m_ptr->removeFromObject(key);
    }

    Json &Json::operator[](size_t index)
    {
        return (*m_ptr)[index];
    }

    Json &Json::operator[](const std::string &key)
    {
        return (*m_ptr)[key];
    }

    const Json &Json::operator[](size_t index) const
    {
        return (*m_ptr)[index];
    }

    const Json &Json::operator[](const std::string &key) const
    {
        return (*m_ptr)[key];
    }

    Json &Json::operator=(const Json &other)
    {
        if (this != &other) // 防止自赋值
        {
            m_ptr = other.m_ptr ? other.m_ptr->clone() : std::make_unique<JsonNull>();
        }
        return *this;
    }

    Json &Json::operator=(Json &&other) noexcept
    {
        if (this != &other) // 防止自赋值
        {
            m_ptr = std::move(other.m_ptr);
        }
        return *this;
    }

    bool Json::operator==(const Json &other) const
    {
        check();
        if (m_ptr == other.m_ptr)
            return true;
        else if (type() != other.type())
            return false;
        else
            return m_ptr->equals(other.m_ptr.get());
    }

    bool Json::operator<(const Json &other) const
    {
        check();
        if (m_ptr == other.m_ptr)
            return false;
        else if (m_ptr->type() != other.m_ptr->type())
            return m_ptr->type() < other.m_ptr->type();
        else
            return m_ptr->less(other.m_ptr.get());
    }

    const std::string Json::dump() const
    {
        std::string str;
        dump(str, 0);
        return str;
    }

    void Json::dump(std::string &str, size_t depth) const
    {
        check();
        m_ptr->dump(str, depth);
    }

    arrayiter Json::arrayBegin()
    {
        check();
        return m_ptr->arrayBegin();
    }

    const_arrayiter Json::const_arrayBegin() const
    {
        check();
        return m_ptr->const_arrayBegin();
    }

    arrayiter Json::arrayEnd()
    {
        check();
        return m_ptr->arrayEnd();
    }

    const_arrayiter Json::const_arrayEnd() const
    {
        check();
        return m_ptr->const_arrayEnd();
    }

    objectiter Json::objectBegin()
    {
        check();
        return m_ptr->objectBegin();
    }

    const_objectiter Json::const_objectBegin() const
    {
        check();
        return m_ptr->const_objectBegin();
    }

    objectiter Json::objectEnd()
    {
        check();
        return m_ptr->objectEnd();
    }

    const_objectiter Json::const_objectEnd() const
    {
        check();
        return m_ptr->const_objectEnd();
    }

    void checkIndex(const std::string &str, size_t index)
    {
        if (index >= str.size())
        {
            throw myJsonException("Unexpected end", index);
        }
    };

    //  去空格
    void parseWhiteSpace(const std::string &str, size_t &index)
    {
        while (index < str.size() && (str[index] == ' ' || str[index] == '\r' || str[index] == '\n' || str[index] == '\t'))
        {
            index++;
        }
    }

    Json parseLiteral(const std::string &literal, Json target, const std::string &str, size_t &index)
    {
        if (str.compare(index, literal.length(), literal) == 0)
        {
            index += literal.length();
            return target;
        }
        else
        {
            throw myJsonException("[ERROR]: expected (" + literal + ") got (" + str.substr(index, literal.length()) + ")", index);
        }
    };

    // parse string
    Json parseString(const std::string &str, size_t &index)
    {
        std::string out;
        index++; // 跳过起点
        while (1)
        {
            if (index == str.size())
                throw myJsonException("Unexpected end", index);

            if (str[index] == '\"') //  字符串终点
            {
                break;
            }
            else if (str[index] == '\\') // 转义字符
            {
                index++;
                switch (str[index])
                {
                case '\"':
                    out += '\"';
                    break;
                case '\\':
                    out += '\\';
                    break;
                case '/':
                    out += '/';
                    break;
                case 'b':
                    out += '\b';
                    break;
                case 'f':
                    out += '\f';
                    break;
                case 'n':
                    out += '\n';
                    break;
                case 'r':
                    out += '\r';
                    break;
                case 't':
                    out += '\t';
                    break;
                default:
                    throw myJsonException("unkown sequence:" + std::string(1, str[index - 1]) + std::string(1, str[index]), index);
                    break;
                }
            }
            else
            {
                out += (str[index]);
            }
            index++;
        }
        index++;
        return Json(out);
    }

    Json parseNumber(const std::string &str, size_t &index)
    {
        size_t processed = 0;
        double value = 0;
        try
        {
            // 尝试转成double
            value = std::stod(str.substr(index), &processed);
        }
        catch (const std::invalid_argument &)
        {
            throw myJsonException("Invalid number format!", index);
        }
        catch (const std::out_of_range &)
        {
            throw myJsonException("Number out of range!", index);
        }
        // if we get here ,processed must be more than 0
        index += processed;
        return Json(value);
    }

    // 声明一下
    Json parseJson(const std::string &in, size_t &index, size_t depth);

    Json parseArray(const std::string &str, size_t &index, size_t depth)
    {
        array out;
        parseWhiteSpace(str, index);
        checkIndex(str, index);
        if (str[index] == ']')
            return Json(out);
        while (1)
        {
            try
            {
                Json test = parseJson(str, index, depth); // 值直接作为json解析
                out.emplace_back(test);
            }
            catch (const myJsonException &e)
            {
                throw myJsonException("[ERROR] array parse wrong, " + std::string(e.what()), index);
            }
            parseWhiteSpace(str, index);
            checkIndex(str, index);

            if (str[index] == ']')
                break;
            if (str[index] != ',')
                throw myJsonException("[ERROR] array format wrong", index);
            index++;
        }
        index++;
        return Json(out);
    }

    Json parseObject(const std::string &str, size_t &index, size_t depth)
    {
        object out;
        parseWhiteSpace(str, index);
        checkIndex(str, index);
        if (str[index] == '}')
            return Json(out);
        while (1)
        {
            try
            {
                parseWhiteSpace(str, index);
                checkIndex(str, index);

                std::string key = parseString(str, index).getString(); // 先解析key

                parseWhiteSpace(str, index);
                checkIndex(str, index);

                if (str[index] != ':') // 必须是冒号 后面跟value
                {
                    char _str[1] = {0};
                    snprintf(_str, 1, "%c", str[index]);
                    throw myJsonException("[ERROR]: object parsing, expect ':', got " + std::string(_str), index);
                }
                index++;

                parseWhiteSpace(str, index);
                checkIndex(str, index);

                Json value = parseJson(str, index, depth); // value作为json解析
                out.emplace(key, value);
            }
            catch (const myJsonException &e)
            {
                throw myJsonException("[ERROR] object parse wrong, " + std::string(e.what()), index);
            }

            parseWhiteSpace(str, index);
            checkIndex(str, index);

            if (str[index] == '}')
                break;
            if (str[index] != ',')
                throw myJsonException("[ERROR] object format wrong", index);
            index++;
        }
        index++;
        return Json(out);
    }

    // index解析开始的位置 depth深度
    Json parseJson(const std::string &in, size_t &index, size_t depth)
    {
        if (depth > MAXDEPTH)
        {
            throw myJsonException("exceeded maxinum nesting depth", 0);
        }
        parseWhiteSpace(in, index);
        checkIndex(in, index);
        if (in[index] == 'n') // null
        {
            return parseLiteral("null", Json(nullptr), in, index);
        }
        else if (in[index] == 't') // true
        {
            return parseLiteral("true", Json(true), in, index);
        }
        else if (in[index] == 'f') // false
        {
            return parseLiteral("false", Json(false), in, index);
        }
        else if (in[index] == '\"') // start of string
        {
            return parseString(in, index);
        }
        else if (in[index] == '[') // start of array
        {
            return parseArray(in, ++index, ++depth);
        }
        else if (in[index] == '{') // start of object
        {
            return parseObject(in, ++index, ++depth);
        }
        else
        {
            return parseNumber(in, index);
        }
    }

    // json parse
    Json parse(const std::string &in)
    {
        size_t index = 0;
        size_t depth = 0;
        return parseJson(in, index, depth);
    }
}
