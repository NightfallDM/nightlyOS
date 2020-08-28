#ifndef __MMU_H_
#define __MMU_H_

// see more detail in "test/PGROUNDUP_test/result*"
#define PGROUNDUP(sz) ((sz)+PAGESIZE-1) & ~(PAGESIZE-1)
//the first page address after address of "sz"

#define PGROUNDDOWN(sz) ((sz) & ~(PAGESIZE-1))
//the first page address before address of "sz"

#endif //end __MMU_H_
