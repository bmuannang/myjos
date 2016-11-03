//io.c
void cgaputc(int c);
void setCursor();
void clearScreen();
void showWelcome();
void panic();

//string.c
void* memmove(void*, const void*, uint);
void* memset(void*, int, uint);
int strlen(const char *s);

//mm.c
void kinit(void *vstart, void *vend, int first);
void freeVM(void *vstart, void *vend);
void kfree(char *v);


