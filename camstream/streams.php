<?


	function getdir($dir)
	{
		$handle=opendir($dir);
		while (false!==($file = readdir($handle))) 
		{ 
			if ($file != "." && $file != "..") 
			{ 	
				$ret[]="$dir/$file";
			} 
	    	} 
		return ($ret);
	}

	function getstreams()
	{
		return (getdir("streams"));
	}
	 
	/** Extract timestamp from moviefilename
	 */   
	function movie2timestamp($movie)
	{
		preg_match("/\/([0-9]*)\..*$/",$movie,$matches);
		return ($matches[1]);
	}
	
	/** returns all movies and corresponding thumbs
	 * $from and $to specify start/end time
	 *
	 * $ret[$moviefilename]=array($thumbfilename1,$thumbfilename2....)
	 */
	function getmovies($stream,$from=0,$to=0)
	{
	    	$movies=getdir("streams/$stream");
	    	foreach ($movies as $movie)
	    	{
			if (is_file("$movie"))
			{
				$timestamp=movie2timestamp($movie);
				if ($from && $timestamp<$from)
					continue;
				if ($to && $timestamp>$to)
					continue;					
				//get thumbs
				$thumbs=getdir("$movie.thumbs");
				asort($thumbs);
				$thumbs=array_reverse($thumbs);
				$ret[$movie]=$thumbs;
	   		}
		}
		//sorteer
		ksort($ret);
		return(array_reverse($ret));
	}

	//print tijd op vriendelijke manier
	function printtime($timestamp)
	{
		$ago=time()-$timestamp;
		$days=(int)($ago/(3600*24));
		$hours=(int)(($ago%(3600*24))/3600);
		
		if ($days)
			echo "$days dagen en";
		echo " $hours uur geleden.";
	}


	function head()
	{
		echo "<html>";
		echo "<head>";
		echo "<title>Welkom bij BirdyCam!</title>";			
		echo "<style>
		<!--
		      body,td,a,p,.h{font-family:arial,sans-serif}
		       .h{color:#3366cc}
		       .q{color:#00c}
		       .ts td{padding:0}.ts{border-collapse:collapse}-->
		       h1 {
		               background-color:888800;
		               font-size: 130%;
		               color: #ffffff;
		               padding-left: 10px;
		        }
		 	#thumb
			{
				text-align: center;			
			}
			a 
			{
				background-color: #880000;
				color: #ffffff;
				border-color: black;
				border-style: solid;
				border-width: 1px;
				text-decoration: none;
				padding-left: 5px;
				padding-right: 5px;
				font-weight: bold;
				
			}
			a:hover
			{
				color: yellow;
			}
			#logo
			{
				position:fixed;
				top: 5px;
				right: 5px;
				z-index: 0;
			}
			#news
			{
			}
																			
		</style>";
		echo "</head>";
		echo "<body bgcolor=#ffffff text=#000000 link=#0000cc vlink=#551a8b alink=#ff0000>";
		echo "<image src='streams/birdy.png' id='logo'>";
	}
	
	function foot()
	{
	
	        echo "</body></html>";
		
	}