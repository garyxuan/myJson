//
//  myJson.hpp
//  myJson
//
//  Created by garyxuan on 2024/7/16.
//
#pragma once
#include <string>
#include <vector>
#include <map>
#define MAXDEPTH 10 // object最大嵌套深度

#define THROW_INVALID_TYPE_EXCEPTION(type)         \
    const char *str_type = myJson::toString(type); \
    throw myJson::myJsonException("Invalid type: Attempted to call " + std::string(__func__) + " on a JsonValue of type " + std::string(str_type), 0)

namespace myJson
{
    class Json;
    using array = std::vector<Json>;
    using object = std::map<std::string, Json>;
    using arrayiter = array::iterator;
    using const_arrayiter = array::const_iterator;
    using objectiter = object::iterator;
    using const_objectiter = object::const_iterator;

    // 异常类
    class myJsonException : public std::runtime_error
    {
    public:
        myJsonException(const std::string &message, size_t position) : std::runtime_error(message), pos(position) {}
        size_t getPosition() const { return pos; }

    private:
        size_t pos;
    };

    // 值类型枚举
    enum class JsonValueType
    {
        NUL,    // 空
        NUMBER, // 数值
        BOOL,   // bool true/false
        STRING, // 字符串
        ARRAY,  // 数组
        OBJECT  // 对象
    };

    // JsonValue基础类类 定义接口函数
    class JsonValue
    {
    public:
        // get the type
        virtual JsonValueType type() const = 0;

        // compare
        virtual bool equals(const JsonValue *other) const = 0;
        virtual bool less(const JsonValue *other) const = 0;

        // get
        virtual double getNumber() const = 0;
        virtual bool getBool() const = 0;
        virtual const std::string &getString() const = 0;
        virtual const array &getArray() const = 0;
        virtual const object &getObject() const = 0;

        // operatot []
        virtual Json &operator[](size_t index) = 0;
        virtual Json &operator[](const std::string &key) = 0;
        virtual const Json &operator[](size_t index) const = 0;
        virtual const Json &operator[](const std::string &key) const = 0;

        // set
        virtual void setNumber(double value) = 0;
        virtual void setBool(bool value) = 0;
        virtual void setString(const std::string &value) = 0;
        virtual void setArray(const array &value) = 0;
        virtual void setObject(const object &value) = 0;

        // add
        virtual void addToArray(const Json &value) = 0;
        virtual void addToObject(const std::string &key, const Json &value) = 0;

        // remove
        virtual void removeFromArray(size_t index) = 0;
        virtual void removeFromObject(const std::string &key) = 0;

        // clone
        virtual std::unique_ptr<JsonValue> clone() const = 0;

        // dump
        virtual void dump(std::string &str, size_t depth) const = 0;

        // iter
        virtual arrayiter arrayBegin() = 0;
        virtual const_arrayiter const_arrayBegin() const = 0;
        virtual arrayiter arrayEnd() = 0;
        virtual const_arrayiter const_arrayEnd() const = 0;
        virtual objectiter objectBegin() = 0;
        virtual const_objectiter const_objectBegin() const = 0;
        virtual objectiter objectEnd() = 0;
        virtual const_objectiter const_objectEnd() const = 0;

        virtual ~JsonValue() noexcept {};
    };

    // Json类型
    class Json
    {
    private:
        std::unique_ptr<JsonValue> m_ptr;

    public:
        // constructor
        Json() noexcept;
        Json(std::nullptr_t) noexcept;
        Json(int value);
        Json(double value);
        Json(bool value);
        Json(const std::string &value);
        Json(std::string &&value);
        Json(const char *value);
        Json(const array &value);
        Json(array &&value);
        Json(const object &value);
        Json(object &&value);

        Json(const Json &other);
        Json(Json &&other) noexcept;

        ~Json() noexcept;

        // 判断类型
        JsonValueType type() const;
        bool is_null() const;
        bool is_number() const;
        bool is_bool() const;
        bool is_string() const;
        bool is_array() const;
        bool is_object() const;

        double getNumber() const;
        bool getBool() const;
        const std::string &getString() const;
        const array &getArray() const;
        const object &getObject() const;

        void setNumber(double value);
        void setBool(double value);
        void setString(const std::string &value);
        void setArray(const array &value);
        void setObject(const object &value);

        void addToArray(const Json &value);
        void addToObject(const std::string &key, const Json &value);
        void removeFromArray(size_t index);
        void removeFromObject(const std::string &key);

        // compare
        Json &operator[](size_t index);
        Json &operator[](const std::string &key);
        const Json &operator[](size_t index) const;
        const Json &operator[](const std::string &key) const;
        Json &operator=(const Json &other);
        Json &operator=(Json &&other) noexcept;

        bool operator==(const Json &other) const;
        bool operator<(const Json &other) const;
        // 下面的都是用上面的2个重载
        bool operator!=(const Json &other) const { return !(*this == other); };
        bool operator<=(const Json &other) const { return !(other < *this); };
        bool operator>(const Json &other) const { return other < *this; };
        bool operator>=(const Json &other) const { return !(*this < other); };

        const std::string dump() const;
        void dump(std::string &str, size_t depth) const;

        arrayiter arrayBegin();
        const_arrayiter const_arrayBegin() const;
        arrayiter arrayEnd();
        const_arrayiter const_arrayEnd() const;
        objectiter objectBegin();
        const_objectiter const_objectBegin() const;
        objectiter objectEnd();
        const_objectiter const_objectEnd() const;

        inline void check() const
        {
            if (!m_ptr)
            {
                throw myJsonException("Bad Json access: m_ptr is null", 0);
            }
        }
    };

    Json parse(const std::string &in);

    inline const char *toString(JsonValueType type)
    {
        switch (type)
        {
        case myJson::JsonValueType::NUL:
            return "null";
        case myJson::JsonValueType::NUMBER:
            return "number";
        case myJson::JsonValueType::BOOL:
            return "bool";
        case myJson::JsonValueType::STRING:
            return "string";
        case myJson::JsonValueType::ARRAY:
            return "array";
        case myJson::JsonValueType::OBJECT:
            return "object";
        default:
            return "unkown";
        }
    }

    inline std::ostream &operator<<(std::ostream &os, myJson::JsonValueType type)
    {
        return os << std::string(toString(type));
    }
};
