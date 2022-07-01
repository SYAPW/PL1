#include "system/includes.h"
#include "asm/spi.h"
#include "asm/clock.h"



// static const u8 SPI1_DO[2] = {
//     IO_PORTB_01,//'A'
//     IO_PORTA_08 //'B'
// };
// static const u8 SPI2_DO[2] = {
//     IO_PORTB_10,//'A'
//     IO_PORT_DM  //'B'
// };
// #define LED_SPI                 JL_SPI1
// #define LED_SPI_PORT            'B'
// #define LED_SPI_DAT_BAUD        8000000
// #define LED_SPI_REST_BAUD       1000000
// #define LED_SPI_CLOCK_BASE		clk_get("lsb")

// static OS_SEM led_spi_sem;
// static u32 spi_do = 0;
// static u8 led_spi_busy = 0;
// static u8 led_spi_sus = 0;

// ___interrupt
// void led_spi_isr()
// {
//     LED_SPI->CON &= ~BIT(13);   //关闭中断
//     LED_SPI->CON |=  BIT(14);   //清pnding
//     os_sem_post(&led_spi_sem);
//     led_spi_busy = 0;
// }

// void led_spi_init(void)
// {
//     if ((u32)LED_SPI == (u32)JL_SPI1) {
//         spi_do = SPI1_DO[LED_SPI_PORT - 'A'];
//         SFR(JL_IOMAP->CON1, 4, 1, LED_SPI_PORT - 'A');
//         request_irq(IRQ_SPI1_IDX, 0, led_spi_isr, 0);
//     } else {
//         spi_do = SPI2_DO[LED_SPI_PORT - 'A'];
//         SFR(JL_IOMAP->CON1, 16, 1, LED_SPI_PORT - 'A');
//         request_irq(IRQ_SPI2_IDX, 0, led_spi_isr, 0);
//     }
//     gpio_set_die(spi_do, 1);
//     gpio_set_direction(spi_do, 0);
//     gpio_set_pull_up(spi_do, 0);
//     gpio_set_pull_down(spi_do, 0);
//     gpio_write(spi_do, 0);

//     os_sem_create(&led_spi_sem, 1);

//     LED_SPI->CON = 0x4021;
// }

// void led_spi_rgb_to_24byte(u8 r, u8 g, u8 b, u8 *buf, int idx)
// {
//     buf = buf + idx * 24;
//     u32 dat = ((g << 16) | (r << 8) | b);
//     for (u8 i = 0; i < 24; i ++) {
//         if (dat & BIT(23 - i)) {
//             *(buf + i) = 0x7c;
//         } else {
//             *(buf + i) = 0x60;
//         }
//     }
// }

// void led_spi_rest()
// {
//     u8 tmp_buf[16] = {0};
//     LED_SPI->BAUD = LED_SPI_CLOCK_BASE / LED_SPI_REST_BAUD - 1;
//     LED_SPI->CON |= BIT(14);
//     LED_SPI->ADR = (u32)tmp_buf;
//     LED_SPI->CNT = 16;
//     while (!(LED_SPI->CON & BIT(15)));
//     LED_SPI->CON |= BIT(14);
// }

// void led_spi_send_rgbbuf(u8 *rgb_buf, u16 led_num) //rgb_buf的大小 至少要等于 led_num * 24
// {
//     if (!led_num) {
//         return;
//     }
//     while (led_spi_sus) {
//         os_time_dly(1);
//     }
//     led_spi_busy = 1;
//     led_spi_rest();
//     LED_SPI->BAUD = LED_SPI_CLOCK_BASE / LED_SPI_DAT_BAUD - 1;
//     LED_SPI->CON |= BIT(14);
//     LED_SPI->ADR = (u32)rgb_buf;
//     LED_SPI->CNT = led_num * 24;
//     while (!(LED_SPI->CON & BIT(15)));
//     LED_SPI->CON |= BIT(14);
//     led_spi_busy = 0;
// }

