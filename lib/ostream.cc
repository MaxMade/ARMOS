#include <ostream.h>
#include <type_traits.h>

using namespace lib;
atomic<int> ostreamBase::cpu_lock(-1);
