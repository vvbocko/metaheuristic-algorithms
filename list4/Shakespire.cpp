#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>


void ShakespireAlgorithm()
{

}

int main()
{
    // 1. zdanie
    // 2. vector charów
    // 3. Random permutation of lowercase letters
    // 4. sprawdzanie czy na danej pozycji jest dany char
    //
    std::string sentence = "";
    std::getline(std::cin, sentence);
    std::cout << sentence << std::endl;
    
    int charCount = 0;
    std::vector<char> quote(charCount);

    for(char c : sentence)
    {
        quote[charCount] = c;
        charCount++;
    }
    std::cout << charCount << std::endl;

    // for(int i = 0; i < charCount; i++)
    // {
    //     std::cout << quote[i] << std::endl;
    // }

    // std::vector<char> quote = {'T', 'o', ' ', 'b', 'e', ' '};
    // std::vector<char> randomPermutation(27);
}