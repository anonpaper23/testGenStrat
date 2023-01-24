function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/Bus
Selector3 */
	this.urlHashMap["Autopilot:11"] = "Autopilot.c:45";
	/* <Root>/Bus
Selector4 */
	this.urlHashMap["Autopilot:12"] = "Autopilot.c:50";
	/* <Root>/Pitch_Autopilot */
	this.urlHashMap["Autopilot:13"] = "Autopilot.c:22,58,84&Autopilot.h:34";
	/* <Root>/Roll_Autopilot */
	this.urlHashMap["Autopilot:14"] = "Autopilot.c:64,88&Autopilot.h:35";
	/* <Root>/Yaw_Damper */
	this.urlHashMap["Autopilot:15"] = "Autopilot.c:70,92&Autopilot.h:36";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
	RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
