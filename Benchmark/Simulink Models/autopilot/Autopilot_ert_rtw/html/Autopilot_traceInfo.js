function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/Air Data */
	this.urlHashMap["Autopilot:2"] = "Autopilot.c:70&Autopilot.h:43";
	/* <Root>/Inertial */
	this.urlHashMap["Autopilot:3"] = "Autopilot.c:60&Autopilot.h:44";
	/* <Root>/AP Eng */
	this.urlHashMap["Autopilot:4"] = "Autopilot.h:45";
	/* <Root>/HDG Mode */
	this.urlHashMap["Autopilot:5"] = "Autopilot.h:46";
	/* <Root>/ALT Mode */
	this.urlHashMap["Autopilot:6"] = "Autopilot.h:47";
	/* <Root>/HDG Ref */
	this.urlHashMap["Autopilot:7"] = "Autopilot.h:48";
	/* <Root>/Turn Knob */
	this.urlHashMap["Autopilot:8"] = "Autopilot.h:49";
	/* <Root>/ALT Ref */
	this.urlHashMap["Autopilot:9"] = "Autopilot.h:50";
	/* <Root>/Pitch Wheel */
	this.urlHashMap["Autopilot:10"] = "Autopilot.h:51";
	/* <Root>/Bus
Selector3 */
	this.urlHashMap["Autopilot:11"] = "Autopilot.c:69";
	/* <Root>/Bus
Selector4 */
	this.urlHashMap["Autopilot:12"] = "Autopilot.c:59";
	/* <Root>/Pitch_Autopilot */
	this.urlHashMap["Autopilot:13"] = "Autopilot.c:82,120,131&Autopilot.h:37";
	/* <Root>/Roll_Autopilot */
	this.urlHashMap["Autopilot:14"] = "Autopilot.c:76,124&Autopilot.h:36";
	/* <Root>/Yaw_Damper */
	this.urlHashMap["Autopilot:15"] = "Autopilot.c:90,128&Autopilot.h:38";
	/* <Root>/Aileron Cmd */
	this.urlHashMap["Autopilot:16"] = "Autopilot.h:56";
	/* <Root>/Elevator Cmd */
	this.urlHashMap["Autopilot:17"] = "Autopilot.h:57";
	/* <Root>/Rudder Cmd */
	this.urlHashMap["Autopilot:18"] = "Autopilot.h:58";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
	RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
