#include "common.h"
#include "include.h"

void find6Point(uint8 *src)
{
    uint8 line_num[] = {26, 30, 38};
    Coordinate site[3];
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
    {
        temp_right = getCurvature(site);
        Site_t site1;
        site1.x = 60;
        site1.y = 80;
        LCDShowNum(site1, (uint32)(temp_right > 0 ? temp_right : -1 * temp_right));
        site[0] = getNewCoordinate(site[0]);
        site[1] = getNewCoordinate(site[1]);
        site[2] = getNewCoordinate(site[2]);
        temp_right = getCurvature(site);
        site1.x = 60;
        site1.y = 100;
        LCDShowNum(site1, (uint32)(temp_right > 0 ? temp_right : -1 * temp_right));
    }

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
    {
        temp_left = getCurvature(site);
        Site_t site1;
        site1.x = 10;
        site1.y = 80;
        LCDShowNum(site1, (uint32)(temp_left > 0 ? temp_left : -1 * temp_left));
        site[0] = getNewCoordinate(site[0]);
        site[1] = getNewCoordinate(site[1]);
        site[2] = getNewCoordinate(site[2]);
        temp_left = getCurvature(site);
        site1.x = 10;
        site1.y = 100;
        LCDShowNum(site1, (uint32)(temp_left > 0 ? temp_left : -1 * temp_left));
    }
}