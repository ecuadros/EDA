#ifndef __XTRAIT_H__
#define __XTRAIT_H__

#include "keynode.h"
template <typename _K, typename _V, 
            typename _CompareFn = std::less< KeyNode<_K, _V> & >>
struct XTrait
{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Node      = KeyNode<_K, _V>;
    using  CompareFn = _CompareFn;
};

#endif