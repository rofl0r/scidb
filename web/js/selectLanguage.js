// Select wanted language

function setLang(event, lang)
{
  var doit = false;

  if ('which' in event)
    doit = event.which == 1;
  else if ('button' in event)
    doit = event.button == 1;

  if ('which' in event && event.which == 2)
  {
     if (  navigator.userAgent.search("Gecko") >= 0
        || 'opera' in window
        || ('vendor' in navigator && navigator.vendor.search("KDE") >= 0))
    {
      doit = true;
    }
  }

  if (doit)
    setCookie('lang', lang, 720);

  return doit;
}

// vi:set ts=2 sw=2 et:
