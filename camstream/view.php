<?
require_once("streams.php");


head();

$movie_name=$_REQUEST[movie_name];
$start=(int)$_REQUEST[start];
$time=movie2timestamp($movie_name);
$src="http://".$_SERVER["HTTP_HOST"]."/";
if ($start)
{
    //indirect omdat we een startpositie hebben
    $src.="get.php?movie_name=$movie_name&start=$start";
    
}
else
{
    //rechtstreek via apache
    $src.=$movie_name;
}

echo "<h1>Opname van ";
printtime($time);
echo "</h1>";

echo date("d/m/Y",$time);
echo " ".date("G:i",$time);
if ($start)
echo " ($start seconden vanaf het begin)";

echo "<br>";

echo "
<embed type='video/x-ms-asf' 
  name='video'
  width='368px'
  height='294px'
  src='$src' />
";

echo "<br>";

echo "<a href='$src'>Download</a>";

echo "<span id='$movie_name'>";
echo GetRatingHtml($movie_name);
echo "</span>";					
	
foot();