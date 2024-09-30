#include <iostream>
#include <map>
#include <string>
#include <CORBA.h>
#include "cetcd_skel.h"  // Gerado automaticamente pelo compilador IDL
#include "cetcd_impl.h"

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
    CEtcdImpl(const std::string& objId) : id(objId) {}

    char* id() override {
        return CORBA::string_dup(id.c_str());
    }

    CORBA::Boolean put(const char* key, const char* value) override {
        std::string keyStr = key;
        std::string valueStr = value;

        bool keyExists = table.find(keyStr) != table.end();
        table[keyStr] = valueStr;

        return !keyExists;
    }

    char* get(const char* key) override {
        std::string keyStr = key;

        if (table.find(keyStr) != table.end()) {
            return CORBA::string_dup(table[keyStr].c_str());
        } else {
            throw InvalidKey();
        }
    }

    void del(const char* key) override {
        std::string keyStr = key;

        if (table.find(keyStr) != table.end()) {
            table.erase(keyStr);
        } else {
            throw InvalidKey();
        }
    }
};

int main(int argc, char* argv[]) {
    try {
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        CORBA::Object_var poaObj = orb->resolve_initial_references("RootPOA");
        PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(poaObj);
        PortableServer::POAManager_var poaManager = rootPOA->the_POAManager();
        poaManager->activate();

        CEtcdImpl* cetcdImpl = new CEtcdImpl("CEtcd-Server-1");

        PortableServer::ObjectId_var objId = rootPOA->activate_object(cetcdImpl);
        CORBA::Object_var cetcdRef = rootPOA->id_to_reference(objId);
        CEtcd_var cetcd = CEtcd::_narrow(cetcdRef);

        CORBA::Object_var namingContextObj = orb->resolve_initial_references("NameService");
        CosNaming::NamingContext_var namingContext = CosNaming::NamingContext::_narrow(namingContextObj);

        CosNaming::Name name;
        name.length(1);
        name[0].id = CORBA::string_dup("CEtcd");
        name[0].kind = CORBA::string_dup("");

        namingContext->rebind(name, cetcd);

        std::cout << "Servidor CORBA CEtcd iniciado e aguardando requisições..." << std::endl;

        orb->run();
    }
    catch (const CORBA::Exception& ex) {
        std::cerr << "Erro CORBA: " << ex._name() << std::endl;
    }
    catch (const InvalidKey& ex) {
        std::cerr << "Exceção de chave inválida: " << ex.what() << std::endl;
    }
    return 0;
}
