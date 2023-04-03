#pragma once

namespace VTE
{
namespace Parser
{

enum class SeqEnum
{
    NONE,        /* placeholder, no sequence parsed */

    IGNORE,      /* no-op character */
    GRAPHIC,     /* graphic character */
    CONTROL,     /* control character */
    ESCAPE,      /* escape sequence */
    CSI,         /* control sequence function */
    DCS,         /* device control string */
    OSC,         /* operating system control */
    SCI,         /* single character control function */
    APC,         /* application program command */
    PM,          /* privacy message */
    SOS,         /* start of string */

    N,
};

enum class SeqIntermediateCharacterEnum
{
    NONE    = 0,

    SPACE   = ' ',  /* 02/00 */
    BANG    = '!',  /* 02/01 */
    DQUOTE  = '"',  /* 02/02 */
    HASH    = '#',  /* 02/03 */
    CASH    = '$',  /* 02/04 */
    PERCENT = '%',  /* 02/05 */
    AND     = '&',  /* 02/06 */
    SQUOTE  = '\'', /* 02/07 */
    POPEN   = '(',  /* 02/08 */
    PCLOSE  = ')',  /* 02/09 */
    MULT    = '*',  /* 02/10 */
    PLUS    = '+',  /* 02/11 */
    COMMA   = ',',  /* 02/12 */
    MINUS   = '-',  /* 02/13 */
    DOT     = '.',  /* 02/14 */
    SLASH   = '/',  /* 02/15 */
};

#define VTE_SEQ_MAKE_INTERMEDIATE(c) ((c) - ' ' + 1)

enum class SeqIntermediateEnum
{
    NONE      = 0,

    SPACE     = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::SPACE)),
    BANG      = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::BANG)),
    DQUOTE    = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::DQUOTE)),
    HASH      = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::HASH)),
    CASH      = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::CASH)),
    PERCENT   = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::PERCENT)),
    AND       = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::AND)),
    SQUOTE    = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::SQUOTE)),
    POPEN     = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::POPEN)),
    PCLOSE    = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::PCLOSE)),
    MULT      = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::MULT)),
    PLUS      = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::PLUS)),
    COMMA     = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::COMMA)),
    MINUS     = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::MINUS)),
    DOT       = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::DOT)),
    SLASH     = VTE_SEQ_MAKE_INTERMEDIATE(static_cast<char>(SeqIntermediateCharacterEnum::SLASH)),
};

enum class SeqParameterCharacterEnum
{
    NONE  = 0,

    /* Numbers; not used         *  03/00..03/09 */
    /* COLON is reserved         = ':'   * 03/10 */
    /* SEMICOLON is reserved     = ';'   * 03/11 */
    LT    = '<', /* 03/12 */
    EQUAL = '=', /* 03/13 */
    GT    = '>', /* 03/14 */
    WHAT  = '?'  /* 03/15 */
};

#define VTE_SEQ_MAKE_PARAMETER(c) ('?' - (c) + 1)

enum class SeqParameterEnum
{
    NONE  = 0,

    LT    = VTE_SEQ_MAKE_PARAMETER(static_cast<char>(SeqParameterCharacterEnum::LT)),
    EQUAL = VTE_SEQ_MAKE_PARAMETER(static_cast<char>(SeqParameterCharacterEnum::EQUAL)),
    GT    = VTE_SEQ_MAKE_PARAMETER(static_cast<char>(SeqParameterCharacterEnum::GT)),
    WHAT  = VTE_SEQ_MAKE_PARAMETER(static_cast<char>(SeqParameterCharacterEnum::WHAT)),
};

enum class CommandEnum
{
    /* Implemented: */

