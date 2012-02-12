// popup image window

function popupImageWindow(url, wd, ht, title)
{
  var attrs, popup, wd, ht;

  if (0) {
    x = 20;
    y = 20;
  } else {
    x = parseInt((screen.availWidth - wd)/2);
    y = parseInt((screen.availHeight - ht)/2);
  }

  attrs = "left=" + x + ",top=" + y + ",screenX=" + x + ",screenY=" + y + ",width=" + wd + ",height=" + ht + ",menubar=no,toolbar=no,location=no,status=no,scrollbars=no";

  popup = window.open("", "", attrs);
  popup.focus();
  popup.document.open();

  with (popup) {
    document.write('<html><head>');
    document.write('<scr' + 'ipt type="text/javascr' + 'ipt" language="JavaScr' + 'ipt">');
    document.write("function click() { window.close(); } "); // close on clock
    document.write("document.onmousedown=click ");
    document.write('</scr' + 'ipt>');
    document.write('<title>' + title + '</title></head>');
    document.write('<' + 'body onblur="window.close();" '); // close if focus lost
    document.write('marginwidth="0" marginheight="0" leftmargin="0" topmargin="0">');
    document.write('<img src="' + url + '"border="0">');
    document.write('</body></html>');
    popup.document.close();
  }
}
