<?
	require_once("streams.php");

	//head zooi
	head();

	//stappen grootte, dus hoeveel uur per keer laten zien
	$step=(4*3600);

	//default setttings
	$to=time();
		
	
	//get requests
	if ($_REQUEST[from])
		$from=$_REQUEST[from];
	
	if ($_REQUEST[to])
		$to=$_REQUEST[to];
		
	if ($to > time())
	    $to=time();
		
	//get data
	if ($_REQUEST[top])
	    $movies=DbGetTop(20);
	else
	    $movies=getmovies("biercam",$from, $to, 240);
	
	//dermine oldest and newest movie timestamps
	$newest=array_keys($movies);
	$newest=movie2timestamp($newest[0]);
	$oldest=array_reverse(array_keys($movies));
	$oldest=movie2timestamp($oldest[0]);
	
	$nav.="<a href='?'>Nu</a> ";
	if (!$_REQUEST[top])
	{
    	    $nav.="<a href='?to=".($oldest-1)."'>Iets eerder</a> ";
    	    $nav.="<a href='?to=".($to-24*3600)."'>1 dag eerder</a> ";
	    
	}
//anders fixen:	if ($to < time())
//		$nav.="<a href='?to=".($newest+$step)."'>Later</a>";
		
	$nav.=" <a href='?top=jahwohl'>Top 20</a>";
	$nav.=" <a href='javascript:pop(\"live.php?url=mms://lounge.datux.nl:9192&desc=BirdyCam Live stream voor Windows Media Speler\");'>Livestream 1</a>";
	$nav.=" <a href='javascript:pop(\"live.php?url=mmsh://lounge.datux.nl:9192&desc=BirdyCam Live stream voor VLC\");'>Livestream 2</a>";
	$nav.=" <a href='javascript:pop(\"live.php?url=http://lounge.datux.nl:9192&desc=BirdyCam Live stream voor Mplayer\");'>Livestream 3</a>";


	//print navigation
	echo "$nav";
	
	if ($_REQUEST[top])
	{
	    echo "<h1>Top 20</h1>";
	    echo "Hieronder een overzicht van de momenten met de hoogste scores. ";
	}	
        elseif (!$_REQUEST[to])
        {
            echo "
            <span id='news'>
            <h1>Het Laatste Birdy Nieuws</h1>
            <table width=90%>
            <tr><td>
	    

            <b>23/07</b>
	    De kaasch is geupgrade met een extra nasi-module die al 8 dagen in de koelkast stond. Nee, het is geen kots!
            <p>

            <b>22/07</b>
	    De top20 werkt nu! Stem op de filmpjes die leuk zijn zodat ze hoger komen. Je kan vanaf nu ook
	    op de plaatjes klikken, zodat je de opname gelijk vanaf dat tijdstip ziet.

            <p>
            <b>18/07</b>
            Jajajaja ...na deze broeierige dagen is de kaasch lekker uitgezakt en gesmolten. De pizza punten vinden reedsch
gretigh aftrek door de vogelsch!
            De kaasch begint langzaam te veranderen 1 zompige moeras smurry. WERKELIJKWAAR SENSATIONEEL!

            </tr></table>
            </span>
        ";
        }


	//print the grand movie table
	$pos=1;
	foreach ($movies as $movie=>$thumbs)
	{
		echo "<h1>";
		if ($_REQUEST[top])
		    echo "#$pos: ";
		$time=movie2timestamp($movie);
		printtime($time);
		echo ":</h1>";
		
		echo "<table>";
		
		$nr=0;
		echo "<tr>";
		foreach ($thumbs as $thumb)
		{
			$thumbtime=thumb2timestamp($thumb);
			$start=$thumbtime-$time;
			if ($start<0)
			    $start=0;
			echo "<td id=thumb>";
			echo "<a id=thumb href='javascript:pop(\"view.php?movie_name=$movie&start=$start\");'>";
			echo "<img src='$thumb'>";
			echo "</a>";
			echo "<br>";
			echo date("G:i",$thumbtime);
			
			$nr++;
			if ($nr==5)
			{
				$nr=0;
				echo "</tr><tr>";
			}
		}
		echo "</tr></table>";

		echo "<a href='javascript:pop(\"view.php?movie_name=$movie\");'>";
		echo "Zien!</a>";
		echo " <a href='$movie'>Download</a>";
		echo " ".date("d/m/Y",$time);
		
		
		echo "<span id='$movie'>";
		echo GetRatingHtml($movie);
		echo "</span>";
		echo "<br><br>";
		$pos++;		
	}

	echo "<p>";
	echo "$nav";

	foot();
