// true_conf_task.cpp
// 
//tested with latest versions:
//|*****|Windows 10|WSL - Ubuntu-20.04|
//|MSVC | x64 x86  |                  |
//|GCC  |          |x64               |
//|Clang| x86      |x64               | 

#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <limits>
#include <bitset>
#include <exception>
#include <type_traits>

using namespace std;

#define DEBUG

using DATA_T = short;
using SIZE_T = unsigned long;
using MAP_T = std::map<SIZE_T, DATA_T>;
using VEC_T = std::vector<DATA_T>;

int main()
{
	cout << "TrueConf test task. Chicherin Alexey."<<endl;
#ifdef DEBUG
	auto print = [&](const MAP_T& map, const VEC_T& vec)->void
	{
		cout << " vec:";
		if (vec.size() == 0)cout << "[EMPTY]";
		for (auto& v : vec)cout << v << " ";
		cout << endl;

		cout << " map:";
		if (map.size() == 0)cout << "[EMPTY]";
		for (auto& m : map)cout << m.second << " ";
		cout << endl;

	};
#endif
///0. Input
	auto is_all_digits = [&](const std::string& str)
	{
		if (std::count_if(str.begin(), str.end(), [](unsigned char c) {return std::isdigit(c); })
			==
			str.size()) return true;
		else return false;
	};

	bool is_cin_ok = false;
	std::string s_size;
	SIZE_T i_size;

	while (!is_cin_ok)
	{
		cout << "0. Enter the size of map and vector in range [0," << std::numeric_limits<SIZE_T>::max() << "] : size = ";

		try
		{
			cin >> s_size;

			static_assert(is_same<SIZE_T, unsigned long>::value, "stoul return wrong data type. Change stoul to correct data type conversion.");
			if (is_all_digits(s_size)) i_size = stoul(s_size.data());
			else throw std::out_of_range("Wrong value");
		}

		catch (const std::exception& e)
		{
			std::cerr <<"ERROR: "<< e.what()<<std::endl;
			continue;
		}

		is_cin_ok = true;
	}
///

#ifdef DEBUG
	std::cout << " s_size = " << s_size << std::endl;
	std::cout << " i_size = " << i_size << std::endl;
#endif

///init
	MAP_T mp;
	VEC_T vec;
	static_assert(std::numeric_limits<SIZE_T>::max() <= SIZE_MAX,"(SIZE_T <= SIZE_MAX) != true. Possible loss of data on vector insert. Type of SIZE_T should be less than std::size_t");
///

/// 1. Generate 
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_int_distribution<DATA_T> distr(1, 9);//[1,9]

	cout << "1. Generate random values. Fill map and vector. - ";
	try 
	{
		for (SIZE_T i = 0; i < i_size; i++)
		{
			vec.emplace_back(distr(gen));
			mp.emplace(std::make_pair(i, distr(gen)));
		}
	}
	catch (const std::exception& e)
	{
		cerr << "ERROR: " << e.what() << endl;
		return 0;
	}
	cout << "OK" << endl;
///

#ifdef DEBUG
	print(mp, vec);
#endif

/// 2. Remove 
	std::uniform_int_distribution<DATA_T> distr_rem(0, 15);//[0,15]
	SIZE_T i_remove = distr_rem(gen);
	if (i_remove > i_size)i_remove = i_size;

	cout << "2. Remove " << i_remove<<" values from map::end and " << i_remove << " from vector::end. - ";
	try 
	{
		for (SIZE_T i = 0; i < i_remove; i++)
		{
			vec.erase(--vec.cend());
			mp.erase(--mp.cend());
		}
	}
	catch (const std::exception& e)
	{
		cerr << "ERROR: " << e.what() << endl;
		return 0;
	}
	cout << "OK" << endl;
///

#ifdef DEBUG
	print(mp, vec);
#endif

/// 3. Synchronize
	//true in bitset mean that container store specific num (9876543210)
	bitset<10> bs_vec; 
	bitset<10> bs_map;
	bitset<10> bs_sync;

	cout << "3. Synchronize values in map and vector. - ";
	try 
	{
		if (vec.size() != mp.size())
			throw range_error("vec.size() != mp.size()");

		for (SIZE_T i = 0; i < vec.size(); i++)
		{
			bs_vec.set(vec[i]);
			bs_map.set(mp[i]);
		}

		bs_sync = bs_vec & bs_map;

		//remove non bs_sync nums
		for (auto it = vec.begin(); it != vec.end();)
			if (!bs_sync.test(*it))
				it=vec.erase(it);
			else it++;

		for (auto it = mp.begin(); it != mp.end();)
			if (!bs_sync.test(it->second))
				it=mp.erase(it);
			else it++;

	}
	catch (const std::exception& e)
	{
		cerr << "ERROR: " << e.what() << endl;
		return 0;
	}
	cout << "OK" << endl;
/// 

#ifdef DEBUG
	cout << " numbers  9876543210" << endl;
	cout << " bs_vec = " << bs_vec.to_string()  << endl;
	cout << " bs_map = " << bs_map.to_string()  << endl;
	cout << " bs_sync= " << bs_sync.to_string() << endl;
	print(mp, vec);
#endif

	return 0;
}
