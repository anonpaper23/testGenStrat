function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/Cmd Limit */
	this.urlHashMap["attitude_controller:5"] = "attitude_controller.c:91,100";
	/* <Root>/Disp Gain */
	this.urlHashMap["attitude_controller:6"] = "attitude_controller.c:66";
	/* <Root>/Disp Limit */
	this.urlHashMap["attitude_controller:7"] = "attitude_controller.c:57,67";
	/* <Root>/Int Gain */
	this.urlHashMap["attitude_controller:8"] = "attitude_controller.c:110";
	/* <Root>/Not engaged */
	this.urlHashMap["attitude_controller:11"] = "attitude_controller.c:31";
	/* <Root>/Rate Gain */
	this.urlHashMap["attitude_controller:12"] = "attitude_controller.c:87";
	/* <Root>/Rate Limit */
	this.urlHashMap["attitude_controller:13"] = "attitude_controller.c:72,82";
	/* <Root>/Sum */
	this.urlHashMap["attitude_controller:14"] = "attitude_controller.c:68";
	/* <Root>/Sum1 */
	this.urlHashMap["attitude_controller:15"] = "attitude_controller.c:81";
	/* <Root>/Sum2 */
	this.urlHashMap["attitude_controller:16"] = "attitude_controller.c:86";
	/* <S2>/Constant */
	this.urlHashMap["attitude_controller:23:4"] = "msg=rtwMsg_notTraceable&block=attitude_controller/ResetIntegratorInternal/Constant";
	/* <S2>/Constant1 */
	this.urlHashMap["attitude_controller:23:5"] = "msg=rtwMsg_notTraceable&block=attitude_controller/ResetIntegratorInternal/Constant1";
	/* <S2>/Constant2 */
	this.urlHashMap["attitude_controller:23:6"] = "attitude_controller.c:38,105";
	/* <S2>/Product */
	this.urlHashMap["attitude_controller:23:7"] = "attitude_controller.h:34&attitude_controller_data.c:22";
	/* <S2>/Product1 */
	this.urlHashMap["attitude_controller:23:8"] = "attitude_controller.c:111";
	/* <S2>/Saturation */
	this.urlHashMap["attitude_controller:23:9"] = "attitude_controller.c:46,55";
	/* <S2>/Sum */
	this.urlHashMap["attitude_controller:23:10"] = "attitude_controller.c:112";
	/* <S2>/Switch */
	this.urlHashMap["attitude_controller:23:11"] = "attitude_controller.c:37,44";
	/* <S2>/Switch1 */
	this.urlHashMap["attitude_controller:23:12"] = "attitude_controller.c:102,118";
	/* <S2>/X */
	this.urlHashMap["attitude_controller:23:13"] = "attitude_controller.c:34,104,109&attitude_controller.h:29";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
	RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
