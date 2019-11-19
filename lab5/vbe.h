#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_


#define VBE_FUNCTION            0x4F00    //function to call
#define VBE_MODE_INFO           0x01    //return VBE mode information
#define VBE_SET_MODE            0x02    //set VBE mode
#define INT                     0x10
#define INDEX_MODE              0x105 
#define CTRL_INFO               0x00    //get the control info
#define OFFSET                 0x0000FFFF  //off set of the VbeBlockInfo pointers
#endif /* _LCOM_VBE_H */