    NONE, /* placeholder */
    GRAPHIC, /* graphics character */
    ACS, /* announce code structure */
    BEL, /* bell */
    BS, /* backspace */
    CBT, /* cursor backward tabulation */
    CHA, /* cursor horizontal absolute */
    CHT, /* cursor horizontal forward tabulation */
    CNL, /* cursor next line */
    CPL, /* cursor previous line */
    CR, /* carriage return */
    CTC, /* cursor tabulation control */
    CUB, /* cursor backward */
    CUD, /* cursor down */
    CUF, /* cursor forward */
    CUP, /* cursor position */
    CUU, /* cursor up */
    DA1, /* primary device attributes */
    DA2, /* secondary device attributes */
    DA3, /* tertiary device attributes */
    DCH, /* delete character */
    DECALN, /* screen alignment pattern */
    DECKPAM, /* keypad application mode */
    DECKPNM, /* keypad numeric mode */
    DECPCTERM_OR_XTERM_RPM, /* pcterm or xterm restore DEC private mode */
    DECRC, /* restore cursor */
    DECREQTPARM, /* request terminal parameters */
    DECREQTPARM_OR_WYCDIR, /* request terminal parameters or set current character attributes */
    DECRQCRA, /* request checksum of rectangular area */
    DECRQM_DEC, /* request mode dec */
    DECRQM_ECMA, /* request mode ecma */
    DECRQSS, /* request selection or setting */
    DECRQTSR, /* request terminal state report */
    DECSC, /* save cursor */
    DECSCL, /* select conformance level */
    DECSCUSR, /* set cursor style */
    DECSED, /* selective erase in display */
    DECSEL, /* selective erase in line */
    DECSIXEL, /* SIXEL graphics */
    DECSLPP, /* set lines per page */
    DECSLPP_OR_XTERM_WM, /* set lines per page or xterm window management */
    DECSLRM_OR_SCOSC, /* set left and right margins or SCO save cursor */
    DECSR, /* secure reset */
    DECST8C, /* set tab at every 8 columns */
    DECSTBM, /* set top and bottom margins */
    DECSTR, /* soft terminal reset */
    DL, /* delete line */
    DSR_DEC, /* device status report dec */
    DSR_ECMA, /* device status report ecma */
    ECH, /* erase character */
    ED, /* erase in display */
    EL, /* erase in line */
    FF, /* form feed */
    GnDm, /* Gn designate 9m charset */
    HPA, /* horizontal position absolute */
    HPR, /* horizontal position relative */
    HT, /* horizontal tab */
    HTJ, /* character tabulation with justification */
    HTS, /* horizontal tab set */
    HVP, /* horizontal and vertical position */
    ICH, /* insert character */
    IL, /* insert line */
    IND, /* index */
    LF, /* line feed */
    LS0, /* locking shift 0 */
    LS1, /* locking shift 1 */
    LS1R, /* locking shift 1 right */
    LS2, /* locking shift 2 */
    LS2R, /* locking shift 2 right */
    LS3, /* locking shift 3 */
    LS3R, /* locking shift 3 right */
    NEL, /* next line */
    OSC, /* operating system command */
    REP, /* repeat */
    RI, /* reverse index */
    RIS, /* reset to initial state */
    RM_DEC, /* reset mode dec */
    RM_ECMA, /* reset mode ecma */
    SCORC, /* SCO restore cursor */
    SCOSC, /* SCO save cursor */
    SCP, /* select character path */
    SD, /* scroll down */
    SD_OR_XTERM_IHMT, /* scroll down or xterm initiate highlight mouse tracking */
    SGR, /* select graphics rendition */
    SM_DEC, /* set mode dec */
    SM_ECMA, /* set mode ecma */
    SPD, /* select presentation directions */
    SS2, /* single shift 2 */
    SS3, /* single shift 3 */
    SU, /* scroll up */
    SUB, /* substitute */
    TBC, /* tab clear */
    TCC, /* tabulation centred on character */
    TSR, /* tabulation stop remove */
    VPA, /* vertical line position absolute */
    VPR, /* vertical line position relative */
    VT, /* vertical tag */
    XTERM_RPM, /* xterm restore DEC private mode */
    XTERM_SHIFTESCAPE, /* xterm set shift-escape */
    XTERM_SMGRAPHICS, /* xterm graphics attributes */
    XTERM_SPM, /* xterm save private mode */
    XTERM_WM, /* xterm window management */

    /* Unimplemented: */

