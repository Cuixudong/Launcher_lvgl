/*********************
 *      INCLUDES
 *********************/
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void launcher_icon_create(lv_obj_t * par);
static void launcher_static_icon_create(lv_obj_t * par);

#define SHADOW_LABEL_EN     0
#define MAX_LANGUAGE_NUM    4
#define MAX_ICON_NUM        27
static int i_launcher_page = 3;
static int i_launcher_sta_icon = 3;
static bool b_virtual_btn = true;
static int i_icon_language = 1;

static lv_obj_t * img_vir_btn[3];
static lv_obj_t * led_icon_page[3];

#define LED_ICON_PAGE_DIS 40

static lv_obj_t * launcher_bg;
static lv_obj_t * launcher_page[3];
static lv_obj_t * launcher_tab_icon;
static lv_obj_t * launcher_taskbar;

static lv_obj_t * cube;

static lv_obj_t * icon_img[MAX_ICON_NUM];
static lv_obj_t * icon_name[MAX_ICON_NUM];
static lv_point_t icon_pos[24];
static lv_point_t ani_now[24];

LV_IMG_DECLARE(screenbg)
LV_IMG_DECLARE(girl)

LV_IMG_DECLARE(icon_011)
LV_IMG_DECLARE(icon_014)
LV_IMG_DECLARE(icon_017)
LV_IMG_DECLARE(icon_021)
LV_IMG_DECLARE(icon_024)
LV_IMG_DECLARE(icon_027)
LV_IMG_DECLARE(icon_031)
LV_IMG_DECLARE(icon_034)
LV_IMG_DECLARE(icon_037)
LV_IMG_DECLARE(icon_041)
LV_IMG_DECLARE(icon_044)
LV_IMG_DECLARE(icon_047)
LV_IMG_DECLARE(icon_051)
LV_IMG_DECLARE(icon_054)
LV_IMG_DECLARE(icon_057)
LV_IMG_DECLARE(icon_061)
LV_IMG_DECLARE(icon_064)
LV_IMG_DECLARE(icon_067)
LV_IMG_DECLARE(icon_071)
LV_IMG_DECLARE(icon_074)
LV_IMG_DECLARE(icon_077)
LV_IMG_DECLARE(icon_081)
LV_IMG_DECLARE(icon_084)
LV_IMG_DECLARE(icon_087)
LV_IMG_DECLARE(icon_091)
LV_IMG_DECLARE(icon_094)
LV_IMG_DECLARE(icon_097)

static const lv_img_dsc_t * img_array_l[MAX_ICON_NUM] =
{
     &icon_011,     &icon_014,     &icon_017,
     &icon_021,     &icon_024,     &icon_027,
     &icon_031,     &icon_034,     &icon_037,
     &icon_041,     &icon_044,     &icon_047,
     &icon_051,     &icon_054,     &icon_057,
     &icon_061,     &icon_064,     &icon_067,
     &icon_071,     &icon_074,     &icon_077,
     &icon_081,     &icon_084,     &icon_087,
     &icon_091,     &icon_094,     &icon_097,
};

static int move_icon_id[3][8] =
{
    {21,8 ,16,15,6 ,17,19,7 },
    {9 ,10,11,12,13,14,1 ,3 },
    {20,0 ,22,23,24,25,4 ,5 }
};
static int stat_icon_id[] = {26,2,18};

static const char * icon_name_tal[MAX_LANGUAGE_NUM][MAX_ICON_NUM]=
{
    {
        "Wireless",
        "Address book",
        "Phone",
        "Gyroscopes",
        "Browser",
        "Editing",
        "Clock",
        "Compass",
        "Photo",
        "Market",
        "Program",
        "QR code",
        "Tom cat",
        "Switch",
        "Recorder",
        "Calculator",
        "Music",
        "Setting",
        "Camera",
        "Notepad",
        "Developer",
        "E-book",
        "LED light",
        "Video",
        "Video recorder",
        "Data trans",
        "SMS",
    },
    {
        "无线",
        "通讯录",
        "通话",
        "陀螺仪",
        "浏览器",
        "文档编辑",
        "时钟",
        "指南针",
        "电子相册",
        "应用市场",
        "用户程序",
        "二维码",
        "汤姆猫",
        "开关控制",
        "录音机",
        "计算器",
        "音乐",
        "系统设置",
        "相机",
        "记事本",
        "开发者",
        "电子书",
        "LED灯",
        "视频",
        "录像机",
        "数据传输",
        "短信",
    },
    {
        "無線",
        "通訊錄",
        "通話",
        "陀螺儀",
        "騮覽器",
        "文檔編輯",
        "時鐘",
        "指南針",
        "電子相冊",
        "應用市場",
        "用戶程序",
        "二維碼",
        "湯姆貓",
        "開關控制",
        "錄音機",
        "計算器",
        "音樂",
        "系統設置",
        "相機",
        "記事本",
        "開發者",
        "電子書",
        "LED燈",
        "視頻",
        "錄像機",
        "數據傳輸",
        "短信",
    },
    {
    }
};


LV_FONT_DECLARE(IconFont12)

static const char * lang_tab =
{
    "英文\n"
    "中文简体\n"
    "中文繁体\n"
    "韩文"
};

void app_calendar_create(lv_obj_t * parent);
void app_foc_create(lv_obj_t * parent);

static bool launcher_set_flag = false;
#define WIN_OBJ_PAGE 1
#define WIN_OBJ_CLOSE_BTN 2
#define WIN_OBJ_SWT 3
#define WIN_OBJ_DROP 4
#define WIN_OBJ_BTN 5
static lv_obj_t * win_obj[9];
void launcher_set_win_close_event_cb1(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED)
    {
        lv_obj_del(win_obj[WIN_OBJ_PAGE]);
        launcher_set_flag = false;
    }
}
void launcher_set_obj_change_event_cb1(lv_obj_t * obj, lv_event_t event)
{
    if(obj == win_obj[WIN_OBJ_SWT])
    {
        if(event == LV_EVENT_VALUE_CHANGED)
        {
            b_virtual_btn = lv_switch_get_state(obj);
            launcher_icon_create(launcher_page[1]);
            launcher_static_icon_create(launcher_page[2]);
        }
    }
    else if(obj == win_obj[WIN_OBJ_DROP])
    {
        if(event == LV_EVENT_VALUE_CHANGED)
        {
            i_icon_language = lv_dropdown_get_selected(obj);
            launcher_icon_create(launcher_page[1]);
            launcher_static_icon_create(launcher_page[2]);
        }
    }
}
void launcher_set_create1(lv_obj_t * par)
{
    if(launcher_set_flag == true)
    {
        return;
    }
    else {
        win_obj[WIN_OBJ_PAGE] = lv_page_create(par,NULL);
        lv_obj_set_size(win_obj[WIN_OBJ_PAGE],LV_HOR_RES_MAX*3>>2,LV_VER_RES_MAX>>2);
        lv_win_set_drag(win_obj[WIN_OBJ_PAGE],true);
        lv_obj_set_style_local_bg_opa(win_obj[WIN_OBJ_PAGE],LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_OPA_0);
        lv_obj_set_style_local_radius(win_obj[WIN_OBJ_PAGE],LV_PAGE_PART_BG,LV_STATE_DEFAULT,5);
        lv_obj_set_style_local_border_width(win_obj[WIN_OBJ_PAGE],LV_PAGE_PART_BG,LV_STATE_DEFAULT,1);
        lv_page_set_scrollable_fit(win_obj[WIN_OBJ_PAGE],LV_FIT_PARENT);
        lv_obj_align(win_obj[WIN_OBJ_PAGE],lv_scr_act(),LV_ALIGN_CENTER,0,0);

        win_obj[WIN_OBJ_CLOSE_BTN] = lv_btn_create(win_obj[WIN_OBJ_PAGE], NULL);
        lv_obj_set_style_local_bg_color(win_obj[WIN_OBJ_CLOSE_BTN],LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_COLOR_BLUE);
        lv_obj_set_style_local_bg_opa(win_obj[WIN_OBJ_CLOSE_BTN],LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_OPA_20);
        lv_obj_set_style_local_radius(win_obj[WIN_OBJ_CLOSE_BTN],LV_BTN_PART_MAIN,LV_STATE_DEFAULT,3);
        lv_obj_set_style_local_border_width(win_obj[WIN_OBJ_CLOSE_BTN],LV_BTN_PART_MAIN,LV_STATE_DEFAULT,1);
        lv_obj_set_size(win_obj[WIN_OBJ_CLOSE_BTN],52,32);
        lv_obj_align(win_obj[WIN_OBJ_CLOSE_BTN],win_obj[WIN_OBJ_PAGE],LV_ALIGN_IN_TOP_RIGHT,-15,15);

        win_obj[5] = lv_label_create(win_obj[WIN_OBJ_CLOSE_BTN],NULL);
        lv_label_set_text(win_obj[5],"关闭");
        lv_obj_set_style_local_text_color(win_obj[5],LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_RED);
        lv_obj_set_event_cb(win_obj[WIN_OBJ_CLOSE_BTN], launcher_set_win_close_event_cb1);

        win_obj[6] = lv_label_create(win_obj[WIN_OBJ_PAGE],win_obj[5]);
        lv_label_set_text(win_obj[6],"桌面参数设置");
        lv_obj_align(win_obj[6],win_obj[WIN_OBJ_PAGE],LV_ALIGN_IN_TOP_MID,0,25);

        win_obj[7] = lv_label_create(win_obj[WIN_OBJ_PAGE],win_obj[5]);
        lv_label_set_text(win_obj[7],"虚拟按键开关");
        lv_obj_align(win_obj[7],win_obj[WIN_OBJ_PAGE],LV_ALIGN_IN_TOP_LEFT,32,75);
        win_obj[WIN_OBJ_SWT] = lv_switch_create(win_obj[WIN_OBJ_PAGE],NULL);
        if(b_virtual_btn == true)
        {
            lv_switch_on(win_obj[WIN_OBJ_SWT],LV_ANIM_ON);
        }
        else
        {
            lv_switch_off(win_obj[WIN_OBJ_SWT],LV_ANIM_ON);
        }
        lv_obj_align(win_obj[WIN_OBJ_SWT],win_obj[7],LV_ALIGN_OUT_RIGHT_MID,15,0);
        lv_obj_set_event_cb(win_obj[WIN_OBJ_SWT], launcher_set_obj_change_event_cb1);

        static char * dropdown_text = "英语\n中文简体\n中文繁体\n韩文";
        win_obj[8] = lv_label_create(win_obj[WIN_OBJ_PAGE],win_obj[5]);
        lv_label_set_text(win_obj[8],"图标名称语言");
        lv_obj_align(win_obj[8],win_obj[7],LV_ALIGN_OUT_BOTTOM_LEFT,0, 25);

        win_obj[WIN_OBJ_DROP] = lv_dropdown_create(win_obj[WIN_OBJ_PAGE],NULL);
        lv_dropdown_set_dir(win_obj[WIN_OBJ_DROP],LV_DROPDOWN_DIR_DOWN);
        lv_dropdown_set_options(win_obj[WIN_OBJ_DROP],dropdown_text);
        lv_dropdown_set_selected(win_obj[WIN_OBJ_DROP],i_icon_language);
        lv_obj_align(win_obj[WIN_OBJ_DROP],win_obj[8],LV_ALIGN_OUT_RIGHT_MID,15, 0);
        lv_obj_set_event_cb(win_obj[WIN_OBJ_DROP], launcher_set_obj_change_event_cb1);

        launcher_set_flag = true;
    }
}

