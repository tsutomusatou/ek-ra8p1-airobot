#include <sys/stat.h>
#include <errno.h>

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _read(int file, char *ptr, int len)
{
    errno = EINVAL;
    return -1;
}

int _write(int file, char *ptr, int len)
{
    // 必要なら UART 出力に差し替え可能
    return len;
}

void _exit(int status)
{
    while (1) { }
}

void _kill(int pid, int sig)
{
    return;
}

int _getpid(void)
{
    return 1;
}
