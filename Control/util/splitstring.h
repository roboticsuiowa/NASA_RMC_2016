#include <iostream>
#include <string.h>
#include <vector>
#include <sstream> 
#define DEBUG 0

/**
 * Description: function splits the string at the spaces and stores it into a vector
 * Input: the string to be divided up
 * Output: the vector of each string
 */
std::vector<std::string> splitString(std::string str)
{
    std::string buf; // Have a buffer string
    std::stringstream ss(str); // Insert the string into a stream
    std::vector<std::string> tokens; // Create vector to hold our words
    
    while (ss >> buf)
        tokens.push_back(buf);
    
#if DEBUG    
    for(int i=0; i<tokens.size(); i++)
    {
        std::cout<<tokens[i]<<std::endl;
    }
#endif  
	return tokens;  
}
