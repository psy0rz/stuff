<?
	require_once("streams2.php");

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
	$nav.=" <a href='view.php?url=mms://lounge.datux.nl:9192&desc=Biercam Live stream 1'>Livestream 1</a>";
	$nav.=" <a href='view.php?url=mmsh://lounge.datux.nl:9192&desc=Biercam Live stream 2'>Livestream 2</a>";
	$nav.=" <a href='view.php?url=http://lounge.datux.nl:9192&desc=Biercam Live stream 3'>Livestream 3</a>";


	//print navigation
	echo "$nav";
	
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
		
		
		echo "<span id='$movie'>";
		$rating[votes]="25";
		$rating[mine]="2";
		$rating[average]=2;
		$rating[movie_name]=$movie;
		echo GetRatingHtml($rating);
		echo "</span>";
		
	}

	echo "<p>";
	echo "$nav";

	foot();
