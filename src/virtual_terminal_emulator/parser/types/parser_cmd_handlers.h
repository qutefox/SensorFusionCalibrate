


#if !defined(_VTE_CMD_HANDLER) || !defined(_VTE_CMD_HANDLER_R) || !defined(_VTE_CMD_HANDLER_NOP)
#error "Must define _VTE_CMD_HANDLER, _VTE_CMD_HANDLER_R and _VTE_CMD_HANDLER_NOP before including this file"
#endif

_VTE_CMD_HANDLER_NOP(NONE) /* placeholder */
_VTE_CMD_HANDLER(GRAPHIC) /* graphics character */
_VTE_CMD_HANDLER_NOP(ACK) /* acknowledge */
_VTE_CMD_HANDLER(ACS) /* announce code structure */
_VTE_CMD_HANDLER(BEL) /* bell */
_VTE_CMD_HANDLER_NOP(BPH) /* break permitted here */
_VTE_CMD_HANDLER(BS) /* backspace */
_VTE_CMD_HANDLER(CBT) /* cursor backward tabulation */
_VTE_CMD_HANDLER_NOP(CCH) /* cancel character */
_VTE_CMD_HANDLER(CHA) /* cursor horizontal absolute */
_VTE_CMD_HANDLER(CHT) /* cursor horizontal forward tabulation */
_VTE_CMD_HANDLER_NOP(CMD) /* coding method delimiter */
_VTE_CMD_HANDLER(CNL) /* cursor next line */
_VTE_CMD_HANDLER(CPL) /* cursor previous line */
_VTE_CMD_HANDLER(CR) /* carriage return */
_VTE_CMD_HANDLER(CTC) /* cursor tabulation control */
_VTE_CMD_HANDLER(CUB) /* cursor backward */
_VTE_CMD_HANDLER(CUD) /* cursor down */
_VTE_CMD_HANDLER(CUF) /* cursor forward */
_VTE_CMD_HANDLER(CUP) /* cursor position */
_VTE_CMD_HANDLER(CUU) /* cursor up */
_VTE_CMD_HANDLER_NOP(CVT) /* cursor line tabulation */
_VTE_CMD_HANDLER_NOP(CnD) /* Cn designate */
_VTE_CMD_HANDLER(DA1) /* primary device attributes */
_VTE_CMD_HANDLER(DA2) /* secondary device attributes */
_VTE_CMD_HANDLER(DA3) /* tertiary device attributes */
_VTE_CMD_HANDLER_NOP(DAQ) /* define area qualification */
_VTE_CMD_HANDLER_NOP(DC1) /* device control 1 / XON */
_VTE_CMD_HANDLER_NOP(DC2) /* device control 2 */
_VTE_CMD_HANDLER_NOP(DC3) /* device control 3 / XOFF */
_VTE_CMD_HANDLER_NOP(DC4) /* device control 4 */
_VTE_CMD_HANDLER(DCH) /* delete character */
_VTE_CMD_HANDLER_NOP(DECAC) /* assign color */
_VTE_CMD_HANDLER(DECALN) /* screen alignment pattern */
_VTE_CMD_HANDLER_NOP(DECANM) /* ansi mode */
_VTE_CMD_HANDLER_NOP(DECARR) /* auto repeat rate */
_VTE_CMD_HANDLER_NOP(DECATC) /* alternate text color */
_VTE_CMD_HANDLER_NOP(DECAUPSS) /* assign user preferred supplemental sets */
_VTE_CMD_HANDLER_NOP(DECBI) /* back index */
_VTE_CMD_HANDLER_NOP(DECCARA) /* change attributes in rectangular area */
_VTE_CMD_HANDLER_NOP(DECCKD) /* copy key default */
_VTE_CMD_HANDLER_NOP(DECCRA) /* copy rectangular area */
_VTE_CMD_HANDLER_NOP(DECCRTST) /* CRT saver time */
_VTE_CMD_HANDLER_NOP(DECDC) /* delete column */
_VTE_CMD_HANDLER_NOP(DECDHL_BH) /* double width double height line: bottom half */
_VTE_CMD_HANDLER_NOP(DECDHL_TH) /* double width double height line: top half */
_VTE_CMD_HANDLER_NOP(DECDLD) /* dynamically redefinable character sets extension */
_VTE_CMD_HANDLER_NOP(DECDLDA) /* down line load allocation */
_VTE_CMD_HANDLER_NOP(DECDMAC) /* define macro */
_VTE_CMD_HANDLER_NOP(DECDWL) /* double width single height line */
_VTE_CMD_HANDLER_NOP(DECEFR) /* enable filter rectangle */
_VTE_CMD_HANDLER_NOP(DECELF) /* enable local functions */
_VTE_CMD_HANDLER_NOP(DECELR) /* enable locator reporting */
_VTE_CMD_HANDLER_NOP(DECERA) /* erase rectangular area */
_VTE_CMD_HANDLER_NOP(DECES) /* enable session */
_VTE_CMD_HANDLER_NOP(DECFI) /* forward index */
_VTE_CMD_HANDLER_NOP(DECFNK) /* dec function key / XTERM bracketed paste */
_VTE_CMD_HANDLER_NOP(DECFRA) /* fill rectangular area */
_VTE_CMD_HANDLER_NOP(DECIC) /* insert column */
_VTE_CMD_HANDLER_NOP(DECINVM) /* invoke macro */
_VTE_CMD_HANDLER_NOP(DECKBD) /* keyboard language selection */
_VTE_CMD_HANDLER(DECKPAM) /* keypad application mode */
_VTE_CMD_HANDLER(DECKPNM) /* keypad numeric mode */
_VTE_CMD_HANDLER_NOP(DECLANS) /* load answerback message */
_VTE_CMD_HANDLER_NOP(DECLBAN) /* load banner message */
_VTE_CMD_HANDLER_NOP(DECLBD) /* locator button define */
_VTE_CMD_HANDLER_NOP(DECLFKC) /* local function key control */
_VTE_CMD_HANDLER_NOP(DECLL) /* load leds */
_VTE_CMD_HANDLER_NOP(DECLTOD) /* load time of day */
_VTE_CMD_HANDLER_NOP(DECPAK) /* program alphanumeric key */
_VTE_CMD_HANDLER_NOP(DECPCTERM) /* pcterm */
_VTE_CMD_HANDLER(DECPCTERM_OR_XTERM_RPM) /* pcterm or xterm restore DEC private mode */
_VTE_CMD_HANDLER_NOP(DECPFK) /* program function key */
_VTE_CMD_HANDLER_NOP(DECPKA) /* program key action */
_VTE_CMD_HANDLER_NOP(DECPKFMR) /* program key free memory report */
_VTE_CMD_HANDLER_NOP(DECPS) /* play sound */
_VTE_CMD_HANDLER_NOP(DECRARA) /* reverse attributes in rectangular area */
_VTE_CMD_HANDLER(DECRC) /* restore cursor */
_VTE_CMD_HANDLER_NOP(DECREGIS) /* ReGIS graphics */
_VTE_CMD_HANDLER(DECREQTPARM) /* request terminal parameters */
_VTE_CMD_HANDLER(DECREQTPARM_OR_WYCDIR) /* request terminal parameters or set current character attributes */
_VTE_CMD_HANDLER_NOP(DECRPAK) /* report all modifier/alphanumeric key state */
_VTE_CMD_HANDLER_NOP(DECRPDE) /* report displayed extent */
_VTE_CMD_HANDLER_NOP(DECRPFK) /* report function key definition */
_VTE_CMD_HANDLER_NOP(DECRPKT) /* report key type */
_VTE_CMD_HANDLER(DECRQCRA) /* request checksum of rectangular area */
_VTE_CMD_HANDLER_NOP(DECRQDE) /* request display extent */
_VTE_CMD_HANDLER_NOP(DECRQKT) /* request key type */
_VTE_CMD_HANDLER_NOP(DECRQLP) /* request locator position */
_VTE_CMD_HANDLER(DECRQM_DEC) /* request mode dec */
_VTE_CMD_HANDLER(DECRQM_ECMA) /* request mode ecma */
_VTE_CMD_HANDLER_NOP(DECRQPKFM) /* request program key free memory */
_VTE_CMD_HANDLER_NOP(DECRQPSR) /* request presentation state report */
_VTE_CMD_HANDLER(DECRQSS) /* request selection or setting */
_VTE_CMD_HANDLER(DECRQTSR) /* request terminal state report */
_VTE_CMD_HANDLER_NOP(DECRQUPSS) /* request user preferred supplemental set */
_VTE_CMD_HANDLER_NOP(DECRSPS) /* restore presentation state */
_VTE_CMD_HANDLER_NOP(DECRSTS) /* restore terminal state */
_VTE_CMD_HANDLER_NOP(DECSACE) /* select attribute change extent */
_VTE_CMD_HANDLER_NOP(DECSASD) /* select active status display */
_VTE_CMD_HANDLER(DECSC) /* save cursor */
_VTE_CMD_HANDLER_NOP(DECSCA) /* select character protection attribute */
_VTE_CMD_HANDLER(DECSCL) /* select conformance level */
_VTE_CMD_HANDLER_NOP(DECSCP) /* select communication port */
_VTE_CMD_HANDLER_NOP(DECSCPP) /* select columns per page */
_VTE_CMD_HANDLER_NOP(DECSCS) /* select communication speed */
_VTE_CMD_HANDLER(DECSCUSR) /* set cursor style */
_VTE_CMD_HANDLER_NOP(DECSDDT) /* select disconnect delay time */
_VTE_CMD_HANDLER_NOP(DECSDPT) /* select digital printed data type */
_VTE_CMD_HANDLER(DECSED) /* selective erase in display */
_VTE_CMD_HANDLER(DECSEL) /* selective erase in line */
_VTE_CMD_HANDLER_NOP(DECSERA) /* selective erase rectangular area */
_VTE_CMD_HANDLER_NOP(DECSEST) /* energy saver time */
_VTE_CMD_HANDLER_NOP(DECSFC) /* select flow control */
_VTE_CMD_HANDLER_NOP(DECSGR) /* DEC select graphics rendition */
_VTE_CMD_HANDLER_R(DECSIXEL) /* SIXEL graphics */
_VTE_CMD_HANDLER_NOP(DECSKCV) /* set key click volume */
_VTE_CMD_HANDLER_NOP(DECSLCK) /* set lock key style */
_VTE_CMD_HANDLER_NOP(DECSLE) /* select locator events */
_VTE_CMD_HANDLER(DECSLPP) /* set lines per page */
_VTE_CMD_HANDLER(DECSLPP_OR_XTERM_WM) /* set lines per page or xterm window management */
_VTE_CMD_HANDLER_NOP(DECSLRM) /* set left and right margins */
_VTE_CMD_HANDLER(DECSLRM_OR_SCOSC) /* set left and right margins or SCO save cursor */
_VTE_CMD_HANDLER_NOP(DECSMBV) /* set margin bell volume */
_VTE_CMD_HANDLER_NOP(DECSMKR) /* select modifier key reporting */
_VTE_CMD_HANDLER_NOP(DECSNLS) /* set lines per screen */
_VTE_CMD_HANDLER_NOP(DECSPMA) /* session page memory allocation */
_VTE_CMD_HANDLER_NOP(DECSPP) /* set port parameter */
_VTE_CMD_HANDLER_NOP(DECSPPCS) /* select pro printer character set */
_VTE_CMD_HANDLER_NOP(DECSPRTT) /* select printer type */
_VTE_CMD_HANDLER(DECSR) /* secure reset */
_VTE_CMD_HANDLER_NOP(DECSRFR) /* select refresh rate */
_VTE_CMD_HANDLER_NOP(DECSSCLS) /* set scroll speed */
_VTE_CMD_HANDLER_NOP(DECSSDT) /* select status display line type */
_VTE_CMD_HANDLER_NOP(DECSSL) /* select setup language */
_VTE_CMD_HANDLER(DECST8C) /* set tab at every 8 columns */
_VTE_CMD_HANDLER(DECSTBM) /* set top and bottom margins */
_VTE_CMD_HANDLER_NOP(DECSTGLT) /* select color lookup table */
_VTE_CMD_HANDLER(DECSTR) /* soft terminal reset */
_VTE_CMD_HANDLER_NOP(DECSTRL) /* set transmit rate limit */
_VTE_CMD_HANDLER_NOP(DECSTUI) /* set terminal unit id */
_VTE_CMD_HANDLER_NOP(DECSWBV) /* set warning bell volume */
_VTE_CMD_HANDLER_NOP(DECSWL) /* single width single height line */
_VTE_CMD_HANDLER_NOP(DECSZS) /* select zero symbol */
_VTE_CMD_HANDLER_NOP(DECTID) /* select terminal id */
_VTE_CMD_HANDLER_NOP(DECTME) /* terminal mode emulation */
_VTE_CMD_HANDLER_NOP(DECTST) /* invoke confidence test */
_VTE_CMD_HANDLER_NOP(DECUDK) /* user defined keys */
_VTE_CMD_HANDLER_NOP(DECUS) /* update session */
_VTE_CMD_HANDLER(DL) /* delete line */
_VTE_CMD_HANDLER_NOP(DLE) /* data link escape */
_VTE_CMD_HANDLER_NOP(DMI) /* disable manual input */
_VTE_CMD_HANDLER_NOP(DOCS) /* designate other coding system */
_VTE_CMD_HANDLER(DSR_DEC) /* device status report dec */
_VTE_CMD_HANDLER(DSR_ECMA) /* device status report ecma */
_VTE_CMD_HANDLER_NOP(DTA) /* dimension text area */
_VTE_CMD_HANDLER_NOP(EA) /* erase in area */
_VTE_CMD_HANDLER(ECH) /* erase character */
_VTE_CMD_HANDLER(ED) /* erase in display */
_VTE_CMD_HANDLER_NOP(EF) /* erase in field */
_VTE_CMD_HANDLER(EL) /* erase in line */
_VTE_CMD_HANDLER_NOP(EM) /* end of medium */
_VTE_CMD_HANDLER_NOP(EMI) /* enable manual input */
_VTE_CMD_HANDLER_NOP(ENQ) /* enquire */
_VTE_CMD_HANDLER_NOP(EOT) /* end of transmission */
_VTE_CMD_HANDLER_NOP(EPA) /* end of protected area */
_VTE_CMD_HANDLER_NOP(ESA) /* end of selected area */
_VTE_CMD_HANDLER_NOP(ETB) /* end of transmissionblock */
_VTE_CMD_HANDLER_NOP(ETX) /* end of text */
_VTE_CMD_HANDLER(FF) /* form feed */
_VTE_CMD_HANDLER_NOP(FNK) /* function key */
_VTE_CMD_HANDLER_NOP(FNT) /* font selection */
_VTE_CMD_HANDLER_NOP(GCC) /* graphic character combination */
_VTE_CMD_HANDLER_NOP(GSM) /* graphic size modification */
_VTE_CMD_HANDLER_NOP(GSS) /* graphic size selection */
_VTE_CMD_HANDLER_NOP(GnDMm) /* Gn designate multibyte 9m charset */
_VTE_CMD_HANDLER(GnDm) /* Gn designate 9m charset */
_VTE_CMD_HANDLER(HPA) /* horizontal position absolute */
_VTE_CMD_HANDLER_NOP(HPB) /* horizontal position backward */
_VTE_CMD_HANDLER(HPR) /* horizontal position relative */
_VTE_CMD_HANDLER(HT) /* horizontal tab */
_VTE_CMD_HANDLER(HTJ) /* character tabulation with justification */
_VTE_CMD_HANDLER(HTS) /* horizontal tab set */
_VTE_CMD_HANDLER(HVP) /* horizontal and vertical position */
_VTE_CMD_HANDLER(ICH) /* insert character */
_VTE_CMD_HANDLER_NOP(IDCS) /* identify DCS */
_VTE_CMD_HANDLER_NOP(IGS) /* identify graphic subrepertoire */
_VTE_CMD_HANDLER(IL) /* insert line */
_VTE_CMD_HANDLER(IND) /* index */
_VTE_CMD_HANDLER_NOP(INT) /* interrupt */
_VTE_CMD_HANDLER_NOP(IRR) /* identify revised registration */
_VTE_CMD_HANDLER_NOP(IS1) /* information separator 1 / unit separator (US) */
_VTE_CMD_HANDLER_NOP(IS2) /* information separator 2 / record separator (RS) */
_VTE_CMD_HANDLER_NOP(IS3) /* information separator 3 / group separator (GS) */
_VTE_CMD_HANDLER_NOP(IS4) /* information separator 4 / file separator (FS) */
_VTE_CMD_HANDLER_NOP(JFY) /* justify */
_VTE_CMD_HANDLER(LF) /* line feed */
_VTE_CMD_HANDLER(LS0) /* locking shift 0 */
_VTE_CMD_HANDLER(LS1) /* locking shift 1 */
_VTE_CMD_HANDLER(LS1R) /* locking shift 1 right */
_VTE_CMD_HANDLER(LS2) /* locking shift 2 */
_VTE_CMD_HANDLER(LS2R) /* locking shift 2 right */
_VTE_CMD_HANDLER(LS3) /* locking shift 3 */
_VTE_CMD_HANDLER(LS3R) /* locking shift 3 right */
_VTE_CMD_HANDLER_NOP(MC_DEC) /* media copy dec */
_VTE_CMD_HANDLER_NOP(MC_ECMA) /* media copy ecma */
_VTE_CMD_HANDLER_NOP(MINTTY_PROGRESS) /* set progress report */
_VTE_CMD_HANDLER_NOP(MW) /* message waiting */
_VTE_CMD_HANDLER_NOP(NAK) /* negative acknowledge */
_VTE_CMD_HANDLER_NOP(NBH) /* no break permitted here */
_VTE_CMD_HANDLER(NEL) /* next line */
_VTE_CMD_HANDLER_NOP(NP) /* next page */
_VTE_CMD_HANDLER_NOP(NUL) /* nul */
_VTE_CMD_HANDLER(OSC) /* operating system command */
_VTE_CMD_HANDLER_NOP(PEC) /* presentation expand or contract */
_VTE_CMD_HANDLER_NOP(PFS) /* page format selection */
_VTE_CMD_HANDLER_NOP(PLD) /* partial line forward */
_VTE_CMD_HANDLER_NOP(PLU) /* partial line backward */
_VTE_CMD_HANDLER_NOP(PP) /* preceding page */
_VTE_CMD_HANDLER_NOP(PPA) /* page position absolute */
_VTE_CMD_HANDLER_NOP(PPB) /* page position backward */
_VTE_CMD_HANDLER_NOP(PPR) /* page position relative */
_VTE_CMD_HANDLER_NOP(PTX) /* parallel texts */
_VTE_CMD_HANDLER_NOP(PU1) /* private use 1 */
_VTE_CMD_HANDLER_NOP(PU2) /* private use 2 */
_VTE_CMD_HANDLER_NOP(QUAD) /* quad */
_VTE_CMD_HANDLER(REP) /* repeat */
_VTE_CMD_HANDLER(RI) /* reverse index */
_VTE_CMD_HANDLER(RIS) /* reset to initial state */
_VTE_CMD_HANDLER_NOP(RLOGIN_MML) /* RLogin music macro language */
_VTE_CMD_HANDLER(RM_DEC) /* reset mode dec */
_VTE_CMD_HANDLER(RM_ECMA) /* reset mode ecma */
_VTE_CMD_HANDLER_NOP(RM_HP) /* reset mode hp */
_VTE_CMD_HANDLER_NOP(SACS) /* set additional character separation */
_VTE_CMD_HANDLER_NOP(SAPV) /* select alternative presentation variants */
_VTE_CMD_HANDLER_NOP(SCO) /* select character orientation */
_VTE_CMD_HANDLER(SCORC) /* SCO restore cursor */
_VTE_CMD_HANDLER(SCOSC) /* SCO save cursor */
_VTE_CMD_HANDLER(SCP) /* select character path */
_VTE_CMD_HANDLER_NOP(SCS) /* set character spacing */
_VTE_CMD_HANDLER(SD) /* scroll down */
_VTE_CMD_HANDLER_NOP(SDS) /* start directed string */
_VTE_CMD_HANDLER(SD_OR_XTERM_IHMT) /* scroll down or xterm initiate highlight mouse tracking */
_VTE_CMD_HANDLER_NOP(SEE) /* select editing extent */
_VTE_CMD_HANDLER_NOP(SEF) /* sheet eject and feed */
_VTE_CMD_HANDLER(SGR) /* select graphics rendition */
_VTE_CMD_HANDLER_NOP(SHS) /* select character spacing */
_VTE_CMD_HANDLER_NOP(SIMD) /* select implicit movement direction */
_VTE_CMD_HANDLER_NOP(SL) /* scroll left */
_VTE_CMD_HANDLER_NOP(SLH) /* set line home */
_VTE_CMD_HANDLER_NOP(SLL) /* set line limit */
_VTE_CMD_HANDLER_NOP(SLS) /* set line spacing */
_VTE_CMD_HANDLER(SM_DEC) /* set mode dec */
_VTE_CMD_HANDLER(SM_ECMA) /* set mode ecma */
_VTE_CMD_HANDLER_NOP(SM_HP) /* set mode hp */
_VTE_CMD_HANDLER_NOP(SOH) /* start of heading */
_VTE_CMD_HANDLER_NOP(SPA) /* start of protected area */
_VTE_CMD_HANDLER(SPD) /* select presentation directions */
_VTE_CMD_HANDLER_NOP(SPH) /* set page home */
_VTE_CMD_HANDLER_NOP(SPI) /* spacing increment */
_VTE_CMD_HANDLER_NOP(SPL) /* set page limit */
_VTE_CMD_HANDLER_NOP(SPQR) /* select print quality and rapidity */
_VTE_CMD_HANDLER_NOP(SR) /* scroll right */
_VTE_CMD_HANDLER_NOP(SRCS) /* set reduced character separation */
_VTE_CMD_HANDLER_NOP(SRS) /* start reversed string */
_VTE_CMD_HANDLER(SS2) /* single shift 2 */
_VTE_CMD_HANDLER(SS3) /* single shift 3 */
_VTE_CMD_HANDLER_NOP(SSA) /* start of selected area */
_VTE_CMD_HANDLER_NOP(SSU) /* set size unit */
_VTE_CMD_HANDLER_NOP(SSW) /* set space width */
_VTE_CMD_HANDLER_NOP(ST) /* string terminator */
_VTE_CMD_HANDLER_NOP(STAB) /* selective tabulation */
_VTE_CMD_HANDLER_NOP(STS) /* set transmit state */
_VTE_CMD_HANDLER_NOP(STX) /* start of text */
_VTE_CMD_HANDLER(SU) /* scroll up */
_VTE_CMD_HANDLER(SUB) /* substitute */
_VTE_CMD_HANDLER_NOP(SVS) /* select line spacing */
_VTE_CMD_HANDLER_NOP(SYN) /* synchronise */
_VTE_CMD_HANDLER_NOP(TAC) /* tabulation aligned centre */
_VTE_CMD_HANDLER_NOP(TALE) /* tabulation aligned leading edge */
_VTE_CMD_HANDLER_NOP(TATE) /* tabulation aligned trailing edge */
_VTE_CMD_HANDLER(TBC) /* tab clear */
_VTE_CMD_HANDLER(TCC) /* tabulation centred on character */
_VTE_CMD_HANDLER(TSR) /* tabulation stop remove */
_VTE_CMD_HANDLER_NOP(TSS) /* thine space specification */
_VTE_CMD_HANDLER(VPA) /* vertical line position absolute */
_VTE_CMD_HANDLER_NOP(VPB) /* line position backward */
_VTE_CMD_HANDLER(VPR) /* vertical line position relative */
_VTE_CMD_HANDLER(VT) /* vertical tag */
_VTE_CMD_HANDLER_NOP(VTS) /* line tabulation set */
_VTE_CMD_HANDLER_NOP(WYCAA) /* redefine character display attribute association */
_VTE_CMD_HANDLER_NOP(WYCDIR) /* set current character attributes */
_VTE_CMD_HANDLER_NOP(WYDHL_BH) /* single width double height line: bottom half */
_VTE_CMD_HANDLER_NOP(WYDHL_TH) /* single width double height line: top half */
_VTE_CMD_HANDLER_NOP(WYLSFNT) /* load soft font */
_VTE_CMD_HANDLER_NOP(WYSCRATE) /* set smooth scroll rate */
_VTE_CMD_HANDLER_NOP(XDGSYNC) /* synchronous update */
_VTE_CMD_HANDLER_NOP(XTERM_CHECKSUM_MODE) /* xterm DECRQCRA checksum mode */
_VTE_CMD_HANDLER_NOP(XTERM_GETXRES) /* xterm get X resource */
_VTE_CMD_HANDLER_NOP(XTERM_IHMT) /* xterm initiate highlight mouse tracking */
_VTE_CMD_HANDLER_NOP(XTERM_MLHP) /* xterm memory lock hp bugfix */
_VTE_CMD_HANDLER_NOP(XTERM_MODKEYS) /* xterm set key modifier options */
_VTE_CMD_HANDLER_NOP(XTERM_MUHP) /* xterm memory unlock hp bugfix */
_VTE_CMD_HANDLER_NOP(XTERM_POPCOLORS) /* xterm pop color palette stack */
_VTE_CMD_HANDLER_NOP(XTERM_POPSGR) /* xterm pop SGR stack */
_VTE_CMD_HANDLER_NOP(XTERM_PTRMODE) /* xterm set pointer mode */
_VTE_CMD_HANDLER_NOP(XTERM_PUSHCOLORS) /* xterm push color palette stack */
_VTE_CMD_HANDLER_NOP(XTERM_PUSHSGR) /* xterm push SGR stack */
_VTE_CMD_HANDLER_NOP(XTERM_REPORTCOLORS) /* xterm Report color palette stack */
_VTE_CMD_HANDLER_NOP(XTERM_REPORTSGR) /* xterm SGR report */
_VTE_CMD_HANDLER(XTERM_RPM) /* xterm restore DEC private mode */
_VTE_CMD_HANDLER_NOP(XTERM_RQTCAP) /* xterm request termcap/terminfo */
_VTE_CMD_HANDLER_NOP(XTERM_RRV) /* xterm reset resource value */
_VTE_CMD_HANDLER_NOP(XTERM_RTM) /* xterm reset title mode */
_VTE_CMD_HANDLER(XTERM_SHIFTESCAPE) /* xterm set shift-escape */
_VTE_CMD_HANDLER(XTERM_SMGRAPHICS) /* xterm graphics attributes */
_VTE_CMD_HANDLER(XTERM_SPM) /* xterm save private mode */
_VTE_CMD_HANDLER_NOP(XTERM_STCAP) /* xterm set termcap/terminfo */
_VTE_CMD_HANDLER_NOP(XTERM_STM) /* xterm set title mode */
_VTE_CMD_HANDLER(XTERM_WM) /* xterm window management */
