#include "common.h"
#include "include.h"

void find6Point(uint8 *src)
{
    uint8 line_num[] = {26, 30, 38};
    Site_t site[3];
    int32 temp_left = 0, temp_right = 0;

    uint8 flag = 1;
    for (int s = 0; s < 3; s++)
    {
        int i = line_num[s];
        int j = 40;
        while (j < 80 && 0 == src(i, j))
        {
            j++;
        }
        if (j == 80)
        {
            flag = 0;
            break;
        }
        site[s].x = i;
        site[s].y = j;
    }
    if (flag)
        temp_right = getCurvature(site);
    flag = 1;
    for (int s = 0; s < 3; s++)
    {
        int i = line_num[s];
        int j = 40;
        while (j >= 0 && 0 == src(i, j))
        {
            j--;
        }
        if (j < 0)
        {
            flag = 0;
            break;
        }
        site[s].x = i;
        site[s].y = j;
    }
    if (flag)
        temp_left = getCurvature(site);
    site[0].x = 10;
    site[0].y = 80;
    LCDShowNum(site[0], (uint32)(temp_left > 0 ? temp_left : -1 * temp_left));
    site[0].x = 60;
    LCDShowNum(site[0], (uint32)(temp_right > 0 ? temp_right : -1 * temp_right));
}