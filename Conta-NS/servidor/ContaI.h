#ifndef CONTAI_OK3IEJ_H_
#define CONTAI_OK3IEJ_H_

#include "ContaS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Conta_i : public virtual POA_Conta {
private:
    float meusaldo;
public:
  // Constructor
  Conta_i ();
  // Destructor
  virtual ~Conta_i ();
  virtual std::string id ();
  virtual ::CORBA::Float saldo ();
  virtual void deposito (::CORBA::Float valor);
  virtual void saque (::CORBA::Float valor);
  virtual void transfere (
    ::CORBA::Float valor,
    ::Conta_ptr dest);
  virtual void shutdown (const std::string senha);
};


#endif /* CONTAI_H_  */
