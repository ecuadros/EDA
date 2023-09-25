#ifndef __GRAPH_H__  
#define __GRAPH_H__
#include<iostream>
#include <sstream> 
#include "xtrait.h"
namespace GraphNamespace {

#define INF 999999
using namespace std;
template <typename T>
inline T convertFromString(const std::string &str);
template <typename Traits>
class NodeQ{
  public:
    using KeyNode = typename Traits::Node;
    using value_type= typename Traits::value_type;
    using LinkedValueType= typename Traits::LinkedValueType;
  private:
    using Node = NodeQ<Traits>;
  public:
    KeyNode m_data;
    Node   *m_pNext;//
  public:
    NodeQ(value_type el1,LinkedValueType el2, Node *pNext = nullptr) 
        : m_data(el1,el2), m_pNext(pNext)
    {};
    value_type getData(){ return m_data.getData();}
    value_type &getDataRef(){return m_data.getDataRef();}
    LinkedValueType getValue(){ return m_data.getValue();}
    LinkedValueType &getValueRef(){return m_data.getValueRef();}

    void      setpNext(Node *pNext)  {   m_pNext = pNext;  }
    Node     *getpNext()               {   return getpNextRef();   }
    Node    *&getpNextRef()            {   return m_pNext;   }
};
//Traits
template <typename Traits>
struct Data_Key_G{
    using  value_type = typename Traits::value_type;
    using  LinkedValueType= typename Traits::LinkedValueType;
    using  Node      = NodeQ<Traits>;
    using  CompareFn = greater<value_type>;
};

//Declaration of Traits in order to use in my LinkedList
using TraitIntInt= XTrait<int,int>;
using Traits_Data_Key_G= Data_Key_G<TraitIntInt>;
template <typename Traits>
class arista {
	public:
	  using Node = typename Traits::Node;
		using value_type= typename Traits::value_type;
    using LinkedValueType= typename Traits::LinkedValueType;
    Node datoDestino;
    
	public:
    arista(const Node pTmp): datoDestino(pTmp) {}
		arista<Traits> *sgteArista;
		int pesoArista=1;// Weight by default
};
template <typename Traits>
class vertice {
	public:
	  using Node = typename Traits::Node;
		using value_type= typename Traits::value_type;
    using LinkedValueType= typename Traits::LinkedValueType;
    Node datoOrign;
    
	public:
    vertice(const value_type& val1, const LinkedValueType& val2) : datoOrign(val1, val2) {}
		arista<Traits> *adyacente;
		vertice<Traits> *sgteVertice;
};

template<class Traits>
class grafo{
  public:
		using value_type= typename Traits::value_type;
    using LinkedValueType= typename Traits::LinkedValueType;
    using Node= typename Traits::Node;//At this context Node is NodeQ
	public:
		typedef arista<Traits> *parista;
		typedef vertice<Traits> *pvertice;
		grafo();
		void insertarVertice(const value_type &val_1,const LinkedValueType &val_2);
		void insertarArista(const value_type &x,const value_type &y);
		void imprimirGrafo();
		void Write(){imprimirGrafo();};
		void Read(const std::string& filename);
		~grafo();
	private:
		pvertice pGrafo;
};
template <class T>
grafo<T>::grafo(){
	pGrafo=NULL;
	
}
template <class T>
grafo<T>::~grafo(){
	cout<<endl;
	pvertice p,rp;
	parista r,ra;
	p= pGrafo;
	while(p!=nullptr){
		r= p->adyacente;
		while(r!=nullptr){
			ra=r;
			r=r->sgteArista;
			delete ra;
		}
		rp=p;
		p=p->sgteVertice;
		delete rp;
	}
}
template <class Traits>
void grafo<Traits>::insertarVertice(const value_type &val_1,const LinkedValueType &val_2){
	pvertice p;
	p = new vertice<Traits>(val_1, val_2);
	p->adyacente=nullptr;
	p->sgteVertice= pGrafo;
	pGrafo=p;
}
template <typename Traits>
void grafo<Traits>::insertarArista(const value_type &x,const value_type &y){
	pvertice p,pp;
	parista a;
	parista r;
	bool st=false;
	bool stt=false;
	p= pGrafo;
	pp= pGrafo;
	if (p!= nullptr){
		//Find Node with value_data is x
		while(p!=nullptr){
			if(p->datoOrign.getData()==x){
				st=true;
				break;
			}
			else{
				p=p->sgteVertice;	
			}
		}

		while(pp!=nullptr){
			if(pp->datoOrign.getData()==y){
				stt=true;
				break;
			}
			else{
				pp=pp->sgteVertice;	
			}
		}

		if(p!=nullptr && st==true && pp!=nullptr && stt==true){
			a= new arista<Traits>(pp->datoOrign);
			//a->datoDestino=pp;
			if(p->adyacente==nullptr){
				a->sgteArista=p->adyacente;
				p->adyacente=a;
			}
			else{
				r=p->adyacente;
				p->adyacente=a;
				a->sgteArista=r;
			}
		}
		else{
			cout<<"No se encontro vertice origen"<<endl;
		}
	}
}

template <class Traits>
void grafo<Traits>::imprimirGrafo(){
	pvertice p;
	parista a;
	p= pGrafo;
	if(p==nullptr) {
		cout<<"Grafo vacio"<<endl;
	}
	else
	{
		while(p!=nullptr){
			//cout<<"V"<<p->datoOrign.getData()<<"-->>";
			cout<<"V"<<p->datoOrign.getData()<<"{"<<p->datoOrign.getValue()<<"}"<<"-->>";
			a=p->adyacente;
			while(a!=nullptr){
				cout<<"V"<<a->datoDestino.getData()<<"{"<<a->datoDestino.getValue()<<"}"<<",";
				a=a->sgteArista;
			}
			cout<<endl;
			p=p->sgteVertice;
		}
	}	
}
template <class Traits>
void grafo<Traits>::Read(const std::string& filename){
    std::ifstream is(filename); 
    if (!is.is_open()) { 
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    string tmp_flow,num;
    size_t count;
    value_type  value;
    LinkedValueType kvalue;
    while (getline(is,tmp_flow)) { 
        std::istringstream iss(tmp_flow);
        count = 0;
        while (iss >> num && count < 2) {
            if(count==0)
                value = GraphNamespace::convertFromString<value_type>(num);
            else
                kvalue = GraphNamespace::convertFromString<LinkedValueType>(num);  
            count++;
        }
        insertarVertice(value,kvalue);
    } 

    is.close();

}
// operator<<
template <typename T>
inline ostream &operator<<(ostream &os, grafo<T> &obj){
    obj.imprimirGrafo();
    return os;
}
//  operator>>
template <typename T>
inline T convertFromString(const std::string &str) {
    std::istringstream iss(str);
    T value;
    iss >> value;
    return value;
}
template <typename T>
inline istream & operator>>(istream &is,grafo<T> &obj){
    string tmp_flow,num;
    size_t count;
    using vt      = typename T::value_type;
    using lvt     = typename T::LinkedValueType ;
    vt  value;
    lvt kvalue;
    while (getline(is,tmp_flow)) { 
        istringstream iss(tmp_flow);
        count = 0;
        while (iss >> num && count < 2) {
            if(count==0){
                vt num_f = convertFromString<vt>(num);
                value=num_f;
            } 
            else{
                lvt num_f = convertFromString<lvt>(num);
                kvalue=num_f; 
            }      
            count++;
        }
        obj.insertarVertice(value,kvalue);
    }
    return is;    
}


}

#endif
