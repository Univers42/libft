void    ft_assert(int cond)
{
    volatile char *nullptr;

    if (!cond)
    {
        nullptr = (char *)0;
        *nullptr = 0;
    }
}
