#pragma once
#include <string>
// Exceptions
class unknown_color : public std::exception {
public:
	unknown_color(const std::string& name) :
		s{ std::string{ "unknown color [" } +name + "]" }
	{}
	const char* what() const override {
		return s.c_str();
	}
private:
	std::string s;
};

class unknown_shape : public std::exception {
public:
	unknown_shape(const std::string& name) :
		s{ std::string{ "unknown color [" } +name + "]" }
	{}
	const char* what() const override {
		return s.c_str();
	}
private:
	std::string s;
};

class invalid_position : public std::exception {
public:
	invalid_position(const std::string& name) :
		s{ std::string{ "unknown color [" } +name + "]" }
	{}
	const char* what() const override {
		return s.c_str();
	}
private:
	std::string s;
};

class read_exception : public std::exception {
public:
	read_exception(const std::string& string) :
		s{ string }
	{}
	const char* what() const override {
		return s.c_str();
	}
private:
	std::string s;
};

class end_of_file : public std::exception {
public:
	end_of_file() {}

	const char* what() const override {
		return s.c_str();
	}
private:
	std::string s = "EOF";
};