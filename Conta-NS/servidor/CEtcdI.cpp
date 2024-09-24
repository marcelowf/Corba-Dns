#include <iostream>
#include <orbsvcs/CosNamingC.h>
#include "CEtcdI.h"

CEtcd_i::CEtcd_i(const std::string& id) : id_(id) {
    std::cout << "CEtcd object created with ID: " << id_ << std::endl;
}

CEtcd_i::~CEtcd_i() {
    std::cout << "CEtcd object with ID: " << id_ << " is being destroyed" << std::endl;
}
char* CEtcdI::id() {
    return CORBA::string_dup(id_.c_str());
}

CORBA::Boolean CEtcdI::put(const char* key, const char* val) {
    std::string keyStr = key;
    std::string valStr = val;

    auto result = store_.insert({keyStr, valStr});
    if (!result.second) {
        store_[keyStr] = valStr;
        return false;
    }
    return true;
}

char* CEtcdI::get(const char* key) throw(KeyValueStore::InvalidKey) {
    std::string keyStr = key;
    auto it = store_.find(keyStr);
    if (it == store_.end()) {
        throw KeyValueStore::InvalidKey("Key does not exist");
    }
    return CORBA::string_dup(it->second.c_str());
}

void CEtcdI::del(const char* key) throw(KeyValueStore::InvalidKey) {
    std::string keyStr = key;
    auto it = store_.find(keyStr);
    if (it == store_.end()) {
        throw KeyValueStore::InvalidKey("Key does not exist");
    }
    store_.erase(it);
}

// Novo metodo reset
void CEtcdI::reset() {
    store_.clear();
    std::cout << "All key-value have been cleared." << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
        
        CORBA::Object_var poaObj = orb->resolve_initial_references("RootPOA");
        PortableServer::POA_var poa = PortableServer::POA::_narrow(poaObj);
        PortableServer::POAManager_var poaManager = poa->the_POAManager();
        
        CEtcdI* etcdImpl = new CEtcdI("etcd-001");
        PortableServer::ObjectId_var etcdId = poa->activate_object(etcdImpl);
        CORBA::Object_var etcdRef = poa->id_to_reference(etcdId);

        CORBA::Object_var namingContextObj = orb->resolve_initial_references("NameService");
        CosNaming::NamingContext_var namingContext = CosNaming::NamingContext::_narrow(namingContextObj);

        CosNaming::Name name;
        name.length(1);
        name[0].id = CORBA::string_dup("CEtcd");
        name[0].kind = CORBA::string_dup("");

        namingContext->rebind(name, etcdRef);

        poaManager->activate();

        std::cout << "Servidor pronto..." << std::endl;

        orb->run();
    } catch (CORBA::Exception& e) {
        std::cerr << "Exceção: " << e._name() << std::endl;
    }

    return 0;
}