// void led_spi_send_rgbbuf_isr(u8 *rgb_buf, u16 led_num) //rgb_buf的大小 至少要等于 led_num * 24
// {
//     if (!led_num) {
//         return;
//     }
//     while (led_spi_sus) {
//         os_time_dly(1);
//     }
//     led_spi_busy = 1;
//     os_sem_pend(&led_spi_sem, 0);
//     led_spi_rest();
//     LED_SPI->BAUD = LED_SPI_CLOCK_BASE / LED_SPI_DAT_BAUD - 1;
//     LED_SPI->CON |= BIT(14);
//     LED_SPI->ADR = (u32)rgb_buf;
//     LED_SPI->CNT = led_num * 24;
//     LED_SPI->CON |= BIT(13);//打开中断
// }

// u8 led_spi_suspend(void)
// {
//     if (led_spi_sus) {
//         return 1;
//     }
//     if (led_spi_busy) {
//         return 1;
//     }
//     LED_SPI->CON |=  BIT(14);
//     LED_SPI->CON &= ~BIT(0);
//     led_spi_sus = 1;
//     return 0;
// }

// u8 led_spi_resume(void)
// {
//     if (!led_spi_sus) {
//         return 0;
//     }
//     gpio_set_die(spi_do, 1);
//     gpio_set_direction(spi_do, 0);
//     gpio_set_pull_up(spi_do, 0);
//     gpio_set_pull_down(spi_do, 0);
//     gpio_write(spi_do, 0);
//     LED_SPI->CON = 0x4021;
//     led_spi_sus = 0;
//     return 0;
// }

// static u8 spi_dat_buf[24 * 2] __attribute__((aligned(4)));
// extern void wdt_clear();
// void led_spi_test(void)
// {
//     printf("******************  led spi test  *******************\n");
//     led_spi_init();
//     u8 cnt = 0;
//     u8 pulse = 0;
//     while (1) {
//         cnt ++;
//         led_spi_rgb_to_24byte(cnt, 255 - cnt, 0, spi_dat_buf, 0);
//         led_spi_rgb_to_24byte(0, 0, cnt, spi_dat_buf, 1);
// #if 1
//         led_spi_send_rgbbuf(spi_dat_buf, 2);        //等待的方式，建议用在发的数据量小的场合
// #else
//         led_spi_send_rgbbuf_isr(spi_dat_buf, 2);    //中断的方式，建议用在发的数据量大的场合
// #endif
//         os_time_dly(2);
//         wdt_clear();
//     }
// }


#define RGB_NUMBER (10)//总的灯数量
#define RGB_CODE0 0x40//高2bit 低6bit 0xC0 右移1位 0x60  //0x60
#define RGB_CODE1 0x70//高4bit 低4bit 0xF0 右移1位 0x78  //0x7c
#define SYS_CLK_MAX (192000000L)//芯片最高时钟 695 240M 696 192M
#define SPI_CLK_MAX (64000000L)//spi最高的时钟 这个和cpu库有关系 最高80 spi 60M也够 设不了80找珠海哪库
#define SPI_SEND_WAY_SYNC 0//阻塞方式发送数据 同步
#define SPI_SEND_WAY_ASYNC 1//非阻塞方式发送数据 异步

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned short
#endif

#ifndef s16
#define s16 short
#endif

#ifndef u32
#define u32 unsigned int
#endif

#ifndef s16
#define s16 signed short
#endif

#define RGB_DEV_TIMER_OUT sys_hi_timer_add//sys_hi_timeout_add//sys_s_hi_timerout_add

// RGB数据 数据排序 顺序 参考灯的用户手册
typedef struct _RGB_COLOUR_{
    u8 g;//绿
    u8 r;//红
    u8 b;//蓝
}RGB_COLOUR;

