<?
require_once("streams.php");

head();

echo "<h1>$_REQUEST[desc]</h1>";

echo "
<embed type='video/x-ms-asf' 
  name='video'
  src='$_REQUEST[url]' />
";

echo "<br>";
echo "<a href='$_REQUEST[url]'>Download</a>";
foot();