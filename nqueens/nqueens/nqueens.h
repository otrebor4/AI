#ifndef NQUEENS_H
#define NQUEENS_H

#include <set>
#include <string>
#include <sstream>
#include "AVLTree.h"
#include "pairKeyComp.h"
#include "Heap.h"

namespace str
{
	std::string to_string(int v)
	{
		std::stringstream ss;
		ss << v;
		return ss.str();
	}
}

float randF()
{
	return (float) (std::rand() * 1.0f )/ (RAND_MAX * 1.0f);
}

class genotype
{
private:
	float _conflicts;
	int size;
	int * g;
	
	//intilsize an empty genotype
	void init(int size)
	{
		if( g != NULL)
			delete [] g;
		this->size = size;
		g = new int[this->size];
		reset();
	}
	
public:	
	
	genotype()
	{
		g = NULL;
	}
	//intilialize an empty genotyp
	genotype(int size)
	{
		g = NULL;
		init(size);
	}
	
	
	//create an new genotype from parentA, parentB
	genotype(int size, int crtp, genotype & parentA, genotype & parentB)
	{
		g = NULL;
		init(size);
		int * v = new int[this->size];
		for( int i = 0; i < crtp; i++)
		{
			v[i] = parentA[i];
		}
		int i = crtp;
		int b = 0;
		while( i < this->size)
		{
			if(!have( parentB[b], i, v )){
				v[i] = parentB[b];
				i++;
			}
			b++;
		}
		setGenotype(v);
	}
	
	//copy constructor
	genotype(const genotype & other)
	{
		g = NULL;
		init(other.size);
		setGenotype(other.g );
	}
	
	//release array memory
	~genotype()
	{
		delete [] g;
		g = NULL;
	}
	
	//swap values on the positions a and b
	void swap( int a, int b)
	{
		int t = g[a];
		g[a] = g[b];
		g[b] = t;
	}
	
	//since the valid numbers are from 0-(N-1)
	//seting all to -1 mean an empty genotype
	void reset()
	{
		_conflicts = -1;
		for(int i = 0; i < size; i++){
			g[i] = -1;
		}
	}
	
	//check if value v is the sub array from 0-range of the array g
	//if g is NULL then g is the genotype g, if the range is -1 then is from range 0-size
	bool have( int v, int range = -1, int * g = NULL)
	{
		if (g == NULL)
			g = this->g;
			
		if ( range == -1)
			range = size;
		for(int i = 0; i < range; i++){
			if( g[i] == v)
				return true;
		}
		return false;
	}
	
	//try to set an array of genotype values, if the genotype is invalid reset genotype raturn false
	//if the genotype is valid return true
	bool set(int val [])
	{
		for(int i = 0; i < size; i++){
			if( have(val[i], i) ){
				reset();
				return false;
			}
			g[i] = val[i];
		}
		_conflicts = -1;
		return true;
	}
	
	//set genotype data withoud first check
	void setGenotype(int val [])
	{
		for(int i = 0; i < size; i++){
			g[i] = val[i];
		}
		_conflicts = -1;
	}
	
	//return the numbers of conflicts on the genotype
	float conflicts()
	{
		if (_conflicts == -1){
			_conflicts = 0;
			for(int j = 0; j < size-1; j++)
			{
				for( int i = j+1; i < size; i++)
				{
					if( g[j] == (g[i]+(i-j)) || g[j] == (g[i]-(i-j)))
						_conflicts++;
				}
			}
		}
		return  _conflicts;
	}	
	
	int& operator[](unsigned int i){ return g[i];}
	
	//make genotype to string representation
	std::string str()
	{
		std::string out;
		out += "[ ";
		for(int i = 0; i < size; i++)
		{
			out += str::to_string( g[i] );
			if( i < size-1)
				out += ", ";
		}
		out += " ]";
		return out;
	}
	
	//make genotype to string representation
	std::string str() const
	{
		std::string out;
		out += "[ ";
		for(int i = 0; i < size; i++)
		{
			out += str::to_string( g[i] );
			if( i < size-1)
				out += ", ";
		}
		out += " ]";
		return out;
	}
	std::string draw()
	{
		std::string out;
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
				if( g[j] == i)
					out += "+";
				else
					out += "-";
			}
			out += "\n";
		}
		return out;
	}


	void operator=(const genotype &rhs)
	{
		init(rhs.size);
		setGenotype(rhs.g);
	}

	bool operator == (const genotype& rhs) const { return this->str()==rhs.str(); }
	bool operator != (const genotype& rhs) const { return !( (*this) == rhs) ; }
	bool operator <  (const genotype& rhs) const { return  this->str() < rhs.str(); }
	bool operator >  (const genotype& rhs) const { return  rhs < (*this); }
	bool operator <= (const genotype& rhs) const { return  !( (*this) > rhs);}
	bool operator >= (const genotype& rhs) const { return  !( (*this) < rhs); }
};





//typedef AVLTree< pairKeyComp<float,genotype> > queue;
typedef pairKeyComp<float,genotype> MyPair;
typedef heap<MyPair, sub::MIN> minheap;
typedef heap<MyPair, sub::MAX> maxheap;
//typedef heap<pairKeyComp<float,genotype>, sub::MIN> minheap;
static const float e = 0.000001f;

