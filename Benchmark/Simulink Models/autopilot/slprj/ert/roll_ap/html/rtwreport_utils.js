// Copyright 2011-2013 The MathWorks, Inc.


function local_onload() {
    if (typeof top.rtwreport_document_frame !== "undefined") {
        var docObj = window.document;
        var alink =  docObj.getElementById("linkToText_plain");
        if (alink) {
            alink.href = "matlab:coder.internal.editUrlTextFile('" + alink.href + "')";
        }
        alink = docObj.getElementById("linkToCS");
        if (alink) {
            alink.href = "matlab:coder.internal.viewCodeConfigsetFromReport('" + alink.href + "');";
        }
    }
}
