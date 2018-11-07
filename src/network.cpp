//Morgane Chapuis
#include "network.h"
#include "random.h"
#include <iostream>

//pas d'initialisation du constructeur: constructeur par défaut

void Network::resize(const size_t &n){
	
	values.resize(n, RNG.normal());
}

bool Network::add_link(const size_t &a, const size_t &b){
	bool node_a = false;
	bool node_b = false;
	bool lien = false;
	
	if (a == b){
		return false;
	}
	
	if (a < values.size()){
		node_a = true;
	} 
	 
	if (b < values.size()){
		node_b = true;
	}
    
    auto rang = links.equal_range(a);
    
    for (auto i = rang.first; i != rang.second; ++i){
		if ( i->second == b){
			lien = true;
		}
	}
    
	if((node_a) and (node_b) and (lien == false)){
		links.insert( std::pair<size_t,size_t> (a, b) );
		links.insert( std::pair<size_t,size_t> (b, a) );
		return true;
	} else {
		return false;
	  }	
}

size_t Network::random_connect(const double &mean_deg ){
	
	if(!links.empty()) {
		std::multimap<size_t,size_t>::iterator it;
		for (it=links.begin(); it !=links.end(); ++it){
		links.erase(it);
		}
	} 
	
	size_t liens_crees (0);
	
	///On crée des nodes aléatoirement, répertoiriées dans un tableau
		std::vector<int> int_aleatoires (values.size(), 0.0);
		RNG.uniform_int(int_aleatoires, 0, values.size());
		
	for(unsigned int i(0); i < values.size(); ++i){ 
	///Pour chaque node créée on lui associe des liens:
		///a) definition du nb de liens à faire:
		int degree_n = RNG.poisson(mean_deg);
		
		///b) on definit toutes les nodes avec lesquelles la node i doit se lier
		std::vector<int> nodes_aleatoires (values.size(), 0.0);
		nodes_aleatoires = int_aleatoires;
		nodes_aleatoires.resize(degree_n);
		
		///c) création des liens:
		for(int j(0); j < degree_n; ++j){
			///ici aller voir moodle
			if(add_link(int_aleatoires[i], nodes_aleatoires[j])){
					liens_crees += links.count(i);
			}
		}
	}
	 return liens_crees;
}

size_t Network::set_values(const std::vector<double> &new_nodes){
	
	size_t nb_nodes_reset = new_nodes.size();
	values = new_nodes;
	return nb_nodes_reset;	
}

size_t Network::size() const{
	if(! values.empty()){return values.size();}
	else {  int vide (0);
		return vide;}
}

size_t Network::degree(const size_t &n) const{
	return links.count(n);
}

double Network::value(const size_t &n) const{
	return values[n];
}

std::vector<double> Network::sorted_values() const{
	std::vector<double> values_sorted = values;
    std::sort(values_sorted.begin(), values_sorted.end(), std::greater<double>());
    return values_sorted; 
}

std::vector<size_t> Network::neighbors(const size_t &a) const{
//pour une key rentrée a, on sort un tableau listant tous ses liens

 std::vector<size_t> voisins;
 
 if ( a < size()){
		 
	auto voisin = links.equal_range(a);
		
	for (auto i = voisin.first; i != voisin.second; ++i){
			voisins.push_back(i->second);
			
	}
 }	
  return voisins;
	
}

