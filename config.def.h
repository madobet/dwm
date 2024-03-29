/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
// static const char *fonts[]          = { "Iosevka Term:size=10", "monospace:size=10" };
static const char dmenufont[]           = "monospace:size=10";
static const char col_gray1[]           = "#222222";
static const char col_gray2[]           = "#444444";
static const char col_gray3[]           = "#bbbbbb";
static const char col_gray4[]           = "#eeeeee";
static const char col_fg_color[]        = "#f9cec0";
static const char col_bg_color[]        = "#000000";
static const char col_cursor_color[]    = "#f9cec0";
static const char col_black[]           = "#544b49";
static const char col_red[]             = "#a55344";
static const char col_green[]           = "#a8b55e";
static const char col_yellow[]          = "#e3b27d";
static const char col_blue[]            = "#6d94c4";
static const char col_magenta[]         = "#6a6dac";
static const char col_cyan[]            = "#00b8b4";
static const char col_white[]           = "#fde2c9";
static const char col_bright_black[]    = "#6c524b";
static const char col_bright_red[]      = "#f7ac90";
static const char col_bright_green[]    = "#91dc84";
static const char col_bright_yellow[]   = "#e7c5a4";
static const char col_bright_blue[]     = "#bad5e2";
static const char col_bright_magenta[]  = "#897ed8";
static const char col_bright_cyan[]     = "#4fdad5";
static const char col_bright_white[]    = "#fdf2e4";
static const char col_pure[]            = "#ffffff";
static const char col_bg_black[]        = "#0f0f0f";
static const char col_grey[]            = "#373B41";
static const char col_bright_grey[]     = "#707880";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_fg_color, col_bg_color, col_gray2 },
	[SchemeSel]  = { col_bright_white, col_red,  col_black  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",      NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",   NULL,       NULL,       1 << 8,       0,           -1 },
	{ NULL,        NULL,  "floating_term", 0,            1,           -1 },
	{ "Tilda",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY  Mod4Mask
#define MODKEY2 Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() dmenu 的显示器输出？*/
static const char *dmenurun[]   = { NULL };
static const char *cmdlock[] 	= {"dm-tool", "lock", NULL};
// static const char *dmenuapp[]   = { "dmenu_path" ,"|" ,"dmenu" ,"-b" ,"-f" ,"-i" ,"-l" ,"18" ,"-p" ,"" ,"-fn" ,"monospace-9:Medium" ,"-bd" ,"1" ,"-w" ,"618" ,"-c" ,"-F" ,"-H" ,"${XDG_CACHE_HOME:-~/.cache}/runmenu.history" ,"|" ,"${SHELL:-'/bin/sh'}" ,"&" , NULL};
static const char *dmenuapp[]   = { "dmenu_path" ,"|" ,"dmenu" ,"-b" ,"-f" ,"-i" ,"-l" ,"18" ,"-p" ,"" ,"-fn" ,"monospace-9:Medium" ,"-bd" ,"1" ,"-w" ,"618" ,"-c" ,"-F" ,"-H" ,"${XDG_CACHE_HOME:-~/.cache}/runmenu.history" , NULL};
static const char *appmenu[]    = { "rofi", "-columns", "2", "-show-icons", "-icon-theme", "Papirus", "-no-lazy-grab", "-show", "drun", "-theme", "themes/appsmenu.rasi", NULL};
static const char *copyqboard[] = { "copyq", "toggle", NULL };
static const char *termmain[]   = { "st",      NULL };
static const char *termbakup[]  = { "xterm",   NULL };
static const char *filemanag[]  = { "pcmanfm", NULL };
static const char *volumeup[]   = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *volumedown[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
static const char *mute[]       = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "toggle", NULL };
static const char *micmute[]    = { "pactl", "set-source-mute", "@DEFAULT_SOURCE@", "toggle", NULL };
static const char *brightup[]   = { "xbacklight", "-inc", "5", NULL };
static const char *brightdown[] = { "xbacklight", "-dec", "5", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_l,      spawn,     	   {.v = cmdlock } },
	{ MODKEY,                       XK_s,      spawn,          {.v = appmenu }},
    { MODKEY,                       XK_q,      spawn,          {.v = copyqboard }},
    { MODKEY,                       XK_e,      spawn,          {.v = filemanag }},
    { 0,         XF86XK_AudioRaiseVolume,      spawn,          {.v = volumeup }},
    { 0,         XF86XK_AudioLowerVolume,      spawn,          {.v = volumedown }},
	{ 0,                XF86XK_AudioMute,      spawn,          {.v = mute    }},
	{ 0,             XF86XK_AudioMicMute,      spawn,          {.v = micmute }},
    { 0,          XF86XK_MonBrightnessUp,      spawn,          {.v = brightup }},
    { 0,        XF86XK_MonBrightnessDown,      spawn,          {.v = brightdown }},
	{ MODKEY,                       XK_Return, spawn,          {.v = termmain } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termbakup } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_l,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_h,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_u,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_i,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask|ControlMask, XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask|ControlMask, XK_e,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termmain } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
