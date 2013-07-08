#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include "multinomial.h"


/* Adapted from Aaron Williams multicool.c 
Author: Aaron Williams (haron@uvic.ca)
Website: http://www.math.mcgill.ca/haron/

This code is distributed without conditions

This program outputs the permutations of any multiset in cool-lex order.
The program stores a single permutation in a singly-linked list, and then
each successive permutation in O(1)-time while using O(1) additional pointers.

Input is read from stdin as follows: n e_1 e_2 ... e_n where e_i <= e_{i+1} for all i. 

For example, if input.txt is a file containing the following single line
6 1 1 2 3 3 3 
then multicool < input.txt will output the cool-lex order for the permutations of {1,1,2,3,3,3}:
333211
133321
313321
etc */

// C++ version: James M. Curran (j.curran@auckland.ac.nz)

extern "C" {
	class Multicool{
		struct list_el {
			int v;
			struct list_el * n;
		};
		typedef struct list_el item;

		item *h;
		item *t;
		item *i;

    int *m_pnInitialState; // stored so that the current state can be reset to the initial state
		int *m_pnCurrState;
		int m_nLength;
		bool m_bFirst;

	public:
		Multicool(int *x, int nx){
			i = (item *)NULL;
			h = NULL;
      m_pnInitialState = new int[nx];
			m_pnCurrState = new int[nx];
			m_nLength = nx;
			m_bFirst = true;

			for(int ctr = 0; ctr < nx; ctr++){
				t = new item;
				t->v = INT_MAX;
				t->v = x[ctr];
				m_pnCurrState[ctr] = x[ctr];
        m_pnInitialState[ctr] = x[ctr];
				t->n = h;
				h = t;

				if(h->n && h->n->v > h->v){
					// std::cout << "Error" << std::endl;
          // should programme some sensible action here but I don't know what
				}

				if(ctr+1 == 2){
					i = h;
				}
			}
		};

		~Multicool(void){
      delete [] m_pnInitialState;
			delete [] m_pnCurrState;

			while(t){
				item *next = t->n;
				delete t;
				t = next;
			}
		};
    
    void getInitialState(int *pnSet){
      int ctr;
      
      for(ctr = 0; ctr < m_nLength; ctr++){
        pnSet[ctr] = m_pnInitialState[ctr];
      }
    };
    
    
    int getLength(void){
      return m_nLength;
    };
    
    
    void reset(void){
      delete [] m_pnCurrState;

			while(t){
				item *next = t->n;
				delete t;
				t = next;
			}
      
      i = (item *)NULL;
  		h = NULL;
    	m_pnCurrState = new int[m_nLength];
			m_bFirst = true;

			for(int ctr = 0; ctr < m_nLength; ctr++){
				t = new item;
				t->v = INT_MAX;
				t->v = m_pnInitialState[ctr];
				m_pnCurrState[ctr] = m_pnInitialState[ctr];
    		t->n = h;
				h = t;

				if(h->n && h->n->v > h->v){
					//std::cout << "Error" << std::endl;
          // should programme some sensible action here but I don't know what
				}

				if(ctr+1 == 2){
					i = h;
				}
			}
      
    }

		void setState(item *b){
			item *y;
			y = b;
			int ctr = 0;

			while(y) {
				m_pnCurrState[ctr++] =  y->v;
				y = y->n ;
			}
		}

		bool hasNext(void){
			item *j;
			item *t;
			item *s;

			if(m_bFirst){
				setState(h);
				m_bFirst = false;
				return true;
			}else{
				j = i->n;

				if(j->n || j->v < h->v) { 
					if (j->n && i->v >= j->n->v) {
						s = j; 
					} else {
						s = i;
					}
					t = s->n;
					s->n = t->n;
					t->n = h;
					if(t->v < h->v) {
						i = t;
					}
					j = i->n;
					h = t;
					setState(h);
					return true;
				}else{
					return false;
				}
			}
		}

		void getState(int *state){
			for(int ctr = 0; ctr < m_nLength; ctr++)
				state[ctr] = m_pnCurrState[ctr];
		};
	};



//	int test(void){
//		int *i  = new int[4];
//		i[0] = 1;
//		i[1] = 1;
//		i[2] = 2;
//		i[3] = 2;
//
//		Multicool m(i, 4);
//
//		while(m.hasNext()){
//			m.getState(i);
//
//			for(int ctr = 0; ctr < 4; ctr++){
//				std::cout << i[ctr];
//			}
//			std::cout << std::endl;
//		}
//
//		return 0;
//	}
  
 	std::vector<Multicool*> vpm;


	void MC(int *set, int *nx, int *id){
		vpm.push_back(new Multicool(set, *nx));
		*id = (int)vpm.size() - 1;
	}
  
  void all(int *id, int *pnResult){
    Multicool *pm = vpm[*id];
    
    pm->reset();
    int *set = new int[pm->getLength()];
    pm->getInitialState(set);
    
    int i = 0;
    int j;
    int nLength = pm->getLength();
    
    while(pm->hasNext()){
      pm->getState(set);
      for(j = 0; j < pm->getLength(); j++){
        pnResult[i * nLength + j] = set[j];
      }
      i++;
    }
    
    delete [] set;
  }

	void next(int *set, int *bnext, int *id){
		Multicool *pm = vpm[*id];

		if(pm->hasNext()){
			pm->getState(set);
			*bnext = 1;
		}else{
			pm->getState(set);
			*bnext = 0;
		}
	}
  
  // multinomial coefficient
  void multinomCoeff(int *x, int *nx, int *result){
    multinomial::SVI v(*nx);
    int i;
    
    for(i = 0; i < *nx; i++){
      v.at(i) = x[i];
    }
    
    *result = (int)multinomial::multi<unsigned long>(v);
  }
}
