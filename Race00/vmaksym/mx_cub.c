#include <stdio.h>
#include <unistd.h>

void mx_printchar(char c);

void mx_cube(int n)
{
    if (n <= 1)
    {
        return;
    }

    //int straight_slash = n;
    int minus = n * 2;
    int slash = n / 2;

    for (int i = 0; i < 1 + slash; i++)
    {
        mx_printchar(' ');
    }

    mx_printchar('+');
    for(int m = 0; m < minus; m++)
    {
        mx_printchar('-');
    }
    mx_printchar('+');
    mx_printchar('\n');

    for (int i = 0; i < slash; i++)
    {
        for (int j = 0; j < slash - i; j++)
        {
            mx_printchar(' ');
        }
        mx_printchar('/');
        for (int j = 0; j < minus; j++)
        {
            mx_printchar(' ');
        }
        mx_printchar('/');
        for (int j = 0; j < i; j++)
        {
            mx_printchar(' ');
        }
        mx_printchar('|');
        mx_printchar('\n');
    }

    mx_printchar('+');
    for(int m = 0; m < minus; m++)
    {
        mx_printchar('-');
    }
    mx_printchar('+');
    for (int i = 0; i < slash; i++) 
    {
      mx_printchar(' ');
    }
    mx_printchar('|');
    mx_printchar('\n');

    for (int i = 0; i < n - slash; i++)
    {
        mx_printchar('|');
        for (int j = 0; j < minus; j++)
        {
            mx_printchar(' ');
        }
        mx_printchar('|');

      for (int j = 0; j < slash; j++) 
        {
        mx_printchar(' ');
      }
      if (i == n - slash - 1) 
        {
        mx_printchar('+');
      }
      else 
        {
        mx_printchar('|');
      }
      mx_printchar('\n');
    }

    for (int i = 0; i < slash; i++) 
    {
      mx_printchar('|');
      for (int j = 0; j < minus; j++) 
        {
        mx_printchar(' ');
      }
      mx_printchar('|');
      for (int j = 0; j < slash - 1 - i; j++) 
        {
        mx_printchar(' ');
      }
      mx_printchar('/');
      mx_printchar('\n');
    }

    mx_printchar('+');
    for(int m = 0; m < minus; m++)
    {
        mx_printchar('-');
    }
    mx_printchar('+');
    mx_printchar('\n');
}

int main(void)
{
	mx_cube(21);
	return 0;
}
