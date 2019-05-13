#include "include.h"
#include "common.h"
uint8 zbthhh = 14;
//ov7725_eagle_reg[45].val
struct FlowValue flow_value[] =
    {
        {"B_W", &(ov7725_eagle_reg[45].val), 1},
        {"zbt", &(zbthhh), 1},
        {"end", NULL, 0}};

struct FlowValue *now_flow_value = flow_value;

void showFlowValue()
{
    LCD_clear(WHITE);
    Site_t site = {0, 80};
    LCD_str(site, (uint8 *)(now_flow_value->name), BLACK, WHITE);
    Site_t site1 = {60, 80};
    LCD_num(site1, (uint32)(*(now_flow_value->value)), GREEN, WHITE);
}

void nextFlowValue()
{
    now_flow_value++;
    if (0 == strcmp(now_flow_value->name, "end"))
    {
        now_flow_value = flow_value;
    }
    showFlowValue();
}

void beforeFlowValue()
{
    if (now_flow_value == flow_value)
    {
        while (strcmp(now_flow_value->name, "end") != 0)
        {
            now_flow_value++;
        }
    }
    now_flow_value--;
    showFlowValue();
}

void addFlowValue()
{
    *(now_flow_value->value) += now_flow_value->step;
    showFlowValue();
    if (strcmp(now_flow_value->name, "B_W") == 0)
        SCCB_WriteByte(ov7725_eagle_reg[45].addr, ov7725_eagle_reg[45].val);
}

void subFlowValue()
{
    *(now_flow_value->value) -= now_flow_value->step;
    showFlowValue();
    if (strcmp(now_flow_value->name, "B_W") == 0)
        SCCB_WriteByte(ov7725_eagle_reg[45].addr, ov7725_eagle_reg[45].val);
}