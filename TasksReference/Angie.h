#pragma once
#include <iostream>

class Angie {

public:

	template<typename T> // polymorph my console logger
	static void log(T value) {
		std::cout << value << std::endl;
	}

	Angie() {}
	~Angie() {}


};