    ACK, /* acknowledge */
    BPH, /* break permitted here */
    CCH, /* cancel character */
    CMD, /* coding method delimiter */
    CVT, /* cursor line tabulation */
    CnD, /* Cn designate */
    DAQ, /* define area qualification */
    DC1, /* device control 1 / XON */
    DC2, /* device control 2 */
    DC3, /* device control 3 / XOFF */
    DC4, /* device control 4 */
    DECAC, /* assign color */
    DECANM, /* ansi mode */
    DECARR, /* auto repeat rate */
    DECATC, /* alternate text color */
    DECAUPSS, /* assign user preferred supplemental sets */
    DECBI, /* back index */
    DECCARA, /* change attributes in rectangular area */
    DECCKD, /* copy key default */
    DECCRA, /* copy rectangular area */
    DECCRTST, /* CRT saver time */
    DECDC, /* delete column */
    DECDHL_BH, /* double width double height line: bottom half */
    DECDHL_TH, /* double width double height line: top half */
    DECDLD, /* dynamically redefinable character sets extension */
    DECDLDA, /* down line load allocation */
    DECDMAC, /* define macro */
    DECDWL, /* double width single height line */
    DECEFR, /* enable filter rectangle */
    DECELF, /* enable local functions */
    DECELR, /* enable locator reporting */
    DECERA, /* erase rectangular area */
    DECES, /* enable session */
    DECFI, /* forward index */
    DECFNK, /* dec function key / XTERM bracketed paste */
    DECFRA, /* fill rectangular area */
    DECIC, /* insert column */
    DECINVM, /* invoke macro */
    DECKBD, /* keyboard language selection */
    DECLANS, /* load answerback message */
    DECLBAN, /* load banner message */
    DECLBD, /* locator button define */
    DECLFKC, /* local function key control */
    DECLL, /* load leds */
    DECLTOD, /* load time of day */
    DECPAK, /* program alphanumeric key */
    DECPCTERM, /* pcterm */
    DECPFK, /* program function key */
    DECPKA, /* program key action */
    DECPKFMR, /* program key free memory report */
    DECPS, /* play sound */
    DECRARA, /* reverse attributes in rectangular area */
    DECREGIS, /* ReGIS graphics */
    DECRPAK, /* report all modifier/alphanumeric key state */
    DECRPDE, /* report displayed extent */
    DECRPFK, /* report function key definition */
    DECRPKT, /* report key type */
    DECRQDE, /* request display extent */
    DECRQKT, /* request key type */
    DECRQLP, /* request locator position */
    DECRQPKFM, /* request program key free memory */
    DECRQPSR, /* request presentation state report */
    DECRQUPSS, /* request user preferred supplemental set */
    DECRSPS, /* restore presentation state */
    DECRSTS, /* restore terminal state */
    DECSACE, /* select attribute change extent */
    DECSASD, /* select active status display */
    DECSCA, /* select character protection attribute */
    DECSCP, /* select communication port */
    DECSCPP, /* select columns per page */
    DECSCS, /* select communication speed */
    DECSDDT, /* select disconnect delay time */
    DECSDPT, /* select digital printed data type */
    DECSERA, /* selective erase rectangular area */
    DECSEST, /* energy saver time */
    DECSFC, /* select flow control */
    DECSGR, /* DEC select graphics rendition */
    DECSKCV, /* set key click volume */
    DECSLCK, /* set lock key style */
    DECSLE, /* select locator events */
    DECSLRM, /* set left and right margins */
    DECSMBV, /* set margin bell volume */
    DECSMKR, /* select modifier key reporting */
    DECSNLS, /* set lines per screen */
    DECSPMA, /* session page memory allocation */
    DECSPP, /* set port parameter */
    DECSPPCS, /* select pro printer character set */
    DECSPRTT, /* select printer type */
    DECSRFR, /* select refresh rate */
    DECSSCLS, /* set scroll speed */
    DECSSDT, /* select status display line type */
    DECSSL, /* select setup language */
    DECSTGLT, /* select color lookup table */
    DECSTRL, /* set transmit rate limit */
    DECSTUI, /* set terminal unit id */
    DECSWBV, /* set warning bell volume */
    DECSWL, /* single width single height line */
    DECSZS, /* select zero symbol */
    DECTID, /* select terminal id */
    DECTME, /* terminal mode emulation */
    DECTST, /* invoke confidence test */
    DECUDK, /* user defined keys */
    DECUS, /* update session */
    DLE, /* data link escape */
    DMI, /* disable manual input */
    DOCS, /* designate other coding system */
    DTA, /* dimension text area */
    EA, /* erase in area */
    EF, /* erase in field */
    EM, /* end of medium */
    EMI, /* enable manual input */
    ENQ, /* enquire */
    EOT, /* end of transmission */
    EPA, /* end of protected area */
    ESA, /* end of selected area */
    ETB, /* end of transmissionblock */
    ETX, /* end of text */
    FNK, /* function key */
    FNT, /* font selection */
    GCC, /* graphic character combination */
    GSM, /* graphic size modification */
    GSS, /* graphic size selection */
    GnDMm, /* Gn designate multibyte 9m charset */
    HPB, /* horizontal position backward */
    IDCS, /* identify DCS */
    IGS, /* identify graphic subrepertoire */
    INT, /* interrupt */
    IRR, /* identify revised registration */
    IS1, /* information separator 1 / unit separator (US) */
    IS2, /* information separator 2 / record separator (RS) */
    IS3, /* information separator 3 / group separator (GS) */
    IS4, /* information separator 4 / file separator (FS) */
    JFY, /* justify */
    MC_DEC, /* media copy dec */
    MC_ECMA, /* media copy ecma */
    MINTTY_PROGRESS, /* set progress report */
    MW, /* message waiting */
    NAK, /* negative acknowledge */
    NBH, /* no break permitted here */
    NP, /* next page */
    NUL, /* nul */
    PEC, /* presentation expand or contract */
    PFS, /* page format selection */
    PLD, /* partial line forward */
    PLU, /* partial line backward */
    PP, /* preceding page */
    PPA, /* page position absolute */
    PPB, /* page position backward */
    PPR, /* page position relative */
    PTX, /* parallel texts */
    PU1, /* private use 1 */
    PU2, /* private use 2 */
    QUAD, /* quad */
    RLOGIN_MML, /* RLogin music macro language */
    RM_HP, /* reset mode hp */
    SACS, /* set additional character separation */
    SAPV, /* select alternative presentation variants */
    SCO, /* select character orientation */
    SCS, /* set character spacing */
    SDS, /* start directed string */
    SEE, /* select editing extent */
    SEF, /* sheet eject and feed */
    SHS, /* select character spacing */
    SIMD, /* select implicit movement direction */
    SL, /* scroll left */
    SLH, /* set line home */
    SLL, /* set line limit */
    SLS, /* set line spacing */
    SM_HP, /* set mode hp */
    SOH, /* start of heading */
    SPA, /* start of protected area */
    SPH, /* set page home */
    SPI, /* spacing increment */
    SPL, /* set page limit */
    SPQR, /* select print quality and rapidity */
    SR, /* scroll right */
    SRCS, /* set reduced character separation */
    SRS, /* start reversed string */
    SSA, /* start of selected area */
    SSU, /* set size unit */
    SSW, /* set space width */
    ST, /* string terminator */
    STAB, /* selective tabulation */
    STS, /* set transmit state */
    STX, /* start of text */
    SVS, /* select line spacing */
    SYN, /* synchronise */
    TAC, /* tabulation aligned centre */
    TALE, /* tabulation aligned leading edge */
    TATE, /* tabulation aligned trailing edge */
    TSS, /* thine space specification */
    VPB, /* line position backward */
    VTS, /* line tabulation set */
    WYCAA, /* redefine character display attribute association */
    WYCDIR, /* set current character attributes */
    WYDHL_BH, /* single width double height line: bottom half */
    WYDHL_TH, /* single width double height line: top half */
    WYLSFNT, /* load soft font */
    WYSCRATE, /* set smooth scroll rate */
    XDGSYNC, /* synchronous update */
    XTERM_CHECKSUM_MODE, /* xterm DECRQCRA checksum mode */
    XTERM_GETXRES, /* xterm get X resource */
    XTERM_IHMT, /* xterm initiate highlight mouse tracking */
    XTERM_MLHP, /* xterm memory lock hp bugfix */
    XTERM_MODKEYS, /* xterm set key modifier options */
    XTERM_MUHP, /* xterm memory unlock hp bugfix */
    XTERM_POPCOLORS, /* xterm pop color palette stack */
    XTERM_POPSGR, /* xterm pop SGR stack */
    XTERM_PTRMODE, /* xterm set pointer mode */
    XTERM_PUSHCOLORS, /* xterm push color palette stack */
    XTERM_PUSHSGR, /* xterm push SGR stack */
    XTERM_REPORTCOLORS, /* xterm Report color palette stack */
    XTERM_REPORTSGR, /* xterm SGR report */
    XTERM_RQTCAP, /* xterm request termcap/terminfo */
    XTERM_RRV, /* xterm reset resource value */
    XTERM_RTM, /* xterm reset title mode */
    XTERM_STCAP, /* xterm set termcap/terminfo */
    XTERM_STM, /* xterm set title mode */

