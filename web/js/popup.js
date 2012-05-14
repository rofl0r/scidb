// Popup window with appropriate size for selected image

var itemWidths		= new Array();
var itemHeights	= new Array();
var itemTitles		= new Array();
var itemUrls		= new Array();
var overflow		= "auto";

function prepareThumbnails()
{
	var tableItems = document.getElementById("thumbnails");

	if (tableItems)
	{
		for (var r = 0; r < tableItems.rows.length; r++)
		{
			var columnItems = tableItems.rows[r];

			for (var c = 0; c < columnItems.cells.length; c++)
			{
				var columnItem = columnItems.cells[c];

				if (columnItem.nodeName == "TD")
				{
					var itemLink = getFirstChildWithTagName(columnItem, "A");

					if (itemLink)
					{
						var itemImage = getFirstChildWithTagName(itemLink, "IMG");

						if (itemImage)
						{
							var id	= itemImage.getAttribute("src");
							var dim	= itemImage.getAttribute("name").split("x");

							itemWidths[id]		= parseInt(dim[0]);
							itemHeights[id]	= parseInt(dim[1]);
							itemTitles[id]		= itemImage.getAttribute("title");
							itemUrls[id]		= itemLink.getAttribute("href");

							itemImage.onmouseover = imageEnter;
							itemImage.onmouseout = imageLeave;
							itemImage.onclick = showImage;
							itemImage.className = "bordersOff";
						}
					}
				}
			}
		}
	}
}

function getFirstChildWithTagName(element, tagName)
{
	for (var i = 0; i < element.childNodes.length; i++) {
		if (element.childNodes[i].nodeName == tagName)
			return element.childNodes[i];
	}
}

function getFirstImageWithNodeName(element, nodeName)
{
	for (var i = 0; i < element.childNodes.length; i++) {
		if (element.childNodes[i].nodeName == nodeName)
			return element.childNodes[i];
	}
}

function imageEnter(obj) {
	this.className = "bordersOn";
	return true;
}

function imageLeave(obj) {
	this.className = "bordersOff";
	return true;
}

function getViewport()
{
	var width	= 600;
	var height	= 400;

	if (window.innerHeight)
	{
		width = window.innerWidth;
		height = window.innerHeight;
	}
	else if (document.documentElement && document.documentElement.clientWidth != 0)
	{
		width = document.documentElement.clientWidth;
		height = document.documentElement.clientHeight;
	}
	else if (document.body)
	{
		width = document.body.clientWidth;
		height = document.body.clientHeight;
	}

	return new Array(width, height);
}

function showImage(obj)
{
	var id		= this.getAttribute("src");
	var blanket = document.getElementById("imageBlanket");
	var popup	= document.getElementById("imagePopup");
	var image	= getFirstImageWithNodeName(popup, "IMG");

	// disabled scrollbars
	overflow = document.body.style.overflow;
	document.body.style.overflow = "hidden";

	// positioning
	popup.style.display = "block";
	resizeImageWindow(id);

	// make image
	image.width = itemWidths[id];
	image.height = itemHeights[id];
	image.src = itemUrls[id];
	popup.onclick = closeImage;
	blanket.onclick = closeImage;

	// add resize event handler
	addEvent(window, "resize", resizeImageWindow, id);

	return false;
}

function resizeImageWindow(id)
{
	var popup				= document.getElementById("imagePopup");
	var blanket 			= document.getElementById("imageBlanket");
	var viewport			= getViewport();
	var viewportWidth		= viewport[0];
	var viewportHeight	= viewport[1];
	var popupWidth			= itemWidths[id];
	var popupHeight		= itemHeights[id];
	var blanketHeight;
	var popupX;
	var popupY;

	// positioning blanket
	blanketHeight = viewportHeight + "px";
	blanketWidth = viewportWidth + "px";
	blanket.style.height = blanketHeight + "px";
	blanket.style.display = "block";

	// positioning image
	if (popupHeight > viewportHeight)
		popupWidth += 20;
	if (popupWidth > viewportWidth)
		popupHeight += 20;
	popupWidth = Math.min(viewportWidth, popupWidth);
	popupHeight = Math.min(viewportHeight, popupHeight);
	popupX = Math.round((viewportWidth - popupWidth)/2);
	popupY = Math.round((viewportHeight - popupHeight)/2);

	popup.style.width = popupWidth + "px";
	popup.style.height = popupHeight + "px";
	popup.style.left = popupX + "px";
	popup.style.top = popupY + "px";

	return true;
}

function closeImage(obj)
{
	var blanket	= document.getElementById("imageBlanket");
	var popup	= document.getElementById("imagePopup");
	var image	= getFirstImageWithNodeName(popup, "IMG");

	popup.style.display = "none";
	blanket.style.display = "none";
	popup.style.width = "0";
	popup.style.height = "0";
	image.width = 0;
	image.height = 0;
	image.src = "";

	// remove resize event handler
	removeEvent(window, "resize", resizeImageWindow);

	// enable scrollbars
	document.body.style.overflow = overflow;

	return false;
}

function addEvent(obj, type, fn, arg)
{
	obj["e" + type + fn] = fn;
	obj[type + fn] = function() { obj["e" + type + fn](arg); }

	if (obj.attachEvent)
		obj.attachEvent("on" + type, obj[type + fn]);
	else
		obj.addEventListener(type, obj[type + fn], false);
}

function removeEvent(obj, type, fn)
{
	if (obj.detachEvent)
		obj.detachEvent("on" + type, obj[type + fn]);
	else
		obj.removeEventListener(type, obj[type + fn], false);

	obj["e" + type + fn] = null;
	obj[type + fn] = null;
}

// vi:set ts=3 sw=3:
