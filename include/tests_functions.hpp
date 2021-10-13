#pragma once
#include <string>
#include <functional>
#include <stdexcept>

#define _TEST_ERROR_STRING (std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + std::string(" in ") + std::string(__PRETTY_FUNCTION__))

#define TEST_TRUE(result) _TEST_TRUE((result), _TEST_ERROR_STRING)
#define TEST_FALSE(result) _TEST_TRUE(!(result), _TEST_ERROR_STRING)
#define TEST_THROW(exception, function_body) _TEST_THROW<exception>([&]()function_body, _TEST_ERROR_STRING)

void _TEST_TRUE(bool result, std::string error){
    if(!result){
        throw std::runtime_error(error.c_str());
    }
}

template<typename E>
void _TEST_THROW(std::function<void()> function, std::string error){
    bool is_catch = false;
    try{
        function();
    }catch (const E& e){
        is_catch = true;
    }
    _TEST_TRUE(is_catch, error);
}


