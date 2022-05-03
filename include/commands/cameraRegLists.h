#include <stdint.h>
#include <stdio.h>

typedef struct MT9D111RegLst {
	unsigned char ucPageAddr;
	unsigned char ucRegAddr;
	unsigned short usValue;
} s_RegList;



s_RegList test_reg_list1[] = {
    {2,0,2},
    {2,6,8},
};
s_RegList test_reg_list2[] = {
    {1,3,5},
    {1,7,9},
    {1,11,13},
};
#define NUM_REG_LISTS 2
s_RegList * reg_lists[NUM_REG_LISTS] = {
    test_reg_list1,
    test_reg_list2
};


uint8_t getRegisterListSize(int listIdx){
    return (uint8_t) sizeof(reg_lists[listIdx]) / sizeof(s_RegList) * 3;
}
void getRegisterList(int listIdx, uint8_t * data){
    s_RegList * regList = reg_lists[listIdx];
    int listSize = (int) getRegisterListSize(listIdx);
    int i = 0;
    while(i < listSize){
        data[i++] = (uint8_t) regList->ucPageAddr;
        data[i++] = (uint8_t) regList->ucRegAddr;
        data[i++] = (uint8_t) regList->usValue;
        regList++;
    }
}