#include "ContaI.h"
#include <iostream>

using namespace std;

extern CORBA::ORB_var orb;

// Implementation skeleton constructor
Conta_i::Conta_i ()
{
    meusaldo = 0.0;
}

// Implementation skeleton destructor
Conta_i::~Conta_i ()
{
}

std::string Conta_i::id ()
{
    cout << "retornando id\n";
    return string("1234-5");
}

::CORBA::Float Conta_i::saldo ()
{
    cout << "retornando saldo\n";
    return meusaldo;
}

void Conta_i::deposito (::CORBA::Float valor)
{
    cout << "deposito de " << valor << endl;
    meusaldo += valor;
}

void Conta_i::saque (::CORBA::Float valor)
{
    cout << "saque de " << valor << endl;
    if (valor <= meusaldo)
        meusaldo -= valor;
    else
        throw SaldoInsuficiente();
}

void Conta_i::transfere (
  ::CORBA::Float valor,
  ::Conta_ptr dest)
{
    // exercicio
}

void Conta_i::shutdown (
  const std::string senha)
{
    if (senha == "12345")
        orb->shutdown();
}

