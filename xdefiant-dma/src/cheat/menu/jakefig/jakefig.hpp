#pragma once

#include <filesystem>
#include <fstream>
#include <istream>

#include "../menu.hpp"
#include "../../../utils/dependencies/json/json.hpp"

template <typename T>
struct item_t
{
    T* pointer;
    std::string category;
    std::string name;
    T default_value;
};

namespace jakefig
{
    inline std::vector<item_t<bool>> booleans;
    inline std::vector<item_t<int>> ints;
    inline std::vector<item_t<float>> floats;

    inline std::vector<std::string>cache{ };

    void add_item( bool* pointer, std::string category, std::string name, bool def_value );
    void add_item( int* pointer, std::string category, std::string name, int def_value );
    void add_item( float* pointer, std::string category, std::string name, float def_value );

    void initialize( );

    bool load( std::string path );
    bool save( std::string path );
};