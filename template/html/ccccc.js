function setClassToSpecificColumnBadForLowValue(id, low, high) {
$('td#' + id).each(function(){
    var t = $(this);
    var val = parseInt( t.text(), 10 );
    if (val <= low) { t.addClass('bad'); }
    else if (val <= high) { t.addClass('warning'); }
});
}

function setClassToSpecificColumnBadForHighValue(id, low, high) {
$('td#' + id).each(function(){
    var t = $(this);
    var val = parseInt( t.text(), 10 );
    if (val >= high) { t.addClass('bad'); }
    else if (val >= low) { t.addClass('warning'); }
});
}

function setClassToColumn() {
	setClassToSpecificColumnBadForHighValue('MVG', 10, 30);
	setClassToSpecificColumnBadForHighValue('LOCpro', 25, 50);
}

function onDocLoaded() {
	setClassToColumn()
	$("table#myTable").tablesorter({theme : 'blue', widgets: [ "resizable" ], widgetOptions : {} });
}

