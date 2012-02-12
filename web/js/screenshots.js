// screenshots.js

var itemImages  = new Array();
var itemWidths  = new Array();
var itemHeights = new Array();
var itemTitles  = new Array();
var itemUrls    = new Array();

function init()
{
  var tableItems = document.getElementById('screenshots');

  for (var r = 0; r < tableItems.rows.length; r++)
  {
    var columnItems = tableItems.rows[r];

    for (var c = 0; c < columnItems.cells.length; c++)
    {
      var columnItem = columnItems.cells[c];

      if (columnItem.nodeName == "TD")
      {
        var itemLink  = getFirstChildWithTagName(columnItem, 'A');
        var itemImage = getFirstChildWithTagName(itemLink, 'IMG');
        var id        = itemImage.getAttribute('src');

        itemImages[id]  = itemImage;
        itemWidths[id]  = itemImage.getAttribute('iwidth');
        itemHeights[id] = itemImage.getAttribute('iheight');
        itemTitles[id]  = itemImage.getAttribute('alt');
        itemUrls[id]    = itemLink.getAttribute('href');
      }
    }
  }

  for (var id in itemImages)
  {
    itemImages[id].onclick = showImage;
  }
}

function getFirstChildWithTagName(element, tagName)
{
  for (var i = 0; i < element.childNodes.length; i++) {
    if (element.childNodes[i].nodeName == tagName)
      return element.childNodes[i];
  }
}

function showImage(id)
{
  var id = this.getAttribute('src');
  popupImageWindow(itemUrls[id], itemWidths[id], itemHeights[id], itemTitles[id]);
  return false;
}

// vi:set ts=2 sw=2: et:
