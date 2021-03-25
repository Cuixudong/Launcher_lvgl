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

/*
    lv_obj_t * qr_code;
    lv_coord_t qr_size = 200;
    char * qr_data = "9420&520";
    qr_code = lv_qrcode_create(parent,qr_size,LV_COLOR_BLACK,LV_COLOR_WHITE);
    lv_qrcode_update(qr_code,qr_data,strlen(qr_data));
    lv_obj_set_drag(qr_code,true);
    lv_obj_set_drag_throw(qr_code,true);
*/

#define FOC_SIZE 200

static lv_obj_t * main_page;
static lv_obj_t * close_btn;
static lv_obj_t * run_btn;
static lv_obj_t * title;
static lv_style_t style_line[4];
static lv_obj_t * foc_amp_bar[3];
static lv_obj_t * foc_amp_bar_lab[3];
static bool auto_run = false;
static bool foc_amp_view = false;
static lv_point_t foc_line_points[4][2];
static lv_obj_t * foc_amp_line[4];
static lv_obj_t * foc_amp_arc;
static lv_color_t color_tab[4];
static lv_task_t * task;
static lv_task_cb_t user_task_cb(lv_task_t * task);

static void calendar_close_event_cb1(lv_obj_t * obj, lv_event_t event)
{
    if((obj == close_btn)&&(event == LV_EVENT_SHORT_CLICKED))
    {
        lv_obj_del(main_page);
        lv_task_del(task);
    }
    else if((obj == run_btn)&&(event == LV_EVENT_SHORT_CLICKED))
    {
        if(auto_run)
        {
            auto_run = false;
        }
        else
        {
            auto_run = true;
        }
    }
}

static void foc_refresh(void)
{
    int i;
    int amp[3];
    foc_line_points[3][0].x = 0;
    foc_line_points[3][0].y = 0;
    foc_line_points[3][1].x = 0;
    foc_line_points[3][1].y = 0;
    //计算基本坐标
    for(i=0;i<3;i++)
    {
        amp[i] = lv_slider_get_value(foc_amp_bar[i]);
        lv_label_set_text_fmt(foc_amp_bar_lab[i],"val:%5d",amp[i]);
        foc_line_points[i][0].x = 0;
        foc_line_points[i][0].y = 0;
        foc_line_points[i][1].y = (_lv_trigo_sin(-120 * i     ) * amp[i]) >> LV_TRIGO_SHIFT;
        foc_line_points[i][1].x = (_lv_trigo_sin(-120 * i + 90) * amp[i]) >> LV_TRIGO_SHIFT;

        foc_line_points[3][1].x += foc_line_points[i][1].x;
        foc_line_points[3][1].y += foc_line_points[i][1].y;
    }
    //变换到屏幕坐标
    for(i=0;i<3;i++)
    {
        if(foc_amp_view)//偏移矢量连续视图
        {
            if(i > 0)
            {
                foc_line_points[i][0].x += foc_line_points[i-1][1].x >> LV_TRIGO_SHIFT;
                foc_line_points[i][0].y += foc_line_points[i-1][1].y >> LV_TRIGO_SHIFT;
                foc_line_points[i][1].y += foc_line_points[i-1][1].y >> LV_TRIGO_SHIFT;
                foc_line_points[i][1].x += foc_line_points[i-1][1].x >> LV_TRIGO_SHIFT;
            }
        }
        foc_line_points[i][0].x += FOC_SIZE/2;
        foc_line_points[i][0].y += FOC_SIZE/2;
        foc_line_points[i][1].x += FOC_SIZE/2;
        foc_line_points[i][1].y += FOC_SIZE/2;
        lv_line_set_points(foc_amp_line[i],foc_line_points[i],2);
    }
    foc_line_points[3][0].x += FOC_SIZE/2;
    foc_line_points[3][0].y += FOC_SIZE/2;
    foc_line_points[3][1].x += FOC_SIZE/2;
    foc_line_points[3][1].y += FOC_SIZE/2;
    lv_line_set_points(foc_amp_line[3],foc_line_points[3],2);
}

static void slider_change_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        if((obj == foc_amp_bar[0])||(obj == foc_amp_bar[1])||(obj == foc_amp_bar[2]))
        {
            foc_refresh();
        }
    }
}

static lv_task_cb_t user_task_cb(lv_task_t * task)
{
    int i;
    static int t = 0;
    if(auto_run)
    {
        for(i=0;i<3;i++)
        {
            lv_slider_set_value(foc_amp_bar[i],(_lv_trigo_sin(-240 * i + t%360) * FOC_SIZE/3) >> LV_TRIGO_SHIFT,LV_ANIM_ON);
        }
        foc_refresh();
        t++;
    }
//    lv_obj_invalidate(foc_amp_line[i]);
}

void app_foc_create(lv_obj_t * parent)
{
    int i;
    color_tab[0] = LV_COLOR_RED;
    color_tab[1] = LV_COLOR_GREEN;
    color_tab[2] = LV_COLOR_BLUE;
    color_tab[3] = LV_COLOR_CYAN;
    main_page = lv_page_create(parent,NULL);
    lv_obj_set_size(main_page,LV_HOR_RES_MAX,LV_VER_RES_MAX);
    //lv_win_set_drag(main_page,true);
    //lv_obj_set_style_local_bg_opa(main_page,LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_OPA_50);
    lv_obj_set_style_local_bg_color(main_page,LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_COLOR_WHITE);
    lv_obj_set_style_local_radius(main_page,LV_PAGE_PART_BG,LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_border_width(main_page,LV_PAGE_PART_BG,LV_STATE_DEFAULT,0);
    lv_page_set_scrollable_fit(main_page,LV_FIT_PARENT);
    lv_obj_align(main_page,parent,LV_ALIGN_CENTER,0,0);

    close_btn = lv_btn_create(main_page,NULL);
    lv_obj_set_style_local_bg_color(close_btn,LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_COLOR_WHITE);
    lv_obj_set_style_local_bg_opa(close_btn,LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_OPA_60);
    lv_obj_set_style_local_radius(close_btn,LV_BTN_PART_MAIN,LV_STATE_DEFAULT,3);
    lv_obj_set_style_local_border_width(close_btn,LV_BTN_PART_MAIN,LV_STATE_DEFAULT,1);
    lv_obj_set_size(close_btn,52,32);
    lv_obj_align(close_btn,main_page,LV_ALIGN_IN_TOP_RIGHT,-15,15);

    lv_obj_t * label = lv_label_create(close_btn,NULL);
    lv_label_set_text(label,"关闭");
    lv_obj_set_style_local_text_color(label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_RED);
    lv_obj_set_event_cb(close_btn, calendar_close_event_cb1);

    title = lv_label_create(main_page,NULL);
    lv_label_set_text(title,"FOC 单元测试");
    lv_obj_align(title,main_page,LV_ALIGN_IN_TOP_MID,0,30);

    foc_amp_arc = lv_arc_create(main_page,NULL);
    lv_arc_set_angles(foc_amp_arc,0,360);
    lv_obj_set_size(foc_amp_arc, FOC_SIZE+30, FOC_SIZE+30);
    lv_obj_set_style_local_radius(foc_amp_arc,LV_PAGE_PART_BG,LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_border_width(foc_amp_arc,LV_PAGE_PART_BG,LV_STATE_DEFAULT,0);
    lv_obj_set_style_local_line_width(foc_amp_arc,LV_ARC_PART_BG,LV_STATE_DEFAULT,3);
    lv_obj_set_style_local_line_width(foc_amp_arc,LV_ARC_PART_INDIC,LV_STATE_DEFAULT,3);
    lv_obj_set_style_local_line_color(foc_amp_arc,LV_ARC_PART_INDIC,LV_STATE_DEFAULT,LV_COLOR_YELLOW);
    lv_obj_align(foc_amp_arc,main_page,LV_ALIGN_CENTER,0,0);

    run_btn = lv_btn_create(main_page,close_btn);
    lv_btn_set_state(run_btn,LV_BTN_STATE_RELEASED);
    lv_obj_set_style_local_value_str(run_btn,LV_BTN_PART_MAIN,LV_BTN_STATE_RELEASED,"自动");
    lv_obj_set_width(run_btn,72);
    lv_obj_align(run_btn,main_page,LV_ALIGN_IN_BOTTOM_RIGHT,-20,-20);

    for(i=0;i<4;i++)
    {
        if(i<3)
        {
            foc_amp_bar[i] = lv_slider_create(main_page,NULL);
            lv_slider_set_type(foc_amp_bar[i],LV_SLIDER_TYPE_NORMAL);
            lv_slider_set_range(foc_amp_bar[i],-FOC_SIZE/2,FOC_SIZE/2);
            lv_slider_set_anim_time(foc_amp_bar[i],300);
            lv_slider_set_value(foc_amp_bar[i],100,LV_ANIM_ON);
            lv_obj_align(foc_amp_bar[i],main_page,LV_ALIGN_IN_BOTTOM_LEFT,20,-(3-i) * 32);
            foc_amp_bar_lab[i] = lv_label_create(main_page,NULL);
            lv_label_set_text_fmt(foc_amp_bar_lab[i],"val:100");
            lv_obj_align(foc_amp_bar_lab[i],foc_amp_bar[i],LV_ALIGN_OUT_RIGHT_MID,20,0);
            lv_obj_set_event_cb(foc_amp_bar[i],slider_change_event_cb);
        }

        lv_style_init(&style_line[i]);
        lv_style_set_line_width(&style_line[i], LV_STATE_DEFAULT, 3);
        lv_style_set_line_color(&style_line[i], LV_STATE_DEFAULT, color_tab[i]);
        lv_style_set_line_rounded(&style_line[i], LV_STATE_DEFAULT, true);

        foc_amp_line[i] = lv_line_create(main_page,NULL);
        if(i<3)//初始三相向量
        {
            foc_line_points[i][1].y = (_lv_trigo_sin(-120 * i     ) * FOC_SIZE/2) >> LV_TRIGO_SHIFT;
            foc_line_points[i][1].x = (_lv_trigo_sin(-120 * i + 90) * FOC_SIZE/2) >> LV_TRIGO_SHIFT;
        }
        else//合成向量
        {
            foc_line_points[i][1].y = (_lv_trigo_sin(-30     ) * 0) >> LV_TRIGO_SHIFT;
            foc_line_points[i][1].x = (_lv_trigo_sin(-30 + 90) * 0) >> LV_TRIGO_SHIFT;
        }
        foc_line_points[i][0].x = 0;
        foc_line_points[i][0].y = 0;
        foc_line_points[i][0].x += FOC_SIZE/2;//偏移屏幕中央
        foc_line_points[i][0].y += FOC_SIZE/2;
        foc_line_points[i][1].x += FOC_SIZE/2;
        foc_line_points[i][1].y += FOC_SIZE/2;

        lv_obj_add_style(foc_amp_line[i], LV_LINE_PART_MAIN, &style_line[i]);
        lv_obj_align(foc_amp_line[i],main_page,LV_ALIGN_CENTER,-FOC_SIZE/4,-FOC_SIZE/4);
        lv_line_set_points(foc_amp_line[i],foc_line_points[i],2);
        //lv_line_set_auto_size(foc_amp_line[i],true);

//        printf("line %3d:x:%5d,y:%5d,x:%5d,y:%5d\r\n",i,\
//               foc_line_points[i][0].x,foc_line_points[i][0].y,foc_line_points[i][1].x,foc_line_points[i][1].y);
//        printf("x:%d,y:%d\n",lv_obj_get_x(foc_amp_line[i]),lv_obj_get_y(foc_amp_line[i]));
    }
    lv_obj_align(foc_amp_arc,foc_amp_line[0],LV_ALIGN_CENTER,-3,FOC_SIZE/4);

    task = lv_task_create(user_task_cb, 20, LV_TASK_PRIO_MID, NULL);
}
