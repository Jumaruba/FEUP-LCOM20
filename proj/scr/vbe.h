#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

/**
 * @file vbe.h
 * @brief This file contains macros for the vbe implementation
 */


#define VBE_FUNCTION            0x4F00      /**<@brief function to call*/ 
#define VBE_MODE_INFO           0x01        /**<@brief return VBE mode information*/
#define VBE_SET_MODE            0x02        /**<@brief set VBE mode*/
#define INT                     0x10        /**<@brief call the function int*/ 
#define INDEX_MODE              0x105       /**<@brief set the index mode*/
#define CTRL_INFO               0x00        /**<@brief get the control info*/
#define OFFSET                  0x0000FFFF  /**<@brief off set of the VbeBlockInfo pointers*/ 
#define TRANSPARENT             5
#endif /* _LCOM_VBE_H */