// spi数据 rgb 数据排序 顺序 参考灯的用户手册
typedef struct _SPI_COLOUR_{
    u8 g[8];
    u8 r[8];
    u8 b[8];
}SPI_COLOUR;

typedef struct _RGB_DEV_CODE_{
    u8 code_0;
    u8 code_1;
}RGB_CODE;

typedef struct _RGB_DEV_INFO_{
    u8 spi_port;//spi
    u32 number;//灯总颗数
    RGB_CODE code;
    u32 sys_max_clk;//系统最高时钟 芯片最高时钟 695 240M 696 192M
    u32 spi_max_clk;//SPI最高时钟 spi最高的时钟 这个和cpu库有关系 最高80 spi 60M也够 设不了80找珠海哪库
    bool spi_send_way;//1为异步发送spi数据 0为同步发送spi数据
    RGB_COLOUR *rgb_buff;//灯颜色 存储buff
    SPI_COLOUR *spi_buff;//spi 数据buff

    u8 change_flag;
    u8 rgb_updata_flag;
}RGB_DEV_INFO;

static RGB_COLOUR rgb_buff[RGB_NUMBER]={0};//灯颜色 存储buff
static SPI_COLOUR spi_buff[RGB_NUMBER+1] __attribute__((aligned(4)));//spi 数据buff

static RGB_DEV_INFO drive_info={
    .spi_port = SPI1,
    .number = RGB_NUMBER,
    .spi_send_way = SPI_SEND_WAY_ASYNC,
    .sys_max_clk = SYS_CLK_MAX,//系统最高时钟 芯片最高时钟 695 240M 696 192M
    .spi_max_clk = SPI_CLK_MAX,//SPI最高时钟 spi最高的时钟 这个和cpu库有关系 最高80 spi 60M也够 设不了80找珠海哪库
    .code = {
        .code_0 = RGB_CODE0,
        .code_1 = RGB_CODE1,
    },

    .rgb_buff = rgb_buff,
    .spi_buff = spi_buff,
    .change_flag = 0
};

static RGB_DEV_INFO *_this_ = NULL;

//把单个rgb数据转换成spi数
static void rgb_set_only(SPI_COLOUR *spi_buff,RGB_COLOUR *rgb,RGB_CODE *code){
    if(!spi_buff || !rgb){
        return;
    }

    for(int i=0;i<8;i++){
       if(rgb->r&BIT(7-i)){         // 先取高位
            spi_buff->r[i]=code->code_1;
        }else{
            spi_buff->r[i]=code->code_0;
        }
        if(rgb->g&BIT(7-i)){
            spi_buff->g[i]=code->code_1;
        }else{
            spi_buff->g[i]=code->code_0;
        }
        if(rgb->b&BIT(7-i)){
            spi_buff->b[i]=code->code_1;
        }else{
            spi_buff->b[i]=code->code_0;
        }
    }
}

//设置第几颗rgb颜色
/*
priv：rgb 配置数据
colour：rgb 颜色
number:离io口最近的那颗灯为第一颗，最后一颗可以表示为 -1或者(USER_RGB_NUMBER-1)
*/
void rgb_colour_only_set(RGB_COLOUR colour,s32 number){
    s32 mapp_number = number;

    if(!_this_ || _this_->change_flag || !_this_->rgb_buff){
        return;
    }

    _this_->rgb_updata_flag = 1;
    mapp_number%=_this_->number;
    mapp_number = mapp_number<0?(_this_->number+mapp_number):mapp_number;
    _this_->rgb_buff[mapp_number]=colour;
    _this_->rgb_updata_flag = 0;
    return;
}

/**
 * @brief 设置为同样颜色
 *
 * @param colour 目标颜色
 */
void rgb_same_colour(RGB_COLOUR colour){
    if(!_this_ || !_this_->number){
        return;
    }
    printf("------------COLOUR GO------------\n");
    for(s32 i=0;i<_this_->number;i++){
        rgb_colour_only_set(colour,i);
    }
}

