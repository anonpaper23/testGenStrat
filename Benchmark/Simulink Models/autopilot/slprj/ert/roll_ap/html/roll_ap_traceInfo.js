function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/Basic Roll Mode */
	this.urlHashMap["roll_ap:9"] = "roll_ap.c:144,146,188&roll_ap.h:41";
	/* <Root>/Eng switch */
	this.urlHashMap["roll_ap:10"] = "roll_ap.c:157,160,172";
	/* <Root>/Heading Mode */
	this.urlHashMap["roll_ap:11"] = "roll_ap.c:33,35,191&roll_ap.h:40";
	/* <Root>/Mode switch */
	this.urlHashMap["roll_ap:12"] = "roll_ap.c:128,130,142&roll_ap.h:34";
	/* <Root>/Roll Reference */
	this.urlHashMap["roll_ap:14"] = "roll_ap.c:41,43,126";
	/* <Root>/zero */
	this.urlHashMap["roll_ap:32"] = "roll_ap.c:158,163";
	/* <S2>/Abs */
	this.urlHashMap["roll_ap:18"] = "roll_ap.c:98,106";
	/* <S2>/Not engaged */
	this.urlHashMap["roll_ap:20"] = "roll_ap.c:84,87";
	/* <S2>/Ref Threshold */
	this.urlHashMap["roll_ap:21"] = "roll_ap.c:51,69";
	/* <S2>/Ref Threshold1 */
	this.urlHashMap["roll_ap:22"] = "roll_ap.c:52,72";
	/* <S2>/TK Threshold */
	this.urlHashMap["roll_ap:23"] = "roll_ap.c:100,112";
	/* <S2>/TK switch */
	this.urlHashMap["roll_ap:24"] = "roll_ap.c:97,103,121";
	/* <S2>/minus six */
	this.urlHashMap["roll_ap:25"] = "roll_ap.c:47,57";
	/* <S2>/or */
	this.urlHashMap["roll_ap:26"] = "roll_ap.c:50,66";
	/* <S2>/ref switch */
	this.urlHashMap["roll_ap:27"] = "roll_ap.c:46,54,81";
	/* <S2>/six */
	this.urlHashMap["roll_ap:28"] = "roll_ap.c:48,60";
	/* <S2>/three */
	this.urlHashMap["roll_ap:29"] = "roll_ap.c:99,109";
	/* <S2>/zero */
	this.urlHashMap["roll_ap:30"] = "roll_ap.c:49,63";
	/* <S3>/Enable */
	this.urlHashMap["roll_ap:19:3"] = "roll_ap.c:83,95";
	/* <S3>/FixPt
Data Type
Duplicate1 */
	this.urlHashMap["roll_ap:19:4"] = "msg=rtwMsg_notTraceable&block=roll_ap/Roll%20Reference/Latch%20Phi/FixPt%20Data%20Type%20Duplicate1";
	/* <S3>/FixPt
Unit Delay1 */
	this.urlHashMap["roll_ap:19:5"] = "roll_ap.c:85,101,123&roll_ap.h:39";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
	RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
