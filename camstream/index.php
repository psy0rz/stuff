<?
	require_once("streams.php");

	//head zooi
	head();


	//stappen grootte, dus hoeveel uur per keer laten zien
	$step=(4*3600);

	//default setttings
	$from=time()-$step;
	$to=time();
		
	
	//get requests
	if ($_REQUEST[from])
		$from=$_REQUEST[from];
	
	if ($_REQUEST[to])
		$to=$_REQUEST[to];
		
	//get data
	$movies=getmovies("biercam",$from, $to);
	
	$nav="<a href='?from=".($from-$step)."&to=".($to-$step)."'>Eerder</a> ";
	
	if ($from+$step <time())
		$nav.="<a href='?from=".($from+$step)."&to=".($to+$step)."'>Later</a>";
		
	$nav.=" <a href='?'>Nu</a>";
	$nav.=" <a href='view.php?url=mms://lounge.datux.nl:9192&desc=BirdyCam Live stream 1'>Livestream 1</a>";
	$nav.=" <a href='view.php?url=mmsh://lounge.datux.nl:9192&desc=BirdyCam Live stream 2'>Livestream 2</a>";
	$nav.=" <a href='view.php?url=http://lounge.datux.nl:9192&desc=BirdyCam Live stream 3'>Livestream 3</a>";


	//print navigation
	echo "$nav";

	if (!$_REQUEST[from])
	{	
    	    echo "
	    <span id='news'>
	    <h1>Het Laatste Birdy Nieuws</h1>
	    <table width=90%>
	    <tr><td>
	    <b>15/07</b> 
    	    De kaasch is voorzien van enige luchtgaten en pizzapunten. Nadat rick zn handen 2x gewassen had stonk ie nog naar kaas. :)
	    <p>
	    <b>18/07</b> 
    	    Jajajaja ...na deze broeierige dagen is de kaasch lekker uitgezakt en gesmolten. De pizza punten vinden reedsch gretigh aftrek door de vogelsch! 
	    De kaasch begint langzaam te veranderen 1 zompige moeras smurry. WERKELIJKWAAR SENSATIONEEL!
	    </tr></table>
	    </span>
	";
	}

	//print the grand movie table
	foreach ($movies as $movie=>$thumbs)
	{
		echo "<h1>";
		$time=movie2timestamp($movie);
		printtime($time);
		echo "</h1>";
		
		echo "<table>";
		
		$nr=0;
		echo "<tr>";
		foreach ($thumbs as $thumb)
		{
			echo "<td id=thumb>";
			echo "<image src='$thumb'><br>";
			$thumbtime=filectime($thumb);
			echo date("G:i",$thumbtime);
			
			$nr++;
			if ($nr==5)
			{
				$nr=0;
				echo "</tr><tr>";
			}
		}
		echo "</tr></table>";
		echo "<a href='view.php?url=http://$_SERVER[HTTP_HOST]/$movie&desc=";
		echo "Opname van ".date("d/m/Y G:i",$time);
		echo "'>Zien!</a>";
		echo " <a href='$movie'>Download</a>";
		echo " ".date("d/m/Y",$time);
	}

	echo "<p>";
	echo "$nav";

	foot();
