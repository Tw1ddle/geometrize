#pragma once

#include "chaiscript/chaiscript.hpp"

#define ADD_CONST_VAR(Class, Name) try { module->add(chaiscript::const_var(&Class::Name), #Name); } catch(...) { assert(0 && #Name); }
#define ADD_FREE_FUN(Name) try { module->add(chaiscript::fun(&Name), #Name); } catch(...) { assert(0 && #Name); }
#define ADD_MEMBER(Class, Name) try { module->add(chaiscript::fun(&Class::Name), #Name); } catch(...) { assert(0 && #Name); }
#define ADD_CONST_REF_MEMBER(Class, Name) try { module->add(chaiscript::fun([](const Class &r) -> decltype(auto) { return (r.Name); }), #Name); } catch(...) { assert(0 && #Name); }
#define ADD_TYPE(Class) try { module->add(chaiscript::user_type<Class>(), #Class); } catch(...) { assert(0 && #Class); }
#define ADD_BASE_CLASS(Base, Derived) try { module->add(chaiscript::base_class<Base, Derived>()); } catch (...) { assert(0 && #Base); }
#define ADD_CONSTRUCTOR(Class, Signature) try { module->add(chaiscript::constructor<Signature>(), #Class); } catch(...) { assert(0 && #Signature); }
#define ADD_GLOBAL_CONST(Name, Value) try { module->add_global_const(chaiscript::const_var(Value), Name); } catch (...) { assert(0 && Name); }
