function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/Basic Yaw Damper */
	this.urlHashMap["yaw_damper:5"] = "yaw_damper.c:27,45,51,65";
	/* <Root>/Constant */
	this.urlHashMap["yaw_damper:17"] = "yaw_damper.c:48";
	/* <Root>/Eng switch */
	this.urlHashMap["yaw_damper:18"] = "yaw_damper.c:47,70";
	/* <S1>/Constant */
	this.urlHashMap["yaw_damper:24"] = "yaw_damper.c:29";
	/* <S1>/MinMax */
	this.urlHashMap["yaw_damper:23"] = "yaw_damper.c:28,39";
	/* <S1>/Product */
	this.urlHashMap["yaw_damper:9"] = "yaw_damper.c:40";
	/* <S1>/Rate Gain */
	this.urlHashMap["yaw_damper:10"] = "yaw_damper.c:52";
	/* <S1>/Rud Limit */
	this.urlHashMap["yaw_damper:11"] = "yaw_damper.c:55,64";
	/* <S1>/Sum1 */
	this.urlHashMap["yaw_damper:12"] = "yaw_damper.c:37&yaw_damper.h:30";
	/* <S1>/gravity */
	this.urlHashMap["yaw_damper:13"] = "yaw_damper.c:38";
	/* <S1>/sin */
	this.urlHashMap["yaw_damper:14"] = "yaw_damper.c:41";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
	RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
