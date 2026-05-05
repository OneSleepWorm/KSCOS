#include "../inc/filemenu.h"
#include "../inc/key.h"
#include "../littlefs/lfs.h"
#include "../littlefs/lfs_config.h"
#include "../inc/KSCdraw.h"

#include <stdio.h>

ksc_style_t menu_style0={
    .colorck=rred,
    .colorbk=wwhite,
    .width=80,
    .height=20,
    .state=0,
    .custom=0,//
    .sdx=1,
    .sdy=1,
    .d_and_r=0,
    ._type=_string_extra|_custom_label1,
};

ksc_style_t menu_style1={
    .colorck=rred,
    .colorbk=wwhite,
    .width=80,
    .height=20,
    .state=0,
    .custom=0,
    .sdx=100,
    .sdy=1,
    .d_and_r=0,
    ._type=_string_extra|_custom_label2,
};

ksc_style_t menu_style2={
    .colorck=bblue,
    .colorbk=bblue,
    .width=80,
    .height=10,
    .state=0,
    .custom=0,
    .sdx=0,
    .sdy=0,
    .d_and_r=0,
    ._type=_box|_custom_label3,
};

ksc_menu_config_t filemenu_config={
    .mdw=150,
    .mdh=10,
    .menu_hnum=4,
    .menu_wnum=1,
    .menu_obj_num=1,
    .menu_index=0
};
Img_File mfile[8]={0};



lfs_t lfs;
lfs_dir_t dir;

int filedir(ksc_menu_t* menu);
ksc_menu_t* filemenu_init(KSC_buf* screen){
    static ksc_style_t* style[3]={
        &menu_style2,
        &menu_style0,
        &menu_style1,
    };
    static ksc_menu_t filemenu={
    .config=&filemenu_config,
    .list=mfile,
    .style=style,
    };
    if(screen == NULL){
        screen = kgetscreen();
    }
    filemenu.config->menu_obj_num = sizeof(style)/sizeof(style[0]);
    lfs_mount(&lfs,&cfg);
    lfs_dir_open(&lfs,&dir,"/");
    filedir(&filemenu);
    // printf("menu_num:%d\n",filemenu.config->menu_num);
    // KSC_menu_draw(screen,&filemenu,10,10);
    return &filemenu;
}

int filedir(ksc_menu_t* menu){
    uint8_t num = 0;
    struct lfs_info info;
    // 读取目录中的文件信息
    lfs_dir_read(&lfs,&dir,&info);
    //lfs_dir_read(&lfs,&dir,&info);//过滤.和..目录
    while(lfs_dir_read(&lfs,&dir,&info)){
        uint8_t len = strlen(info.name);
        memcpy(menu->list[num].name,info.name,len+1);
        menu->list[num].size = info.size;
        menu->list[num].type = info.type;
        menu->list[num].data = NULL;
        // printf("name:%s\n",info.name);
        num++;
    }
    menu->config->menu_num = num;
    return num;
}

#define KEY_UP KEY_A8
#define KEY_DOWN KEY_A2
#define KEY_LEFT KEY_A4
#define KEY_RIGHT KEY_A6
#define KEY_ENTER KEY_A5


int filemenu_update(ksc_menu_t* menu,uint8_t key){
    static char path[32]={0};
    path[0] = '/';
    // path[1] = '\0';
    int ret = 0;
    if(key == KEY_RIGHT){
        // printf("path:%s\n",menu->list[menu->config->menu_index].name);
        lfs_dir_close(&lfs,&dir);
        // if(strcmp(path,"/") != 0){
        //     strcat(path,"/");
        // }
        strcat(path,"/");
        strcat(path,menu->list[menu->config->menu_index].name);
        // strcat(path,"/");
        if (!(ret = lfs_dir_open(&lfs,&dir,path))){
            filedir(menu);
        }
        // printf("ret:%d\n",ret);
    }else if(key == KEY_LEFT){
        lfs_dir_close(&lfs,&dir);
        char* p = strrchr(path,'/');
        if(p){
            *p = '\0';
            if(path[0] == '\0'){
                path[0] = '/';
                path[1] = '\0';
            }
        }else{
            memset(path,0,sizeof(path));
            path[0] = '/';
            // path[1] = '\0';
        }
        if (!(ret = lfs_dir_open(&lfs,&dir,path))){
            filedir(menu);
        }
    }
    else if(key == KEY_UP){
        if(menu->config->menu_index == 0){
            return 0;
        }
        menu->config->menu_index--;
    }
    else if(key == KEY_DOWN){
        if(menu->config->menu_index >= menu->config->menu_num-1){
            return 0;
        }
        menu->config->menu_index++;
    }
    printf("path:%s\n",path);
    return ret;
}
