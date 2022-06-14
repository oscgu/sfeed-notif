#include <X11/X.h>
#include <X11/Xlib.h>
#include <bits/types/__FILE.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "notif.h"

/* macros */
#define XK_Q 24
// #define DEBUG

/* function implementations */
static unsigned long
get_color_from_hex(char *hexcol, Display *display)
{
    XColor xcol;
    XParseColor(display, DefaultColormap(display, 0), hexcol, &xcol);
    XAllocColor(display, DefaultColormap(display, 0), &xcol);

    return xcol.pixel;
}

static void
destroy_notif(Notif *notif)
{
    XFlush(notif->display);
    XCloseDisplay(notif->display);
    free(notif);
}

void
show_notif(Notif *notif, const NotifPosition *notifpos, const char *title)
{
    XEvent event;
    #ifdef DEBUG
        printf("X: %d, Y: %d\n", notifpos->width, notifpos->height);
    #endif
    XCreateFontCursor(notif->display, 1);
    Window window = XCreateSimpleWindow(notif->display, RootWindow(notif->display, notif->default_screen), notifpos->width, notifpos->height, notif->notifGeometry.width, notif->notifGeometry.height, notif->notifGeometry.border, get_color_from_hex(notif->notifColors.foreground, notif->display), get_color_from_hex(notif->notifColors.background, notif->display));
    XSetForeground(notif->display, DefaultGC(notif->display, notif->default_screen), get_color_from_hex(notif->notifColors.foreground, notif->display));
    XSelectInput(notif->display, window, ExposureMask | KeyPressMask | ButtonPressMask);
    XSetTransientForHint(notif->display, window, RootWindow(notif->display, notif->default_screen));
    XStoreName(notif->display, window, title);
    XMapWindow(notif->display, window);

    for (;;)
    {
        XNextEvent(notif->display, &event);
        switch (event.type) {
        case Expose:
            XDrawString(notif->display, window, DefaultGC(notif->display, notif->default_screen), notif->notifGeometry.insideX, notif->notifGeometry.insideY, notif->message, strlen(notif->message));
            break;
        case KeyPress:
            if (event.xkey.keycode == XK_Q)
            {
                #ifdef DEBUG
                    printf("User pressed Q(uit)\n");
                #endif
                goto die;
            }
            break;
        case ButtonPress:
            if (event.xbutton.button == Button1)
            {
                #ifdef DEBUG
                    int x = event.xbutton.x;
                    int y = event.xbutton.y;
                    printf("Mouse click at x: %d y: %d\n", x, y);
                #endif
                goto die;
            }
        }
    }

die:
    destroy_notif(notif);
}


Notif *
create_notif(char *message, NotifGeometry *notifGeometry, NotifColors *notifColors)
{
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) 
    {
        fprintf(stderr, "Cannot open display:\nFile: %s\nLine: %d\n", __FILE__, __LINE__);
        exit(1);
    }

    int default_screen = DefaultScreen(display);

    Notif *notif = malloc(sizeof(Notif));
    notif->default_screen = default_screen;
    notif->display = display;
    notif->notifColors = *notifColors;
    notif->notifGeometry = *notifGeometry;
    notif->message = message;

    return notif;
}
