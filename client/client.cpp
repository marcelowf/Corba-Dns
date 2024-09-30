#include <iostream>
#include <CORBA.h>
#include "cetcdC.h"  // Inclui o código gerado a partir do IDL

int main(int argc, char* argv[]) {
    try {
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        CORBA::Object_var obj = orb->resolve_initial_references("NameService");
        CosNaming::NamingContext_var namingContext = CosNaming::NamingContext::_narrow(obj);

        CosNaming::Name name;
        name.length(1);
        name[0].id = CORBA::string_dup("CEtcdService");

        CORBA::Object_var cetcdObj = namingContext->resolve(name);
        CEtcdModule::CEtcd_var cetcd = CEtcdModule::CEtcd::_narrow(cetcdObj);

        if (CORBA::is_nil(cetcd)) {
            std::cerr << "Erro: objeto CEtcd é nulo" << std::endl;
            return 1;
        }

        std::cout << "ID do objeto CEtcd: " << cetcd->id() << std::endl;

        CORBA::Boolean result = cetcd->put("language", "C++");
        std::cout << "Inserido language: C++ -> Resultado: " << (result ? "Novo" : "Atualizado") << std::endl;

        try {
            char* value = cetcd->get("language");
            std::cout << "Valor de 'language': " << value << std::endl;
            CORBA::string_free(value);
        } catch (const CEtcdModule::InvalidKey& e) {
            std::cerr << "Erro: Chave 'language' não encontrada. Motivo: " << e.reason << std::endl;
        }

        result = cetcd->put("language", "Python");
        std::cout << "Atualizado language: Python -> Resultado: " << (result ? "Novo" : "Atualizado") << std::endl;

        try {
            char* value = cetcd->get("language");
            std::cout << "Valor atualizado de 'language': " << value << std::endl;
            CORBA::string_free(value);
        } catch (const CEtcdModule::InvalidKey& e) {
            std::cerr << "Erro: Chave 'language' não encontrada. Motivo: " << e.reason << std::endl;
        }

        try {
            cetcd->del("language");
            std::cout << "Chave 'language' removida com sucesso." << std::endl;
        } catch (const CEtcdModule::InvalidKey& e) {
            std::cerr << "Erro: Falha ao remover chave 'language'. Motivo: " << e.reason << std::endl;
        }

        try {
            cetcd->get("language");
        } catch (const CEtcdModule::InvalidKey& e) {
            std::cerr << "Erro esperado: Chave 'language' não encontrada após remoção. Motivo: " << e.reason << std::endl;
        }

        orb->destroy();
    } catch (const CORBA::Exception& e) {
        std::cerr << "Exceção CORBA: " << e._name() << std::endl;
        return 1;
    }
    return 0;
}
