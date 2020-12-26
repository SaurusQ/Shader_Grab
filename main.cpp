
#include <iostream>
#include <fstream>
#include <ios>
#include <string>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "<infile> [searchterm] [outfile]" << std::endl;
        return -1;
    }

    std::ifstream infile(argv[1], std::ios::binary);
    std::string outname = "out.txt";
    if(argc > 3) outname = argv[4];
    std::ofstream outfile(outname);

    if(!infile.is_open())
    {
        std::cout << "Can't open file: " << argv[1] << std::endl;
        return -1;
    }

    char c;
    int shaderNum = 0;
    std::string start = "#version";
    if(argc > 2) start = argv[3];
    char* buff = new char[start.length() + 1];
    buff[start.length()] = '\0';

    while(infile.get(c))
    {
        if(c == start[0])
        {
            buff[0] = c;
            infile.read(buff + 1, start.length() - 1);            
            //std::cout << buff << "\n" << start << "\n\n" << std::endl;
            if(start.compare(buff) == 0)
            {
                std::cout << "found" << std::endl;
                outfile << "SHADER #" << shaderNum++ << "------------------------------------------------------------" << std::endl;
                outfile << "\n";
                outfile << buff;
                int tabs = 0;
                bool main = false;
                auto insertTabs = [&]()
                {
                    for(int i = 0; i < tabs; i++)
                        outfile << '\t';
                };

                while(!infile.eof() && (!main || tabs > 0))
                {
                    infile.get(c);
                    switch(c)
                    {
                    case ';':
                        outfile << c;
                        outfile << '\n';
                        insertTabs();
                        break;
                    case '{':
                        main = true;
                        outfile << c;
                        tabs++;
                        outfile << '\n';
                        insertTabs();
                        break;
                    case '}':
                        tabs--;
                        outfile << '\n';
                        insertTabs();
                        outfile << c;
                        outfile << '\n';
                        insertTabs();
                        break;
                    default:
                        outfile << c;
                        break;
                    }
                }
                outfile << "\n\n";
            }
        }
    }

    outfile.close();
    infile.close();

    return 0;
}