static void launcher_taskbar_create(lv_obj_t * par)
{
    if(launcher_taskbar == NULL)
    {
        launcher_taskbar = lv_label_create(par,NULL);
    }
    lv_label_set_text(launcher_taskbar,"中国移动 星期六 2020.08.31");
    lv_obj_align(launcher_taskbar,launcher_page[0],LV_ALIGN_IN_LEFT_MID,LV_HOR_RES_MAX >> 5,0);
}

static lv_event_cb_t tab_move_event_cb(lv_obj_t * tab, lv_event_t e)
{
    if(tab == launcher_tab_icon)
    {
        if(e == LV_EVENT_VALUE_CHANGED)
        {
            printf("page change\n");
        }
    }
}

static lv_event_cb_t icon_click_event_cb(lv_obj_t * icon, lv_event_t e)
{
    int i;
    if(e == LV_EVENT_SHORT_CLICKED)
    {
        for(i=0;i<MAX_ICON_NUM;i++)
        {
            if(icon == icon_img[i])
            {
                if(launcher_set_flag == true)
                {
                    lv_obj_del(win_obj[WIN_OBJ_PAGE]);
                    launcher_set_flag = false;
                }
                printf("icon num:%d\n",i);
                if(i == 4)  /* clock */
                {
                    app_calendar_create(lv_scr_act());
                }
                else if(i == 3)  /* mpu */
                {
                    app_foc_create(lv_scr_act());
                }
            }
        }
    }
    if(e == LV_EVENT_LONG_PRESSED)
    {
        for(i=0;i<MAX_ICON_NUM;i++)
        {
            if(icon == icon_img[i])
            {
                printf("long press icon num:%d\n",i);
            }
        }
        launcher_set_create1(lv_scr_act());
        //launcher_set_create();
    }
}

static void launcher_icon_create(lv_obj_t * par)
{
    int i;
#if SHADOW_LABEL_EN
    lv_obj_t * shadow_label;
#endif
    int icw = lv_obj_get_width(par)/30;
    int lp = (lv_obj_get_width(par) - 4 * 98 - icw * 3) / 2;
    if(launcher_tab_icon == NULL)
    {
        launcher_tab_icon = lv_tabview_create(par,NULL);
        lv_tabview_add_tab(launcher_tab_icon,"A PAGE");
        lv_tabview_add_tab(launcher_tab_icon,"B PAGE");
        lv_tabview_add_tab(launcher_tab_icon,"C PAGE");
        lv_tabview_set_btns_pos(launcher_tab_icon,LV_TABVIEW_TAB_POS_NONE);
        lv_obj_set_style_local_bg_opa(launcher_tab_icon,LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_OPA_0);
        lv_obj_set_size(launcher_tab_icon,lv_obj_get_width(lv_obj_get_parent(launcher_tab_icon)),lv_obj_get_height(lv_obj_get_parent(launcher_tab_icon)));
        lv_cont_set_fit(lv_page_get_scrl(lv_tabview_get_tab(launcher_tab_icon,0)),LV_FIT_PARENT);
        lv_cont_set_fit(lv_page_get_scrl(lv_tabview_get_tab(launcher_tab_icon,1)),LV_FIT_PARENT);
        lv_cont_set_fit(lv_page_get_scrl(lv_tabview_get_tab(launcher_tab_icon,2)),LV_FIT_PARENT);
    }
    for(i = 0;i < MAX_ICON_NUM - i_launcher_sta_icon;i++)
    {
        if(icon_img[i] == NULL)
        {
            icon_img[i] = lv_imgbtn_create(lv_tabview_get_tab(launcher_tab_icon,i/8),NULL);
            lv_obj_set_event_cb(icon_img[i],icon_click_event_cb);
        }
        lv_imgbtn_set_src(icon_img[i],LV_BTN_STATE_RELEASED,img_array_l[move_icon_id[i/8][i%8]]);
        lv_imgbtn_set_src(icon_img[i],LV_BTN_STATE_PRESSED,img_array_l[move_icon_id[i/8][i%8]]);

        if(icon_name[i] == NULL)
        {
            icon_name[i] = lv_label_create(lv_tabview_get_tab(launcher_tab_icon,i/8),NULL);
            //lv_obj_set_event_cb(icon_name[i],icon_click_event_cb);
        }
        lv_obj_set_style_local_text_font(icon_name[i],LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&IconFont12);
        lv_label_set_text(icon_name[i],icon_name_tal[i_icon_language][move_icon_id[i/8][i%8]]);

#if 1
        lv_obj_set_style_local_text_color(icon_name[i],LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_WHITE);
#endif
        if((i%8) == 0)
        {
            lv_obj_align(icon_img[i],lv_tabview_get_tab(launcher_tab_icon,i/8),LV_ALIGN_IN_TOP_LEFT, lp,icw*2);
        }
        else if((i%8) == 4)
        {
            lv_obj_align(icon_img[i],icon_img[ i - 4],LV_ALIGN_OUT_BOTTOM_MID, 0,icw*3);
        }
        else
        {
            lv_obj_align(icon_img[i],icon_img[ i - 1], LV_ALIGN_OUT_RIGHT_MID, icw,0);
        }
        lv_obj_align(icon_name[i],icon_img[i],LV_ALIGN_OUT_BOTTOM_MID, 0,10);
        icon_pos[i].x = lv_obj_get_x(icon_img[i]);
        icon_pos[i].y = lv_obj_get_y(icon_img[i]);
#if SHADOW_LABEL_EN
    shadow_label = lv_label_create(par,icon_name[i]);
    lv_obj_set_style_local_text_color(shadow_label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);
    lv_obj_align(shadow_label,icon_name[i],LV_ALIGN_CENTER,-1,-1);
#endif
    }

    lv_obj_set_event_cb(launcher_tab_icon,tab_move_event_cb);
}

LV_IMG_DECLARE(key_back)
LV_IMG_DECLARE(key_home)
LV_IMG_DECLARE(key_tool)

LV_FONT_DECLARE(IconFont12)

static const lv_img_dsc_t * img_array_vir_key[3] =
{
    &key_back,
    &key_home,
    &key_tool
};

static void launcher_static_icon_create(lv_obj_t * par)
{
    int i;
#if SHADOW_LABEL_EN
    lv_obj_t * shadow_label;
#endif
    if(b_virtual_btn == true)
    {
        for(i=0;i<3;i++)
        {
            if(img_vir_btn[i] == NULL)
            {
                img_vir_btn[i] = lv_imgbtn_create(par,NULL);
            }
            lv_imgbtn_set_src(img_vir_btn[i],LV_BTN_STATE_RELEASED,img_array_vir_key[i]);
            lv_imgbtn_set_src(img_vir_btn[i],LV_BTN_STATE_PRESSED,img_array_vir_key[i]);

            lv_obj_align(img_vir_btn[i],par,LV_ALIGN_IN_BOTTOM_MID,\
                         (1-i) * lv_obj_get_width(par) >> 2,-lv_obj_get_height(par)>>4);
            lv_obj_set_style_local_image_recolor(img_vir_btn[i], \
                                                 LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
            lv_obj_set_style_local_image_recolor_opa(img_vir_btn[i], \
                                                     LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
            if(lv_obj_get_hidden(img_vir_btn[i]) == true)
            lv_obj_set_hidden(img_vir_btn[i],false);
        }
    }
    else
    {
        for(i=0;i<3;i++)
        {
            if(lv_obj_get_hidden(img_vir_btn[i]) == false)
            lv_obj_set_hidden(img_vir_btn[i],true);
        }
    }
    for(i=0;i<i_launcher_sta_icon;i++)
    {
        if(icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i] == NULL)
        {
            icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i] = lv_label_create(par,NULL);
            //lv_obj_set_event_cb(icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i],icon_click_event_cb);
        }
        lv_obj_set_style_local_text_font(icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i],LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&IconFont12);
        lv_label_set_text(icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i],icon_name_tal[i_icon_language][stat_icon_id[i]]);
