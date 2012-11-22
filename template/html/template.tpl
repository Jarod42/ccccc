<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<title>CCCCCC report</title>
<link rel="stylesheet" href="file:///{{cccccRoot:h}}/3rd/__jquery.tablesorter/themes/blue/style.css" type="text/css" media="print, projection, screen" />
<link rel="stylesheet" type="text/css" href="file:///{{cccccRoot:h}}/template/html/ccccc.css" media="all" />
<script type="text/javascript" src="file:///{{cccccRoot:h}}/3rd/jquery.js"></script>
<script type="text/javascript" src="file:///{{cccccRoot:h}}/3rd/__jquery.tablesorter/jquery.tablesorter.js"></script>
<script type="text/javascript" src="file:///{{cccccRoot:h}}/3rd/jquery.tablesorter.widgets.js"></script>
<script type="text/javascript" src="file:///{{cccccRoot:h}}/3rd/dragtable.js"></script>
<script type="text/javascript" src="file:///{{cccccRoot:h}}/template/html/ccccc.js"></script>
</head>
<body onload="onDocLoaded();">
<h1>CCCCC Software Metrics Report</h1>
<h2>Definitions.</h2>
<ul>
<li>LOCphy = Number of physical lines<br/>It include blank line and line with code or comment.</li>
<li>LOCpro = Number of lines with code<br/>Number of lines of with source code. (Line may also have comment)<br/>
<span class="warning">high value from 25</span>. <span class="bad">Very high value from 50</span>.
</li>
<li>LOCcom = Lines of Comments<br/>Number of lines of comment identified. (Line may also have code)</li>
<li>LOCbl = Number of blank-line.</li>
<li>MVG = <a href="http://en.wikipedia.org/wiki/Cyclomatic_complexity">McCabe's Cyclomatic Complexity</a><br/>
Number of linearly independent paths through a program's source code.<br/> It is nearly equivalent to the number of branches.<br/>
<span class="warning">high value from 10</span>. <span class="bad">Very high value from 30</span>.
</li>
</ul>
<h2>Report.</h2>
<table width="100%" id="myTable" class="tablesorter draggable">
<thead>
<tr><th>Filename</th><th>line</th><th>Namespaces Name</th><th>Classes Name</th><th>Prototype</th><th>LOCphy</th><th>LOCpro</th><th>LOCcom</th><th>LOCbl</th><th>MVG</th></tr>
</thead>
<tbody>
{{#InFiles}}{{#InFunctions}}	<tr><td>{{filename:h}}</td><td>{{lineDefinition}}</td><td>{{namespacesName:h}}</td><td>{{classesName:h}}</td><td>{{funcName:h}}</td><td class="LOCphy">{{LOCphy}}</td><td class="LOCpro">{{LOCpro}}</td><td class="LOCcom">{{LOCcom}}</td><td class="LOCbl">{{LOCbl}}</td><td class="MVG">{{MVG}}</td></tr>
{{/InFunctions}}{{/InFiles}}</tbody>
</table>
</body>
</html>
