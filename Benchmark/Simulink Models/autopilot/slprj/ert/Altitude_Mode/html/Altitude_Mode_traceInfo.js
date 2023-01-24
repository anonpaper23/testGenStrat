function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/ALT G limit */
	this.urlHashMap["Altitude_Mode:7"] = "Altitude_Mode.c:51";
	/* <Root>/Gamma Cmd */
	this.urlHashMap["Altitude_Mode:10"] = "Altitude_Mode.c:87";
	/* <Root>/Lookup Table */
	this.urlHashMap["Altitude_Mode:27"] = "Altitude_Mode.c:82&Altitude_Mode_private.h:32,35";
	/* <Root>/Min TAS */
	this.urlHashMap["Altitude_Mode:29"] = "Altitude_Mode.c:63";
	/* <Root>/MinMax */
	this.urlHashMap["Altitude_Mode:28"] = "Altitude_Mode.c:62,71";
	/* <Root>/Not ALT */
	this.urlHashMap["Altitude_Mode:14"] = "Altitude_Mode.c:47";
	/* <Root>/Other G limit */
	this.urlHashMap["Altitude_Mode:15"] = "Altitude_Mode.c:52";
	/* <Root>/Product */
	this.urlHashMap["Altitude_Mode:16"] = "Altitude_Mode.c:74";
	/* <Root>/Saturation */
	this.urlHashMap["Altitude_Mode:31"] = "Altitude_Mode.c:92,101";
	/* <Root>/Sum */
	this.urlHashMap["Altitude_Mode:18"] = "Altitude_Mode.c:79&Altitude_Mode.h:30";
	/* <Root>/Sum1 */
	this.urlHashMap["Altitude_Mode:19"] = "Altitude_Mode.c:88";
	/* <Root>/Sum2 */
	this.urlHashMap["Altitude_Mode:20"] = "Altitude_Mode.c:128";
	/* <Root>/Sum3 */
	this.urlHashMap["Altitude_Mode:21"] = "Altitude_Mode.c:103";
	/* <Root>/Switch */
	this.urlHashMap["Altitude_Mode:22"] = "Altitude_Mode.c:50,60";
	/* <Root>/negate */
	this.urlHashMap["Altitude_Mode:23"] = "Altitude_Mode.c:138";
	/* <S1>/Unit Conversion */
	this.urlHashMap["Altitude_Mode:8:2"] = "Altitude_Mode.c:73";
	/* <S2>/Unit Conversion */
	this.urlHashMap["Altitude_Mode:9:2"] = "Altitude_Mode.c:86";
	/* <S4>/Delay Input2 */
	this.urlHashMap["Altitude_Mode:17:4"] = "Altitude_Mode.c:129,165,184&Altitude_Mode.h:36";
	/* <S4>/Difference Inputs1 */
	this.urlHashMap["Altitude_Mode:17:5"] = "Altitude_Mode.c:127";
	/* <S4>/Difference Inputs2 */
	this.urlHashMap["Altitude_Mode:17:6"] = "Altitude_Mode.c:164";
	/* <S4>/FixPt
Data Type
Duplicate */
	this.urlHashMap["Altitude_Mode:17:7"] = "msg=rtwMsg_notTraceable&block=Altitude_Mode/Rate%20Limiter/FixPt%20Data%20Type%20Duplicate";
	/* <S4>/Probe */
	this.urlHashMap["Altitude_Mode:17:16"] = "Altitude_Mode.c:22&Altitude_Mode.h:31";
	/* <S4>/delta fall limit */
	this.urlHashMap["Altitude_Mode:17:9"] = "Altitude_Mode.c:137";
	/* <S4>/delta rise limit */
	this.urlHashMap["Altitude_Mode:17:10"] = "Altitude_Mode.c:134";
	/* <S5>/Constant */
	this.urlHashMap["Altitude_Mode:30:19"] = "msg=rtwMsg_notTraceable&block=Altitude_Mode/ResetIntegratorExternal/Constant";
	/* <S5>/Constant1 */
	this.urlHashMap["Altitude_Mode:30:20"] = "msg=rtwMsg_notTraceable&block=Altitude_Mode/ResetIntegratorExternal/Constant1";
	/* <S5>/Product */
	this.urlHashMap["Altitude_Mode:30:22"] = "Altitude_Mode.h:42&Altitude_Mode_data.c:22";
	/* <S5>/Product1 */
	this.urlHashMap["Altitude_Mode:30:23"] = "Altitude_Mode.c:175";
	/* <S5>/Saturation */
	this.urlHashMap["Altitude_Mode:30:24"] = "Altitude_Mode.c:116,125";
	/* <S5>/Sum */
	this.urlHashMap["Altitude_Mode:30:25"] = "Altitude_Mode.c:176";
	/* <S5>/Switch */
	this.urlHashMap["Altitude_Mode:30:26"] = "Altitude_Mode.c:109,114";
	/* <S5>/Switch1 */
	this.urlHashMap["Altitude_Mode:30:27"] = "Altitude_Mode.c:169,182";
	/* <S5>/X */
	this.urlHashMap["Altitude_Mode:30:28"] = "Altitude_Mode.c:106,171,174&Altitude_Mode.h:37";
	/* <S6>/Data Type
Duplicate */
	this.urlHashMap["Altitude_Mode:17:8:4"] = "msg=rtwMsg_notTraceable&block=Altitude_Mode/Rate%20Limiter/Saturation%20Dynamic/Data%20Type%20Duplicate";
	/* <S6>/Data Type
Propagation */
	this.urlHashMap["Altitude_Mode:17:8:5"] = "msg=rtwMsg_notTraceable&block=Altitude_Mode/Rate%20Limiter/Saturation%20Dynamic/Data%20Type%20Propagation";
	/* <S6>/LowerRelop1 */
	this.urlHashMap["Altitude_Mode:17:8:6"] = "Altitude_Mode.c:154";
	/* <S6>/Switch */
	this.urlHashMap["Altitude_Mode:17:8:7"] = "Altitude_Mode.c:142,151";
	/* <S6>/Switch2 */
	this.urlHashMap["Altitude_Mode:17:8:8"] = "Altitude_Mode.c:153,162";
	/* <S6>/UpperRelop */
	this.urlHashMap["Altitude_Mode:17:8:9"] = "Altitude_Mode.c:143";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
	RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
