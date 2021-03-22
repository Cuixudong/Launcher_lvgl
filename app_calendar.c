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

static lv_obj_t * main_page;
static lv_obj_t * close_btn;
static lv_obj_t * calendar;

static lv_task_t * task;
static lv_task_cb_t user_task_cb(lv_task_t * task);

static void calendar_close_event_cb1(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_SHORT_CLICKED)
    {
        lv_obj_del(main_page);
        lv_task_del(task);
    }
}

LV_IMG_DECLARE(clock_bg)
LV_IMG_DECLARE(clock_hou)
LV_IMG_DECLARE(clock_min)
LV_IMG_DECLARE(clock_sec)
static const lv_img_dsc_t * clock_img_array_l[4] =
{
    &clock_bg,
    &clock_hou,
    &clock_min,
    &clock_sec,
};
static lv_obj_t * clock[4];

static lv_task_cb_t user_task_cb(lv_task_t * task)
{
    lv_img_set_angle(clock[3],lv_tick_get()*3/50%3600);
    lv_img_set_angle(clock[2],lv_tick_get()*3/3000%3600);
    lv_img_set_angle(clock[1],lv_tick_get()*3/36000%3600);
}

void app_calendar_create(lv_obj_t * parent)
{
    int i;
    main_page = lv_page_create(parent,NULL);
    lv_obj_set_size(main_page,LV_HOR_RES_MAX,LV_VER_RES_MAX);
    lv_obj_set_style_local_bg_opa(main_page,LV_PAGE_PART_BG,LV_STATE_DEFAULT,LV_OPA_50);
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

    calendar = lv_calendar_create(main_page,NULL);
    lv_obj_set_style_local_radius(calendar,LV_CALENDAR_PART_BG,LV_STATE_DEFAULT,3);
    lv_obj_set_style_local_border_width(calendar,LV_CALENDAR_PART_BG,LV_STATE_DEFAULT,1);

    lv_obj_align(calendar,main_page,LV_ALIGN_IN_TOP_MID,0,50);

    for(i=0;i<4;i++)
    {
        clock[i] = lv_img_create(main_page,NULL);
        lv_img_set_src(clock[i],clock_img_array_l[i]);
        lv_obj_align(clock[i],calendar,LV_ALIGN_OUT_BOTTOM_MID,0,20);
    }

    task = lv_task_create(user_task_cb, 30, LV_TASK_PRIO_MID, NULL);
}

