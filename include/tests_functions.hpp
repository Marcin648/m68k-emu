#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <stdexcept>

#define _TEST_ERROR_STRING (std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + std::string(" in ") + std::string(__PRETTY_FUNCTION__))

#define TEST_TRUE(result) _TEST_TRUE((result), _TEST_ERROR_STRING)
#define TEST_FALSE(result) _TEST_TRUE(!(result), _TEST_ERROR_STRING)
#define TEST_THROW(exception, function_body) _TEST_THROW<exception>([&]()function_body, _TEST_ERROR_STRING)
#define TEST_NO_THROW(function_body) _TEST_NO_THROW([&]()function_body, _TEST_ERROR_STRING)

std::string test_name = "general";
std::string test_label = "";

void TEST_NAME(std::string text){
    test_name = text;
}

void TEST_LABEL(std::string text){
    test_label = text;
}

void _TEST_TRUE(bool result, std::string error){
    std::cout << "Test[" + test_name + "]: " + test_label;
    if(!result){
        std::cout << " > FAIL" << std::endl;
        throw std::runtime_error(error.c_str());
    }
    std::cout << " > PASS" << std::endl;
}

template<typename E>
void _TEST_THROW(std::function<void()> function, std::string error){
    bool is_catch = false;
    error = error + " no throw. " + typeid(E).name() + " expected.";
    try{
        function();
    }catch (const E& e){
        is_catch = true;
    }
    _TEST_TRUE(is_catch, error);
}

void _TEST_NO_THROW(std::function<void()> function, std::string error){
    bool is_catch = false;
    try{
        function();
    }catch (const std::exception& e){
        error = error + " no throw expected.\n  what(): " + e.what();
        is_catch = true;
    }
    _TEST_TRUE(!is_catch, error);
}