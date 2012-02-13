// Functions for reading/creating/deleting cookies

function setCookie(name, value, days)
{
	if (days)
	{
		var date = new Date();
		date.setTime(date.getTime() + (days*24*60*60*1000));
		var expires = "; expires=" + date.toGMTString();
	}
	else
	{
		var expires = "";
	}

	document.cookie = name + "=" + value + expires + "; path=/";
}

function getCookie(name)
{
	var nameEQ = name + "=";
	var cookieParts = document.cookie.split(";");

	for (var i = 0; i < cookieParts.length; i++)
	{
		var part = cookieParts[i];

		while (part.charAt(0) == " ")
			part = part.substring(1, part.length);

		if (part.indexOf(nameEQ) == 0)
			return part.substring(nameEQ.length, part.length);
	}

	return null;
}

function unsetCookie(name)
{
	setCookie(name, "", -1);
}

// vi:set ts=3 sw=3:
