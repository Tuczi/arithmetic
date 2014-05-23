#include "number/interval.hpp"
#include <iostream>
#include <ostream>

/*!
 * Make comparison of doing simple Math operations (+, -, *, /) on diffrent precision
 */
template <int PRECISION> void make(std::string str1, std::string str2)
{
	tuczi::proper_interval<PRECISION> interval(str1), interval2(str2), tmp;
	std::cout<<"On "<<PRECISION<<" Bytes, initial intervals:\n"<<interval<<"\n"<<interval2<<"\n";
	
	tmp = interval+interval2;
	std::cout<<"Operation + : "<<tmp<<"\n";
	tmp = interval-interval2;
	std::cout<<"Operation - : "<<tmp<<"\n";
	tmp = interval*interval2;
	std::cout<<"Operation * : "<<tmp<<"\n";
	tmp = interval/interval2;
	std::cout<<"Operation / : "<<tmp<<"\n\n";
}
 
int main(int argc, char** argv)
{
	std::string tmp, tmp2;
	std::cin>>tmp>>tmp2;
	
	make<1000>(tmp, tmp2);
	make<100>(tmp, tmp2);
	make<16>(tmp, tmp2);
	make<12>(tmp, tmp2);
	make<10>(tmp, tmp2);
	make<8>(tmp, tmp2);
	
	return EXIT_SUCCESS;
}
