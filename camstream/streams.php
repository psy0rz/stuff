<?
	require_once("streamsdb.php");

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
	
	function getthumbs($movie)
	{
	    $thumbs=getdir("$movie.thumbs");
	    asort($thumbs);
	    $thumbs=array_reverse($thumbs);
	    return ($thumbs);
	}	
    
	/** Extract timestamp from thumbfilename
	 */   
	function thumb2timestamp($thumb)
	{
		//streams/biercam/1184845598.asf.thumbs/000013.jpeg
		preg_match('/^.*\/([0-9]*)\.asf\.thumbs\/([0-9]*)\.jpeg$/',$thumb,$matches);
		//echo $thumb;
		//print_R($matches);
		return ($matches[1] + 30 + ($matches[2] * 60));
		
	}
	/** returns all movies and corresponding thumbs
	 * $from and $to specify start/end time
    	 *
	 * $ret[$moviefilename]=array($thumbfilename1,$thumbfilename2....)
	 */
	function getmovies($stream,$from=0,$to=0, $maxthumbs=0)
	{
		$thumbscount=0;
	    	$movies=getdir("streams/$stream");
		//sorteer
		rsort($movies);
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
				$thumbs=getthumbs($movie);
				if ($thumbs)
				{
				    $ret[$movie]=$thumbs;
				    $thumbscount+=count($thumbs);
				}
				
				//kappen als we genoeg hebben
				if ($maxthumbs && $thumbscount>=$maxthumbs)
				    break;
	   		}
		}
		return($ret);
	}
	
	
	//function ratemovie($movie,$rating

	//print tijd op vriendelijke manier
	function printtime($timestamp)
	{
		$ago=time()-$timestamp;
		$days=(int)($ago/(3600*24));
		$hours=(int)(($ago%(3600*24))/3600);
		
		if ($days)
			echo "$days dagen en";
		echo " $hours uur geleden";
	}


	function head()
	{
		echo "<html>";
		echo "<head>";
		echo "<title>Welkom bij BirdyCam!</title>";			
		echo "<style>
		
		      body,td,a,p,.h{font-family:arial,sans-serif}
		       .h{color:#3366cc}
		       .q{color:#00c}
		       .ts td{padding:0}.ts{border-collapse:collapse}
		       h1 {
		               background-color: #888800;
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

			#thumb a
			{
				background: none;
				border-style: none;
				padding: 0px;
			}
			
			#logo
			{
				position:absolute;
				top: 5px;
				right: 5px;
				z-index:10;
			}
			#vote
			{
				position:absolute;
				right: 10px;
			}
			#vote img
			{
			    cursor: pointer;
			}
			.selected
			{
				border-style: solid;
				border-color: black;
				border-width: 0px;
				border-bottom-width: 2px;
			}
																			
		</style>";
		echo "<script>";
		echo "
		
		function Vote(movie_name, vote_value)
		{
			span = document.getElementById(movie_name);
			span.innerHTML = '<span id=vote>Moment...</span>';
        		// Create new JS element
			var jsel = document.createElement('SCRIPT');
         		jsel.type = 'text/javascript';
         		jsel.src ='vote.php?movie_name='+movie_name+'&vote_value='+vote_value+'&random='+Math.random();
        
			// Append JS element (therefore executing the 'AJAX' call)
         		document.body.appendChild (jsel);
 		}
		
		var newwindow;
		function pop(url)
		{
		        newwindow=window.open(url,'name','height=450,width=500');
			        if (window.focus) {newwindow.focus()}
		}
				
		";
		echo "</script>";						
		echo "</head>";
		echo "<body bgcolor=#ffffff text=#000000 link=#0000cc vlink=#551a8b alink=#ff0000>";
		echo "<img src='streams/birdy.png' id='logo'>";
	}
	
	function foot()
	{
	
	        echo "</body></html>";
		
	}

	function GetRatingHtml($movie_name,$voted=0)
	{
		$rating=DbGetRating($movie_name);
		//print_r($rating);
                $ret=" <span id='vote'>";

		
		$ret.="Gem: ";
                for ($star=1;$star<6;$star++)
                {
                        $ret.="<img onclick='Vote(\"$movie_name\",$star);' src='";
                        if ($rating[avg] >= ($star))
                                $ret.="full.gif";
                        elseif ($rating[avg] >=($star-0.5))
                                $ret.="half.gif";
                        else
                                $ret.="empty.gif";

                        $ret.="'";

                        if ($star == $rating[vote_value])
                                $ret.=" class='selected'";
                        $ret.=">";
                }
		if ($rating[count]==1)
                  	$ret.=" (1 beoordeling)";
		elseif ($rating[count]>1)
                  	$ret.=" ($rating[count] beoordelingen)";
		if ($voted)
		{
		    $ret.="<br>Bedankt, je stem is verwerkt!";
		}
		if (!$rating[vote_value])
		    $ret.="<br><b>Stem nu, klik op een ster!</b>";
		

                $ret.="</span>";
		return($ret);
	}
