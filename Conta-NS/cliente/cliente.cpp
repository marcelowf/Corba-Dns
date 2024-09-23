//
// CLIENTE DE CONTA BANCÁRIA SIMPLES
//
// Sistemas Distribuídos
// Escola Politécnica -- PUCPR
// (C) Prof. Luiz Lima Jr. (luiz.lima@pucpr.br)
//

#include <iostream>
#include <string>
#include <ContaC.h>
#include <orbsvcs/CosNamingC.h>

using namespace std;
using namespace CORBA;
using namespace CosNaming;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "USO: " << argv[0] << " <nome_do_servidor>\n";
        return 1;
    }

    try {
	    // 1. Inicializa ORB
        ORB_var orb = ORB_init(argc, argv, "ORB");
	
        // 2. Obtém referência para objeto distribuído (da IOR no servidor de nomes)
        Object_ptr tmp = orb->resolve_initial_references("NameService");
        NamingContext_var ns = NamingContext::_narrow(tmp);   // ns: o servidor de nomes

        Name nome(1);
        nome.length(1);
        nome[0].id = string_dup(argv[1]);

        tmp = ns->resolve(nome);

        // Object_ptr tmp = orb->string_to_object(argv[1]);   // le do arquivo
        Conta_var conta = Conta::_narrow(tmp);
    
        // 3. Usa objeto (chama métodos)

        cout << "Digite valor para deposito: ";
        float valor;
        cin >> valor;
        conta->deposito(valor);
        
        try {
            cout << "Digite valor para saque: ";
            cin >> valor;
            conta->saque(valor);
        } catch (const SaldoInsuficiente& ) {
            cerr << "saldo insuficiente!\n";
        }
        cout << "saldo = " << conta->saldo() << endl;

	    // 4. Finalizações
        orb->destroy();
    } catch (const CORBA::Exception& e) {
        cerr << "Erro CORBA: " << e << endl;
    }
	return 0;
}