class NQueens
{
private:
	//mutation rate
	float mut_prob;
	
	//numbers of queens
	int N;
	
	//population zise
	int pop_size;
	
	//population continer
	minheap population;
	
	//to don't have duplicates genotypes
	set<genotype> p_set;
	
	//random make a float between 0,1 return true if random float is lest or equals to rate
	bool prob( float rate)
	{
		return randF() <= rate;
	}
	
public:
	NQueens(int N, float mut_rate)
	{
		this->N = N;
		this->pop_size =  (N >= 10 ? N * 10 : 100);
		this->mut_prob = mut_rate;
		
		initPopulation();
	}
	
	//initialize population
	void initPopulation()
	{
		int * geno =getSequence();
		for(int i = 0; i < pop_size; )
		{
			int * t = makePermutation(geno, N);
			delete [] geno;
			geno = t;
			genotype g(N);
			if( g.set(geno) && p_set.insert(g).second){
				MyPair p( fitness(g) , g);
				population.push( p );
				i++;
			}
		}
		delete [] geno;
	}
	
	
	genotype defaultGenotype()
	{
		genotype geno(N);
		int * s = getSequence();
		geno.setGenotype(s);
		delete [] s;
		return geno;
	}
	
	genotype randomGenotype()
	{
		bool find = true;
		int * t =getSequence();
		genotype g(N);
		while( find)
		{
			int * geno = makePermutation(t, N);
			delete [] t;
			
			if(g.set(geno))
				find = false;
			t = geno;
		}
		delete [] t;
		return g;
	}
	
	//return a permutation of the array given
	int * makePermutation( int * arr, int size)
	{
		int * _new = new int[size];
		for( int i = 0; i < size; i++)
		{
			_new[i] = arr[i];
		}
		
		for( int i = 0; i < size/4; i++)
		{
			int a = std::rand()%size;
			int b = std::rand()%size;
			int t = _new[a];
			_new[a] = _new[b];
			_new[b] = t;
			
		}
		return _new;
	}
	
	//return an array sequence with the values 0 to (N-1)
	int* getSequence()
	{
		int * arr = new int[N];
		for( int i = 0; i < N; i++)
		{
			arr[i] = i;
		}
		return arr;
	}
	
	float fitness( genotype & g)
	{
		return 1.0f/(g.conflicts() + e );
	}
	
	
	genotype populationUpdate()
	{
		genotype best = defaultGenotype();
		
		maxheap parents = getMatingPool();
		while(!parents.empty())
		{
			genotype p_a = parents.pop().value;
			genotype p_b = parents.pop().value;
			if( p_a == p_b )
				continue;
			int i = std::rand()%N;
			//create two new childs
			genotype child_a(N, i, p_a, p_b);
			genotype child_b(N, i, p_b, p_b);
			
			//random mutation
			if( prob(  mut_prob) )
				mutate( child_a);
			if(prob( mut_prob) )
				mutate( child_b);
			//if child_a is a new permutation add it to the population
			if( p_set.insert(child_a).second )
				population.push( MyPair( fitness(child_a),child_a));
			//if child_a is a new permutation add it to the population
			if( p_set.insert( child_b).second)
				population.push( MyPair(fitness(child_a),child_a));
				
			if( fitness(child_a) > fitness(best))
				best = child_a;
			if( fitness(child_b) > fitness(best))
				best = child_b;
			
		}
		
		while( population.size() != pop_size)
		{
			//if population is biger that required population, 
			//remove the genotypes with the wors fitness
			if( population.size() > pop_size)
				population.pop();
				
			else{
				//if populations is lest, then create a random genotype to add to population
				genotype g = randomGenotype();
				if( p_set.insert( g ).second)
					population.push(MyPair( fitness(g), g));
			}
		}
		return best;
		
	}
	
	//select two random postion on the genotype and swap values
	void mutate(genotype & g)
	{
		int a = std::rand()%N;
		int b = std::rand()%N;
		g.swap(a,b);
	}
	
	//generate mating pool:
	//select 3 random genotypes
	//run the tornament, add winner genotype to the mating pool if the genotype does not exist on the mating pool
	maxheap getMatingPool()
	{
		maxheap heap;
		set<genotype> temp;
		while( heap.size() < pop_size*.05f)
		{
			genotype a = randomSelection();
			genotype b = randomSelection();
			genotype c = randomSelection();
			genotype w = runTournament( a, b, c );
			if(temp.insert(w).second){
				heap.push( MyPair( fitness(w), w));
			}
		}
		return heap;
	}
	
	//select wich of the three genotype have a higher fitness
	genotype & runTournament( genotype& a, genotype& b, genotype& c)
	{
		if( fitness(a) > fitness(b))
		{
			if(fitness(a) > fitness(c))
				return a;
			return c;
		}
		if( fitness(b) > fitness(c))
			return b;
		return c;
	}
	
	//select random genotype from the population
	genotype randomSelection()
	{
		int size = population.size();
		return population[ std::rand()%size].value;
	}
	
};















#endif
