#include "shell.h"

/**
 * _strlen - Calculate the length of a string.
 *
 * @s: The string to be measured.
 *
 * Return: The length of the string as an integer.
 */
int _strlen(char *s)
{
    int i = 0;

    if (!s)
        return (0);

    while (*s++)
        i++;
    return (i);
}

/**
 * _strcmp - Compare two strings lexicographically.
 *
 * @s1: The first string for comparison.
 * @s2: The second string for comparison.
 *
 * Return: Negative if s1 < s2, positive if s1 > s2, zero if s1 == s2.
 */
int _strcmp(char *s1, char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

/**
 * starts_with - Determine if a string begins with another string.
 *
 * @haystack: The string to be searched.
 * @needle: The substring to be found.
 *
 * Return: Address of the next character of haystack or NULL.
 */
char *starts_with(const char *haystack, const char *needle)
{
    while (*needle)
        if (*needle++ != *haystack++)
            return (NULL);
    return ((char *)haystack);
}

/**
 * _strcat - Append one string to another string.
 *
 * @dest: The destination string.
 * @src: The source string.
 *
 * Return: Pointer to the destination string.
 */
char *_strcat(char *dest, char *src)
{
    char *ret = dest;

    while (*dest)
        dest++;
    while ((*dest++ = *src++))
        ;
    return (ret);
}

