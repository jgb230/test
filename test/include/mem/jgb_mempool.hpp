#ifndef JGB_MEMPOOL_HPP
#define JGB_MEMPOOL_HPP
//对齐
#define JGB_ALIGN(size, boundary) \
    (((size) + ((boundary) - 1)) & ~((boundary) - 1))

#define JGB_ALIGN_D(size) APR_ALIGN(size, 8)



#endif