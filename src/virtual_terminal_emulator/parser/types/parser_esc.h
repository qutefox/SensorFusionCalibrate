


#if !defined(_VTE_SEQ) || !defined(_VTE_NOQ)
#error "Must define _VTE_SEQ and _VTE_NOQ before including this file"
#endif

_VTE_NOQ(DECDHL_TH, ESCAPE, '3', NONE, 1, HASH, 0 ) /* double width double height line: top half */
_VTE_NOQ(DECDHL_BH, ESCAPE, '4', NONE, 1, HASH, 0 ) /* double width double height line: bottom half */
_VTE_NOQ(DECSWL, ESCAPE, '5', NONE, 1, HASH, 0 ) /* single width single height line */
_VTE_NOQ(DECBI, ESCAPE, '6', NONE, 0, NONE, 0 ) /* back index */
_VTE_NOQ(DECDWL, ESCAPE, '6', NONE, 1, HASH, 0 ) /* double width single height line */
_VTE_SEQ(DECSC, ESCAPE, '7', NONE, 0, NONE, 0 ) /* save cursor */
_VTE_SEQ(DECRC, ESCAPE, '8', NONE, 0, NONE, 0 ) /* restore cursor */
_VTE_SEQ(DECALN, ESCAPE, '8', NONE, 1, HASH, 0 ) /* screen alignment pattern */
_VTE_NOQ(DECFI, ESCAPE, '9', NONE, 0, NONE, 0 ) /* forward index */
_VTE_NOQ(WYDHL_TH, ESCAPE, ':', NONE, 1, HASH, 0 ) /* single width double height line: top half */
_VTE_NOQ(WYDHL_BH, ESCAPE, ';', NONE, 1, HASH, 0 ) /* single width double height line: bottom half */
_VTE_NOQ(DECANM, ESCAPE, '<', NONE, 0, NONE, 0 ) /* ansi mode */
_VTE_SEQ(DECKPAM, ESCAPE, '=', NONE, 0, NONE, 0 ) /* keypad application mode */
_VTE_SEQ(DECKPNM, ESCAPE, '>', NONE, 0, NONE, 0 ) /* keypad numeric mode */
_VTE_NOQ(BPH, ESCAPE, 'B', NONE, 0, NONE, 0 ) /* break permitted here */
_VTE_NOQ(NBH, ESCAPE, 'C', NONE, 0, NONE, 0 ) /* no break permitted here */
_VTE_SEQ(IND, ESCAPE, 'D', NONE, 0, NONE, 0 ) /* index */
_VTE_SEQ(NEL, ESCAPE, 'E', NONE, 0, NONE, 0 ) /* next line */
_VTE_NOQ(SSA, ESCAPE, 'F', NONE, 0, NONE, 0 ) /* start of selected area */
_VTE_NOQ(ESA, ESCAPE, 'G', NONE, 0, NONE, 0 ) /* end of selected area */
_VTE_SEQ(HTS, ESCAPE, 'H', NONE, 0, NONE, 0 ) /* horizontal tab set */
_VTE_SEQ(HTJ, ESCAPE, 'I', NONE, 0, NONE, 0 ) /* character tabulation with justification */
_VTE_NOQ(VTS, ESCAPE, 'J', NONE, 0, NONE, 0 ) /* line tabulation set */
_VTE_NOQ(PLD, ESCAPE, 'K', NONE, 0, NONE, 0 ) /* partial line forward */
_VTE_NOQ(PLU, ESCAPE, 'L', NONE, 0, NONE, 0 ) /* partial line backward */
_VTE_SEQ(RI, ESCAPE, 'M', NONE, 0, NONE, 0 ) /* reverse index */
_VTE_SEQ(SS2, ESCAPE, 'N', NONE, 0, NONE, 0 ) /* single shift 2 */
_VTE_SEQ(SS3, ESCAPE, 'O', NONE, 0, NONE, 0 ) /* single shift 3 */
_VTE_NOQ(PU1, ESCAPE, 'Q', NONE, 0, NONE, 0 ) /* private use 1 */
_VTE_NOQ(PU2, ESCAPE, 'R', NONE, 0, NONE, 0 ) /* private use 2 */
_VTE_NOQ(STS, ESCAPE, 'S', NONE, 0, NONE, 0 ) /* set transmit state */
_VTE_NOQ(CCH, ESCAPE, 'T', NONE, 0, NONE, 0 ) /* cancel character */
_VTE_NOQ(MW, ESCAPE, 'U', NONE, 0, NONE, 0 ) /* message waiting */
_VTE_NOQ(SPA, ESCAPE, 'V', NONE, 0, NONE, 0 ) /* start of protected area */
_VTE_NOQ(EPA, ESCAPE, 'W', NONE, 0, NONE, 0 ) /* end of protected area */
_VTE_NOQ(ST, ESCAPE, '\\', NONE, 0, NONE, 0 ) /* string terminator */
_VTE_NOQ(DMI, ESCAPE, '`', NONE, 0, NONE, 0 ) /* disable manual input */
_VTE_NOQ(INT, ESCAPE, 'a', NONE, 0, NONE, 0 ) /* interrupt */
_VTE_NOQ(EMI, ESCAPE, 'b', NONE, 0, NONE, 0 ) /* enable manual input */
_VTE_SEQ(RIS, ESCAPE, 'c', NONE, 0, NONE, 0 ) /* reset to initial state */
_VTE_NOQ(CMD, ESCAPE, 'd', NONE, 0, NONE, 0 ) /* coding method delimiter */
_VTE_NOQ(XTERM_MLHP, ESCAPE, 'l', NONE, 0, NONE, 0 ) /* xterm memory lock hp bugfix */
_VTE_NOQ(XTERM_MUHP, ESCAPE, 'm', NONE, 0, NONE, 0 ) /* xterm memory unlock hp bugfix */
_VTE_SEQ(LS2, ESCAPE, 'n', NONE, 0, NONE, 0 ) /* locking shift 2 */
_VTE_SEQ(LS3, ESCAPE, 'o', NONE, 0, NONE, 0 ) /* locking shift 3 */
_VTE_SEQ(LS3R, ESCAPE, '|', NONE, 0, NONE, 0 ) /* locking shift 3 right */
_VTE_SEQ(LS2R, ESCAPE, '}', NONE, 0, NONE, 0 ) /* locking shift 2 right */
_VTE_SEQ(LS1R, ESCAPE, '~', NONE, 0, NONE, 0 ) /* locking shift 1 right */
