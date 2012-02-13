// Open new window in case of an external link.

function externalLinks()
{
	if (!document.getElementsByTagName)
		return;

	var anchors = document.getElementsByTagName("a");

	for (var i = 0; i < anchors.length; i++)
	{
		var anchor = anchors[i];

		if (anchor.getAttribute("href") && anchor.getAttribute("rel") == "external")
			anchor.target = "_blank";
	}
}

window.onload = externalLinks;

// vi:set ts=3 sw=3:
