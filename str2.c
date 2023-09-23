#include "shell.h"

/**
 * _strcpy - Copy a string to a destination buffer.
 * @dest: The destination buffer.
 * @src: The source string.
 *
 * Return: Pointer to the destination buffer.
 */
char *_strcpy(char *dest, char *src)
{
	char *ret = dest;

	if (dest == src || src == 0)
		return (dest);
	while ((*dest++ = *src++))
		;
	return (ret);
}

/**
 * _strdup - Create a copy of a string.
 * @str: The string to be copied.
 *
 * Return: Pointer to the copied string.
 */
char *_strdup(const char *str)
{
	char *ret;
	int length = 0;

	if (str == NULL)
		return (NULL);
	while (str[length])
		length++;
	ret = malloc(sizeof(char) * (length + 1));
	if (!ret)
		return (NULL);
	return (_strcpy(ret, str));
}

/**
 *_puts - Print a string to the standard output.
 *@str: The string to be printed.
 *
 * Return: Nothing
 */
void _puts(char *str)
{
	if (!str)
		return;
	while (*str)
		_putchar(*str++);
}

/**
 * _putchar - Write a character to the standard output.
 * @c: The character to be written.
 *
 * Return: On success, return 1. On error, return -1 and set errno appropriately.
 */
int _putchar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(1, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