    N,
    FIRST_NOT_IMPL = ACK
};

/*
enum class ParserReply
{
#define _VTE_REPLY(cmd,type,final,pintro,intermediate,code) VTE_REPLY_##cmd,
#include "types/parser_reply.h"
#undef _VTE_REPLY

    VTE_REPLY_N,
};

enum class ParserCharset
{
#define _VTE_CHARSET_PASTE(name) VTE_CHARSET_##name,
#define _VTE_CHARSET(name) _VTE_CHARSET_PASTE(name)
#define _VTE_CHARSET_ALIAS_PASTE(name1,name2) VTE_CHARSET_##name1 = VTE_CHARSET_##name2,
#define _VTE_CHARSET_ALIAS(name1,name2) _VTE_CHARSET_ALIAS_PASTE(name1,name2)
#include "types/parser_charset.h"
#undef _VTE_CHARSET_PASTE
#undef _VTE_CHARSET
#undef _VTE_CHARSET_ALIAS_PASTE
#undef _VTE_CHARSET_ALIAS
};

enum class ParserOperatingSystemCommand
{
#define _VTE_OSC(osc,value) VTE_OSC_##osc = value,
#include "types/parser_osc.h"
#undef _VTE_OSC

    VTE_OSC_N
};

enum class SelectGraphicRendition
{
#define _VTE_SGR(name, value) VTE_SGR_##name = value,
#define _VTE_NGR(...)
#include "types/parser_sgr.h"
#undef _VTE_SGR
#undef _VTE_NGR
};

enum class DecSelectGraphicRendition
{
#define _VTE_SGR(name, value) VTE_DECSGR_##name = value,
#define _VTE_NGR(...)
#include "types/parser_decsgr.h"
#undef _VTE_SGR
#undef _VTE_NGR
};

#define VTE_CHARSET_CHARSET_MASK   ((1U << 16) - 1U)
#define VTE_CHARSET_SLOT_OFFSET    (16)
#define VTE_CHARSET_GET_CHARSET(c) ((c) & VTE_CHARSET_CHARSET_MASK)
#define VTE_CHARSET_GET_SLOT(c)    ((c) >> VTE_CHARSET_SLOT_OFFSET)
*/

