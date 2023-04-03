


#if !defined(_VTE_SEQ) || !defined(_VTE_NOQ)
#error "Must define _VTE_SEQ and _VTE_NOQ before including this file"
#endif

_VTE_SEQ(ICH, CSI, '@', NONE, 0, NONE, 0 ) /* insert character */
_VTE_NOQ(SL, CSI, '@', NONE, 1, SPACE, 0 ) /* scroll left */
_VTE_SEQ(CUU, CSI, 'A', NONE, 0, NONE, 0 ) /* cursor up */
_VTE_NOQ(SR, CSI, 'A', NONE, 1, SPACE, 0 ) /* scroll right */
_VTE_SEQ(CUD, CSI, 'B', NONE, 0, NONE, 0 ) /* cursor down */
_VTE_NOQ(GSM, CSI, 'B', NONE, 1, SPACE, 0 ) /* graphic size modification */
_VTE_SEQ(CUF, CSI, 'C', NONE, 0, NONE, 0 ) /* cursor forward */
_VTE_NOQ(GSS, CSI, 'C', NONE, 1, SPACE, 0 ) /* graphic size selection */
_VTE_SEQ(CUB, CSI, 'D', NONE, 0, NONE, 0 ) /* cursor backward */
_VTE_NOQ(FNT, CSI, 'D', NONE, 1, SPACE, 0 ) /* font selection */
_VTE_SEQ(CNL, CSI, 'E', NONE, 0, NONE, 0 ) /* cursor next line */
_VTE_SEQ(CPL, CSI, 'F', NONE, 0, NONE, 0 ) /* cursor previous line */
_VTE_NOQ(JFY, CSI, 'F', NONE, 1, SPACE, 0 ) /* justify */
_VTE_NOQ(TSS, CSI, 'E', NONE, 1, SPACE, 0 ) /* thine space specification */
_VTE_SEQ(CHA, CSI, 'G', NONE, 0, NONE, 0 ) /* cursor horizontal absolute */
_VTE_NOQ(SPI, CSI, 'G', NONE, 1, SPACE, 0 ) /* spacing increment */
_VTE_SEQ(CUP, CSI, 'H', NONE, 0, NONE, 0 ) /* cursor position */
_VTE_NOQ(QUAD, CSI, 'H', NONE, 1, SPACE, 0 ) /* quad */
_VTE_SEQ(CHT, CSI, 'I', NONE, 0, NONE, 0 ) /* cursor horizontal forward tabulation */
_VTE_NOQ(SSU, CSI, 'I', NONE, 1, SPACE, 0 ) /* set size unit */
_VTE_SEQ(ED, CSI, 'J', NONE, 0, NONE, 0 ) /* erase in display */
_VTE_NOQ(PFS, CSI, 'J', NONE, 1, SPACE, 0 ) /* page format selection */
_VTE_SEQ(DECSED, CSI, 'J', WHAT, 0, NONE, 0 ) /* selective erase in display */
_VTE_SEQ(EL, CSI, 'K', NONE, 0, NONE, 0 ) /* erase in line */
_VTE_NOQ(SHS, CSI, 'K', NONE, 1, SPACE, 0 ) /* select character spacing */
_VTE_SEQ(DECSEL, CSI, 'K', WHAT, 0, NONE, 0 ) /* selective erase in line */
_VTE_SEQ(IL, CSI, 'L', NONE, 0, NONE, 0 ) /* insert line */
_VTE_NOQ(SVS, CSI, 'L', NONE, 1, SPACE, 0 ) /* select line spacing */
_VTE_SEQ(DL, CSI, 'M', NONE, 0, NONE, 0 ) /* delete line */
_VTE_NOQ(IGS, CSI, 'M', NONE, 1, SPACE, 0 ) /* identify graphic subrepertoire */
_VTE_NOQ(EF, CSI, 'N', NONE, 0, NONE, 0 ) /* erase in field */
_VTE_NOQ(EA, CSI, 'O', NONE, 0, NONE, 0 ) /* erase in area */
_VTE_NOQ(IDCS, CSI, 'O', NONE, 1, SPACE, 0 ) /* identify DCS */
_VTE_SEQ(DCH, CSI, 'P', NONE, 0, NONE, 0 ) /* delete character */
_VTE_NOQ(PPA, CSI, 'P', NONE, 1, SPACE, 0 ) /* page position absolute */
_VTE_NOQ(XTERM_PUSHCOLORS, CSI, 'P', NONE, 1, HASH, 0 ) /* xterm push color palette stack */
_VTE_NOQ(SEE, CSI, 'Q', NONE, 0, NONE, 0 ) /* select editing extent */
_VTE_NOQ(PPR, CSI, 'Q', NONE, 1, SPACE, 0 ) /* page position relative */
_VTE_NOQ(XTERM_POPCOLORS, CSI, 'Q', NONE, 1, HASH, 0 ) /* xterm pop color palette stack */
_VTE_NOQ(PPB, CSI, 'R', NONE, 1, SPACE, 0 ) /* page position backward */
_VTE_NOQ(XTERM_REPORTCOLORS, CSI, 'R', NONE, 1, HASH, 0 ) /* xterm Report color palette stack */
_VTE_SEQ(SU, CSI, 'S', NONE, 0, NONE, 0 ) /* scroll up */
_VTE_SEQ(SPD, CSI, 'S', NONE, 1, SPACE, 0 ) /* select presentation directions */
_VTE_SEQ(XTERM_SMGRAPHICS, CSI, 'S', WHAT, 0, NONE, 0 ) /* xterm graphics attributes */
_VTE_SEQ(SD_OR_XTERM_IHMT, CSI, 'T', NONE, 0, NONE, 0 ) /* scroll down or xterm initiate highlight mouse tracking */
_VTE_NOQ(DTA, CSI, 'T', NONE, 1, SPACE, 0 ) /* dimension text area */
_VTE_NOQ(XTERM_RTM, CSI, 'T', GT, 0, NONE, 0 ) /* xterm reset title mode */
_VTE_NOQ(NP, CSI, 'U', NONE, 0, NONE, 0 ) /* next page */
_VTE_NOQ(SLH, CSI, 'U', NONE, 1, SPACE, 0 ) /* set line home */
_VTE_NOQ(PP, CSI, 'V', NONE, 0, NONE, 0 ) /* preceding page */
_VTE_NOQ(SLL, CSI, 'V', NONE, 1, SPACE, 0 ) /* set line limit */
_VTE_SEQ(CTC, CSI, 'W', NONE, 0, NONE, 0 ) /* cursor tabulation control */
_VTE_NOQ(FNK, CSI, 'W', NONE, 1, SPACE, 0 ) /* function key */
_VTE_SEQ(DECST8C, CSI, 'W', WHAT, 0, NONE, 0 ) /* set tab at every 8 columns */
_VTE_SEQ(ECH, CSI, 'X', NONE, 0, NONE, 0 ) /* erase character */
_VTE_NOQ(SPQR, CSI, 'X', NONE, 1, SPACE, 0 ) /* select print quality and rapidity */
_VTE_NOQ(CVT, CSI, 'Y', NONE, 0, NONE, 0 ) /* cursor line tabulation */
_VTE_NOQ(SEF, CSI, 'Y', NONE, 1, SPACE, 0 ) /* sheet eject and feed */
_VTE_SEQ(CBT, CSI, 'Z', NONE, 0, NONE, 0 ) /* cursor backward tabulation */
_VTE_NOQ(PEC, CSI, 'Z', NONE, 1, SPACE, 0 ) /* presentation expand or contract */
_VTE_NOQ(SRS, CSI, '[', NONE, 0, NONE, 0 ) /* start reversed string */
_VTE_NOQ(SSW, CSI, '[', NONE, 1, SPACE, 0 ) /* set space width */
_VTE_NOQ(PTX, CSI, '\\', NONE, 0, NONE, 0 ) /* parallel texts */
_VTE_NOQ(SACS, CSI, '\\', NONE, 1, SPACE, 0 ) /* set additional character separation */
_VTE_NOQ(SDS, CSI, ']', NONE, 0, NONE, 0 ) /* start directed string */
_VTE_NOQ(SAPV, CSI, ']', NONE, 1, SPACE, 0 ) /* select alternative presentation variants */
_VTE_NOQ(SIMD, CSI, '^', NONE, 0, NONE, 0 ) /* select implicit movement direction */
_VTE_NOQ(STAB, CSI, '^', NONE, 1, SPACE, 0 ) /* selective tabulation */
_VTE_NOQ(GCC, CSI, '_', NONE, 1, SPACE, 0 ) /* graphic character combination */
_VTE_SEQ(HPA, CSI, '`', NONE, 0, NONE, 0 ) /* horizontal position absolute */
_VTE_NOQ(TATE, CSI, '`', NONE, 1, SPACE, 0 ) /* tabulation aligned trailing edge */
_VTE_SEQ(HPR, CSI, 'a', NONE, 0, NONE, 0 ) /* horizontal position relative */
_VTE_NOQ(TALE, CSI, 'a', NONE, 1, SPACE, 0 ) /* tabulation aligned leading edge */
_VTE_SEQ(REP, CSI, 'b', NONE, 0, NONE, 0 ) /* repeat */
_VTE_NOQ(TAC, CSI, 'b', NONE, 1, SPACE, 0 ) /* tabulation aligned centre */
_VTE_SEQ(DA1, CSI, 'c', NONE, 0, NONE, 0 ) /* primary device attributes */
_VTE_SEQ(TCC, CSI, 'c', NONE, 1, SPACE, 0 ) /* tabulation centred on character */
_VTE_SEQ(DA3, CSI, 'c', EQUAL, 0, NONE, 0 ) /* tertiary device attributes */
_VTE_SEQ(DA2, CSI, 'c', GT, 0, NONE, 0 ) /* secondary device attributes */
_VTE_SEQ(VPA, CSI, 'd', NONE, 0, NONE, 0 ) /* vertical line position absolute */
_VTE_SEQ(TSR, CSI, 'd', NONE, 1, SPACE, 0 ) /* tabulation stop remove */
_VTE_SEQ(VPR, CSI, 'e', NONE, 0, NONE, 0 ) /* vertical line position relative */
_VTE_NOQ(SCO, CSI, 'e', NONE, 1, SPACE, 0 ) /* select character orientation */
_VTE_SEQ(HVP, CSI, 'f', NONE, 0, NONE, 0 ) /* horizontal and vertical position */
_VTE_NOQ(SRCS, CSI, 'f', NONE, 1, SPACE, 0 ) /* set reduced character separation */
_VTE_SEQ(TBC, CSI, 'g', NONE, 0, NONE, 0 ) /* tab clear */
_VTE_NOQ(SCS, CSI, 'g', NONE, 1, SPACE, 0 ) /* set character spacing */
_VTE_NOQ(DECLFKC, CSI, 'g', NONE, 1, MULT, 0 ) /* local function key control */
_VTE_SEQ(SM_ECMA, CSI, 'h', NONE, 0, NONE, 0 ) /* set mode ecma */
_VTE_NOQ(SLS, CSI, 'h', NONE, 1, SPACE, 0 ) /* set line spacing */
_VTE_NOQ(SM_HP, CSI, 'h', GT, 0, NONE, 0 ) /* set mode hp */
_VTE_SEQ(SM_DEC, CSI, 'h', WHAT, 0, NONE, 0 ) /* set mode dec */
_VTE_NOQ(MC_ECMA, CSI, 'i', NONE, 0, NONE, 0 ) /* media copy ecma */
_VTE_NOQ(SPH, CSI, 'i', NONE, 1, SPACE, 0 ) /* set page home */
_VTE_NOQ(MC_DEC, CSI, 'i', WHAT, 0, NONE, 0 ) /* media copy dec */
_VTE_NOQ(HPB, CSI, 'j', NONE, 0, NONE, 0 ) /* horizontal position backward */
_VTE_NOQ(SPL, CSI, 'j', NONE, 1, SPACE, 0 ) /* set page limit */
_VTE_NOQ(VPB, CSI, 'k', NONE, 0, NONE, 0 ) /* line position backward */
_VTE_SEQ(SCP, CSI, 'k', NONE, 1, SPACE, 0 ) /* select character path */
_VTE_SEQ(RM_ECMA, CSI, 'l', NONE, 0, NONE, 0 ) /* reset mode ecma */
_VTE_NOQ(RM_HP, CSI, 'l', GT, 0, NONE, 0 ) /* reset mode hp */
_VTE_SEQ(RM_DEC, CSI, 'l', WHAT, 0, NONE, 0 ) /* reset mode dec */
_VTE_SEQ(SGR, CSI, 'm', NONE, 0, NONE, 0 ) /* select graphics rendition */
_VTE_NOQ(DECSGR, CSI, 'm', WHAT, 0, NONE, 0 ) /* DEC select graphics rendition */
_VTE_NOQ(XTERM_MODKEYS, CSI, 'm', GT, 0, NONE, 0 ) /* xterm set key modifier options */
_VTE_SEQ(DSR_ECMA, CSI, 'n', NONE, 0, NONE, 0 ) /* device status report ecma */
_VTE_NOQ(XTERM_RRV, CSI, 'n', GT, 0, NONE, 0 ) /* xterm reset resource value */
_VTE_SEQ(DSR_DEC, CSI, 'n', WHAT, 0, NONE, 0 ) /* device status report dec */
_VTE_NOQ(DAQ, CSI, 'o', NONE, 0, NONE, 0 ) /* define area qualification */
_VTE_NOQ(DECSSL, CSI, 'p', NONE, 0, NONE, 0 ) /* select setup language */
_VTE_NOQ(DECSSCLS, CSI, 'p', NONE, 1, SPACE, 0 ) /* set scroll speed */
_VTE_SEQ(DECSTR, CSI, 'p', NONE, 1, BANG, 0 ) /* soft terminal reset */
_VTE_SEQ(DECSCL, CSI, 'p', NONE, 1, DQUOTE, 0 ) /* select conformance level */
_VTE_NOQ(XTERM_PUSHSGR, CSI, 'p', NONE, 1, HASH, 0 ) /* xterm push SGR stack */
_VTE_SEQ(DECRQM_ECMA, CSI, 'p', NONE, 1, CASH, 0 ) /* request mode ecma */
_VTE_NOQ(DECSDPT, CSI, 'p', NONE, 1, PCLOSE, 0 ) /* select digital printed data type */
_VTE_NOQ(DECSPPCS, CSI, 'p', NONE, 1, MULT, 0 ) /* select pro printer character set */
_VTE_SEQ(DECSR, CSI, 'p', NONE, 1, PLUS, 0 ) /* secure reset */
_VTE_NOQ(DECLTOD, CSI, 'p', NONE, 1, COMMA, 0 ) /* load time of day */
_VTE_NOQ(DECARR, CSI, 'p', NONE, 1, MINUS, 0 ) /* auto repeat rate */
_VTE_NOQ(XTERM_PTRMODE, CSI, 'p', GT, 0, NONE, 0 ) /* xterm set pointer mode */
_VTE_SEQ(DECRQM_DEC, CSI, 'p', WHAT, 1, CASH, 0 ) /* request mode dec */
_VTE_NOQ(DECLL, CSI, 'q', NONE, 0, NONE, 0 ) /* load leds */
_VTE_SEQ(DECSCUSR, CSI, 'q', NONE, 1, SPACE, 0 ) /* set cursor style */
_VTE_NOQ(DECSCA, CSI, 'q', NONE, 1, DQUOTE, 0 ) /* select character protection attribute */
_VTE_NOQ(XTERM_POPSGR, CSI, 'q', NONE, 1, HASH, 0 ) /* xterm pop SGR stack */
_VTE_NOQ(DECSDDT, CSI, 'q', NONE, 1, CASH, 0 ) /* select disconnect delay time */
_VTE_NOQ(MINTTY_PROGRESS, CSI, 'q', NONE, 1, PERCENT, 0 ) /* set progress report */
_VTE_SEQ(DECSR, CSI, 'q', NONE, 1, MULT, 0 ) /* secure reset */
_VTE_NOQ(DECELF, CSI, 'q', NONE, 1, PLUS, 0 ) /* enable local functions */
_VTE_NOQ(DECTID, CSI, 'q', NONE, 1, COMMA, 0 ) /* select terminal id */
_VTE_NOQ(DECCRTST, CSI, 'q', NONE, 1, MINUS, 0 ) /* CRT saver time */
_VTE_SEQ(DECSTBM, CSI, 'r', NONE, 0, NONE, 0 ) /* set top and bottom margins */
_VTE_NOQ(DECSKCV, CSI, 'r', NONE, 1, SPACE, 0 ) /* set key click volume */
_VTE_NOQ(DECCARA, CSI, 'r', NONE, 1, CASH, 0 ) /* change attributes in rectangular area */
_VTE_NOQ(DECSCS, CSI, 'r', NONE, 1, MULT, 0 ) /* select communication speed */
_VTE_NOQ(DECSMKR, CSI, 'r', NONE, 1, PLUS, 0 ) /* select modifier key reporting */
_VTE_NOQ(DECSEST, CSI, 'r', NONE, 1, MINUS, 0 ) /* energy saver time */
_VTE_SEQ(DECPCTERM_OR_XTERM_RPM, CSI, 'r', WHAT, 0, NONE, 0 ) /* pcterm or xterm restore DEC private mode */
_VTE_SEQ(DECSLRM_OR_SCOSC, CSI, 's', NONE, 0, NONE, 0 ) /* set left and right margins or SCO save cursor */
_VTE_NOQ(DECSPRTT, CSI, 's', NONE, 1, CASH, 0 ) /* select printer type */
_VTE_NOQ(DECSFC, CSI, 's', NONE, 1, MULT, 0 ) /* select flow control */
_VTE_SEQ(XTERM_SHIFTESCAPE, CSI, 's', GT, 0, NONE, 0 ) /* xterm set shift-escape */
_VTE_SEQ(XTERM_SPM, CSI, 's', WHAT, 0, NONE, 0 ) /* xterm save private mode */
_VTE_SEQ(DECSLPP_OR_XTERM_WM, CSI, 't', NONE, 0, NONE, 0 ) /* set lines per page or xterm window management */
_VTE_NOQ(DECSWBV, CSI, 't', NONE, 1, SPACE, 0 ) /* set warning bell volume */
_VTE_NOQ(DECSRFR, CSI, 't', NONE, 1, DQUOTE, 0 ) /* select refresh rate */
_VTE_NOQ(DECRARA, CSI, 't', NONE, 1, CASH, 0 ) /* reverse attributes in rectangular area */
_VTE_NOQ(XTERM_STM, CSI, 't', GT, 0, NONE, 0 ) /* xterm set title mode */
_VTE_SEQ(SCORC, CSI, 'u', NONE, 0, NONE, 0 ) /* SCO restore cursor */
_VTE_NOQ(DECSMBV, CSI, 'u', NONE, 1, SPACE, 0 ) /* set margin bell volume */
_VTE_NOQ(DECSTRL, CSI, 'u', NONE, 1, DQUOTE, 0 ) /* set transmit rate limit */
_VTE_SEQ(DECRQTSR, CSI, 'u', NONE, 1, CASH, 0 ) /* request terminal state report */
_VTE_NOQ(DECSCP, CSI, 'u', NONE, 1, MULT, 0 ) /* select communication port */
_VTE_NOQ(DECRQKT, CSI, 'u', NONE, 1, COMMA, 0 ) /* request key type */
_VTE_NOQ(DECRQUPSS, CSI, 'u', WHAT, 0, NONE, 0 ) /* request user preferred supplemental set */
_VTE_NOQ(DECSLCK, CSI, 'v', NONE, 1, SPACE, 0 ) /* set lock key style */
_VTE_NOQ(DECRQDE, CSI, 'v', NONE, 1, DQUOTE, 0 ) /* request display extent */
_VTE_NOQ(DECCRA, CSI, 'v', NONE, 1, CASH, 0 ) /* copy rectangular area */
_VTE_NOQ(DECRPKT, CSI, 'v', NONE, 1, COMMA, 0 ) /* report key type */
_VTE_NOQ(WYCAA, CSI, 'w', NONE, 0, NONE, 0 ) /* redefine character display attribute association */
_VTE_NOQ(DECRPDE, CSI, 'w', NONE, 1, DQUOTE, 0 ) /* report displayed extent */
_VTE_NOQ(DECRQPSR, CSI, 'w', NONE, 1, CASH, 0 ) /* request presentation state report */
_VTE_NOQ(DECEFR, CSI, 'w', NONE, 1, SQUOTE, 0 ) /* enable filter rectangle */
_VTE_NOQ(DECSPP, CSI, 'w', NONE, 1, PLUS, 0 ) /* set port parameter */
_VTE_SEQ(DECREQTPARM_OR_WYCDIR, CSI, 'x', NONE, 0, NONE, 0 ) /* request terminal parameters or set current character attributes */
_VTE_NOQ(DECFRA, CSI, 'x', NONE, 1, CASH, 0 ) /* fill rectangular area */
_VTE_NOQ(DECES, CSI, 'x', NONE, 1, AND, 0 ) /* enable session */
_VTE_NOQ(DECSACE, CSI, 'x', NONE, 1, MULT, 0 ) /* select attribute change extent */
_VTE_NOQ(DECRQPKFM, CSI, 'x', NONE, 1, PLUS, 0 ) /* request program key free memory */
_VTE_NOQ(DECSPMA, CSI, 'x', NONE, 1, COMMA, 0 ) /* session page memory allocation */
_VTE_NOQ(DECTST, CSI, 'y', NONE, 0, NONE, 0 ) /* invoke confidence test */
_VTE_NOQ(XTERM_CHECKSUM_MODE, CSI, 'y', NONE, 1, HASH, 0 ) /* xterm DECRQCRA checksum mode */
_VTE_SEQ(DECRQCRA, CSI, 'y', NONE, 1, MULT, 0 ) /* request checksum of rectangular area */
_VTE_NOQ(DECPKFMR, CSI, 'y', NONE, 1, PLUS, 0 ) /* program key free memory report */
_VTE_NOQ(DECUS, CSI, 'y', NONE, 1, COMMA, 0 ) /* update session */
_VTE_NOQ(WYSCRATE, CSI, 'z', NONE, 0, NONE, 0 ) /* set smooth scroll rate */
_VTE_NOQ(DECERA, CSI, 'z', NONE, 1, CASH, 0 ) /* erase rectangular area */
_VTE_NOQ(DECELR, CSI, 'z', NONE, 1, SQUOTE, 0 ) /* enable locator reporting */
_VTE_NOQ(DECINVM, CSI, 'z', NONE, 1, MULT, 0 ) /* invoke macro */
_VTE_NOQ(DECPKA, CSI, 'z', NONE, 1, PLUS, 0 ) /* program key action */
_VTE_NOQ(DECDLDA, CSI, 'z', NONE, 1, COMMA, 0 ) /* down line load allocation */
_VTE_NOQ(XTERM_PUSHSGR, CSI, '{', NONE, 1, HASH, 0 ) /* xterm push SGR stack */
_VTE_NOQ(DECSERA, CSI, '{', NONE, 1, CASH, 0 ) /* selective erase rectangular area */
_VTE_NOQ(DECSLE, CSI, '{', NONE, 1, SQUOTE, 0 ) /* select locator events */
_VTE_NOQ(DECSTGLT, CSI, '{', NONE, 1, PCLOSE, 0 ) /* select color lookup table */
_VTE_NOQ(DECSZS, CSI, '{', NONE, 1, COMMA, 0 ) /* select zero symbol */
_VTE_NOQ(XTERM_REPORTSGR, CSI, '|', NONE, 1, HASH, 0 ) /* xterm SGR report */
_VTE_NOQ(DECSCPP, CSI, '|', NONE, 1, CASH, 0 ) /* select columns per page */
_VTE_NOQ(DECRQLP, CSI, '|', NONE, 1, SQUOTE, 0 ) /* request locator position */
_VTE_NOQ(DECSNLS, CSI, '|', NONE, 1, MULT, 0 ) /* set lines per screen */
_VTE_NOQ(DECAC, CSI, '|', NONE, 1, COMMA, 0 ) /* assign color */
_VTE_NOQ(DECKBD, CSI, '}', NONE, 1, SPACE, 0 ) /* keyboard language selection */
_VTE_NOQ(XTERM_POPSGR, CSI, '}', NONE, 1, HASH, 0 ) /* xterm pop SGR stack */
_VTE_NOQ(DECSASD, CSI, '}', NONE, 1, CASH, 0 ) /* select active status display */
_VTE_NOQ(DECIC, CSI, '}', NONE, 1, SQUOTE, 0 ) /* insert column */
_VTE_NOQ(DECATC, CSI, '}', NONE, 1, COMMA, 0 ) /* alternate text color */
_VTE_NOQ(DECFNK, CSI, '~', NONE, 0, NONE, 0 ) /* dec function key / XTERM bracketed paste */
_VTE_NOQ(DECTME, CSI, '~', NONE, 1, SPACE, 0 ) /* terminal mode emulation */
_VTE_NOQ(DECSSDT, CSI, '~', NONE, 1, CASH, 0 ) /* select status display line type */
_VTE_NOQ(DECDC, CSI, '~', NONE, 1, SQUOTE, 0 ) /* delete column */
_VTE_NOQ(DECPS, CSI, '~', NONE, 1, COMMA, 0 ) /* play sound */
