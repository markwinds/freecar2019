#include "common.h"
#include "include.h"

uint8 ADC_selet_now = 0;
int32 ADC_true_vaule[5] = {0};
int32 ADC_max_vaule[5] = {0};
int32 ADC_normal_vaule[5] = {0};
int32 last_ADC_normal_vaule[5] = {0};
int steer_offset = 0;

/****************************************flash操作***********************************************/
void writeADCParamToFlash()
{
    flash_erase_sector(SECTOR_FOR_ADC_COUNT);
    for (int i = 0; i < 5; i++)
    {
        flash_write(SECTOR_FOR_ADC_COUNT, i * PICTURE_NUM_PER_SECTOR * 2, (FLASH_WRITE_TYPE)ADC_max_vaule[i]);
    }
}

void readADCParamToFlash()
{
    for (int i = 0; i < 5; i++)
    {
        ADC_max_vaule[i] = (int32)flash_read(SECTOR_FOR_ADC_COUNT, i * PICTURE_NUM_PER_SECTOR * 2, FLASH_WRITE_TYPE);
    }
}

/***************************************************************************************/

/**
 * @brief       初始化ADC
*/
void initADC()
{
    adc_init(ADC0_DP0);
    adc_init(ADC0_DP1);
    adc_init(ADC0_DP3);
    adc_init(ADC0_DM1);
    adc_init(ADC1_DM1);
    readADCParamToFlash();
    //adc_init(ADC1_DP1);
}

/**
 * @brief       数据的归一化
 * @param       num     ADC采集到的数据
*/
int32 getNormalization(int32 num)
{
    int32 ans = 1000 * (num - 120) / 880;
    return ans < 1 ? 1 : ans;
}

/**
 * @brief       得到差比和
 * @param       mid     ADC采集到的中间数据的值
 * @param       left    采集到的左值
 * @param       right   采集的右值
*/
int32 getDecAdd(int32 mid, int32 left, int32 right)
{
    int32 a1 = mid - left;
    int32 b1 = mid + left;
    int32 a2 = mid - right;
    int32 b2 = mid + right;
    /**将结果放大128倍*/
    return ((a1 * b2 - a2 * b1) << 7) / b1 / b2;
}

/**
 * @brief       从ADC采集数据，并在LCD上显示
*/
void showADCVaule()
{
    int32 ADC_vaule[5];
    ADCn_Ch_e ADC_channel[] = {horizontal_middle, horizontal_left, horizontal_right, vertical_left, vertical_right};
    int32 sum[5] = {0};
    /**ADC采集数据*/
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            sum[j] += adc_once(ADC_channel[j], ADC_10bit);
        }
    }
    /**数据的归一化*/
    for (int j = 0; j < 5; j++)
    {
        ADC_vaule[j] = getNormalization(sum[j] >> 3);
    }
    LCDShowStringDefule(0, 0, "hm");
    LCDShowNumDefule(0, 40, ADC_vaule[0]);
    LCDShowStringDefule(20, 0, "hl");
    LCDShowNumDefule(20, 40, ADC_vaule[1]);
    LCDShowStringDefule(40, 0, "hr");
    LCDShowNumDefule(40, 40, ADC_vaule[2]);
    LCDShowStringDefule(60, 0, "vl");
    LCDShowNumDefule(60, 40, ADC_vaule[3]);
    LCDShowStringDefule(80, 0, "vr");
    LCDShowNumDefule(80, 40, ADC_vaule[4]);
    //judgeRoadFromADC(ADC_vaule[1], ADC_vaule[0], ADC_vaule[2]);
    DELAY_MS(1000);
}

int32 getADCVaule(ADCn_Ch_e ADC_channel)
{
    int32 sum = 0;
    for (int i = 0; i < 8; i++)
    {
        sum += adc_once(ADC_channel, ADC_10bit);
    }
    return sum >> 3;
}

//return ADC_vaule[0] > ADC_vaule[2] ? temp * -1 : temp;

/*************************************************UI的操作****************************************************/
void ADCSelteNext()
{
    ADC_selet_now = (ADC_selet_now + 1) % 5;
}
void ADCSelteBefore()
{
    ADC_selet_now = (ADC_selet_now - 1 + 5) % 5;
}