#if 1
        lv_obj_set_style_local_text_color(icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i],LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_WHITE);
#endif
        if(b_virtual_btn == true)
        {
            lv_obj_align(icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i],\
                         par,LV_ALIGN_IN_BOTTOM_MID,(i-1) * lv_obj_get_width(par) >> 2,\
                         -((lv_obj_get_height(par)>>4) + lv_obj_get_height(img_vir_btn[0])) - 10);
        }
        else
        {
            lv_obj_align(icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i],\
                         par,LV_ALIGN_IN_BOTTOM_MID,(i-1) * lv_obj_get_width(par) >> 2,\
                         -(lv_obj_get_height(par)>>4) - 10);
        }
#if SHADOW_LABEL_EN
    shadow_label = lv_label_create(par,icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i]);
    lv_obj_set_style_local_text_color(shadow_label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);
    lv_obj_align(shadow_label,icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i],\
            LV_ALIGN_CENTER,-1,-1);
#endif
    }
    for(i=0;i<i_launcher_sta_icon;i++)
    {
        if(icon_img[MAX_ICON_NUM - i_launcher_sta_icon + i] == NULL)
        {
            icon_img[MAX_ICON_NUM - i_launcher_sta_icon + i] = lv_imgbtn_create(par,NULL);
            lv_obj_set_event_cb(icon_img[MAX_ICON_NUM - i_launcher_sta_icon + i],icon_click_event_cb);
        }
        lv_imgbtn_set_src(icon_img[MAX_ICON_NUM - i_launcher_sta_icon + i],LV_BTN_STATE_RELEASED,img_array_l[stat_icon_id[i]]);
        lv_imgbtn_set_src(icon_img[MAX_ICON_NUM - i_launcher_sta_icon + i],LV_BTN_STATE_PRESSED,img_array_l[stat_icon_id[i]]);

        if(true)
        {
            lv_obj_align(icon_img[MAX_ICON_NUM - i_launcher_sta_icon + i],\
                         icon_name[MAX_ICON_NUM - i_launcher_sta_icon + i],\
                         LV_ALIGN_OUT_TOP_MID,0, -(lv_obj_get_height(par)>>5));
        }
        //lv_obj_set_style_local_image_recolor(icon_img[MAX_ICON_NUM - i_launcher_sta_icon + i], LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
        //lv_obj_set_style_local_image_recolor_opa(icon_img[MAX_ICON_NUM - i_launcher_sta_icon + i], LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_10);
    }
}
static void launcher_led_create(lv_obj_t * par)
{
    int i;
    for(i=0;i<i_launcher_page;i++)
    {
        if(i == 0)
        {
            if(led_icon_page[i] == NULL)
            {
                led_icon_page[i] = lv_led_create(par,NULL);
            }
            lv_obj_set_style_local_bg_color(led_icon_page[0],LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_COLOR_WHITE);
            lv_obj_set_style_local_border_color(led_icon_page[0],LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_COLOR_WHITE);
            lv_obj_set_style_local_shadow_color(led_icon_page[0],LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_COLOR_WHITE);
            lv_obj_set_style_local_bg_opa(led_icon_page[0],LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_OPA_100);
            lv_obj_set_size(led_icon_page[0],7,7);
            lv_led_set_bright(led_icon_page[0],LV_LED_BRIGHT_MAX);
        }
        else
        {
            if(led_icon_page[i] == NULL)
            {
                led_icon_page[i] = lv_led_create(par,led_icon_page[i-1]);
            }
            lv_obj_set_size(led_icon_page[i],5,5);
            lv_led_set_bright(led_icon_page[i],LV_LED_BRIGHT_MIN);
        }
        lv_obj_align(led_icon_page[i],par,LV_ALIGN_IN_TOP_MID,(i-1)*LED_ICON_PAGE_DIS,6);
    }
}
static void led_icon_page_handle(void)
{
    int i;
    lv_coord_t coordsx = lv_tabview_get_tab(launcher_tab_icon,1)->coords.x1;
    for(i=0;i<i_launcher_page;i++)
    {
        lv_obj_align(led_icon_page[0],lv_obj_get_parent(led_icon_page[0]),LV_ALIGN_IN_TOP_MID,\
                     -(coordsx*LED_ICON_PAGE_DIS/LV_HOR_RES_MAX),6);
        if((lv_tabview_get_tab(launcher_tab_icon,1)->coords.x1) < 0)
        {
            lv_obj_align(led_icon_page[2],lv_obj_get_parent(led_icon_page[2]),LV_ALIGN_IN_TOP_MID,\
                    (coordsx*LED_ICON_PAGE_DIS/LV_HOR_RES_MAX) + LED_ICON_PAGE_DIS,6);
        }
        if((lv_tabview_get_tab(launcher_tab_icon,1)->coords.x1) > 0)
        {
            lv_obj_align(led_icon_page[1],lv_obj_get_parent(led_icon_page[1]),LV_ALIGN_IN_TOP_MID,\
                         (coordsx*LED_ICON_PAGE_DIS/LV_HOR_RES_MAX) - LED_ICON_PAGE_DIS,6);
        }
    }
}

static void user_task_cb(lv_task_t * task)
{

    char buf[52];
    sprintf(buf,"中国联通: 2020年12月27日 星期天 %2d:%02d:%02d",lv_tick_get()/3600000+23,lv_tick_get()/60000%60,lv_tick_get()/1000%60);
    lv_label_set_text(launcher_taskbar,buf);

    led_icon_page_handle();
}

static lv_obj_t * cube;

void launcher_widgets(void)
{
    lv_obj_t * par_scr;
    par_scr = lv_scr_act();
    int i;
    launcher_bg = lv_img_create(par_scr,NULL);
    lv_img_set_src(launcher_bg,&girl);
    lv_img_set_src(launcher_bg,&girl);
    lv_obj_set_size(launcher_bg,LV_HOR_RES_MAX,LV_VER_RES_MAX);

    //lv_obj_set_style_local_image_recolor(launcher_bg, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    //lv_obj_set_style_local_image_recolor_opa(launcher_bg, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_30);
    for(i=0;i<3;i++)
    {
        launcher_page[i] = lv_cont_create(par_scr,NULL);
        lv_obj_set_style_local_bg_opa(launcher_page[i],LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_OPA_0);
        lv_obj_set_style_local_radius(launcher_page[i],LV_PAGE_PART_BG,LV_STATE_DEFAULT,0);
        lv_obj_set_style_local_border_width(launcher_page[i],LV_PAGE_PART_BG,LV_STATE_DEFAULT,0);
        lv_obj_set_width(launcher_page[i],LV_HOR_RES_MAX);
    }
    lv_obj_set_height(launcher_page[0],(LV_VER_RES_MAX>>5) + 10);
    lv_obj_set_height(launcher_page[2],(LV_VER_RES_MAX >> 2) - 10);
    lv_obj_set_height(launcher_page[1],LV_VER_RES_MAX - lv_obj_get_height(launcher_page[0]) - lv_obj_get_height(launcher_page[2]));

    lv_obj_align(launcher_page[0],par_scr,LV_ALIGN_IN_TOP_LEFT,0,0);
    lv_obj_align(launcher_page[2],par_scr,LV_ALIGN_IN_BOTTOM_LEFT,0,0);
    lv_obj_align(launcher_page[1],launcher_page[0],LV_ALIGN_OUT_BOTTOM_LEFT,0,0);

    launcher_taskbar_create(launcher_page[0]);
    launcher_icon_create(launcher_page[1]);
    launcher_static_icon_create(launcher_page[2]);
    launcher_led_create(launcher_page[2]);
    lv_task_create(user_task_cb, 10, LV_TASK_PRIO_MID, NULL);
}
