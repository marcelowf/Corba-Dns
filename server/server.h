#ifndef CETCD_IMPL_H
#define CETCD_IMPL_H

#include <map>
#include <string>
#include <CORBA.h>
#include "cetcd_skel.h"

class InvalidKey : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid key exception.";
    }
};

class CEtcdImpl : public POA_CEtcd {
private:
    std::map<std::string, std::string> table;
    std::string id;

public:
    CEtcdImpl(const std::string& objId);
    char* id() override;
    CORBA::Boolean put(const char* key, const char* value) override;
    char* get(const char* key) override;
    void del(const char* key) override;
};

#endif