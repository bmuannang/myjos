//io.c
void cgaputc(int c);
void setCursor();
void clearScreen();
void showWelcome();
void panic();
void printint(int xx, int base, int sign);
void consputc(int c);

//string.c
void* memmove(void*, const void*, uint);
void* memset(void*, int, uint);
int strlen(const char *s);

//mm.c
void kFreeVMInit(void *vstart, void *vend, int first);
void freeVMRange(void *vstart, void *vend);
void kFreePage(char *v);
void kPageTableInit(void);
int mapPages(pde_t *pgdir, void *va, uint size, uint pa, int perm);
void switchKVM(void);
pde_t* setupKVM(void);
char* kAlloc(void);
pte_t * walkPGDir(pde_t *pgdir, const void *va, int alloc);


//工具
#define LENGTH(x) (sizeof(x)/sizeof((x)[0]))


