#include "fdw.h"
#include "notif.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void
func_wrapper()
{
    NotifGeometry notifGeometry = { .border = 2, .height = 50, .width = 400, .insideX = 10, .insideY = 27 };
    char msg[MAX_MESSAGE_LEN(2, 400, 10, 13)] = "New rss feeds";
    char title[32] = "New rss feeds available!";

    NotifColors notifColors = { .background = "#0f111b", .foreground = "#ecf0c1" };
    Notif *notif = create_notif(msg, &notifGeometry, &notifColors);

    NotifPosition notifPos = { .height = 0, .width = 1500 };
    show_notif(notif, &notifPos, title);
}

int
main()
{
    char feeds[] = "/.sfeed/feeds/";
    char *home = getenv("HOME");
    char fullPath[256];
    strcpy(fullPath, home);
    strcat(fullPath, feeds);

    watch_dir(fullPath, func_wrapper);
}