enum class StateEnum
{
    GROUND,           /* initial state and ground */
    DCS_PASS_ESC,     /* ESC after DCS which may be ESC \ aka C0 ST */
    OSC_STRING_ESC,   /* ESC after OSC which may be ESC \ aka C0 ST */
    ESC,              /* ESC sequence was started */
    ESC_INT,          /* intermediate escape characters */
    CSI_ENTRY,        /* starting CSI sequence */
    CSI_PARAM,        /* CSI parameters */
    CSI_INT,          /* intermediate CSI characters */
    CSI_IGNORE,       /* CSI error; ignore this CSI sequence */
    DCS_ENTRY,        /* starting DCS sequence */
    DCS_PARAM,        /* DCS parameters */
    DCS_INT,          /* intermediate DCS characters */
    DCS_PASS,         /* DCS data passthrough */
    DCS_IGNORE,       /* DCS error; ignore this DCS sequence */
    OSC_STRING,       /* parsing OSC sequence */
    ST_IGNORE,        /* unimplemented seq; ignore until ST */
    SCI,              /* single character introducer sequence was started */

    N,
};

enum class ActionEnum
{
    CLEAR,
    CLEAR_INT,
    CLEAR_INT_AND_PARAMS,
    CLEAR_PARAMS_ONLY,
    IGNORE,
    PRINT,
    EXECUTE,
    COLLECT_ESC,
    COLLECT_CSI,
    COLLECT_DCS,
    COLLECT_PARAM,
    PARAM,
    FINISH_PARAM,
    FINISH_SUBPARAM,
    ESC_DISPATCH,
    CSI_DISPATCH,
    DCS_START,
    DCS_CONSUME,
    DCS_COLLECT,
    DCS_DISPATCH,
    OSC_START,
    OSC_COLLECT,
    OSC_DISPATCH,
    SCI_DISPATCH,
};

} // namespace Parser
} // namespace VTE
