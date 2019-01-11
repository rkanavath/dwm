/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/*   Display modes of the tab bar: never shown, always shown, shown only in */
/*   monocle mode in presence of several windows.                           */
/*   Modes after showtab_nmodes are disabled                                */
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always};
static const int showtab            = showtab_auto; /* Default tab bar show mode */
static const int toptab             = False;    /* False means bottom tab bar */


static const char orange_red[]  = "#ff4500";
static const char blue[]        = "#224488";
static const char bright_blue[] = "#0066ff";
static const char black[]       = "#000000";
static const char gray[]        = "#bbbbbb";
static const char white[]       = "#ffffff";

static const char *fonts[]          = { "Inconsolata:size=18" };
static const char dmenufont[]       = "Inconsolata:size=18";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[SchemeLast][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel] =  { col_gray4, col_cyan,  col_cyan  },
	[SchemeUrg]     = { white,      orange_red, orange_red  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "A", "Z", "E", "Q", "S", "D" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
        { "emacs",       NULL,     NULL,       0,            1,           -1 },
	{ "firefox",     NULL,     NULL,       1 << 8,       1,           -1 },
};

/**
 * Rules hook
 *
 * This function is called once applyrules is done processing a client with the
 * client in question passed as an argument.
 */
void ruleshook(Client *c)
{
    // Certain floating Wine windows always get positioned off-screen. When
    // that happens, this code will center them.
    if (!strcmp(c->class, "Wine") && c->x < 1) {
        c->x = c->mon->mx + (c->mon->mw / 2 - WIDTH(c) / 2);
        c->y = c->mon->my + (c->mon->mh / 2 - HEIGHT(c) / 2);
    }

    // Mark windows that get created offscreen as urgent.
    if (!scanning && !ISVISIBLE(c)) {
        seturgent(c);
    }
}

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
  /* symbol     arrange function */
  /* first entry is default */
  { "[M]",      monocle },
  { "[]=",      tile },    
  { "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
/* static const char *termcmd[]  = { "st", "-e", "tmux", NULL }; */

static Key keys[] = {
	/* modifier             key    function        argument */
	{ MODKEY,               33,    spawn,          {.v = dmenucmd } }, // p
	{ MODKEY|ControlMask,   36,    spawn,          {.v = termcmd } }, // Return
	{ MODKEY,               56,    togglebar,      {0} },             // b
	{ MODKEY|ControlMask,   52,    tabmode,        {-1} },
	{ MODKEY,               44,    focusstack,     {.i = +1 } },      // j
	{ MODKEY,               45,    focusstack,     {.i = -1 } },      // k
	{ MODKEY,               31,    incnmaster,     {.i = +1 } },      // i
	{ MODKEY,               32,    incnmaster,     {.i = -1 } },      // o
	{ MODKEY,               43,    setmfact,       {.f = -0.05} },    // h
	{ MODKEY,               46,    setmfact,       {.f = +0.05} },    // l
	{ MODKEY,               36,    zoom,           {0} },             // Return
	{ MODKEY,               23,    view,           {0} },             // Tab
	{ MODKEY|ControlMask,   54,    killclient,     {0} },             // c
	{ MODKEY,               39,  lastclient,     {0} },	
	{ MODKEY|ControlMask,   28,    setlayout,      {.v = &layouts[0]} }, // t
	{ MODKEY|ControlMask,   41,    setlayout,      {.v = &layouts[2]} }, // f
	{ MODKEY|ControlMask,   47,    setlayout,      {.v = &layouts[1]} }, // m
	{ MODKEY|ControlMask,   65,    setlayout,      {0} },             // space
	{ MODKEY|ControlMask,   65,    togglefloating, {0} },             // space
	{ MODKEY,               19,    view,           {.ui = ~0 } },     // 0
	{ MODKEY|ControlMask,   19,    tag,            {.ui = ~0 } },     // 0
	
	{ MODKEY,               59,    focusmon,       {.i = -1 } },      // comma
	{ MODKEY,               60,    focusmon,       {.i = +1 } },      // period
	{ MODKEY|ControlMask,   59,    tagmon,         {.i = -1 } },      // comma
	{ MODKEY|ControlMask,   60,    tagmon,         {.i = +1 } },      // period
       
	TAGKEYS(                10,                    0)                 // 1
	TAGKEYS(                11,                    1)                 // 2
	TAGKEYS(                12,                    2)                 // 3
	TAGKEYS(                24,                    3)                 // a
	TAGKEYS(                25,                    4)                 // z
	TAGKEYS(                26,                    5)                 // e
	TAGKEYS(                38,                    6)                 // q
	TAGKEYS(                39,                    7)                 // s
	TAGKEYS(                40,                    8)                 // d
	{ MODKEY|ControlMask,     38,    quit,           {0} },             // q
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTabBar,            0,              Button1,        focuswin,       {0} },
};

