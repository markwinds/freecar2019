#include "common.h"
#include "include.h"

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
 * @brief       从ADC采集数据，并计算偏差
*/
int32 getErrorFromADC()
{
    int32 ADC_vaule[5];
    ADCn_Ch_e ADC_channel[] = {horizontal_left, horizontal_middle, horizontal_right, vertical_left, vertical_right};
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
    judgeRoadFromADC(ADC_vaule[1], ADC_vaule[0], ADC_vaule[2], ADC_vaule[3], ADC_vaule[4]);
    /**丢线的判定*/
    if (ADC_vaule[1] < 280)
    {
        return ADC_vaule[0] > ADC_vaule[2] ? -50000 : 50000;
    }

    /**得到纵向和横向的差比和*/
    int32 horizontal_dec_add = getDecAdd(ADC_vaule[1], ADC_vaule[0], ADC_vaule[2]);
    int32 vertical_dec_add = getDecAdd(ADC_vaule[1], ADC_vaule[3], ADC_vaule[4]);
    /**得到权重和补偿值*/
    int32 wright = abs(vertical_dec_add);
    int32 compensate = 0;
    if (ADC_vaule[1] < 500)
    {
        compensate = (int32)(500 / (float)ADC_vaule[1] * 50 - 20);
        compensate = ADC_vaule[0] > ADC_vaule[2] ? -1 * compensate : compensate;
    }
    return ((horizontal_dec_add * (200 - wright) + wright * vertical_dec_add) >> 7) + compensate;
}

/**
 * @brief       从ADC采集数据，并在LCD上显示
*/
void showADCVaule()
{
    int32 ADC_vaule[5];
    ADCn_Ch_e ADC_channel[] = {horizontal_left, horizontal_middle, horizontal_right, vertical_left, vertical_right};
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
    LCDShowNumDefule(0, 40, ADC_vaule[1]);
    LCDShowStringDefule(20, 0, "hl");
    LCDShowNumDefule(20, 40, ADC_vaule[0]);
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
