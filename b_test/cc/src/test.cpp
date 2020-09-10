/*==================================
*   Copyright (C) 2018 All rights reserved.
*   
*   文件名称：test.cpp
*   创 建 者：
*   创建日期：2018年01月18日
*   描    述：
*
================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string/classification.hpp>

using namespace std;

int split(string str,string split, vector<string> &out)
{
	boost::split( out, str, boost::is_any_of(split), boost::token_compress_on );
    return 0;
}

int main()
{
	string bbqId = "1801191416158de13e795103BB164967";
	string robotId = "1801191416158de13e795103RI164966";
	cout << " robotId RI:" << robotId.find("RI") << endl;
	cout << " bbqId BB:" << bbqId.find("BB") << endl;
	cout << " robotId BB:" << robotId.find("BB") << endl;
	cout << "  bbqId RI:" << bbqId.find("RI") << endl;

	string b = "";
	int a = 10;
	std::stringstream ss;
	ss << a;
	ss << "aaaaa" << endl;
	ss << "bb" << endl;
	
	cout << ss.str() << endl;

	string c = ss.str();
	b = ss.str();
	cout << c << endl;
	cout << b << endl;
	string d = "";
	ss >> d;
	cout << d << endl;
	for (a = 1; a < 20; a+=2){
		cout << a << endl;
	}

	vector<string> vStr;
	b = "123/123/123/123";
	d = "/";
    split(b,d,vStr);
    for (auto e: vStr){
        cout << e << endl;
    }


	uint64_t ua = 20;
	uint64_t ub = ua>>60;
	uint64_t uc = ub<<60;
	cout << "----ua:" << (ua) << endl;
	cout << "ub:" << (ub) << endl;
	cout << "uc:" << (uc) << endl;

    int ud = -ua;
    cout << "ud:" << ud << endl;

	string cpa = "A";
	string cpc = "A";
	string cpb = "B";
	cout << "--------cmp--------------" << endl;
	cout << strcmp(cpa.c_str(), cpb.c_str()) << endl; // -1
	cout << strcmp(cpb.c_str(), cpa.c_str()) << endl; // 1
	cout << strcmp(cpc.c_str(), cpa.c_str()) << endl; // 0
}
