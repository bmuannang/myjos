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

#define CR0_PE          0x00000001      // 保护模式
#define CR0_WP          0x00010000      // 写保护
#define CR0_PG          0x80000000      // 分页

#define CR4_PSE         0x00000010      //扩展分页标志，开启:1页大小=4M

#define SEG_KCODE 1  // 内核代码段
#define SEG_KDATA 2  // 内核数据段
#define SEG_KCPU  3  // 内核cpu数据段(每个cpu都有)
#define SEG_UCODE 4  // 用户代码段
#define SEG_UDATA 5  // 用户数据段
#define SEG_TSS   6  // 进程tss段

#define NPTENTRIES      1024    // 页目录表和页表entry数目
#define PGSIZE          4096    // 页大小

#define PDXSHIFT        22      // 一个页目录项管4M, 2^22=4M

#define PTE_P           0x001   // 存在
#define PTE_W           0x002   // 可写
#define PTE_U           0x004   // 用户级
#define PTE_PS          0x080   // 页目录表项中, 开启:1页大小=4M

#define PGSIZE          4096    //一页=4096B
#define PHYSTOP         0xE000000  //物理内存上限=224M

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))
