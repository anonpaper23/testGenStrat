function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/Altitude Mode */
	this.urlHashMap["pitch_ap:12"] = "pitch_ap.c:22,43,127&pitch_ap.h:40";
	/* <Root>/Basic Pitch Mode */
	this.urlHashMap["pitch_ap:13"] = "pitch_ap.c:96,131&pitch_ap.h:41";
	/* <Root>/Constant */
	this.urlHashMap["pitch_ap:14"] = "pitch_ap.c:103";
	/* <Root>/Eng switch */
	this.urlHashMap["pitch_ap:15"] = "pitch_ap.c:102,111";
	/* <Root>/Mode switch */
	this.urlHashMap["pitch_ap:16"] = "pitch_ap.c:72,79&pitch_ap.h:34";
	/* <Root>/Pitch Rate Feedback */
	this.urlHashMap["pitch_ap:18"] = "pitch_ap.c:81,94";
	/* <Root>/Pitch Reference */
	this.urlHashMap["pitch_ap:30"] = "pitch_ap.c:49,70";
	/* <S2>/Product */
	this.urlHashMap["pitch_ap:23"] = "pitch_ap.c:86";
	/* <S2>/Product1 */
	this.urlHashMap["pitch_ap:24"] = "pitch_ap.c:87";
	/* <S2>/Sum3 */
	this.urlHashMap["pitch_ap:25"] = "pitch_ap.c:85";
	/* <S2>/cos */
	this.urlHashMap["pitch_ap:26"] = "pitch_ap.c:88";
	/* <S2>/sin */
	this.urlHashMap["pitch_ap:27"] = "pitch_ap.c:89";
	/* <S3>/Not engaged */
	this.urlHashMap["pitch_ap:35"] = "pitch_ap.c:51";
	/* <S3>/Sum3 */
	this.urlHashMap["pitch_ap:36"] = "pitch_ap.c:62";
	/* <S4>/Unit Conversion */
	this.urlHashMap["pitch_ap:22:2"] = "pitch_ap.c:82";
	/* <S5>/Enable */
	this.urlHashMap["pitch_ap:34:3"] = "pitch_ap.c:50,60";
	/* <S5>/FixPt
Data Type
Duplicate1 */
	this.urlHashMap["pitch_ap:34:4"] = "msg=rtwMsg_notTraceable&block=pitch_ap/Pitch%20Reference/Latch%20Theta/FixPt%20Data%20Type%20Duplicate1";
	/* <S5>/FixPt
Unit Delay1 */
	this.urlHashMap["pitch_ap:34:5"] = "pitch_ap.c:52,63,67&pitch_ap.h:39";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
	RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
