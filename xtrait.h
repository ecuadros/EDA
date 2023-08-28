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

using XTraitFloatStringDesc = XTrait<float, std::string, std::less<KeyNode<int, int> &>>;
using XTraitIntIntAsc = XTrait<int, int, std::greater<KeyNode<int, int>>>;
using XTraitIntIntDesc = XTrait<int, int, std::less<KeyNode<int, int>>>;
using XTraitIntIntAscCompareVal = XTrait<int, int, std::greater<int>>;
using XTraitIntIntDescCompareVal = XTrait<int, int, std::less<int>>;

#endif