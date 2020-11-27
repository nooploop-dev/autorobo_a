#include "key.h"

/*硬件连接：KEY1 ----PE6
            KEY2 ----PE5
            KEY3 ----PE4
            KEY4 ----PE3
*/
uint8_t key_delay_count=0;               //按键软件防抖延时计数变量
uint8_t mode_selection=0;                //模式切换变量 0：遥控模式  1：角度跟随模式  2：角度距离全跟随模式  3：定位目标点移动模式



