#include <ctype.h>
#include <unistd.h>

int main()
{
    char input[1000];
    int i;
    int rtn;
    while ((rtn = read(0, input, 1000)) > 0)
    {
        for (i = 0; i < rtn; i++)
        {
            if (islower(input[i]))
            {
                input[i] = toupper(input[i]);
            }
        }
        write(1, input, rtn);
    }
    return 0;
}
