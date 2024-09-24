#include <iostream>
#include <orbsvcs/CosNamingC.h>
#include "CEtcdC.h"

int main(int argc, char* argv[]) {
    try {
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        CORBA::Object_var namingContextObj = orb->resolve_initial_references("NameService");
        CosNaming::NamingContext_var namingContext = CosNaming::NamingContext::_narrow(namingContextObj);

        CosNaming::Name name;
        name.length(1);
        name[0].id = CORBA::string_dup("CEtcd");
        name[0].kind = CORBA::string_dup("");
        CORBA::Object_var obj = namingContext->resolve(name);

        KeyValueStore::CEtcd_var etcd = KeyValueStore::CEtcd::_narrow(obj);

        std::cout << "ID do objeto: " << etcd->id() << std::endl;

        std::cout << "Inserindo chave 'a': " << etcd->put("a", "1") << std::endl;
        std::cout << "Valor da chave 'a': " << etcd->get("a") << std::endl;

        std::cout << "Removendo chave 'a'" << std::endl;
        etcd->del("a");

        try {
            std::cout << "Tentando obter chave 'a': " << etcd->get("a") << std::endl;
        } catch (KeyValueStore::InvalidKey& e) {
            std::cerr << "Exceção: " << e.message << std::endl;
        }

    } catch (CORBA::Exception& e) {
        std::cerr << "Exceção: " << e._name() << std::endl;
    }

    return 0;
}
