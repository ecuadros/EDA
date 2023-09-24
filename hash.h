#ifndef __HASH_H__  
#define __HASH_H__ 
#include <vector>
#include <optional>
#include <functional>
#include "linkedlist.h"
using namespace std;
template <typename Trait>
class HashMap{
public:
  using value_type = typename Trait::value_type;
  using LinkedValueType =typename Trait::LinkedValueType;
  using Node         = typename Trait::Node;
  using Node_LL=LinkedList<Trait>;
private:
  hash<value_type> hashFunction;
  vector<Node_LL> buckets;
  //vector<std::list<Node>> buckets;
public:
  HashMap(size_t size=100): buckets(size) {}
  void insert(const value_type&key,const LinkedValueType & value){
    size_t index = hashFunction(key) % buckets.size();
    //cout<<hashFunction(key)<<"-"<<index<<endl;
    if(buckets[index].size()==0){
       buckets[index].insert(key,value);
    }
    else{
      auto ItBegin=buckets[index].begin();
      auto ItEnd=buckets[index].end();
      auto iter = ItBegin;
      for(; iter != ItEnd ; ++iter){
        if(*iter==key){
          //cout<<"\nSe ingreso una copia"<<key<<endl;
          buckets[index].insert(key,value);
          return;
        }
      }
    }  
  }
  std::optional<LinkedValueType> get(const value_type& key) {
        size_t index = hashFunction(key) % buckets.size();
        if(buckets[index].size()>0){
          auto ItBegin=buckets[index].begin();
          auto ItEnd=buckets[index].end();
          auto iter = ItBegin;
          for(; iter != ItEnd ; ++iter){
            if(*iter==key){
              return iter.getLinkedValue();
            }
          }
        } 
        return {};
    }
   void remove(const value_type& key) {
        size_t index = hashFunction(key) % buckets.size();
        if(buckets[index].size()>0){
          auto ItBegin=buckets[index].begin();
          auto ItEnd=buckets[index].end();
          auto iter = ItBegin;
          size_t count=0;
          for(; iter != ItEnd ; ++iter){
            count++;
            if(*iter==key){
              //cout<<"\nDelete Node: "<<key<<count<<endl;
              buckets[index].DeleteNode(key,count-1);
              return;
            }
          }
        }
   }      
};
#endif