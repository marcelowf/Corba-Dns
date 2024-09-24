#ifndef CETCDI_H
#define CETCDI_H

#include <map>
#include <string>
#include "CEtcdS.h"

class CEtcdI : public virtual POA_KeyValueStore::CEtcd {
public:
    CEtcdI(const std::string& id);
    virtual ~CEtcdI();

    virtual char* id();
    virtual CORBA::Boolean put(const char* key, const char* val);
    virtual char* get(const char* key) throw(KeyValueStore::InvalidKey);
    virtual void del(const char* key) throw(KeyValueStore::InvalidKey);

    // Novo metodo
    virtual void reset();

private:
    std::string id_;
    std::map<std::string, std::string> store_;
};

#endif
