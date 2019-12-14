/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Iosevka:size=10", "monospace:size=10" };
static const char dmenufont[]       = "Iosevka:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
// static const char col_cyan[]        = "#005577";
static const char col_fg_color[]        = "#dddddd";
static const char col_bg_color[]        = "#000000";
static const char col_cursor_color[]    = "#dddddd";
static const char col_black[]           = "#000000";
static const char col_red1[]            = "#cc0403";
static const char col_green[]           = "#19cb00";
static const char col_yellow[]          = "#cecb00";
static const char col_blue[]            = "#0d73cc";
static const char col_magenta[]         = "#cb1ed1";
static const char col_cyan[]            = "#0dcdcd";
static const char col_white[]           = "#dddddd";
static const char col_bright_black[]    = "#767676";
static const char col_bright_red[]      = "#f2201f";
static const char col_bright_green[]    = "#23fd00";
static const char col_bright_yellow[]   = "#fffd00";
static const char col_bright_blue[]     = "#1a8fff";
static const char col_bright_magenta[]  = "#fd28ff";
static const char col_bright_cyan[]     = "#14ffff";
static const char col_bright_white[]    = "#ffffff";
static const char col_pure[]            = "#ffffff";
static const char col_bg_black[]        = "#0f0f0f";
static const char col_grey[]            = "#373B41";
static const char col_bright_grey[]     = "#707880";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
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
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
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
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenurun, manipulated in spawn() dmenu 的显示器输出？*/
static const char *dmenurun[] = { "dmenu_run", "-b", "-f", "-i", "-l", "20", "-p", "Run", "-fn", "VarelaRound-10:Bold", "-nb", col_bg_color, "-nf", col_fg_color, "-sb", col_bright_white, "-sf", col_bg_color, "-bd", "3", "-w", "600", "-c", "-F", "-H", "${XDG_CACHE_HOME:-\"$HOME/.cache\"}/dmenu_run.history", NULL };
static const char *dmenuapp[] = { "dmenu_run", "-b", "-f", "-i", "-l", "15", "-p", "App", "-fn", "NotoSansSC-10:Bold", "-nb", col_bg_color, "-nf", col_fg_color, "-sb", col_bright_white, "-sf", col_bg_color, "-bd", "0", "-y", "0", "-w", "880", "-c", "-F", "-H", "${XDG_CACHE_HOME:-\"$HOME/.cache\"}/dmenu_app.history", NULL };
static const char *termmain[]  = { "start-term", "main", "float", NULL };
static const char *termbakup[]  = { "start-term", "kitty", "float", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenurun } },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          {.v = dmenuapp } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termmain } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termbakup } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_l,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_h,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_u,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_i,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask|ControlMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
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

