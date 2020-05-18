#include <iostream>
#include <map>
#include <string>

void mapPointers(std::istream& in, std::ostream& out)
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
                    out<<"NULL";
                }
                else
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
            }
        }
        else if (previous_char=='0' && current_char=='x')
        {
            parsing_pointer = true;
            current_pointer_string = "";
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

    mapPointers(std::cin, std::cout);

    return 1;
}
