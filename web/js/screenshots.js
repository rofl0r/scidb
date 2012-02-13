// Popup window with appropriate size for selected image

var itemWidths		= new Array();
var itemHeights	= new Array();
var itemTitles		= new Array();
var itemUrls		= new Array();

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

							itemWidths[id]		= itemImage.getAttribute("iwidth");
							itemHeights[id]	= itemImage.getAttribute("iheight");
							itemTitles[id]		= itemImage.getAttribute("alt");
							itemUrls[id]		= itemLink.getAttribute("href");

							itemImage.onclick = showImage;
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

function showImage(obj)
{
	var id = this.getAttribute("src");
	popupImageWindow(itemUrls[id], itemWidths[id], itemHeights[id], itemTitles[id]);
	return false;
}

// vi:set ts=3 sw=3:
