#pragma once
#include <string>

struct GLFWwindow;

std::string	open_file(GLFWwindow* window, const char* filter);
std::string	save_file(GLFWwindow* window, const char* filter);