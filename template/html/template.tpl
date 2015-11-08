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
<li><b>LOCphy</b> = Number of physical lines<br/>It includes blank line and line with code or comment.</li>
<li><b>LOCpro</b> = Number of lines with code<br/>Number of lines of with source code. (Line may also have comment)<br/>
<span class="warning">high value from 25</span>. <span class="bad">Very high value from 50</span>.
</li>
<li><b>LOCcom</b> = Lines of Comments<br/>Number of lines of comment identified. (Line may also have code)</li>
<li><b>LOCbl</b> = Number of blank-lines.</li>
<li><b>MVG</b> = <a href="http://en.wikipedia.org/wiki/Cyclomatic_complexity">McCabe's Cyclomatic Complexity</a><br/>
Number of linearly independent paths through a program's source code.<br/> It is nearly equivalent to the number of branches.<br/>
<span class="warning">high value from 10</span>. <span class="bad">Very high value from 30</span>.
</li>
<li><b>Block</b> = Max Nested Block Count.<span class="warning">high value from 4</span>. <span class="bad">Very high value from 5</span>.</li>
<li><a href="http://en.wikipedia.org/wiki/Halstead_complexity_measures">Halstead metrics</a>:
<ul>
 <li><b>Vocabulary size</b> = Number of distinct 'tokens'.</li>
 <li><b>Program length</b> = Number of 'tokens'.</li>
 <li><b>Volume</b> = Information contents of the function, measured in mathematical bits.
  <span class="warning">low value from 15</span>. <span class="bad">Very low value from 10</span>.
  <span class="warning">high value from 1000</span>. <span class="bad">Very high value from 1200</span>.
 </li>
 <li><b>Difficulty</b> = Error proneness of the program.</li>
 <li><b>Effort</b> = The effort to implement or understand a function.</li>
 <li><b>Time to implement</b> (in seconds) = Approximation time to implement or understand a function.</li>
 <li><b>Delivered bugs</b> = Estimate for the number of errors in the implementation.</li>
</ul></li>
<li><a href="http://www.verifysoft.com/en_maintainability.html">Maintainability Index</a>:
Computed from line count, mvg and halstead metrics.
<ul>
  <li><b>MIwoc</b> = maintainability index without comments.</li>
  <li><b>MIcw</b> = maintainability index comment weight.</li>
  <li><b>MI</b> = maintainability index (MIwoc + MIcw).
  <span class="warning">low value from 85</span>. <span class="bad">Very low value from 65</span>.
  </li>
</ul></li>
</ul>
<ul>
  <li><b>CallCount</b> = The number of functions called by this function.</li>
  <li><b>CallerCount</b> = The number of times this function is called.
    <span class="warning">low value from 0 (unused function)</span>.
  </li>
<ul>
<h2>Report.</h2>
<em>TIP!</em> Sort multiple columns simultaneously by holding down the shift key and clicking a second, third or even fourth column header!
<table width="100%" id="myTable" class="tablesorter draggable">
<thead>
<tr><th>Filename</th><th>line</th><th>Namespaces Name</th><th>Classes Name</th><th>Prototype</th><th class="LOCphy">LOCphy</th><th class="LOCpro">LOCpro</th><th class="LOCcom">LOCcom</th><th class="LOCbl">LOCbl</th><th class="MVG">MVG</th><th class="NestedBlockCount">Block</th><th class="Halstead_n">Vocabulary size</th><th class="Halstead_N">Program length</th><th class="Halstead_V">Volume</th><th class="Halstead_D">Difficulty</th><th class="Halstead_E">Effort</th><th class="Halstead_T">Time to implement (s)</th><th class="Halstead_B">Delivered bugs</th><th class="MIwoc">MIwoc</th><th class="MIcw">MIcw</th><th class="MI">MI</th><th class="CallCount">CallCount</th><th class="CallerCount">CallerCount</th></tr>
</thead>
<tbody>
{{#ForEachFiles}}{{#ForEachFunctions}}<tr><td>{{filename:h}}</td><td>{{lineDefinition}}</td><td>{{namespacesName:h}}</td><td>{{classesName:h}}</td><td>{{#static}}[static] {{/static}}{{#virtual}}[virtual] {{/virtual}}{{funcName:h}}{{#const}} const{{/const}}{{#override}} [override]{{/override}}</td><td>{{LOCphy}}</td><td>{{LOCpro}}</td><td>{{LOCcom}}</td><td>{{LOCbl}}</td><td>{{MVG}}</td><td>{{NestedBlockCount}}</td><td>{{Halstead_n}}</td><td>{{Halstead_N}}</td><td>{{Halstead_V}}</td><td>{{Halstead_D}}</td><td>{{Halstead_E}}</td><td>{{Halstead_T}}</td><td>{{Halstead_B}}</td><td>{{MIwoc}}</td><td>{{MIcw}}</td><td>{{MI}}</td><td>{{CallCount}}</td><td>{{CallerCount}}</td></tr>
{{/ForEachFunctions}}{{/ForEachFiles}}</tbody>
</table>
<span class="footer">File generated {{Date}} by <a href="https://bitbucket.org/jarod42/ccccc">ccccc</a>.</span>
</body>
</html>
