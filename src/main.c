#include "lvgl/lvgl.h"
#include "lv_drv_conf.h"
#include "display/drm.h"
#include "ui.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <linux/vt.h>

int main(void) {
    printf("\n[DEBUG] Programm-Start...\n");

    // 1. Konsole/Cursor killen
    int vt_fd = open("/dev/tty1", O_RDWR); 
    if (vt_fd >= 0) {
        ioctl(vt_fd, KDSETMODE, KD_GRAPHICS);
        close(vt_fd);
        printf("[DEBUG] Terminal-Grafikmodus aktiviert.\n");
    }

    lv_init();

    // 2. Hardware Driver Init
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    if (drm_disp_drv_init(&disp_drv) < 0) {
        printf("[ERROR] DRM-Initialisierung fehlgeschlagen!\n");
        return -1;
    }
    printf("[DEBUG] DRM-Treiber bereit.\n");

    // 3. Single Buffer (Heap)
    uint32_t width = 1920;
    uint32_t height = 1080;
    uint32_t buf_size = width * height / 10;
    lv_color_t *buf1 = malloc(buf_size * sizeof(lv_color_t));
    
    if (buf1 == NULL) {
        printf("[ERROR] Speicher fuer Buffer konnte nicht reserviert werden!\n");
        return -1;
    }

    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, buf_size);

    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = drm_flush;
    disp_drv.hor_res = width;
    disp_drv.ver_res = height;
    lv_disp_drv_register(&disp_drv);
    printf("[DEBUG] LVGL Display-Treiber registriert.\n");

    // 4. UI Initialisierung
    printf("[DEBUG] Rufe ui_init() auf...\n");
    ui_init(); 

    // 5. Screen-Check & Force-Setup
    if (objects.main != NULL) {
        printf("[DEBUG] objects.main gefunden! Erzwinger Styles...\n");
        
        // Hintergrund auf Schwarz
        lv_obj_set_style_bg_color(objects.main, lv_color_hex(0xFF0000), 0);
        lv_obj_set_style_bg_opa(objects.main, LV_OPA_COVER, 0);
        
        // Sofort laden
        lv_scr_load(objects.main);
        printf("[DEBUG] Screen geladen. Starte Fade-In Animation (200ms)...\n");
        
        // Fade-In Animation
        lv_scr_load_anim(objects.main, LV_SCR_LOAD_ANIM_FADE_IN, 200, 0, false);
    } else {
        printf("[ERROR] objects.main ist NULL! Check deine ui.h/ui.c\n");
    }

    // 6. Loop mit Heartbeat-Counter
    unsigned int counter = 0;
    printf("[DEBUG] Betrete Main-Loop...\n");

    while(1) {
        lv_timer_handler();
        ui_tick(); 
        
        // Alle ~5 Sekunden ein Lebenszeichen ausgeben (5ms * 1000)
        if (counter % 1000 == 0) {
            printf("[HEARTBEAT] Loop laeuft... (Tick: %u)\n", counter);
        }
        
        counter++;
        usleep(5000);
    }

    return 0;
}