void showADCSeletPoint(uint16 colour)
{
    uint8 begin = ADC_selet_now * 20 + 5;
    for (uint8 i = begin; i < begin + 5; i++)
    {
        for (uint8 j = 1; j < 1 + 5; j++)
        {
            Site_t new_site = {j, i};
            LCD_point(new_site, colour);
        }
    }
}

void updateADCVaule()
{
    ADCn_Ch_e ADC_channel[] = {horizontal_middle, horizontal_left, horizontal_right, vertical_left, vertical_right};
    int32 sum[5] = {0};
    /**ADC采集数据*/
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            sum[j] += adc_once(ADC_channel[j], ADC_12bit);
        }
    }
    /**数据的归一化*/
    for (int j = 0; j < 5; j++)
    {
        last_ADC_normal_vaule[j] = ADC_normal_vaule[j];
        ADC_true_vaule[j] = sum[j] >> 3;
        int32 min = ADC_max_vaule[j] / 20;
        ADC_normal_vaule[j] = 1000 * (ADC_true_vaule[j] - min) / (ADC_max_vaule[j] - min);
        ADC_normal_vaule[j] = ADC_normal_vaule[j] < 1 ? 1 : ADC_normal_vaule[j];
    }
    if (ADC_normal_vaule[0] > ADC_middle_max)
    {
        ADC_middle_max = ADC_normal_vaule[0];
    }
}

void showADCvaule()
{
    int32 *name[] = {ADC_true_vaule, ADC_max_vaule, ADC_normal_vaule};
    int32 **p = name;
    uint8 y = 23;
    for (uint8 i = 0; i < 3; i++, p++, y += 32)
    {
        int32 *now = (*p);
        uint8 x = 0;
        for (uint8 j = 0; j < 5; j++, x += 20)
        {
            uint16 color;
            color = i == 0 ? BLUE : (i == 1 ? GREEN : RED);
            LCDShowNum(x, y, now[j], color, WHITE);
            //LCDShowNumDefule(x, y, now[j]);
        }
    }
}

void updateADCMaxVaule()
{
    ADC_max_vaule[ADC_selet_now] = ADC_true_vaule[ADC_selet_now];
}

void initADCUI()
{
    showADCSeletPoint(RED);
    LCDShowStringDefule(0, 6, "hm");
    LCDShowStringDefule(20, 6, "hl");
    LCDShowStringDefule(40, 6, "hr");
    LCDShowStringDefule(60, 6, "vl");
    LCDShowStringDefule(80, 6, "vr");
    updateADCVaule();
    showADCvaule();
}

/**********************************************偏差的计算***************************************************/

/**
 * @brief       从ADC采集数据，并计算偏差
*/
int32 getErrorFromADC()
{
    updateADCVaule();
    judgeRoadFromADC(ADC_normal_vaule[0], ADC_normal_vaule[1], ADC_normal_vaule[2], ADC_normal_vaule[3], ADC_normal_vaule[4]);
    if (ADC_mode == N5)
    {
        /**丢线的判定*/
        if (ADC_normal_vaule[0] < 280)
        {
            return ADC_normal_vaule[1] > ADC_normal_vaule[2] ? -500 : 500;
        }

        /**得到纵向和横向的差比和*/
        int32 horizontal_dec_add = getDecAdd(ADC_normal_vaule[0], ADC_normal_vaule[1], ADC_normal_vaule[2]);
        int32 vertical_dec_add = getDecAdd(ADC_normal_vaule[0], ADC_normal_vaule[3], ADC_normal_vaule[4]);
        /**得到权重和补偿值*/
        int32 wright = abs(vertical_dec_add);
        int32 compensate = 0;
        if (ADC_normal_vaule[0] < 500)
        {
            compensate = (int32)(500 * 50 / ADC_normal_vaule[0] - 20);
            compensate = ADC_normal_vaule[1] > ADC_normal_vaule[2] ? -1 * compensate : compensate;
        }
        return ((horizontal_dec_add * (200 - wright) + wright * vertical_dec_add) >> 7) + compensate; //20000
    }
    else if (ADC_mode == N2mhr)
    {
        return (ADC_normal_vaule[2] - ADC_normal_vaule[0]) * 1000 / (ADC_normal_vaule[2] + ADC_normal_vaule[0]);
    }
    else if (ADC_mode == N2hlvl)
    {
        return (ADC_normal_vaule[3] - ADC_normal_vaule[1]) * 1000 / (ADC_normal_vaule[1] + ADC_normal_vaule[3]);
    }
}