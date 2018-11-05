#include "network.h"
#include <map>
#include <vector>
#include "random.h"


bool not_linked(const Network&, const size_t&, const size_t&);

void Network::resize(const size_t& i) { 
	values = std::vector<double> (i); 
	RandomNumbers RNG;
	RNG.normal(values, 0.0, 1.0);
}

bool Network::add_link(const size_t& a, const size_t& b) {
	if ( (a!=b) and (not_linked(*this, a, b)) and (a<values.size()) and (b<values.size()) ){
		links.insert({{a, b}, {b, a}});
		return true;
	} else return false;
}

size_t Network::random_connect(const double& mean_deg){
	links.clear();
	size_t number_of_links(0);
	size_t degree;
		
	for(size_t i(0); i<values.size(); ++i){
		do { degree=RNG.poisson(mean_deg); }
		while(degree>(values.size()-1)); 
		std::vector<int> new_links(degree);
		for (size_t j(0); j<degree; ++j){ 
			do{
			RNG.uniform_int(new_links, 0, (values.size()-1)); //on a remplit le tableau avec des nb random compris dans les values
			} while((add_link(i, new_links[j])) ==false );
			++number_of_links;			
		} 
	}
	return number_of_links;
}

size_t Network::set_values(const std::vector<double>& new_values) {
	resize(new_values.size());
	for (size_t i(0);  i<values.size(); ++i) {values[i]=new_values[i]; }
	return values.size();
}

size_t Network::size() const { return values.size(); }

size_t Network::degree(const size_t &_n) const {
	if (!links.empty()) return links.count(_n);
	else return 0;
}

double Network::value(const size_t &_n) const { 
	if (values.size()>_n) return values[_n]; 
	else return 0.0;
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> tmp = values;
	std::sort(tmp.begin(), tmp.end());
	std::reverse(tmp.begin(), tmp.end());
	return tmp;
}


std::vector<size_t> Network::neighbors(const size_t& n) const{
	std::vector<size_t> neighbors;
	if (!links.empty()){
		auto range = links.equal_range(n);
		for (auto i = range.first; i!=range.second; ++i){
			neighbors.push_back(i->second);
		}
	}
	return neighbors;
}

bool not_linked(const Network& net, const size_t& a, const size_t& b){
	std::vector<size_t> a_links(net.neighbors(a));
	bool not_linked(true);
	for (auto node: a_links){ if(node==b)  not_linked=false; }
	return not_linked;
}
