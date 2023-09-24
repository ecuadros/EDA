#ifndef __TAG_H__
#define __TAG_H__

template <typename value_type, typename LinkedValueType>
class tagObjectInfo {

public :
    using valuetype   = value_type;
     using Type         = value_type;
    using LinkedValue_Type = LinkedValueType;
public:
    tagObjectInfo(const value_type &_key, LinkedValueType _value) 
        : key(_key), value(_value), UseCounter(0), next(nullptr), prev(nullptr) {}

 
    tagObjectInfo(const tagObjectInfo &objInfo) 
        : key(objInfo.key), value(objInfo.value), UseCounter(0), next(nullptr), prev(nullptr) {}

  
    tagObjectInfo() 
        : UseCounter(0), next(nullptr), prev(nullptr) {}

    value_type& getKeyRef() {
        return key;
    }

    operator value_type() {
        return key;
    }

    Type GetUseCounter() const {
        return UseCounter;
    }

public:
    value_type key;
    LinkedValueType value;
    Type UseCounter;
    tagObjectInfo *next;
    tagObjectInfo *prev;
};
#endif