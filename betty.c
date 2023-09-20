#include "shell.h"
#include <stdio.h>

/**
 * print_alx_africa - Recursive function to print "alx africa"
 * @n: Number of times to print
 */
void print_alx_africa(int n)
{
    if (n <= 0)
        return;

    printf("alx africa\n");
    print_alx_africa(n - 1);
}

/**
 * main - Entry point
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
    print_alx_africa(5);

    return (0);
}

