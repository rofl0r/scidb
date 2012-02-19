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
							var id = itemImage.getAttribute("src");

							itemWidths[id]		= parseInt(itemImage.getAttribute("iwidth"));
							itemHeights[id]	= parseInt(itemImage.getAttribute("iheight"));
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

function showImage(obj)
{
	var id		= this.getAttribute("src");
	var blanket = document.getElementById("imageBlanket");
	var popup	= document.getElementById("imagePopup");
	var image	= getFirstImageWithNodeName(popup, "IMG");

	var popupWidth		= itemWidths[id];
	var popupHeight	= itemHeights[id];
	var viewportWidth;
	var viewportHeight;
	var blanketHeight;
	var popupX;
	var popupY;

	if (typeof window.innerHeight != 'undefined')
	{
		viewportWidth = window.innerWidth;
		viewportHeight = window.innerHeight;
	}
	else
	{
		viewportWidth = document.documentElement.clientWidth;
		viewportHeight = document.documentElement.clientHeight;
	}

	// disabled scrollbars
	overflow = document.body.style.overflow;
	document.body.style.overflow = "hidden";

	// Positioning blanket
	blanketHeight = viewportHeight + "px";
	blanketWidth = viewportWidth + "px";
	blanket.style.height = blanketHeight + "px";
	blanket.style.display = "block";

	// Positioning image
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
	popup.style.display = "block";

	// Make image
	image.width = itemWidths[id];
	image.height = itemHeights[id];
	image.src = itemUrls[id];
	popup.onclick = closeImage;
	blanket.onclick = closeImage;

	return false;
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

	// enable scrollbars
	document.body.style.overflow = overflow;


	return false;
}

// vi:set ts=3 sw=3:
