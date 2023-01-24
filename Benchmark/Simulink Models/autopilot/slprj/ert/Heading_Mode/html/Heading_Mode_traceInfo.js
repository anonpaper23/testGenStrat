function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/Disp Gain */
	this.urlHashMap["Heading_Mode:4"] = "Heading_Mode.c:54,59";
	/* <Root>/Product */
	this.urlHashMap["Heading_Mode:6"] = "Heading_Mode.c:53,56";
	/* <S1>/abs */
	this.urlHashMap["Heading_Mode:26:35"] = "Heading_Mode.c:40";
	/* <S1>/constant */
	this.urlHashMap["Heading_Mode:26:36"] = "Heading_Mode.c:41";
	/* <S1>/constant1 */
	this.urlHashMap["Heading_Mode:26:37"] = "Heading_Mode.c:42";
	/* <S1>/floor */
	this.urlHashMap["Heading_Mode:26:38"] = "Heading_Mode.c:46";
	/* <S1>/product */
	this.urlHashMap["Heading_Mode:26:39"] = "Heading_Mode.c:43";
	/* <S1>/product1 */
	this.urlHashMap["Heading_Mode:26:40"] = "Heading_Mode.c:44";
	/* <S1>/product2 */
	this.urlHashMap["Heading_Mode:26:41"] = "Heading_Mode.c:45";
	/* <S1>/signum */
	this.urlHashMap["Heading_Mode:26:42"] = "Heading_Mode.c:30,47";
	/* <S1>/sum */
	this.urlHashMap["Heading_Mode:26:43"] = "Heading_Mode.c:27";
	/* <S1>/sum1 */
	this.urlHashMap["Heading_Mode:26:44"] = "Heading_Mode.c:39&Heading_Mode.h:30";
	/* <S1>/sum2 */
	this.urlHashMap["Heading_Mode:26:45"] = "Heading_Mode.c:48";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
	RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
