#include <X11/Xlib.h>

/* macros */
#define MAX_MESSAGE_LEN(border, width, insideX, padding) (((width / 4) - (2 * border) - (2 * insideX) - padding))

/* structs */
typedef struct {
    unsigned int width;
    unsigned int height;
} NotifPosition;

typedef struct {
    char *foreground;
    char *background;
} NotifColors;

typedef struct {
    unsigned short int width;
    unsigned short int height;
    unsigned short int border;

    unsigned short int insideX;
    unsigned short int insideY;
} NotifGeometry;

typedef struct {
    NotifColors   notifColors;
    unsigned short int default_screen;
    NotifGeometry notifGeometry;
    Display      *display;
    char         *message;
} Notif;

/* function definitions */
Notif *create_notif(char *message, NotifGeometry *notifGeometry, NotifColors *notifColors);
void show_notif(Notif *notif, const NotifPosition *displayDimensions, const char *title);
