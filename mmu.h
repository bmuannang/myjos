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
