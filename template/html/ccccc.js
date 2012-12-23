function setClassToSpecificColumnBadForLowValue(classid, low, high) {
	var colNumber = -1;
	$.each($('th.' + classid),function(idx,_val) {
    var table = $(this).parent().parent();
    colNumber = 1 + $(this).parent("tr").children().index($(this));
	});
	$('table tr').each(function() {
		var tr = $(this)
		var td = tr.find("td:nth-child(" + colNumber + ")")
		var val = parseInt( td.text(), 10 );
		if (val <= low) { td.addClass('bad'); }
		else if (val <= high) { td.addClass('warning'); }
	});
}

function setClassToSpecificColumnBadForHighValue(classid, low, high) {
	var colNumber = -1;
	$.each($('th.' + classid),function(idx,_val) {
    var table = $(this).parent().parent();
    colNumber = 1 + $(this).parent("tr").children().index($(this));
	});
	$('table tr').each(function() {
		var tr = $(this)
		var td = tr.find("td:nth-child(" + colNumber + ")")
		var val = parseInt( td.text(), 10 );
		if (val >= high) { td.addClass('bad'); }
		else if (val >= low) { td.addClass('warning'); }
	});
}

function setClassToColumn() {
	setClassToSpecificColumnBadForHighValue('MVG', 10, 30);
	setClassToSpecificColumnBadForHighValue('LOCpro', 25, 50);
	setClassToSpecificColumnBadForHighValue('Halstead_V', 1000, 1200);
	setClassToSpecificColumnBadForLowValue('Halstead_V', 10, 15);
	setClassToSpecificColumnBadForHighValue('Halstead_B', 1, 2);
	setClassToSpecificColumnBadForLowValue('MI', 65, 85);
}

function onDocLoaded() {
	setClassToColumn()
	$("table#myTable").tablesorter({theme : 'blue', widgets: [ "resizable" ], widgetOptions : {} });
}

