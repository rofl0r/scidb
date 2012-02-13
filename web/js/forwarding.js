// Automatic redirection dependent on browser language

var cookieEnabled = navigator.cookieEnabled ? true : false;

if (typeof navigator.cookieEnabled == "undefined" && !cookieEnabled)
{
	setCookie("testcookie", "none", 0);
	cookieEnabled = document.cookie.indexOf("testcookie") != -1 ? true : false;
	unsetCookie("testcookie");
}

if (cookieEnabled)
{
	var lang = getCookie("lang");

	if (lang)
	{
		redirect(lang);
	}
	else
	{
		var langInfo = navigator.userLanguage || navigator.browserLanguage || navigator.language;

		if (langInfo)
		{
			langInfo = langInfo.substr(0, 2);

			if (langInfo == "de")
				redirect("de");
		}
	}
}

function redirect(lang)
{
	if (lang != "en")
	{
		var target = lang + "/index.html";

		if (window.location.replace)
			window.location.replace(target);
		else
			window.location = target;
	}
}

// vi:set ts=3 sw=3:
