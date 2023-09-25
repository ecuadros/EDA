#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <algorithm> 
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include <vector>
#include <map>

using namespace std;


template <typename xTraits>
class NodeGraph{
    public:
       using value_type          = typename xTraits::value_type;
        using LinkedValueType       = typename xTraits::LinkedValueType;
        using Data                  = typename xTraits::Node; 
    private:
        using vertex = NodeGraph<xTraits>;
    public:
        Data      m_data;
     NodeGraph(value_type key,LinkedValueType value) : m_data(key,value){};
    
    value_type       getData()      { return m_data.getData();}
    value_type       &getDataRef()  {return m_data.getDataRef();}
    LinkedValueType  getValue()     { return m_data.getValue(); }
    vector<vertex*> Neighbors;
 
};

template <typename xTraits>

struct GTrait
{
    using  value_type           = typename xTraits::value_type;
    using  LinkedValueType      = typename xTraits::LinkedValueType;
    using  NodeG                 = typename xTraits::Node;
    using  CompareFn            = less<value_type>;
};

using grafo=GTrait<XTrait<IX, IX>>;

//========= Conteiner Grafo===================
template <typename GTrait>
class Graph{

    public:

      using value_type          = typename GTrait::value_type;
      using LinkedValueType     = typename GTrait::LinkedValueType;
       using Node              = typename GTrait::NodeG;
      using CompareFn           = typename GTrait::CompareFn;
      using myself              = Graph<GTrait>;
     

    
    protected:
     size_t   m_verTex  = 0;
      CompareFn Compfn;

    public: 
      size_t  size()  const       { return m_verTex;       }
      bool    empty() const       { return size() == 0;  }

    public:
      Graph() {}

    private:
        string m_name = "Empty";
        std::map<value_type, Node> Nodes; // osea nodo o vértice (coleccion)

  public:
     Graph(string name)  : m_name(name){ }
     ~Graph(){
          cout<< "Destroying " << m_name << "..." << endl;
          destroy();
      }

      Node* FindVertex(const value_type &key){
        for (auto it= Nodes.begin(); it != Nodes.end(); ++it) {
            if(it.find(key))
              return *this;
          }
          return nullptr;
      }

       // TODO: Replantear
      void insert(const value_type &key, const LinkedValueType &value){
         Nodes.insert({ key,value}); 
      }

      void insertEdges(const value_type &Ini,const value_type &End){
          Node* IniNode = FindVertex(Ini);
          Node* EndNode = FindVertex(End);
          if(IniNode && EndNode){
            IniNode->Neighbors.push_back(EndNode);
            EndNode->neighbors.push_back(IniNode);
          }
      
      }
    void print(const value_type &key){

         Node* node = FindVertex(key);
        if (node) {
            std::cout << "Neighbors of vertex " << key<< ": ";
            for (Node* neighbor : node->Neighbors) {
                std::cout << neighbor->getData() << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "Vertex " << key << " not found." << std::endl;
        } 
          

    }
  
      void destroy(){
            m_verTex  = 0;
       
      }

};

#endif
