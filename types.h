#ifndef __TYPES_H__
#define __TYPES_H__
#include "xtrait.h"
#include "linkedlist.h"
using TX = float;
using namespace std;

template <typename _value_type, typename _LinkedValueType>
struct Data{
       using value_type = _value_type;
       using LinkedValueType = _LinkedValueType;
};

template <typename Trait>
struct Node_BT
{
    using  value_type = typename Trait::value_type;
    using  LinkedValueType= typename Trait::LinkedValueType;
    using  Node= typename Trait::Node;
    using  CompareFn = greater<value_type>;
};
using Data_Node_int_int= XTrait<int,int>;
using Traits_BT_int_int= Node_BT<Data_Node_int_int>;


template <typename Traits>
struct LLTraitAsc
{
    using  value_type = typename Traits::value_type;
    using  LinkedValueType= typename Traits::LinkedValueType;
    using  Node      = NodeLinkedList<Traits>;
    using  CompareFn = less<value_type>;
};

template <typename Traits>
struct LLTraitDesc
{
    using  value_type = typename Traits::value_type;
    using  LinkedValueType= typename Traits::LinkedValueType;
    using  Node      = NodeLinkedList<Traits>;
    using  CompareFn = greater<value_type>;
};

//Declaration of Traits in order to use in my LinkedList
using TraitLLIntInt= XTrait<int,int>;
using Traits_LLAsc= LLTraitAsc<TraitLLIntInt>;
using Traits_LLDesc= LLTraitDesc<TraitLLIntInt>;
using TraitLLStrInt= XTrait<string,int>;
using Traits_LL_str_int= LLTraitAsc<TraitLLStrInt>;



#endif