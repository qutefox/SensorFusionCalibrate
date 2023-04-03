


#if !defined(_VTE_SEQ) || !defined(_VTE_NOQ)
#error "Must define _VTE_SEQ and _VTE_NOQ before including this file"
#endif

_VTE_NOQ(XTERM_GETXRES, DCS, 'Q', NONE, 1, PLUS, 0 ) /* xterm get X resource */
_VTE_NOQ(RLOGIN_MML, DCS, 'm', NONE, 1, HASH, 0 ) /* RLogin music macro language */
_VTE_NOQ(DECREGIS, DCS, 'p', NONE, 0, NONE, 0 ) /* ReGIS graphics */
_VTE_NOQ(DECRSTS, DCS, 'p', NONE, 1, CASH, 0 ) /* restore terminal state */
_VTE_NOQ(XTERM_STCAP, DCS, 'p', NONE, 1, PLUS, 0 ) /* xterm set termcap/terminfo */
_VTE_SEQ(DECSIXEL, DCS, 'q', NONE, 0, NONE, VTE_DISPATCH_UNRIPE ) /* SIXEL graphics */
_VTE_SEQ(DECRQSS, DCS, 'q', NONE, 1, CASH, 0 ) /* request selection or setting */
_VTE_NOQ(XTERM_RQTCAP, DCS, 'q', NONE, 1, PLUS, 0 ) /* xterm request termcap/terminfo */
_VTE_NOQ(DECLBAN, DCS, 'r', NONE, 0, NONE, 0 ) /* load banner message */
_VTE_SEQ(DECRQTSR, DCS, 's', NONE, 1, CASH, 0 ) /* request terminal state report */
_VTE_NOQ(XDGSYNC, DCS, 's', EQUAL, 0, NONE, 0 ) /* synchronous update */
_VTE_NOQ(DECRSPS, DCS, 't', NONE, 1, CASH, 0 ) /* restore presentation state */
_VTE_NOQ(DECAUPSS, DCS, 'u', NONE, 1, BANG, 0 ) /* assign user preferred supplemental sets */
_VTE_NOQ(DECLANS, DCS, 'v', NONE, 0, NONE, 0 ) /* load answerback message */
_VTE_NOQ(DECLBD, DCS, 'w', NONE, 0, NONE, 0 ) /* locator button define */
_VTE_NOQ(DECPFK, DCS, 'x', NONE, 1, DQUOTE, 0 ) /* program function key */
_VTE_NOQ(DECPAK, DCS, 'y', NONE, 1, DQUOTE, 0 ) /* program alphanumeric key */
_VTE_NOQ(DECDMAC, DCS, 'z', NONE, 1, BANG, 0 ) /* define macro */
_VTE_NOQ(DECCKD, DCS, 'z', NONE, 1, DQUOTE, 0 ) /* copy key default */
_VTE_NOQ(DECDLD, DCS, '{', NONE, 0, NONE, 0 ) /* dynamically redefinable character sets extension */
_VTE_NOQ(DECSTUI, DCS, '{', NONE, 1, BANG, 0 ) /* set terminal unit id */
_VTE_NOQ(DECUDK, DCS, '|', NONE, 0, NONE, 0 ) /* user defined keys */
_VTE_NOQ(WYLSFNT, DCS, '}', NONE, 0, NONE, 0 ) /* load soft font */
_VTE_NOQ(DECRPFK, DCS, '}', NONE, 1, DQUOTE, 0 ) /* report function key definition */
_VTE_NOQ(DECRPAK, DCS, '~', NONE, 1, DQUOTE, 0 ) /* report all modifier/alphanumeric key state */
