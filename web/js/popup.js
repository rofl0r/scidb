// popup image window

function popupImageWindow(url, wd, ht, title)
{
	wd = parseInt(wd);
	ht = parseInt(ht);

	if (navigator.userAgent.search("Firefox") >= 0 || navigator.userAgent.search("Iceweasel") >= 0)
	{
		wd += 2;
		ht += 2;
	}

	if (screen.availWidth - 15 < wd)
		ht += 20;

	// all modern browsers are showing the location bar
	if (screen.availHeight - 80 < ht)
		wd += 20;

	var x = Math.max(0, (screen.availWidth - wd)/2);
	var y = Math.max(0, (screen.availHeight - ht)/2);

	var attrs	= "left=" + x
					+ ",top=" + y
					+ ",width=" + wd
					+ ",height=" + ht
					+ ",menubar=no"
					+ ",addressbar=no"
					+ ",navigationbar=no"
					+ ",toolbar=no"
					+ ",location=no"
					+ ",directories=no"
					+ ",titlebar=no"
					+ ",status=no"
					+ ",resizable=yes"
					+ ",scrollbars=yes";

	var popup = window.open("", "", attrs);

	popup.focus();
	popup.document.open();

	with (popup) {
		document.write('<html><head>');

		// close on click
/*		document.write('<scr' + 'ipt type="text/javascr' + 'ipt" language="JavaScr' + 'ipt">');
		document.write("function click() { window.close(); } ");
		document.write("document.onmousedown=click ");
		document.write('</scr' + 'ipt>');*/

		document.write('<title>' + title + '</title>');
		document.write('</head>');

		document.write('<body ');
//		document.write('onblur="window.close();" '); // close if focus lost
		document.write('marginwidth="0" marginheight="0" leftmargin="0" topmargin="0">');
		document.write('<img src="' + url + '"border="0" hspace="0" vspace="0">');
		document.write('</body>');

		document.write('</html>');

		popup.document.close();
	}
}

// vi:set ts=3 sw=3:
