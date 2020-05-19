/* <editor-fold desc="MIT License">

Copyright(c) 2020 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

void mapPointers(std::istream& in, std::ostream& out, bool map_to_numbers)
{
    std::map<std::string, size_t> pointerMap;
    // make sure the null pointer always maps to pointer_0

    std::string current_pointer_string;
    char previous_char = 0;
    bool parsing_pointer = false;
    while(in)
    {
        int current_get = in.get();
        if (current_get<0 || current_get>255)
        {
            continue;
        }

        char current_char = current_get;

        if (parsing_pointer)
        {
            if ((current_char>='0' && current_char<='9') ||
                (current_char>='a' && current_char<='f') ||
                (current_char>='A' && current_char<='F'))
            {
                // valid heixdimal
                current_pointer_string.push_back(current_char);
            }
            else
            {
                parsing_pointer = false;

                if (current_pointer_string=="0")
                {
                    out<<"NULL"<<current_char;
                }
                else if (map_to_numbers)
                {
                    size_t pointer_number = pointerMap.size();
                    if (auto itr = pointerMap.find(current_pointer_string); itr != pointerMap.end())
                    {
                        pointer_number = itr->second;
                    }
                    else
                    {
                        pointerMap[current_pointer_string] = pointer_number;
                    }

                    out<<"pointer_"<<pointer_number<<current_char;
                }
                else
                {
                    out<<"pointer"<<current_char;
                }
            }
        }
        else if (previous_char=='0')
        {
            if (current_char=='x')
            {
                parsing_pointer = true;
                current_pointer_string = "";
            }
            else
            {
                out.put(previous_char);
                if (current_char!='0') out.put(current_char);
            }
        }
        else if (current_char!='0')
        {
            // we don't want to immediate output a 0 as it could be preceeding a x and need converting to a pointer string
            // bit if we get here so the current_chat isn't a 'x' then not ouptut the previous 0 was inappropriate to now output it.
            if (previous_char=='0') out.put(previous_char);

            out.put(current_char);
        }

        previous_char = current_char;
    }
    if (previous_char=='0')
    {
        // we don't want to ignore a possible trailing '0'
        out.put(previous_char);
    }
}

int main(int argc, char** argv)
{
    const char* input_filename = nullptr;
    const char* output_filename = nullptr;
    bool map_to_numbers = false;
    bool help = false;

    for(int i = 1; i<argc; ++i)
    {
        if (std::string("-n") == argv[i]) map_to_numbers = true;
        else if (std::string("-h") == argv[i]) help = true;
        else
        {
            if (input_filename) output_filename = argv[i];
            else input_filename = argv[i];
        }
    }

    if (help)
    {
        std::cout<<"Command line usage:"<<std::endl;
        std::cout<<argv[0]<<" [-h]                                 // Print help."<<std::endl;
        std::cout<<argv[0]<<" [-n]                                 // Enable mapping to pointer_number, otherwise pointers map to pointer."<<std::endl;
        std::cout<<argv[0]<<" [-n] < input > output                // Read stdin, remap pointers and write to stdout."<<std::endl;
        std::cout<<argv[0]<<" [-n] filename                        // In place replacement of all pointers."<<std::endl;
        std::cout<<argv[0]<<" [-n] input_filename output_filename  // Read input_filename, remap pointers and write to output_filename."<<std::endl;
        std::cout<<"vsgdraw -d -a | "<<argv[0]<<" > output             // Run Vulkan graphics application redirecting console output through pointermap"<<std::endl;
        return 1;
    }

    if (!input_filename)
    {
        mapPointers(std::cin, std::cout, map_to_numbers);
    }
    else if (!output_filename)
    {
        std::ifstream fin(argv[1]);
        if (!fin)
        {
            std::cout<<"Warning: unable able to read file : "<<argv[1]<<std::endl;
            return 0;
        }

        std::stringstream stream_copy;
        stream_copy << fin.rdbuf();

        std::ofstream fout(argv[1]);
        mapPointers(stream_copy, fout, map_to_numbers);
    }
    else
    {
        std::ifstream fin(input_filename);
        if (!fin)
        {
            std::cout<<"Warning: unable able to read file : "<<argv[1]<<std::endl;
            return 0;
        }

        std::ofstream fout(output_filename);

        mapPointers(fin, fout, map_to_numbers);
    }

    return 1;
}