/**
 * @brief 清除颜色
 *
 */
void rgb_clear_colour(void){
    RGB_COLOUR colour={0,0,0};
    if(!_this_ || !_this_->rgb_buff || !_this_->number){
        return;
    }
    rgb_same_colour(colour);
}

/**
 * @brief 把所有颜色数据转换成spi数据
 *
 */
static void rgb_set_all(void){
    if(!_this_ || !_this_->rgb_buff || !_this_->spi_buff || !_this_->number){
        return;
    }

    RGB_COLOUR colour;
    for(s32 i=0;i<_this_->number;i++){
        colour = _this_->rgb_buff[i];
        rgb_set_only(_this_->spi_buff+i,&colour,&(_this_->code));
    }
    return;
}

/**
 * @brief 设置 系统时钟 、spi时钟、spi波特率
 *
 */
static void rgb_spi_set_baud_and_clock(void){
    // spi_set_baud
    if((_this_->sys_max_clk > clk_get("sys")) && _this_->sys_max_clk){
        printf("----SETTING SYS 1111111111111111111-------%lu",clk_get("sys"));
        clk_set("sys",_this_->sys_max_clk);
        printf("----SETTING SYS 2222222222222222222-------%lu",clk_get("sys"));
    }
    if((_this_->spi_max_clk > clk_get("spi")) && _this_->spi_max_clk){
        printf("----SETTING SPI 1111111111111111111-------%lu",clk_get("spi"));
        clk_set("spi",_this_->spi_max_clk);
        printf("----SETTING SPI 2222222222222222222-------%lu",clk_get("spi"));
    }
    printf("----SETTING SPI 33333333333333333333-------%lu",clk_get("spi"));

    u32 clock = spi_get_info_clock(_this_->spi_port);
    int err = spi_set_baud(_this_->spi_port, clock);
    if (err ) {
        printf("-----------0000000000000-------------\n");
    }
    printf("------------111111111111111111------------\n");
}

/**
 * @brief  发送spi数据
 *
 */
static void rgb_spi_send(void){
    if(!_this_ ||
        !_this_->spi_buff ||
        !_this_->rgb_buff ){
        return;
    }
    printf("------------SPI INIT------------\n");
    rgb_spi_set_baud_and_clock();
    memset(&_this_->spi_buff[_this_->number],0,sizeof(SPI_COLOUR));
    rgb_set_all();
    printf("------------SPI FINISH------------\n");
    if(SPI_SEND_WAY_ASYNC ==_this_->spi_send_way){
        spi_clear_pending(_this_->spi_port);
        spi_dma_set_addr_for_isr(_this_->spi_port,(u8*)_this_->spi_buff,sizeof(SPI_COLOUR)*(_this_->number+1),0);
        printf("------------SEND FINISH------------\n");
    }else if(SPI_SEND_WAY_SYNC == _this_->spi_send_way){
        int send_ret = spi_dma_send(_this_->spi_port,(u8*)_this_->spi_buff,sizeof(SPI_COLOUR)*(_this_->number+1));
    }
}

void spi_rgb_demo(void){                   //static
    printf("------------COLOUR SETTING------------\n");
    RGB_COLOUR colour={.b=0,.r=0,.g=0xff};
    rgb_clear_colour();
    printf("------------COLOUR CLEAR------------\n");
    rgb_same_colour(colour);
    printf("------------COLOUR END------------\n");
    rgb_spi_send();
    printf("------------SPI END------------\n");
}

/**
 * @brief spi rgb dev 初始化
 *
 */
void spi_rgb_dev_init(void){
    _this_ = &drive_info;
    int ret = spi_open(_this_->spi_port);

    if(ret<0){
        return;
    }
    printf("------------SPI OPEN111111------------\n");
    //RGB_DEV_TIMER_OUT(_this_,spi_rgb_demo,5000);
}
//gitdemo测试
