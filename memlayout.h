#define KERNBASE 0x80000000         // 内核虚拟地址

#ifndef __ASSEMBLER__

static inline uint v2p(void *a) { return ((uint) (a))  - KERNBASE; }
static inline void *p2v(uint a) { return (void *) ((a) + KERNBASE); }

#endif

#define V2P(a) (((uint) (a)) - KERNBASE)
#define P2V(a) (((void *) (a)) + KERNBASE)

#define V2P_WO(x) ((x) - KERNBASE)    // 类似V2P,没有类型转换,用在汇编文件里面
#define P2V_WO(x) ((x) + KERNBASE)    // 类似P2V,没有类型转换,用在汇编文件里面

#define KSTACKSIZE 4096
